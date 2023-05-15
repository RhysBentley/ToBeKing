// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VRHUDWidget.generated.h"

UCLASS()
class TOBEKING_API UVRHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UPauseWidget* Widget_Pause;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UVictoryWidget* Widget_Victory;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UDefeatWidget* Widget_Defeat;
};
