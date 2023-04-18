// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenuWidget.h"
#include "MainMenuGamemode.generated.h"

UCLASS()
class TOBEKING_API AMainMenuGamemode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMainMenuGamemode();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UMainMenuWidget> HUDReferenceClass;

	UPROPERTY(EditAnywhere)
	class UMainMenuWidget* HUDReference;
};
