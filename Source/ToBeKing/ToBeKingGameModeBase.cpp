// Copyright Epic Games, Inc. All Rights Reserved.


#include "ToBeKingGameModeBase.h"

AToBeKingGameModeBase::AToBeKingGameModeBase()
{
	// Setting the Pawn Class 'PlayerControlled' to the 'Default Pawn Class'
	static ConstructorHelpers::FClassFinder<APawn>PlayerControlledClass(TEXT("/Script/CoreUObject.Class'/Script/ToBeKing.PlayerControlled'"));
	DefaultPawnClass = PlayerControlledClass.Class;
}

