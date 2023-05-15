// Fill out your copyright notice in the Description page of Project Settings.


#include "VictoryWidget.h"
#include "Kismet/GameplayStatics.h"

void UVictoryWidget::NativeConstruct()
{
	ExitToMainMenuButton->OnClicked.AddDynamic(this, &UVictoryWidget::ExitToMainMenuButtonPressed);
	ExitGameButton->OnClicked.AddDynamic(this, &UVictoryWidget::ExitGameButtonPressed);
}

void UVictoryWidget::ExitToMainMenuButtonPressed()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("MainMenu"));
}

void UVictoryWidget::ExitGameButtonPressed()
{
	APlayerController* PlayerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, false);
}