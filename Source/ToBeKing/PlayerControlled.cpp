// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControlled.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
APlayerControlled::APlayerControlled()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Creation of the Component
	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collision"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	// Setting up the attachments of the components
	RootComponent = CapsuleCollision;
	SpringArm->SetupAttachment(RootComponent);
	Camera->SetupAttachment(SpringArm);

	// Setting settings for both the spring arm and character movement components
	SpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 20.0f), FRotator(-60.0f, 0.0f, 0.0f));
	SpringArm->TargetArmLength = 1000.0f;

	// Setting the player to possess this to control this class
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void APlayerControlled::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerControlled::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!CurrentVelocity.IsZero())
	{
		FVector NewLocation = GetActorLocation() + (CurrentVelocity * DeltaTime);
		SetActorLocation(NewLocation);

		// Setting the camera to follow the landscape using a line trace to stay a certain distance from the surface
		FHitResult OutHit;
		FVector Start = CapsuleCollision->GetComponentLocation();
		FVector DownVector = CapsuleCollision->GetUpVector() * -1.0;
		FVector End = ((DownVector * 4000.0f) + Start);
		FCollisionQueryParams CollisionParams;
		if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams))
		{
			if (OutHit.bBlockingHit)
			{
				if (OutHit.Distance < 100.0f || OutHit.Distance > 100)
				{
					float Result = ((OutHit.Distance - 100.0f) * -1.0f) + 100.0f;
					FVector NewActorLocation = GetActorLocation() + (FVector (0.0f, 0.0f, Result));
					SetActorLocation(NewActorLocation);
				}
			}
		}
	}
}

// Called to bind functionality to input
void APlayerControlled::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveX", this, &APlayerControlled::Move_XAxis);
	PlayerInputComponent->BindAxis("MoveY", this, &APlayerControlled::Move_YAxis);
	PlayerInputComponent->BindAxis("Zoom", this, &APlayerControlled::Zoom);
	PlayerInputComponent->BindAction("FasterMovement", IE_Pressed, this, &APlayerControlled::StartFasterMovement);
	PlayerInputComponent->BindAction("FasterMovement", IE_Released, this, &APlayerControlled::StopFasterMovement);
}

// Move this class on the X Axis
void APlayerControlled::Move_XAxis(float AxisValue)
{
	CurrentVelocity.X = FMath::Clamp(AxisValue, -1.0f, 1.0) * Speed;
}

// Move this class on the Y Axis
void APlayerControlled::Move_YAxis(float AxisValue)
{
	CurrentVelocity.Y = FMath::Clamp(AxisValue, -1.0f, 1.0) * Speed;
}

//Change the speed of the camera
void APlayerControlled::StartFasterMovement()
{
	Speed = 1200.0f;
}

void APlayerControlled::StopFasterMovement()
{
	Speed = 600.0f;
}

// Zoom in and out using the length of the spring arm
void APlayerControlled::Zoom(float AxisValue)
{
	CurrentZoomAmount = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 100.0f;
	if (CurrentZoomAmount != 0.0f)
	{
		float NewSpringArmLength = SpringArm->TargetArmLength + (FMath::Clamp(CurrentZoomAmount, -1.0f, 1.0f) * 20.0f);
		SpringArm->TargetArmLength = FMath::Clamp(NewSpringArmLength, 300.0f, 1400.0f);
	}
}






