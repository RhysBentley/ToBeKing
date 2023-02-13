// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyAIController.h"
#include "BuildingBase.h"
//#include "EnemyAISpawner.h"
#include "EnemyAI.generated.h"

UCLASS()
class TOBEKING_API AEnemyAI : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyAI();

	UPROPERTY()
	AEnemyAIController* EnemyAIController;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY()
	TSubclassOf<ABuildingBase> BuildingBase;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* Collision;

	UPROPERTY()
	FTimerHandle AttackBuildingTimerHandle;

	UPROPERTY()
	ABuildingBase* collidedBuilding;

	UPROPERTY()
	bool isMovingToBuilding;

	UPROPERTY(VisibleAnywhere, Category = "Debug")
	AActor* ClosestActor = nullptr;

	//UPROPERTY(EditAnywhere)
	//AEnemyAISpawner* EnemyAISpawner;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void MoveToClosestBuilding();

	void AttackBuilding();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);
};