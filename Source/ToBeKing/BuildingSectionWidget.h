// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BuildingSectionWidget.generated.h"

UCLASS()
class TOBEKING_API UBuildingSectionWidget : public UUserWidget
{
	GENERATED_BODY()
	
	// Building Type Buttons
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button;
	bool ButtonPressed = false;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* ButtonTitle;

	// Building Amounts
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* WoodAmount;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* WoodTitle;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* StoneAmount;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* StoneTitle;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* WheatAmount;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* WheatTitle;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* CoinsAmount;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* CoinsTitle;
};
