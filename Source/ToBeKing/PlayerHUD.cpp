// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"

#include "PlayerHUDWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

APlayerHUD::APlayerHUD()
{
	static ConstructorHelpers::FClassFinder<UPlayerHUDWidget> HUDReferenceAsset(TEXT("/Game/Widgets/Widget_HUD"));
	HUDReferenceClass = HUDReferenceAsset.Class;
}

// Creating the base widget for the HUD
void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();

	if (HUDReferenceClass != nullptr)
	{
		HUDReference = CreateWidget<UPlayerHUDWidget>(GetWorld(), HUDReferenceClass);
	
		if (HUDReference)
		{
			HUDReference->AddToPlayerScreen();
		}
	}
}


