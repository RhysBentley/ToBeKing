// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingBase.h"
#include "PlayerHUD.h"
#include "ResourcesWidget.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

// Sets default values
ABuildingBase::ABuildingBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Temp - Cube Static Mesh
	Cube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube"));

	// Setting the settings for the static mesh
	Cube->SetStaticMesh(BuildingTypeStruct.StaticMesh);
	Cube->SetWorldScale3D(FVector(0.5f, 0.5f, 0.5f));
	RootComponent = Cube;

	// Setting collision to custom and ignoring the camera
	Cube->SetCollisionProfileName(TEXT("Custom"));
	Cube->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	// Setting default for Building Type
	BuildingTypeByte = EBuildingType::LumberMill;

	// Getting the data table for building types
	ConstructorHelpers::FObjectFinder<UDataTable> BuildingTypeDTAsset(TEXT("DataTable'/Game/DataTables/DT_BuildingTypes.DT_BuildingTypes'"));
	BuildingTypeDT = BuildingTypeDTAsset.Object;
	TArray<FName> BuildingTypeRowNames = BuildingTypeDT->GetRowNames();
	FString ContextString;
	for (FName BuildingTypeRowName : BuildingTypeRowNames)
    {
		FBuildingTypeStruct* TempBuildingType = BuildingTypeDT->FindRow<FBuildingTypeStruct>(BuildingTypeRowName, ContextString);
		if (TempBuildingType->BuildingTypeByte == BuildingTypeByte)
		{
			//BuildingTypeStruct = TempBuildingType;
		}
    }
}

// Called when the game starts or when spawned
void ABuildingBase::BeginPlay()
{
	Super::BeginPlay();

	// Starting the timer for the production
	FTimerHandle ProductionTimerHandle;
	GetWorldTimerManager().SetTimer(ProductionTimerHandle, this, &ABuildingBase::ProductionTimer, BuildingTypeStruct.ProductionDuration, true, BuildingTypeStruct.ProductionDuration);

	// Starting a time for delayed BeginPlay (0.01 seconds)
	FTimerHandle DelayedBeginTimerHandle;
	GetWorldTimerManager().SetTimer(DelayedBeginTimerHandle, this, &ABuildingBase::DelayBeginPlay, 0.01f, false, 0.0f);
}

// Called every frame
void ABuildingBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Used as a delay for setting references of both the 'Player Controlled' and 'HUD'
void ABuildingBase::DelayBeginPlay()
{
	PlayerReference = Cast<APlayerControlled>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	APlayerHUD* HUDClassReference = Cast<APlayerHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	HUDReference = HUDClassReference->HUDReference;
}

// Creating the Resources after a delay
void ABuildingBase::ProductionTimer()
{
	switch (BuildingTypeByte)
	{
	case EBuildingType::LumberMill:
		if (PlayerReference != nullptr && HUDReference != nullptr)
		{
			PlayerReference->ResourceList.Wood += BuildingTypeStruct.ProductionAmount;
			HUDReference->Widget_Resources->SetWoodAmount(PlayerReference->ResourceList.Wood);
		}
		break;

	case EBuildingType::Mine:
		if (PlayerReference != nullptr && HUDReference != nullptr)
		{
			PlayerReference->ResourceList.Stone += BuildingTypeStruct.ProductionAmount;
			HUDReference->Widget_Resources->SetStoneAmount(PlayerReference->ResourceList.Stone);
		}
		break;

	case EBuildingType::Farm:
		if (PlayerReference != nullptr && HUDReference != nullptr)
		{
			PlayerReference->ResourceList.Wheat += BuildingTypeStruct.ProductionAmount;
			HUDReference->Widget_Resources->SetWheatAmount(PlayerReference->ResourceList.Wheat);
		}
		break;

	case EBuildingType::Market:
		if (PlayerReference != nullptr && HUDReference != nullptr)
		{
			PlayerReference->ResourceList.Coins += BuildingTypeStruct.ProductionAmount;
			HUDReference->Widget_Resources->SetCoinsAmount(PlayerReference->ResourceList.Coins);
		}
		break;

	default: break;
	}
}