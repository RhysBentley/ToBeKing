// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingBase.h"
#include "PlayerHUD.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

// Sets default values
ABuildingBase::ABuildingBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Temp - Cube Static Mesh
	Cube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> CubeAsset(TEXT("StaticMesh'/Engine/EngineMeshes/Cube.Cube'"));

	// Setting the settings for the static mesh
	Cube->SetStaticMesh(CubeAsset.Object);
	Cube->SetWorldScale3D(FVector(0.5f, 0.5f, 0.5f));
	RootComponent = Cube;

	// Setting collision to custom and ignoring the camera
	Cube->SetCollisionProfileName(TEXT("Custom"));
	Cube->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	// Setting default for Building Type
	BuildingTypeByte = EBuildingType::LumberMill;
}

// Called when the game starts or when spawned
void ABuildingBase::BeginPlay()
{
	Super::BeginPlay();

	// Starting the timer for the production
	FTimerHandle ProductionTimerHandle;
	GetWorldTimerManager().SetTimer(ProductionTimerHandle, this, &ABuildingBase::ProductionTimer, ProductionDuration, true, ProductionDuration);

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
			TempResourceList.Wood = PlayerReference->ResourceList.Wood;
			TempResourceList.Wood += ProductionAmount;
			PlayerReference->ResourceList.Wood = TempResourceList.Wood;
			HUDReference->SetWoodAmount(TempResourceList.Wood);
		}
		break;

	case EBuildingType::Mine:
		if (PlayerReference != nullptr && HUDReference != nullptr)
		{
			TempResourceList.Stone = PlayerReference->ResourceList.Stone;
			TempResourceList.Stone += ProductionAmount;
			PlayerReference->ResourceList.Stone = TempResourceList.Stone;
			HUDReference->SetStoneAmount(TempResourceList.Stone);
		}
		break;

	case EBuildingType::Farm:
		if (PlayerReference != nullptr && HUDReference != nullptr)
		{
			TempResourceList.Wheat = PlayerReference->ResourceList.Wheat;
			TempResourceList.Wheat += ProductionAmount;
			PlayerReference->ResourceList.Wheat = TempResourceList.Wheat;
			HUDReference->SetWheatAmount(TempResourceList.Wheat);
		}
		break;

	case EBuildingType::Market:
		if (PlayerReference != nullptr && HUDReference != nullptr)
		{
			TempResourceList.Coins = PlayerReference->ResourceList.Coins;
			TempResourceList.Coins += ProductionAmount;
			PlayerReference->ResourceList.Coins = TempResourceList.Coins;
			HUDReference->SetCoinsAmount(TempResourceList.Coins);
		}
		break;

	default: break;
	}
}