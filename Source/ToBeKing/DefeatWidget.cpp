// Fill out your copyright notice in the Description page of Project Settings.


#include "DefeatWidget.h"
#include "Kismet/GameplayStatics.h"

void UDefeatWidget::NativeConstruct()
{
	ExitToMainMenuButton->OnClicked.AddDynamic(this, &UDefeatWidget::ExitToMainMenuButtonPressed);
	ExitGameButton->OnClicked.AddDynamic(this, &UDefeatWidget::ExitGameButtonPressed);
}

void UDefeatWidget::ExitToMainMenuButtonPressed()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("MainMenu"));
}

void UDefeatWidget::ExitGameButtonPressed()
{
	APlayerController* PlayerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, false);
}