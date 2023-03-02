// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerControlled.h"
#include "BuildingWidget.generated.h"

UCLASS()
class TOBEKING_API UBuildingWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY()
	UDataTable* BuildingTypeDT;

	// Building Type Buttons
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* LumberMillButton;
	bool LumberMillPressed = false;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* MineButton;
	bool MinePressed = false;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* FarmButton;
	bool FarmPressed = false;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* MarketButton;
	bool MarketPressed = false;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* ArcherTowerButton;
	bool ArcherTowerPressed = false;

	/// Building Type Resource Information
	// Lumber Mill Building Amounts
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* LMWoodAmount;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* LMWoodTitle;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* LMStoneAmount;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* LMStoneTitle;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* LMWheatAmount;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* LMWheatTitle;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* LMCoinsAmount;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* LMCoinsTitle;

	// Mine Building Amounts
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* MiWoodAmount;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* MiWoodTitle;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* MiStoneAmount;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* MiStoneTitle;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* MiWheatAmount;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* MiWheatTitle;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* MiCoinsAmount;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* MiCoinsTitle;

	// Farm Building Amounts
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* FWoodAmount;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* FWoodTitle;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* FStoneAmount;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* FStoneTitle;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* FWheatAmount;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* FWheatTitle;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* FCoinsAmount;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* FCoinsTitle;

	// Market Building Amounts
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* MaWoodAmount;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* MaWoodTitle;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* MaStoneAmount;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* MaStoneTitle;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* MaWheatAmount;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* MaWheatTitle;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* MaCoinsAmount;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* MaCoinsTitle;

	// Tower Archer Building Amounts
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* ATWoodAmount;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* ATWoodTitle;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* ATStoneAmount;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* ATStoneTitle;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* ATWheatAmount;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* ATWheatTitle;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* ATCoinsAmount;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* ATCoinsTitle;

	UPROPERTY()
	APlayerControlled* PlayerReference;

	virtual void NativeConstruct() override;

	TArray<FBuildingTypeStruct> BuildingTypes;

	UFUNCTION()
	void OnClickLumberMillButton();
	UFUNCTION()
	void OnClickMineButton();
	UFUNCTION()
	void OnClickFarmButton();
	UFUNCTION()
	void OnClickMarketButton();
	UFUNCTION()
	void OnClickArcherTowerButton();

	bool FindStruct(FString BuildingTypeString);

	TArray<FName> BuildingTypeDTRowNames;

	FLinearColor ColourGreen = (FColor::Green);
	FLinearColor ColourWhite = (FColor::White);

	UFUNCTION()
	void ResetButtons();

	UFUNCTION()
	void SettingSelectedBuildingType(TEnumAsByte<EBuildingType> BuildingType, FString StructName, bool isEnabled);

	UFUNCTION()
	void Init();

	UFUNCTION()
	void SetBuildingTypeAmount(UTextBlock* WoodAmount, UTextBlock* WoodTitle, UTextBlock* StoneAmount, UTextBlock* StoneTitle, UTextBlock* WheatAmount, UTextBlock* WheatTitle, UTextBlock* CoinsAmount, UTextBlock* CoinsTitle, FResourceList BuildingCost);
};
