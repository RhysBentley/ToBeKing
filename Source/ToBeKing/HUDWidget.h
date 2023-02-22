// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

UCLASS()
class TOBEKING_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	// Widget to set Canvas Panel
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UCanvasPanel* CanvasPanel;

	// Widget for Resources
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UResourcesWidget* Widget_Resources;

	// Widget for Building
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UBuildingWidget* Widget_Building;

	// Widget for Building Information
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UBuildingInformationWidget* Widget_BuildingInformation;
};
