// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingInformationWidget.h"
#include "PlayerControlled.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UBuildingInformationWidget::NativeConstruct()
{
	// Binding the button to the function
	BuildingDestruction->OnClicked.AddDynamic(this, &UBuildingInformationWidget::DestroyBuilding);
}


void UBuildingInformationWidget::SetBuildingInformation(FBuildingTypeStruct BuildingTypeStruct)
{
	BuildingName->SetText(FText::FromString(BuildingTypeStruct.Name));
	BuildingType->SetText(FText::FromString(BuildingTypeStruct.Name));
	//BuildingType->SetText(BuildingTypeStruct.BuildingTypeByte);
	BuildingProductionDuration->SetText(FText::AsNumber(BuildingTypeStruct.ProductionDuration));
	BuildingProductionAmount->SetText(FText::AsNumber(BuildingTypeStruct.ProductionAmount));
	BuildingHealth->SetText(FText::AsNumber(BuildingTypeStruct.health));
	BuildingMaxHealth->SetText(FText::AsNumber(BuildingTypeStruct.maxHealth));
}

void UBuildingInformationWidget::DestroyBuilding()
{
	
}

