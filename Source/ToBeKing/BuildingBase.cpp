// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingBase.h"

#include "BuildingInformationWidget.h"
#include "BuildingWidget.h"
#include "HeadMountedDisplayFunctionLibrary.h"
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
	Root = CreateDefaultSubobject<UBillboardComponent>(TEXT("Root Component"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));

	// Getting the data table for building types and putting them in an array of row names
	ConstructorHelpers::FObjectFinder<UDataTable> BuildingTypeDTAsset(TEXT("DataTable'/Game/DataTables/DT_BuildingTypes.DT_BuildingTypes'"));
	BuildingTypeDT = BuildingTypeDTAsset.Object;
	BuildingTypeDTRowNames = BuildingTypeDT->GetRowNames();

	// Setting the settings for the static mesh
	StaticMesh->SetStaticMesh(BuildingTypeStruct.StaticMesh);
	//StaticMesh->SetWorldScale3D(FVector(0.25f, 0.25f, 0.25f));
	RootComponent = Root;
	StaticMesh->SetupAttachment(RootComponent);
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
	if (BuildingTypeStruct.ProductionDuration != 0.0f)
	{
		GetWorldTimerManager().SetTimer(ProductionTimerHandle, this, &ABuildingBase::ProductionTimer, BuildingTypeStruct.ProductionDuration, true, BuildingTypeStruct.ProductionDuration);
	}

	// Starting a time for delayed BeginPlay (0.01 seconds)
	FTimerHandle DelayedBeginTimerHandle;
	GetWorldTimerManager().SetTimer(DelayedBeginTimerHandle, this, &ABuildingBase::DelayBeginPlay, 0.01f, false, 0.0f);
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
			BuildingTypeStruct.health = BuildingTypeStruct.maxHealth;
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
		if (PlayerReference != nullptr)
		{
			PlayerReference->ResourceList.Wood += BuildingTypeStruct.ProductionAmount;
			PlayerReference->doOncecheckResources = true;
			if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled() == false)
			{
				if (HUDReference != nullptr)
				{
					HUDReference->Widget_Resources->SetWoodAmount(PlayerReference->ResourceList.Wood);
					HUDReference->Widget_Building->Init();
				}
			}
			else
			{
				PlayerReference->ResourcesWidgetReference->SetWoodAmount(PlayerReference->ResourceList.Wood);
				PlayerReference->BuildingWidgetReference->Init();
			}
		}
		break;

	case EBuildingType::Mine:
		if (PlayerReference != nullptr)
		{
			PlayerReference->ResourceList.Stone += BuildingTypeStruct.ProductionAmount;
			PlayerReference->doOncecheckResources = true;
			if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled() == false)
			{
				if (HUDReference != nullptr)
				{
					HUDReference->Widget_Resources->SetStoneAmount(PlayerReference->ResourceList.Stone);
					HUDReference->Widget_Building->Init();
				}
				}
			else
			{
				PlayerReference->ResourcesWidgetReference->SetStoneAmount(PlayerReference->ResourceList.Stone);
				PlayerReference->BuildingWidgetReference->Init();
			}
		}
		break;

	case EBuildingType::Farm:
		if (PlayerReference != nullptr)
		{
			PlayerReference->ResourceList.Wheat += BuildingTypeStruct.ProductionAmount;
			PlayerReference->doOncecheckResources = true;
			if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled() == false)
			{
				if (HUDReference != nullptr)
				{
					HUDReference->Widget_Resources->SetWheatAmount(PlayerReference->ResourceList.Wheat);
					HUDReference->Widget_Building->Init();
				}
			}
			else
			{
				PlayerReference->ResourcesWidgetReference->SetWheatAmount(PlayerReference->ResourceList.Wheat);
				PlayerReference->BuildingWidgetReference->Init();
			}
		}
		break;

	case EBuildingType::Market:
		if (PlayerReference != nullptr)
		{
			PlayerReference->ResourceList.Coins += BuildingTypeStruct.ProductionAmount;
			PlayerReference->doOncecheckResources = true;
			if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled() == false)
			{
				if (HUDReference != nullptr)
				{
					HUDReference->Widget_Resources->SetCoinsAmount(PlayerReference->ResourceList.Coins);
					HUDReference->Widget_Building->Init();
				}
			}
			else
			{
				PlayerReference->ResourcesWidgetReference->SetCoinsAmount(PlayerReference->ResourceList.Coins);
				PlayerReference->BuildingWidgetReference->Init();
			}
		}
		break;

	default: break;
	}
}

void ABuildingBase::DealDamage(float damage)
{
	float tempHealth = BuildingTypeStruct.health;
	tempHealth = tempHealth - damage;
	if (tempHealth <= 0.0f)
	{
		BuildingTypeStruct.health = 0.0f;
		if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled() == false)
		{
			HUDReference->Widget_BuildingInformation->SetBuildingInformation(BuildingTypeStruct);
		}
		Death();
	}
	else
	{
		BuildingTypeStruct.health = tempHealth;
		if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled() == false)
		{
			HUDReference->Widget_BuildingInformation->SetBuildingInformation(BuildingTypeStruct);
		}
	}
}

void ABuildingBase::Death()
{
	GetWorld()->GetTimerManager().ClearTimer(ProductionTimerHandle);
	Destroy();
}
