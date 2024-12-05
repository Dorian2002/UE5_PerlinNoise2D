// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NoiseTester.generated.h"

UCLASS()
class PERLINNOISEPROJECT_API ANoiseTester : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ANoiseTester();

	// Size of the texture
	UPROPERTY(EditAnywhere)
	int32 Size = 250;

	//The path to save the texture
	UPROPERTY(EditAnywhere)
	FString AssetPath = "/Game/MyTexture";

	UPROPERTY(EditAnywhere)
	float Scale = 0.05f;
	
	UPROPERTY(EditAnywhere)
	uint32 Seed = 8173;
	
	UPROPERTY(EditAnywhere)
	float Persistence = 0.5;

	//To number of octaves
	UPROPERTY(EditAnywhere)
	uint32 Octaves = 3;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
