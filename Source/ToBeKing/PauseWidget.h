// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "Blueprint/UserWidget.h"
#include "PauseWidget.generated.h"

UCLASS()
class TOBEKING_API UPauseWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* PlayButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ExitToMainMenuButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ExitGameButton;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void PlayButtonPressed();

	UFUNCTION()
	void ExitToMainMenuButtonPressed();

	UFUNCTION()
	void ExitGameButtonPressed();
};
