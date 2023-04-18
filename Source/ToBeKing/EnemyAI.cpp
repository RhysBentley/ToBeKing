// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAI.h"
#include "AIController.h"
#include "BuildingBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemyAI::AEnemyAI()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));

	// Setting the AI controller
	ConstructorHelpers::FClassFinder<AAIController> AIControllerAsset(TEXT("Class'/Script/ToBeKing.EnemyAIController'"));
	AIControllerClass = AIControllerAsset.Class;
	ConstructorHelpers::FClassFinder<ABuildingBase> BuildingBaseAsset(TEXT("Class'/Script/ToBeKing.BuildingBase'"));
	BuildingBase = BuildingBaseAsset.Class;

	// Setting up attachments
	Collision->SetupAttachment(RootComponent);

	// Setting up the collision of the static mesh
	Collision->SetBoxExtent(FVector(65.0f, 65.0f, 65.0f));
	Collision->SetCollisionProfileName(TEXT("Custom"));
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Collision->SetCollisionResponseToAllChannels(ECR_Ignore);
	Collision->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	Collision->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Overlap);

	// Setting settings
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AEnemyAI::BeginPlay()
{
	Super::BeginPlay();

	Collision->OnComponentBeginOverlap.AddDynamic(this, &AEnemyAI::OnOverlapBegin);
	Collision->OnComponentEndOverlap.AddDynamic(this, &AEnemyAI::OnOverlapEnd);

	EnemyAIController = Cast<AEnemyAIController>(GetController());

	health = maxHealth;

	// Debug to prevent the AI from moving
	if (canMove)
	{
		MoveToClosestBuilding();
	}
}

// Called every frame
void AEnemyAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Checking when moving to see if the building has gone
	if (isMovingToBuilding)
	{
		if (!IsValid(ClosestActor))
		{
			MoveToClosestBuilding();
		}
	}
}

// Function to tell the AI to move to a location
void AEnemyAI::MoveToClosestBuilding()
{
	isMovingToBuilding = true;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), BuildingBase, FoundActors);
	float LowestDistance = 0.0f;
	for (AActor* FoundActor : FoundActors)
	{
		float TempDistance = GetDistanceTo(FoundActor);
		if (TempDistance < LowestDistance || LowestDistance == 0.0f)
		{
			LowestDistance = TempDistance;
			ClosestActor = FoundActor;
		}
	}
	if (ClosestActor != nullptr)
	{
		EnemyAIController->MoveToActor(ClosestActor, 25.0f);
	}
}

// Attacking Functions
void AEnemyAI::AttackBuilding()
{
	if (IsValid(collidedBuilding))
	{
		collidedBuilding->DealDamage(Damage);
	}
	else
	{
		MoveToClosestBuilding();
	}
}

// Collision Functions
void AEnemyAI::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	collidedBuilding = Cast<ABuildingBase>(OtherActor);
	if (collidedBuilding)
	{
		isMovingToBuilding = false;
		GetWorldTimerManager().SetTimer(AttackBuildingTimerHandle, this, &AEnemyAI::AttackBuilding, 1.0f, true, 0.0f);
	}
}

void AEnemyAI::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	GetWorld()->GetTimerManager().ClearTimer(AttackBuildingTimerHandle);
	MoveToClosestBuilding();
}

void AEnemyAI::DealDamage(float damage)
{
	float tempHealth = health;
	tempHealth = tempHealth - damage;
	if (tempHealth <= 0.0f)
	{
		Death();
	}
	else
	{
		health = tempHealth;
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("Health: %f"), health));
	}
}

void AEnemyAI::Death()
{
	Destroy();
}

