// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Overlay.h"
#include "TutorialWidget.h"
#include "MainMenuWidget.generated.h"

UCLASS()
class TOBEKING_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* PlayButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ExitButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UOverlay* MainMenuOverlay;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTutorialWidget* Widget_Tutorial;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void PlayButtonPressed();

	UFUNCTION()
	void ExitButtonPressed();
};
