// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDWidget.h"
#include "BuildingWidget.h"

void UHUDWidget::NativeConstruct()
{
	Widget_Building->SetVisibility(ESlateVisibility::Hidden);
}

