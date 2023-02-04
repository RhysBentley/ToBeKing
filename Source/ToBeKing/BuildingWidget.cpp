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


// Function to find and set the struct on the player controlled class
void UBuildingWidget::FindStruct(FString BuildingTypeString)
{
	// Getting the information for the building types
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

void UBuildingWidget::OnClickLumberMillButton()
{
	PlayerReference = Cast<APlayerControlled>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (PlayerReference->isBuildingMode == true)
	{
		if (!LumberMillPressed)
		{
			FindStruct(FString("LumberMill"));
			// Showing selection
			LumberMillButton->SetBackgroundColor(ColourGreen);
			MineButton->SetBackgroundColor(ColourWhite);
			FarmButton->SetBackgroundColor(ColourWhite);
			MarketButton->SetBackgroundColor(ColourWhite);
			PlayerReference->StaticMesh->SetVisibility(true);
			LumberMillPressed = true;
		}
		else
		{
			PlayerReference->SelectedBuildingType = {};
			PlayerReference->StaticMesh->SetVisibility(false);
			ResetButtonColours();
			LumberMillPressed = false;
		}
	}
}

void UBuildingWidget::OnClickMineButton()
{
	PlayerReference = Cast<APlayerControlled>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (PlayerReference->isBuildingMode == true)
	{
		if (!MinePressed)
		{
			FindStruct(FString("Mine"));
			// Showing selection
			LumberMillButton->SetBackgroundColor(ColourWhite);
			MineButton->SetBackgroundColor(ColourGreen);
			FarmButton->SetBackgroundColor(ColourWhite);
			MarketButton->SetBackgroundColor(ColourWhite);
			PlayerReference->StaticMesh->SetVisibility(true);
			MinePressed = true;
		}
		else
		{
			PlayerReference->SelectedBuildingType = {};
			PlayerReference->StaticMesh->SetVisibility(false);
			ResetButtonColours();
			MinePressed = false;
		}
	}
}

void UBuildingWidget::OnClickFarmButton()
{
	PlayerReference = Cast<APlayerControlled>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (PlayerReference->isBuildingMode == true)
	{
		if (!FarmPressed)
		{
			FindStruct(FString("Farm"));
			// Showing selection
			LumberMillButton->SetBackgroundColor(ColourWhite);
			MineButton->SetBackgroundColor(ColourWhite);
			FarmButton->SetBackgroundColor(ColourGreen);
			MarketButton->SetBackgroundColor(ColourWhite);
			PlayerReference->StaticMesh->SetVisibility(true);
			FarmPressed = true;
		}
		else
		{
			PlayerReference->SelectedBuildingType = {};
			PlayerReference->StaticMesh->SetVisibility(false);
			ResetButtonColours();
			FarmPressed = false;
		}
	}
}

void UBuildingWidget::OnClickMarketButton()
{
	PlayerReference = Cast<APlayerControlled>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (PlayerReference->isBuildingMode == true)
	{
		if (!MarketPressed)
		{
			FindStruct(FString("Market"));
			// Showing selection
			LumberMillButton->SetBackgroundColor(ColourWhite);
			MineButton->SetBackgroundColor(ColourWhite);
			FarmButton->SetBackgroundColor(ColourWhite);
			MarketButton->SetBackgroundColor(ColourGreen);
			PlayerReference->StaticMesh->SetVisibility(true);
			MarketPressed = true;
		}
		else
		{
			PlayerReference->SelectedBuildingType = {};
			PlayerReference->StaticMesh->SetVisibility(false);
			ResetButtonColours();
			MarketPressed = false;
		}
	}
}

void UBuildingWidget::ResetButtonColours()
{
	LumberMillButton->SetBackgroundColor(ColourWhite);
	LumberMillPressed = false;
	MineButton->SetBackgroundColor(ColourWhite);
	MinePressed = false;
	FarmButton->SetBackgroundColor(ColourWhite);
	FarmPressed = false;
	MarketButton->SetBackgroundColor(ColourWhite);
	MarketPressed = false;
}

