// Fill out your copyright notice in the Description page of Project Settings.


#include "TownHallBuilding.h"

void ATownHallBuilding::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	BuildingTypeByte = EBuildingType::TownHall;
}
