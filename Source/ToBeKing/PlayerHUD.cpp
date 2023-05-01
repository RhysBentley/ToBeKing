// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"

#include "HeadMountedDisplayFunctionLibrary.h"
#include "HUDWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

APlayerHUD::APlayerHUD()
{
	static ConstructorHelpers::FClassFinder<UHUDWidget> HUDReferenceAsset(TEXT("/Game/Widgets/Widget_HUD"));
	HUDReferenceClass = HUDReferenceAsset.Class;
}

// Creating the base widget for the HUD
void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();

	// Check to see if we are using a VR Headset
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled() == false)
	{
		if (HUDReferenceClass != nullptr)
		{
			HUDReference = CreateWidget<UHUDWidget>(GetWorld(), HUDReferenceClass);

			if (HUDReference)
			{
				HUDReference->AddToPlayerScreen();
			}
		}
	}
}


