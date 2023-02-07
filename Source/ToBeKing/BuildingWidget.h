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

	void FindStruct(FString BuildingTypeString);

	TArray<FName> BuildingTypeDTRowNames;

	FLinearColor ColourGreen = (FColor::Green);
	FLinearColor ColourWhite = (FColor::White);

	UFUNCTION()
	void ResetButtons();

	UFUNCTION()
	void SettingSelectedBuildingType(TEnumAsByte<EBuildingType> BuildingType, FString StructName, bool isEnabled);
};
