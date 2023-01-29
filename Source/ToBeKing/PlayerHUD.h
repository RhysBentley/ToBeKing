// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

UCLASS()
class TOBEKING_API APlayerHUD : public AHUD
{
	GENERATED_BODY()

public:
	APlayerHUD();

protected:

	virtual void BeginPlay() override;

public:

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UPlayerHUDWidget> HUDReferenceClass;

	UPROPERTY(EditAnywhere)
	class UPlayerHUDWidget* HUDReference;
};
