// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingBase.h"
#include "Components/SphereComponent.h"
#include "EnemyAI.h"
#include "ArcherTowerBuilding.generated.h"

UCLASS()
class TOBEKING_API AArcherTowerBuilding : public ABuildingBase
{
	GENERATED_BODY()

public:

	AArcherTowerBuilding();

	UPROPERTY(EditAnywhere)
	class USphereComponent* EnemyDetection;

	UPROPERTY(EditAnywhere)
	class UArrowComponent* tempSpawnLocationForProjectile;

	// Archer Tower Information
	UPROPERTY(EditAnywhere, Category = "Archer Tower Information")
	float fireRate = 1.0f;

	UPROPERTY()
	FTimerHandle AttackingEnemyTimer;

	UPROPERTY()
	AEnemyAI* EnemyDetected;

	UPROPERTY()
	TArray<AEnemyAI*> EnemiesDetected;

	UPROPERTY()
	bool isShooting;

	// Basic Archer Tower Function

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);

	UFUNCTION()
	void AttackEnemy();
};
