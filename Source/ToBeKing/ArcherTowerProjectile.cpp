// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcherTowerProjectile.h"

#include "EnemyAI.h"
#include "Components/SphereComponent.h"

// Sets default values
AArcherTowerProjectile::AArcherTowerProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	StaticMesh->SetStaticMesh(StaticMeshAsset.Object);

	Collision->SetupAttachment(RootComponent);
	StaticMesh->SetupAttachment(Collision);

	Collision->SetSphereRadius(8.0f);
	StaticMesh->SetRelativeScale3D(FVector (0.1f, 0.1f, 0.1f));

	ProjectileComponent->InitialSpeed = 5000.0f;
	ProjectileComponent->MaxSpeed = 5000.0f;

	Collision->SetCollisionProfileName(TEXT("Custom"));
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Collision->SetCollisionResponseToAllChannels(ECR_Ignore);
	Collision->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);
	ECollisionChannel ChannelType = ECC_GameTraceChannel3;
	Collision->SetCollisionObjectType(ChannelType);
}

void AArcherTowerProjectile::BeginPlay()
{
	Super::BeginPlay();

	Collision->OnComponentBeginOverlap.AddDynamic(this, &AArcherTowerProjectile::OnOverlapBegin);
}

void AArcherTowerProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEnemyAI* tempEnemy = Cast<AEnemyAI>(OtherActor);
	if (IsValid(tempEnemy))
	{
		tempEnemy->DealDamage(damage);
	}
	Destroy();
}


