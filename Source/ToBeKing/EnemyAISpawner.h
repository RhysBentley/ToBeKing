// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyAI.h"
#include "PlayerControlled.h"
#include "Engine/TargetPoint.h"
#include "EnemyAISpawner.generated.h"

UCLASS()
class TOBEKING_API AEnemyAISpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyAISpawner();

	// Components
	UPROPERTY()
	UBillboardComponent* BillboardComponent;


	// References
	UPROPERTY()
	APlayerControlled* PlayerReference;


	// Spawning
	UPROPERTY()
	FTimerHandle WaveTimerHandle;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AEnemyAI> EnemyToSpawn;

	UPROPERTY(EditAnywhere)
	TArray<ATargetPoint*> spawnLocations;

	UPROPERTY()
	int totalSpawned;

	UPROPERTY()
	FEnemySpawning CurrentWave;

	UPROPERTY()
	bool spawningWaveEnded;

	UPROPERTY(EditAnywhere)
	int TotalofEnemyDeaths;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void DelayBeginPlay();

	void BeginEnemyWave();

	void EndOfEnemies();
};
