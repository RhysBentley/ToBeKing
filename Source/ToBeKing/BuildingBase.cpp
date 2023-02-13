// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingBase.h"
#include "PlayerHUD.h"
#include "ResourcesWidget.h"
#include "PlayerControlled.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

// Sets default values
ABuildingBase::ABuildingBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Temp - StaticMesh Static Mesh
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));

	// Getting the data table for building types and putting them in an array of row names
	ConstructorHelpers::FObjectFinder<UDataTable> BuildingTypeDTAsset(TEXT("DataTable'/Game/DataTables/DT_BuildingTypes.DT_BuildingTypes'"));
	BuildingTypeDT = BuildingTypeDTAsset.Object;
	BuildingTypeDTRowNames = BuildingTypeDT->GetRowNames();

	// Setting the settings for the static mesh
	StaticMesh->SetStaticMesh(BuildingTypeStruct.StaticMesh);
	StaticMesh->SetWorldScale3D(FVector(1.5f, 1.5f, 1.5f));
	RootComponent = StaticMesh;
	Collision->SetupAttachment(RootComponent);

	// Setting collision to custom and ignoring the camera
	StaticMesh->SetCollisionProfileName(TEXT("Custom"));
	ECollisionChannel ChannelType = ECC_GameTraceChannel1;
	StaticMesh->SetCollisionObjectType(ChannelType);
	StaticMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	StaticMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	// Setting collision settings
	Collision->SetBoxExtent(FVector(65.0f, 65.0f, 65.0f));
}

// Called when the game starts or when spawned
void ABuildingBase::BeginPlay()
{
	Super::BeginPlay();

	// Starting the timer for the production
	GetWorldTimerManager().SetTimer(ProductionTimerHandle, this, &ABuildingBase::ProductionTimer, BuildingTypeStruct.ProductionDuration, true, BuildingTypeStruct.ProductionDuration);

	// Starting a time for delayed BeginPlay (0.01 seconds)
	FTimerHandle DelayedBeginTimerHandle;
	GetWorldTimerManager().SetTimer(DelayedBeginTimerHandle, this, &ABuildingBase::DelayBeginPlay, 0.01f, false, 0.0f);

	BuildingTypeStruct.health = BuildingTypeStruct.maxHealth;
}

// Called every frame
void ABuildingBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABuildingBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// Setting the structure variable of the type of building
	FString ContextString;
	for (FName BuildingTypeRowName : BuildingTypeDTRowNames)
	{
		FBuildingTypeStruct* TempBuildingType = BuildingTypeDT->FindRow<FBuildingTypeStruct>(BuildingTypeRowName, ContextString);
		if (TempBuildingType->BuildingTypeByte == BuildingTypeByte)
		{
			BuildingTypeStruct = *TempBuildingType;
			StaticMesh->SetStaticMesh(BuildingTypeStruct.StaticMesh);
		}
	}
}


// Used as a delay for setting references of both the 'Player Controlled' and 'HUD'
void ABuildingBase::DelayBeginPlay()
{
	PlayerReference = Cast<APlayerControlled>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	APlayerHUD* HUDClassReference = Cast<APlayerHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	HUDReference = HUDClassReference->HUDReference;
}

// Creating the Resources after a delay
void ABuildingBase::ProductionTimer()
{
	switch (BuildingTypeStruct.BuildingTypeByte)
	{
	case EBuildingType::LumberMill:
		if (PlayerReference != nullptr && HUDReference != nullptr)
		{
			PlayerReference->ResourceList.Wood += BuildingTypeStruct.ProductionAmount;
			PlayerReference->doOncecheckResources = true;
			HUDReference->Widget_Resources->SetWoodAmount(PlayerReference->ResourceList.Wood);
		}
		break;

	case EBuildingType::Mine:
		if (PlayerReference != nullptr && HUDReference != nullptr)
		{
			PlayerReference->ResourceList.Stone += BuildingTypeStruct.ProductionAmount;
			PlayerReference->doOncecheckResources = true;
			HUDReference->Widget_Resources->SetStoneAmount(PlayerReference->ResourceList.Stone);
		}
		break;

	case EBuildingType::Farm:
		if (PlayerReference != nullptr && HUDReference != nullptr)
		{
			PlayerReference->ResourceList.Wheat += BuildingTypeStruct.ProductionAmount;
			PlayerReference->doOncecheckResources = true;
			HUDReference->Widget_Resources->SetWheatAmount(PlayerReference->ResourceList.Wheat);
		}
		break;

	case EBuildingType::Market:
		if (PlayerReference != nullptr && HUDReference != nullptr)
		{
			PlayerReference->ResourceList.Coins += BuildingTypeStruct.ProductionAmount;
			PlayerReference->doOncecheckResources = true;
			HUDReference->Widget_Resources->SetCoinsAmount(PlayerReference->ResourceList.Coins);
		}
		break;

	default: break;
	}
}

void ABuildingBase::Death()
{
	GetWorld()->GetTimerManager().ClearTimer(ProductionTimerHandle);
	Destroy();
}
