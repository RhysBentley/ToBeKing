// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Overlay.h"
#include "Components/CheckBox.h"
#include "MainMenuWidget.generated.h"

UCLASS()
class TOBEKING_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* PlayButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ControlsButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ControlsBackButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* SettingsButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ExitButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UOverlay* MainMenuOverlay;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UOverlay* ControlsOverlay;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCheckBox* ConfirmCheckBox;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void PlayButtonPressed();

	UFUNCTION()
	void ControlsButtonPressed();

	UFUNCTION()
	void ControlsBackButtonPressed();

	UFUNCTION()
	void SettingsButtonPressed();

	UFUNCTION()
	void ExitButtonPressed();

	UFUNCTION()
	void ConfirmCheckBoxChangedState(bool Checked);
};
