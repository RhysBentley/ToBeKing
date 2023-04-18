// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingBase.h"
#include "TownHallBuilding.generated.h"

UCLASS()
class TOBEKING_API ATownHallBuilding : public ABuildingBase
{
	GENERATED_BODY()

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void Death() override;
};
