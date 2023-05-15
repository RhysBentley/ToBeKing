// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UPauseWidget::NativeConstruct()
{
	PlayButton->OnClicked.AddDynamic(this, &UPauseWidget::PlayButtonPressed);
	ExitToMainMenuButton->OnClicked.AddDynamic(this, &UPauseWidget::ExitToMainMenuButtonPressed);
	ExitGameButton->OnClicked.AddDynamic(this, &UPauseWidget::ExitGameButtonPressed);
}

void UPauseWidget::PlayButtonPressed()
{
	SetVisibility(ESlateVisibility::Hidden);
	APlayerController* MyPlayer = Cast<APlayerController>(GEngine->GetFirstLocalPlayerController(GetWorld()));
	if (MyPlayer != NULL)
	{
		MyPlayer->SetPause(false);
	}
}

void UPauseWidget::ExitToMainMenuButtonPressed()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("MainMenu"));
}

void UPauseWidget::ExitGameButtonPressed()
{
	APlayerController* PlayerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, false);
}
