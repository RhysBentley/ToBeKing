// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuGamemode.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

AMainMenuGamemode::AMainMenuGamemode()
{
	static ConstructorHelpers::FClassFinder<UMainMenuWidget> HUDReferenceAsset(TEXT("/Game/Widgets/Widget_MainMenu"));
	HUDReferenceClass = HUDReferenceAsset.Class;
}


// Creating the base widget for the HUD
void AMainMenuGamemode::BeginPlay()
{
	Super::BeginPlay();

	if (HUDReferenceClass != nullptr)
	{
		HUDReference = CreateWidget<UMainMenuWidget>(GetWorld(), HUDReferenceClass);

		if (HUDReference)
		{
			HUDReference->AddToPlayerScreen();
		}
	}

	// Showing the cursor
	APlayerController* PlayerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	PlayerController->SetShowMouseCursor(true);
	FInputModeGameAndUI InputMode;
	PlayerController->SetInputMode(InputMode);
}