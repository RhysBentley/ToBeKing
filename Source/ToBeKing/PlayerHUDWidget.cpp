// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUDWidget.h"
#include "Components/TextBlock.h"

void UPlayerHUDWidget::SetWoodAmount(float Total)
{
	if (WoodAmount != nullptr)
	{
		WoodAmount->SetText(FText::AsNumber(Total));
	}
}

void UPlayerHUDWidget::SetStoneAmount(float Total)
{
	if (StoneAmount != nullptr)
	{
		StoneAmount->SetText(FText::AsNumber(Total));
	}
}

void UPlayerHUDWidget::SetWheatAmount(float Total)
{
	if (WheatAmount != nullptr)
	{
		WheatAmount->SetText(FText::AsNumber(Total));
	}
}


void UPlayerHUDWidget::SetCoinsAmount(float Total)
{
	if (CoinsAmount != nullptr)
	{
		CoinsAmount->SetText(FText::AsNumber(Total));
	}
}
