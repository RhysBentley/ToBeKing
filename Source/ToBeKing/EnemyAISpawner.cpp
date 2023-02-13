// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAISpawner.h"
#include "PlayerControlled.h"
#include "Components/BillboardComponent.h"
#include "Engine/TargetPoint.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

// Sets default values
AEnemyAISpawner::AEnemyAISpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Getting the reference to the enemy AI class
	BillboardComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("Root"));
	ConstructorHelpers::FClassFinder<AEnemyAI> EnemyToSpawnAsset(TEXT("Class'/Script/ToBeKing.EnemyAI'"));
	EnemyToSpawn = EnemyToSpawnAsset.Class;

	// Setting up the component
	RootComponent = BillboardComponent;
}

// Called when the game starts or when spawned
void AEnemyAISpawner::BeginPlay()
{
	Super::BeginPlay();

	// Starting a time for delayed BeginPlay (0.01 seconds)
	FTimerHandle DelayedBeginTimerHandle;
	GetWorldTimerManager().SetTimer(DelayedBeginTimerHandle, this, &AEnemyAISpawner::DelayBeginPlay, 0.01f, false, 0.0f);
}

// Called every frame
void AEnemyAISpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyAISpawner::DelayBeginPlay()
{
	PlayerReference = Cast<APlayerControlled>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	CurrentWave = PlayerReference->EnemySpawningWaves[PlayerReference->WaveNumber - 1];
	GetWorldTimerManager().SetTimer(WaveTimerHandle, this, &AEnemyAISpawner::BeginEnemyWave, CurrentWave.spawningInterval, true, CurrentWave.gracePeriod);
}

void AEnemyAISpawner::BeginEnemyWave()
{
	// Gets a random point from the array and spawns an enemy at that point
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("Wave %d"), PlayerReference->WaveNumber));
	ATargetPoint* spawnLocation = spawnLocations[FMath::RandRange(0, (spawnLocations.Num() - 1))];
	FTransform Transform = spawnLocation->GetTransform();
	FActorSpawnParameters SpawnInfo;
	AEnemyAI* NewActor = GetWorld()->SpawnActor<AEnemyAI>(EnemyToSpawn, Transform.GetLocation(), Transform.Rotator(), SpawnInfo);
	//NewActor->EnemyAISpawner = this;
	totalSpawned++;
	if (totalSpawned == CurrentWave.spawnAmount)
	{
		spawningWaveEnded = true;
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("All spawned")));

		// Temp - Force the next wave		
		TotalofEnemyDeaths = CurrentWave.spawnAmount;
		EndOfEnemies();
	}
}

void AEnemyAISpawner::EndOfEnemies()
{
	if (spawningWaveEnded && TotalofEnemyDeaths == CurrentWave.spawnAmount)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("Next wave is starting")));
		PlayerReference->WaveNumber++;
		GetWorldTimerManager().ClearTimer(WaveTimerHandle);
		totalSpawned = 0;
		if (PlayerReference->EnemySpawningWaves.IsValidIndex(PlayerReference->WaveNumber - 1))
		{
			CurrentWave = PlayerReference->EnemySpawningWaves[PlayerReference->WaveNumber - 1];
			GetWorldTimerManager().SetTimer(WaveTimerHandle, this, &AEnemyAISpawner::BeginEnemyWave, CurrentWave.spawningInterval, true, CurrentWave.gracePeriod);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("End of the Spawning Enemies")));
		}
	}
}

