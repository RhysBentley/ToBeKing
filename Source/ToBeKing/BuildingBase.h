// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildingBase.generated.h"

UENUM()
enum EBuildingType
{
	LumberMill UMETA(DisplayName = "Lumber Mill"),
	Mine UMETA(DisplayName = "Mine"),
	Farm UMETA(DisplayName = "Farm"),
	Market UMETA(DisplayName = "Market")
};

UCLASS()
class TOBEKING_API ABuildingBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuildingBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
