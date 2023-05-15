// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcherTowerBuilding.h"

#include "ArcherTowerProjectile.h"
#include "Components/ArrowComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"

AArcherTowerBuilding::AArcherTowerBuilding()
{
	BuildingTypeByte = EBuildingType::ArcherTower;

	// Setting up the basics for the collision
	EnemyDetection = CreateDefaultSubobject<USphereComponent>(TEXT("Enemy Detection"));
	tempSpawnLocationForProjectile = CreateDefaultSubobject<UArrowComponent>(TEXT("Spawn Arrow Projectile - Temp"));

	RootComponent = Root;
	EnemyDetection->SetupAttachment(Root);
	tempSpawnLocationForProjectile->SetupAttachment(EnemyDetection);

	EnemyDetection->SetSphereRadius(800.0f);
	EnemyDetection->SetCollisionProfileName(TEXT("Custom"));
	EnemyDetection->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	EnemyDetection->SetCollisionResponseToAllChannels(ECR_Ignore);
	EnemyDetection->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);
	ECollisionChannel ChannelType = ECC_GameTraceChannel1;
	EnemyDetection->SetCollisionObjectType(ChannelType);

	tempSpawnLocationForProjectile->SetRelativeLocation(FVector(0.0f, 0.0f, 110.0f));
	tempSpawnLocationForProjectile->SetRelativeScale3D(FVector(2.0f, 2.0f, 2.0f));
	tempSpawnLocationForProjectile->SetHiddenInGame(false);
}

void AArcherTowerBuilding::BeginPlay()
{
	Super::BeginPlay();

	EnemyDetection->OnComponentBeginOverlap.AddDynamic(this, &AArcherTowerBuilding::OnOverlapBegin);
	EnemyDetection->OnComponentEndOverlap.AddDynamic(this, &AArcherTowerBuilding::OnOverlapEnd);
}


void AArcherTowerBuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Setting rotation of the archer to shoot at the enemy
	if (isShooting)
	{
		tempSpawnLocationForProjectile->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(tempSpawnLocationForProjectile->GetComponentLocation(), EnemyDetected->GetActorLocation()));
	}
}


void AArcherTowerBuilding::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (isShooting == true)
	{
		// When the tower is already shooting at an enemy
		AEnemyAI* tempActor = Cast<AEnemyAI>(OtherActor);
		if (IsValid(tempActor))
		{
			EnemiesDetected.Add(tempActor);
		}
	}
	else
	{
		// Start shooting at the enemy
		AEnemyAI* tempActor = Cast<AEnemyAI>(OtherActor);
		if (IsValid(tempActor))
		{
			EnemiesDetected.Add(tempActor);
			EnemyDetected = tempActor;
			GetWorldTimerManager().SetTimer(AttackingEnemyTimer, this, &AArcherTowerBuilding::AttackEnemy, fireRate, true, fireRate);
			isShooting = true;
		}
	}
}

void AArcherTowerBuilding::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	// This will trigger when an enemy dies
	if (isShooting)
	{
		AEnemyAI* tempActor = Cast<AEnemyAI>(OtherActor);
		if (IsValid(tempActor))
		{
			// Seeing if they are in the array
			int foundIndex = EnemiesDetected.Find(tempActor);
			if (foundIndex >= 0)
			{
				// Removing them and setting the targeted enemy
				EnemiesDetected.RemoveAt(foundIndex);
				if (EnemiesDetected.Num() > 0)
				{
					EnemyDetected = EnemiesDetected[0];
				}
				// Else stop shooting since there are no more enemies
				else
				{
					GetWorldTimerManager().ClearTimer(AttackingEnemyTimer);
					isShooting = false;
				}
			}
		}
	}
}

void AArcherTowerBuilding::AttackEnemy()
{
	FTransform Transform = tempSpawnLocationForProjectile->GetComponentTransform();
	FActorSpawnParameters SpawnParams;
	GetWorld()->SpawnActor<AArcherTowerProjectile>(AArcherTowerProjectile::StaticClass(), Transform.GetLocation(), Transform.Rotator(), SpawnParams);
}
