// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayButton->OnClicked.AddDynamic(this, &UMainMenuWidget::PlayButtonPressed);
	ExitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::ExitButtonPressed);
}

void UMainMenuWidget::PlayButtonPressed()
{
	MainMenuOverlay->SetVisibility(ESlateVisibility::Hidden);
	Widget_Tutorial->SetVisibility(ESlateVisibility::Visible);
}

void UMainMenuWidget::ExitButtonPressed()
{
	APlayerController* PlayerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, false);
}
