// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

void UBuildingWidget::NativeConstruct()
{
	// Setting the OnClicked functions
	LumberMillButton->OnClicked.AddDynamic(this, &UBuildingWidget::OnClickLumberMillButton);
	MineButton->OnClicked.AddDynamic(this, &UBuildingWidget::OnClickMineButton);
	FarmButton->OnClicked.AddDynamic(this, &UBuildingWidget::OnClickFarmButton);
	MarketButton->OnClicked.AddDynamic(this, &UBuildingWidget::OnClickMarketButton);
}

void UBuildingWidget::Init()
{
	PlayerReference = Cast<APlayerControlled>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	BuildingTypeDT = PlayerReference->BuildingTypeDT;
	BuildingTypeDTRowNames = BuildingTypeDT->GetRowNames();
	for (FName BuildingTypeRowName : BuildingTypeDTRowNames)
	{
		FString ContextString;
		const FBuildingTypeStruct* TempBuildingType = BuildingTypeDT->FindRow<FBuildingTypeStruct>(BuildingTypeRowName, ContextString);
		switch (TempBuildingType->BuildingTypeByte)
		{
		case EBuildingType::LumberMill:
			SetBuildingTypeAmount(LMWoodAmount, LMWoodTitle, LMStoneAmount, LMStoneTitle, LMWheatAmount, LMWheatTitle, LMCoinsAmount, LMCoinsTitle , TempBuildingType->BuildingCost);
			break;

		case EBuildingType::Mine:
			SetBuildingTypeAmount(MiWoodAmount, MiWoodTitle, MiStoneAmount, MiStoneTitle, MiWheatAmount, MiWheatTitle, MiCoinsAmount, MiCoinsTitle, TempBuildingType->BuildingCost);
			break;

		case EBuildingType::Farm:
			SetBuildingTypeAmount(FWoodAmount, FWoodTitle, FStoneAmount, FStoneTitle, FWheatAmount, FWheatTitle, FCoinsAmount, FCoinsTitle, TempBuildingType->BuildingCost);
			break;

		case EBuildingType::Market:
			SetBuildingTypeAmount(MaWoodAmount, MaWoodTitle, MaStoneAmount, MaStoneTitle, MaWheatAmount, MaWheatTitle, MaCoinsAmount, MaCoinsTitle, TempBuildingType->BuildingCost);
			break;

		default: break;
		}
	}
}

void UBuildingWidget::SetBuildingTypeAmount(UTextBlock* WoodAmount, UTextBlock* WoodTitle, UTextBlock* StoneAmount, UTextBlock* StoneTitle, UTextBlock* WheatAmount, UTextBlock* WheatTitle, UTextBlock* CoinsAmount, UTextBlock* CoinsTitle, FResourceList BuildingCost)
{
	// Setting the Wood building resource indicators
	WoodAmount->SetText(FText::AsNumber(BuildingCost.Wood));
	if (BuildingCost.Wood == 0.0f)
	{
		WoodAmount->SetColorAndOpacity(FLinearColor::Gray);
		WoodTitle->SetColorAndOpacity(FLinearColor::Gray);
	}
	else if (PlayerReference->ResourceList.Wood >= BuildingCost.Wood)
	{
		WoodAmount->SetColorAndOpacity(FLinearColor::Green);
		WoodTitle->SetColorAndOpacity(FLinearColor::Green);
	}
	else
	{
		WoodAmount->SetColorAndOpacity(FLinearColor::Red);
		WoodTitle->SetColorAndOpacity(FLinearColor::Red);
	}

	// Setting the Stone building resource indicators
	StoneAmount->SetText(FText::AsNumber(BuildingCost.Stone));
	if (BuildingCost.Stone == 0.0f)
	{
		StoneAmount->SetColorAndOpacity(FLinearColor::Gray);
		StoneTitle->SetColorAndOpacity(FLinearColor::Gray);
	}
	else if (PlayerReference->ResourceList.Stone >= BuildingCost.Stone)
	{
		StoneAmount->SetColorAndOpacity(FLinearColor::Green);
		StoneTitle->SetColorAndOpacity(FLinearColor::Green);
	}
	else
	{
		StoneAmount->SetColorAndOpacity(FLinearColor::Red);
		StoneTitle->SetColorAndOpacity(FLinearColor::Red);
	}

	// Setting the Wheat building resource indicators
	WheatAmount->SetText(FText::AsNumber(BuildingCost.Wheat));
	if (BuildingCost.Wheat == 0.0f)
	{
		WheatAmount->SetColorAndOpacity(FLinearColor::Gray);
		WheatTitle->SetColorAndOpacity(FLinearColor::Gray);
	}
	else if (PlayerReference->ResourceList.Wheat >= BuildingCost.Wheat)
	{
		WheatAmount->SetColorAndOpacity(FLinearColor::Green);
		WheatTitle->SetColorAndOpacity(FLinearColor::Green);
	}
	else
	{
		WheatAmount->SetColorAndOpacity(FLinearColor::Red);
		WheatTitle->SetColorAndOpacity(FLinearColor::Red);
	}

	// Setting the Coins building resource indicators
	CoinsAmount->SetText(FText::AsNumber(BuildingCost.Coins));
	if (BuildingCost.Coins == 0.0f)
	{
		CoinsAmount->SetColorAndOpacity(FLinearColor::Gray);
		CoinsTitle->SetColorAndOpacity(FLinearColor::Gray);
	}
	else if (PlayerReference->ResourceList.Coins >= BuildingCost.Coins)
	{
		CoinsAmount->SetColorAndOpacity(FLinearColor::Green);
		CoinsTitle->SetColorAndOpacity(FLinearColor::Green);
	}
	else
	{
		CoinsAmount->SetColorAndOpacity(FLinearColor::Red);
		CoinsTitle->SetColorAndOpacity(FLinearColor::Red);
	}
}



void UBuildingWidget::SettingSelectedBuildingType(TEnumAsByte<EBuildingType> BuildingType, FString StructName, bool isEnabled)
{
	LumberMillButton->SetBackgroundColor(ColourWhite);
	MineButton->SetBackgroundColor(ColourWhite);
	FarmButton->SetBackgroundColor(ColourWhite);
	MarketButton->SetBackgroundColor(ColourWhite);
	if (FindStruct(StructName))
	{
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
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("Building Mode is not enabled")));
	}
}

// Function to find and set the struct on the player controlled class
bool UBuildingWidget::FindStruct(FString BuildingTypeString)
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
		return true;
	}
	else
	{
		return false;
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

