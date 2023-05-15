// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UTutorialWidget::NativeConstruct()
{
	PlayButton->OnClicked.AddDynamic(this, &UTutorialWidget::OnClickPlayButton);
}


void UTutorialWidget::OnClickPlayButton()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("Test"));
}
