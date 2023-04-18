// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HUDWidget.h"
#include "PlayerControlled.h"
#include "Components/BillboardComponent.h"
#include "BuildingBase.generated.h"

UCLASS()
class TOBEKING_API ABuildingBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABuildingBase();

	// Components
	UPROPERTY(EditAnywhere)
	UBillboardComponent* Root;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* Collision;

	// Building Information
	UPROPERTY(EditAnywhere, Category = "Building Information")
	TEnumAsByte<EBuildingType> BuildingTypeByte;

	UPROPERTY(EditAnywhere, Category = "Building Information")
	FBuildingTypeStruct BuildingTypeStruct;

	UPROPERTY(VisibleAnywhere, Category = "Building Information")
	UDataTable* BuildingTypeDT;

	UPROPERTY()
	FTimerHandle ProductionTimerHandle;

private:

	UPROPERTY()
	TArray<FName> BuildingTypeDTRowNames;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	// References
	UPROPERTY()
	APlayerControlled* PlayerReference;
	UPROPERTY()
	UHUDWidget* HUDReference;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Production of the Building
	void ProductionTimer();

	// Delayed Begin Play of 0.01 Seconds
	void DelayBeginPlay();

	void DealDamage(float damage);

	// Death function
	UFUNCTION()
	virtual void Death();
};