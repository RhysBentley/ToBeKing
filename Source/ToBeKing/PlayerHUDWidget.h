// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDWidget.generated.h"

UCLASS()
class TOBEKING_API UPlayerHUDWidget : public UUserWidget
{

	GENERATED_BODY()

public:

	// Update Wood total
	void SetWoodAmount(float Total);

	// Update Stone total
	void SetStoneAmount(float Total);

	// Update Wheat total
	void SetWheatAmount(float Total);

	// Update Coins total
	void SetCoinsAmount(float Total);

	// Widget to show the total of Wood
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* WoodAmount;

	// Widget to show the total of Stone
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* StoneAmount;

	// Widget to show the total of Wheat
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* WheatAmount;

	// Widget to show the total of Coins
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* CoinsAmount;

};
