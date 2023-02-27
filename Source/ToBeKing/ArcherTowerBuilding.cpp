// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcherTowerBuilding.h"

#include "Components/ArrowComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"

AArcherTowerBuilding::AArcherTowerBuilding()
{
	BuildingTypeByte = EBuildingType::ArcherTower;

	// Setting up the basics for the collision
	EnemyDetection = CreateDefaultSubobject<USphereComponent>(TEXT("Enemy Detection"));
	tempSpawnLocationForProjectile = CreateDefaultSubobject<UArrowComponent>(TEXT("Spawn Arrow Projectile - Temp"));

	EnemyDetection->SetupAttachment(RootComponent);
	tempSpawnLocationForProjectile->SetupAttachment(EnemyDetection);

	EnemyDetection->SetSphereRadius(500.0f);
	EnemyDetection->SetCollisionProfileName(TEXT("Custom"));
	EnemyDetection->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	EnemyDetection->SetCollisionResponseToAllChannels(ECR_Ignore);
	EnemyDetection->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);
	ECollisionChannel ChannelType = ECC_GameTraceChannel1;
	EnemyDetection->SetCollisionObjectType(ChannelType);

	tempSpawnLocationForProjectile->SetRelativeLocation(FVector(0.0f, 0.0f, 70.0f));
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
	if (canSeeEnemy)
	{
		FRotator tempRotation = UKismetMathLibrary::FindLookAtRotation(tempSpawnLocationForProjectile->GetComponentLocation(), EnemyDetected->GetActorLocation());
		tempSpawnLocationForProjectile->SetWorldRotation(tempRotation);
	}
}


void AArcherTowerBuilding::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	EnemyDetected = Cast<AEnemyAI>(OtherActor);
	if (EnemyDetected)
	{
		canSeeEnemy = true;
		GetWorldTimerManager().SetTimer(AttackingEnemyTimer, this, &AArcherTowerBuilding::AttackEnemy, fireRate, true, fireRate);
	}
}

void AArcherTowerBuilding::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	canSeeEnemy = false;
	GetWorld()->GetTimerManager().ClearTimer(AttackingEnemyTimer);
}

void AArcherTowerBuilding::AttackEnemy()
{
	
}
