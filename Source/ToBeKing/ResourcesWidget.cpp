// Fill out your copyright notice in the Description page of Project Settings.


#include "ResourcesWidget.h"
#include "Components/TextBlock.h"

void UResourcesWidget::SetWoodAmount(float Total)
{
	if (WoodAmount != nullptr)
	{
		WoodAmount->SetText(FText::AsNumber(Total));
	}
}

void UResourcesWidget::SetStoneAmount(float Total)
{
	if (StoneAmount != nullptr)
	{
		StoneAmount->SetText(FText::AsNumber(Total));
	}
}

void UResourcesWidget::SetWheatAmount(float Total)
{
	if (WheatAmount != nullptr)
	{
		WheatAmount->SetText(FText::AsNumber(Total));
	}
}


void UResourcesWidget::SetCoinsAmount(float Total)
{
	if (CoinsAmount != nullptr)
	{
		CoinsAmount->SetText(FText::AsNumber(Total));
	}
}
