// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerControlled.h"
#include "BuildingInformationWidget.generated.h"

UCLASS()
class TOBEKING_API UBuildingInformationWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* BuildingName;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* BuildingType;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* BuildingProductionDuration;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* BuildingProductionAmount;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* BuildingHealth;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* BuildingMaxHealth;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* BuildingDestruction;

	UFUNCTION()
	void SetBuildingInformation(FBuildingTypeStruct BuildingTypeStruct);

	void DestroyBuilding();

	virtual void NativeConstruct() override;
};
