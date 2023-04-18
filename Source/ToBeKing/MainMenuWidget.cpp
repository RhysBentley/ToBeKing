// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayButton->OnClicked.AddDynamic(this, &UMainMenuWidget::PlayButtonPressed);
	ControlsButton->OnClicked.AddDynamic(this, &UMainMenuWidget::ControlsButtonPressed);
	ControlsBackButton->OnClicked.AddDynamic(this, &UMainMenuWidget::ControlsBackButtonPressed);
	SettingsButton->OnClicked.AddDynamic(this, &UMainMenuWidget::SettingsButtonPressed);
	ExitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::ExitButtonPressed);
	ConfirmCheckBox->OnCheckStateChanged.AddDynamic(this, &UMainMenuWidget::ConfirmCheckBoxChangedState);
}

void UMainMenuWidget::PlayButtonPressed()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("Test"));
}

void UMainMenuWidget::ControlsButtonPressed()
{
	MainMenuOverlay->SetVisibility(ESlateVisibility::Hidden);
	ControlsOverlay->SetVisibility(ESlateVisibility::Visible);
}

void UMainMenuWidget::ControlsBackButtonPressed()
{
	MainMenuOverlay->SetVisibility(ESlateVisibility::Visible);
	ControlsOverlay->SetVisibility(ESlateVisibility::Hidden);
}


void UMainMenuWidget::SettingsButtonPressed()
{
	
}

void UMainMenuWidget::ExitButtonPressed()
{
	APlayerController* PlayerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, false);
}

void UMainMenuWidget::ConfirmCheckBoxChangedState(bool Checked)
{
	if (Checked)
	{
		PlayButton->SetIsEnabled(true);
	}
	else
	{
		PlayButton->SetIsEnabled(false);
	}
}
