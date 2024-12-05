// Fill out your copyright notice in the Description page of Project Settings.

#include "PerlinNoiseLibrary.h"
#include "Math/UnrealMathUtility.h"

float UPerlinNoiseLibrary::Fade(float T)
{
    // Smoothing function: 6T^5 - 15T^4 + 10T^3
    return ((6*T - 15)*T + 10)*T*T*T;
}

float UPerlinNoiseLibrary::Lerp(float A, float B, float X)
{
    return A + X * (B - A);
}

float UPerlinNoiseLibrary::Dot2(const FVector2D& A, const FVector2D& B)
{
    return A.X * B.X + A.Y * B.Y;
}

TArray<int32> UPerlinNoiseLibrary::GeneratePermutationTable(int32 Seed)
{
    TArray<int32> PermTable;
    
    PermTable.Reserve(2048);

    // Initialize with sequential values
    for (int32 i = 0; i < 256; ++i)
    {
        PermTable.Add(i);
    }

    // Shuffle the table using a deterministic seed-based method
    FRandomStream RandomStream(Seed);
    for (int32 z = 255; z > 0; --z)
    {
        int32 J = RandomStream.RandRange(0, z);
        
        Swap(PermTable[z], PermTable[J]);
    }
    
    TArray<int32> Temp;
    Temp.Reserve(256);
    
    // Double the table for easy indexing
    for (int32 y = 0; y < 256; ++y)
    {
        Temp.Add(PermTable[y]); // Copy into temporary array
    }

    // Append all elements from Temp to PermTable
    PermTable.Append(Temp);

    return PermTable;
}

FVector2D UPerlinNoiseLibrary::GetConstantVector(int32 v)
{
    // v is the value from the permutation table
    const int32 h = v & 3;
    if(h == 0)
        return FVector2D(1.0, 1.0);
    if(h == 1)
        return FVector2D(-1.0, 1.0);
    if(h == 2)
        return FVector2D(-1.0, -1.0);
    return FVector2D(1.0, -1.0);
}

float UPerlinNoiseLibrary::Generate2DPerlinNoise(
    float X, 
    float Y, 
    float Scale, 
    int32 Seed, 
    float Persistence, 
    int32 Octaves
)
{
    // Generate permutation table based on seed
    TArray<int32> PermTable = GeneratePermutationTable(Seed);

    float NoiseValue = 0.0f;
    float Amplitude = 1.0f;
    float Frequency = Scale;
    float MaxValue = 0.0f;

    // Fractal Brownian Motion (fBm)
    for (int32 Octave = 0; Octave < Octaves; ++Octave)
    {
        // Scale input coordinates
        float SampleX = X * Frequency;
        float SampleY = Y * Frequency;

        // Compute grid cell coordinates
        const int32 Xi = FMath::FloorToInt(SampleX) & 255;
        const int32 Yi = FMath::FloorToInt(SampleY) & 255;

        // Local coordinates within the grid cell
        const float Xf = SampleX - Xi;
        const float Yf = SampleY - Yi;

        const FVector2D TopRight = FVector2D(Xf-1.0, Yf-1.0);
        const FVector2D TopLeft = FVector2D(Xf, Yf-1.0);
        const FVector2D BottomRight = FVector2D(Xf-1.0, Yf);
        const FVector2D BottomLeft = FVector2D(Xf, Yf);
        
        // Hash coordinates to get gradient indices
        const int32 ValueTopRight = PermTable[(PermTable[Xi + 1] + Yi + 1)];
        const int32 ValueTopleft = PermTable[(PermTable[Xi] + Yi + 1)];
        const int32 ValueBottomRight = PermTable[(PermTable[Xi + 1] + Yi)];
        const int32 ValueBottomLeft = PermTable[(PermTable[Xi] + Yi)];
        
        // Compute dot products
        const float DotTopRight = Dot2(TopRight, GetConstantVector(ValueTopRight));
        const float DotTopLeft = Dot2(TopLeft, GetConstantVector(ValueTopleft));
        const float DotBottomRight = Dot2(BottomRight, GetConstantVector(ValueBottomRight));
        const float DotBottomLeft = Dot2(BottomLeft, GetConstantVector(ValueBottomLeft));

        // Interpolation weights
        const float U = Fade(Xf);
        const float V = Fade(Yf);

        // Interpolate
        float NoiseVal = Lerp(
            Lerp(DotBottomLeft, DotTopLeft,V),
            Lerp(DotBottomRight, DotTopRight,V),
            U
            );

        // Accumulate noise
        NoiseValue += NoiseVal * Amplitude;
        MaxValue += Amplitude;

        // Prepare for next octave
        Amplitude *= Persistence;
        Frequency *= 2.0f;
    }

    // Normalize
    return NoiseValue / MaxValue;
}
