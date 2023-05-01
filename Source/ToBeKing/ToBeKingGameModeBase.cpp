// Copyright Epic Games, Inc. All Rights Reserved.


#include "ToBeKingGameModeBase.h"

//#include "HeadMountedDisplayFunctionLibrary.h"
#include "GameFramework/HUD.h"

AToBeKingGameModeBase::AToBeKingGameModeBase()
{
	// Setting the Pawn Class 'PlayerControlled' to the 'Default Pawn Class'
	static ConstructorHelpers::FClassFinder<APawn>PlayerControlledClass(TEXT("/Script/CoreUObject.Class'/Script/ToBeKing.PlayerControlled'"));
	DefaultPawnClass = PlayerControlledClass.Class;

	// Setting the Pawn Class 'PlayerHUD' to the 'HUD Class'
	static ConstructorHelpers::FClassFinder<AHUD>PlayerHUDClass(TEXT("/Script/CoreUObject.Class'/Script/ToBeKing.PlayerHUD'"));
	HUDClass = PlayerHUDClass.Class;
}
