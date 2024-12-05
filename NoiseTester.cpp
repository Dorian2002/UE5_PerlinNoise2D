// Fill out your copyright notice in the Description page of Project Settings.

#include "NoiseTester.h"
#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"
#include "PerlinNoiseLibrary.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Blueprint/UserWidget.h"
#include "UObject/SavePackage.h"


// Sets default values
ANoiseTester::ANoiseTester()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or the object is spawned
void ANoiseTester::BeginPlay()
{
    // Call the parent class's BeginPlay function to ensure the base class logic runs
    Super::BeginPlay();

    // Define the path for the texture asset within the game content director
    FString PackageName = AssetPath; // The full package name includes the asset path
    FString AssetName = FPackageName::GetLongPackageAssetName(PackageName); // Extract the asset's name from the path

    // Check if the asset name is valid; exit if it is not
    if (AssetName.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid asset path. Cannot save texture."));
        return;
    }

    // Attempt to create a new package for the texture
    UPackage* Package = CreatePackage(*PackageName);
    if (!Package)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to create package for texture."));
        return;
    }

    // Create a new 2D texture object inside the package
    UTexture2D* NoiseTexture = NewObject<UTexture2D>(Package, FName(AssetName), RF_Public | RF_Standalone);

    // Initialize the texture's source data with a square resolution and BGRA format
    NoiseTexture->Source.Init(Size, Size, 1, 1, TSF_BGRA8);

    // Prepare a buffer to store pixel data (BGRA format, 4 bytes per pixel)
    TArray<uint8_t> Buffer;
    Buffer.Reserve(Size * 4);

    // Generate Perlin noise and populate the texture's pixel data
    for (int32 i = 0; i < Size; ++i)
    {
        for (int32 y = 0; y < Size; ++y)
        {
            // Generate a Perlin noise value for the current pixel
            float NoiseValue = UPerlinNoiseLibrary::Generate2DPerlinNoise(i, y, Scale, Seed, Persistence, Octaves);

            // Convert the noise value to an 8-bit grayscale intensity (clamped between 0 and 255)
            uint8 GrayscaleValue = FMath::Clamp(((NoiseValue + 1.f) / 2) * 255.0f, 0.0f, 255.0f);

            // Add pixel data to the buffer in BGRA format
            Buffer.Add(GrayscaleValue); // Blue
            Buffer.Add(GrayscaleValue); // Green
            Buffer.Add(GrayscaleValue); // Red
            Buffer.Add(255);        // Alpha (fully opaque here)
        }
    }

    // Lock the texture's first mip level and copy the generated pixel data into it
    uint8* TextureData = NoiseTexture->Source.LockMip(0);
    FMemory::Memcpy(TextureData, Buffer.GetData(), Buffer.Num());
    NoiseTexture->Source.UnlockMip(0);

    // Update the texture's resource to reflect the changes
    NoiseTexture->UpdateResource();

    // Check if the texture object is valid before proceeding
    if (!NoiseTexture)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid texture. Cannot save as asset."));
        return;
    }

    // Mark the package as dirty to indicate it has unsaved changes
    Package->MarkPackageDirty();

    // Notify the asset registry of the new texture asset
    FAssetRegistryModule::AssetCreated(NoiseTexture);

    // Define the file path to save the package
    FString PackageFileName = FPackageName::LongPackageNameToFilename(PackageName, FPackageName::GetAssetPackageExtension());

    // Configure package save options
    FSavePackageArgs SaveArgs;
    SaveArgs.TopLevelFlags = EObjectFlags::RF_Public | EObjectFlags::RF_Standalone; // Asset visibility and independence flags (could be redundant with line 51) 
    SaveArgs.Error = GError; // Set error output to the global error device
    SaveArgs.bForceByteSwapping = false;
    SaveArgs.bWarnOfLongFilename = true; // Optional: warn if the filename is excessively long
    SaveArgs.SaveFlags = SAVE_NoError;   // Ignore errors during the save operation

    // Attempt to save the package and check the result
    bool bSuccess = UPackage::SavePackage(
        Package,
        NoiseTexture,
        *PackageFileName,
        SaveArgs
    );

    // Handle success or failure of the save operation
    if (bSuccess)
    {
        UE_LOG(LogTemp, Log, TEXT("Texture saved as asset: %s"), *PackageFileName);

        // Reload the package to ensure it is properly registered
        UPackage* LoadedPackage = LoadPackage(nullptr, *PackageName, LOAD_None);
        if (LoadedPackage)
        {
            LoadedPackage->FullyLoad();

            // Refresh the Content Browser to display the new asset
            FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
            ContentBrowserModule.Get().SyncBrowserToAssets({ FAssetData(NoiseTexture) });
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to save texture as asset."));
    }
}

// Called every frame
void ANoiseTester::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}