// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "Blueprint/UserWidget.h"
#include "VictoryWidget.generated.h"

UCLASS()
class TOBEKING_API UVictoryWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ExitToMainMenuButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ExitGameButton;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void ExitToMainMenuButtonPressed();

	UFUNCTION()
	void ExitGameButtonPressed();
};
