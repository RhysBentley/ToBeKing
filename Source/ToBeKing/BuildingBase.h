// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerControlled.h"
#include "HUDWidget.h"
#include "Engine/DataTable.h"
#include "BuildingBase.generated.h"

UENUM(BlueprintType)
enum EBuildingType
{
	LumberMill UMETA(DisplayName = "Lumber Mill"),
	Mine UMETA(DisplayName = "Mine"),
	Farm UMETA(DisplayName = "Farm"),
	Market UMETA(DisplayName = "Market")
};

USTRUCT(BlueprintType)
struct FBuildingTypeStruct : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Building Information")
	FText Name;

	UPROPERTY(EditAnywhere, Category = "Building Information")
	UStaticMesh* StaticMesh;

	UPROPERTY(EditAnywhere, Category = "Building Information")
	TEnumAsByte<EBuildingType> BuildingTypeByte;

	UPROPERTY(EditAnywhere, Category = "Building Information")
	float ProductionDuration = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Building Information")
	float ProductionAmount = 10.0f;
};

UCLASS()
class TOBEKING_API ABuildingBase : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Cube;

	// References
	UPROPERTY()
	APlayerControlled* PlayerReference;
	UPROPERTY()
	UHUDWidget* HUDReference;

	UPROPERTY(EditAnywhere, Category = "Building Information")
	TEnumAsByte<EBuildingType> BuildingTypeByte;

	UPROPERTY(EditAnywhere, Category = "Building Information")
	FBuildingTypeStruct BuildingTypeStruct;

	UPROPERTY(VisibleAnywhere, Category = "Building Information")
	UDataTable* BuildingTypeDT;

	UPROPERTY()
	TArray<FName> BuildingTypeDTRowNames;

public:	
	// Sets default values for this actor's properties
	ABuildingBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Production of the Building
	void ProductionTimer();

	// Delayed Begin Play of 0.01 Seconds
	void DelayBeginPlay();
};