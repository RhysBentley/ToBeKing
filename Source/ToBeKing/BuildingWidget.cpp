// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingWidget.h"
#include "Components/Button.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

void UBuildingWidget::NativeConstruct()
{
	// Setting the OnClicked functions
	LumberMillButton->OnClicked.AddDynamic(this, &UBuildingWidget::OnClickLumberMillButton);
	MineButton->OnClicked.AddDynamic(this, &UBuildingWidget::OnClickMineButton);
	FarmButton->OnClicked.AddDynamic(this, &UBuildingWidget::OnClickFarmButton);
	MarketButton->OnClicked.AddDynamic(this, &UBuildingWidget::OnClickMarketButton);
}

void UBuildingWidget::SettingSelectedBuildingType(TEnumAsByte<EBuildingType> BuildingType, FString StructName, bool isEnabled)
{
	LumberMillButton->SetBackgroundColor(ColourWhite);
	MineButton->SetBackgroundColor(ColourWhite);
	FarmButton->SetBackgroundColor(ColourWhite);
	MarketButton->SetBackgroundColor(ColourWhite);
	FindStruct(StructName);
	PlayerReference->StaticMesh->SetVisibility(true);
	PlayerReference->doOncecheckResources = true;

	switch (BuildingType)
	{
	case EBuildingType::LumberMill:
		if (isEnabled)
		{
			LumberMillButton->SetBackgroundColor(ColourGreen);
			LumberMillPressed = true;
			MinePressed = false;
			FarmPressed = false;
			MarketPressed = false;
		}
		else
		{
			LumberMillPressed = false;
			MinePressed = false;
			FarmPressed = false;
			MarketPressed = false;
			PlayerReference->StaticMesh->SetVisibility(false);
		}
		break;

	case EBuildingType::Mine:
		if (isEnabled)
		{
			MineButton->SetBackgroundColor(ColourGreen);
			LumberMillPressed = false;
			MinePressed = true;
			FarmPressed = false;
			MarketPressed = false;
		}
		else
		{
			LumberMillPressed = false;
			MinePressed = false;
			FarmPressed = false;
			MarketPressed = false;
			PlayerReference->StaticMesh->SetVisibility(false);
		}
		break;

	case EBuildingType::Farm:
		if (isEnabled)
		{
			FarmButton->SetBackgroundColor(ColourGreen);
			LumberMillPressed = false;
			MinePressed = false;
			FarmPressed = true;
			MarketPressed = false;
		}
		else
		{
			LumberMillPressed = false;
			MinePressed = false;
			FarmPressed = false;
			MarketPressed = false;
			PlayerReference->StaticMesh->SetVisibility(false);
		}
		break;

	case EBuildingType::Market:
		if (isEnabled)
		{
			MarketButton->SetBackgroundColor(ColourGreen);
			LumberMillPressed = false;
			MinePressed = false;
			FarmPressed = false;
			MarketPressed = true;
		}
		else
		{
			LumberMillPressed = false;
			MinePressed = false;
			FarmPressed = false;
			MarketPressed = false;
			PlayerReference->StaticMesh->SetVisibility(false);
		}
		break;

	default: break;
	}
}

// Function to find and set the struct on the player controlled class
void UBuildingWidget::FindStruct(FString BuildingTypeString)
{
	// Getting the information for the building types
	PlayerReference = Cast<APlayerControlled>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (PlayerReference && PlayerReference->isBuildingMode == true)
	{
		BuildingTypeDT = PlayerReference->BuildingTypeDT;
		BuildingTypeDTRowNames = BuildingTypeDT->GetRowNames();
		for (const FName BuildingTypeRowName : BuildingTypeDTRowNames)
		{
			FString ContextString;
			const FBuildingTypeStruct* TempBuildingType = BuildingTypeDT->FindRow<FBuildingTypeStruct>(BuildingTypeRowName, ContextString);
			if (TempBuildingType->Name == BuildingTypeString)
			{
				PlayerReference->SelectedBuildingType = *TempBuildingType;
				PlayerReference->StaticMesh->SetStaticMesh(TempBuildingType->StaticMesh);
				PlayerReference->StaticMesh->SetRelativeLocation(FVector(0.0f, 0.0f, TempBuildingType->DistanceFromPivot));
			}
		}
	}
}

void UBuildingWidget::OnClickLumberMillButton()
{
	SettingSelectedBuildingType(EBuildingType::LumberMill, FString("LumberMill"), !LumberMillPressed);
}

void UBuildingWidget::OnClickMineButton()
{
	SettingSelectedBuildingType(EBuildingType::Mine, FString("Mine"), !MinePressed);
}

void UBuildingWidget::OnClickFarmButton()
{
	SettingSelectedBuildingType(EBuildingType::Farm, FString("Farm"), !FarmPressed);
}

void UBuildingWidget::OnClickMarketButton()
{
	SettingSelectedBuildingType(EBuildingType::Market, FString("Market"), !MarketPressed);
}

void UBuildingWidget::ResetButtons()
{
	LumberMillButton->SetBackgroundColor(ColourWhite);
	LumberMillPressed = false;
	MineButton->SetBackgroundColor(ColourWhite);
	MinePressed = false;
	FarmButton->SetBackgroundColor(ColourWhite);
	FarmPressed = false;
	MarketButton->SetBackgroundColor(ColourWhite);
	MarketPressed = false;
	PlayerReference->StaticMesh->SetVisibility(false);
}

