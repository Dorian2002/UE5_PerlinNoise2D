// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PerlinNoiseLibrary.generated.h"

UCLASS()
class PERLINNOISEPROJECT_API UPerlinNoiseLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// 2D Perlin Noise 
	UFUNCTION(BlueprintCallable, Category = "Procedural Generation|Noise")
	static float Generate2DPerlinNoise(
		float X, 
		float Y, 
		float Scale = 1.0f, 
		int32 Seed = 0, 
		float Persistence = 0.5f, 
		int32 Octaves = 4
	);

private:
	// Gradient helper functions
	static float Dot2(const FVector2D& A, const FVector2D& B);
	
	// Noise generation helpers
	static float Fade(float T);
	static float Lerp(float A, float B, float X);

	// Permutation table for noise generation
	static TArray<int32> GeneratePermutationTable(int32 Seed);

	static FVector2D GetConstantVector(int32 v);
};
