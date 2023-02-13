// Fill out your copyright notice in the Description page of Project Settings.

// My own classes
#include "PlayerControlled.h"
#include "PlayerHUD.h"
#include "ResourcesWidget.h"
#include "BuildingBase.h"

// Components
#include "BuildingWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/BillboardComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/BoxComponent.h"

#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

// Sets default values
APlayerControlled::APlayerControlled()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Creation of the Component
	RootComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("Root Component"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	StaticMeshRoot = CreateDefaultSubobject<UBillboardComponent>(TEXT("Static Mesh Root"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));

	// Setting up the attachments of the components
	SpringArm->SetupAttachment(RootComponent);
	Camera->SetupAttachment(SpringArm);
	StaticMeshRoot->SetupAttachment(RootComponent);
	StaticMesh->SetupAttachment(StaticMeshRoot);
	Collision->SetupAttachment(StaticMesh);

	// Setting settings for both the spring arm and static mesh building
	SpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 20.0f), FRotator(-60.0f, 0.0f, 0.0f));
	SpringArm->TargetArmLength = 1000.0f;

	StaticMesh->SetWorldScale3D(FVector (1.5f, 1.5f, 1.5f));
	StaticMesh->SetCollisionProfileName(TEXT("Custom"));
	StaticMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);

	// Setting up the collision of the static mesh
	Collision->SetBoxExtent(FVector (65.0f, 65.0f, 65.0f));
	Collision->SetCollisionProfileName(TEXT("Custom"));
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Collision->SetCollisionResponseToAllChannels(ECR_Ignore);
	Collision->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);

	// Setting the player to possess this to control this class
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void APlayerControlled::BeginPlay()
{
	Super::BeginPlay();

	// Starting a time for delayed BeginPlay (0.01 seconds)
	FTimerHandle DelayedBeginTimerHandle;
	GetWorldTimerManager().SetTimer(DelayedBeginTimerHandle, this, &APlayerControlled::DelayBeginPlay, 0.01f, false, 0.0f);

	// Setting Player Controller
	PlayerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	PlayerController->SetShowMouseCursor(true);
	FInputModeGameAndUI InputMode;
	PlayerController->SetInputMode(InputMode);

	Collision->OnComponentBeginOverlap.AddDynamic(this, &APlayerControlled::OnOverlapBegin);
	Collision->OnComponentEndOverlap.AddDynamic(this, &APlayerControlled::OnOverlapEnd);
}

// Called every frame
void APlayerControlled::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Setting the camera to go over the landscape
	if (!CurrentVelocity.IsZero())
	{
		FVector NewLocation = GetActorLocation() + (CurrentVelocity * DeltaTime);
		SetActorLocation(NewLocation);

		// Setting the camera to follow the landscape using a line trace to stay a certain distance from the landscape
		FHitResult OutHit;
		FVector Start = RootComponent->GetComponentLocation();
		FVector DownVector = RootComponent->GetUpVector() * -1.0;
		FVector End = ((DownVector * 4000.0f) + Start);

		FCollisionQueryParams CollisionParams;
		FCollisionObjectQueryParams ObjectParams;
		ObjectParams.AddObjectTypesToQuery(ECC_WorldStatic);

		if (GetWorld()->LineTraceSingleByObjectType(OutHit, Start, End, ObjectParams, CollisionParams))
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

	// Setting the location of the static mesh to the selected building type
	if (isBuildingMode && SelectedBuildingType.Name != "" && StaticMesh->GetVisibleFlag())
	{
		ETraceTypeQuery TraceChannel = TraceTypeQuery1;
		PlayerController->GetHitResultUnderCursorByChannel(TraceChannel, true, InteractHitResult);
		if (InteractHitResult.Distance != 0.0f)
		{
			FVector NewActorLocation = InteractHitResult.Location;
			if (gridEnabled)
			{
				int NewX = NewActorLocation.X;
				int NewY = NewActorLocation.Y;
				NewX = NewX - (NewX % gridSize);
				NewY = NewY - (NewY % gridSize);
				NewActorLocation = FVector(NewX, NewY, InteractHitResult.Location.Z);
			}
			StaticMeshRoot->SetWorldLocation(NewActorLocation);

			// Checking to see if there is enough resources and any overlapping actors
			if (doOncecheckResources)
			{
				Collision->GetOverlappingActors(OverlappingActors, ClassFilter);
				if (OverlappingActors.Num() > 0)
				{
					canBePlaced = false;
				}
				else
				{
					canBePlaced = true;
				}
				if (checkResources(SelectedBuildingType.BuildingCost))
				{
					hasEnoughResources = true;
				}
				else
				{
					hasEnoughResources = false;
				}
				VariableBaseSetMaterial();
				doOncecheckResources = false;
			}
		}
		else
		{
			StaticMesh->SetVisibility(false);
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
	PlayerInputComponent->BindAction("BuildingMode", IE_Pressed, this, &APlayerControlled::BuildingMode);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerControlled::Interact);
}

// Used as a delay for setting references of both the 'Player Controlled' and 'HUD'
void APlayerControlled::DelayBeginPlay()
{
	APlayerHUD* HUDClassReference = Cast<APlayerHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	HUDReference = HUDClassReference->HUDReference;
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

//Change the speed of the camera
void APlayerControlled::StartFasterMovement()
{
	Speed = 1200.0f;
}

void APlayerControlled::StopFasterMovement()
{
	Speed = 600.0f;
}

// Enabling and disabling Building Mode
void APlayerControlled::BuildingMode()
{
	// Turning ON Building Mode
	if (!isBuildingMode)
	{
		isBuildingMode = true;
	}
	// Turning OFF Building Mode
	else
	{
		HUDReference->Widget_Building->ResetButtons();
		isBuildingMode = false;
	}
}

// Used to check if there is enough resources
bool APlayerControlled::checkResources(FResourceList CostInput)
{
	TempResourceList = ResourceList;
	TempResourceList.Wood = TempResourceList.Wood - CostInput.Wood;
	TempResourceList.Stone = TempResourceList.Stone - CostInput.Stone;
	TempResourceList.Wheat = TempResourceList.Wheat - CostInput.Wheat;
	TempResourceList.Coins = TempResourceList.Coins - CostInput.Coins;
	if (TempResourceList.Wood < 0.0f || TempResourceList.Stone < 0.0f || TempResourceList.Wheat < 0.0f || TempResourceList.Coins < 0.0f)
	{
		return false;
	}
	else
	{
		return true;
	}
}

// Setting the material based on several variables
void APlayerControlled::VariableBaseSetMaterial()
{
	if (hasEnoughResources && canBePlaced)
	{
		StaticMesh->SetMaterial(0, GreenMat);
	}
	else if (!hasEnoughResources && canBePlaced)
	{
		StaticMesh->SetMaterial(0, YellowMat);
	}
	else
	{
		StaticMesh->SetMaterial(0, RedMat);
	}
}


// Interact - Left Mouse Button
void APlayerControlled::Interact()
{
	if (isBuildingMode)
	{
		if (SelectedBuildingType.Name != ("") && StaticMesh->GetMaterial(0) != RedMat)
		{
			if (checkResources(SelectedBuildingType.BuildingCost))
			{
				ResourceList = TempResourceList;
				HUDReference->Widget_Resources->SetWoodAmount(ResourceList.Wood);
				HUDReference->Widget_Resources->SetStoneAmount(ResourceList.Stone);
				HUDReference->Widget_Resources->SetWheatAmount(ResourceList.Wheat);
				HUDReference->Widget_Resources->SetCoinsAmount(ResourceList.Coins);
				doOncecheckResources = true;
				FTransform Transform = StaticMesh->GetComponentTransform();
				FActorSpawnParameters SpawnInfo;
				ABuildingBase* NewActor = GetWorld()->SpawnActor<ABuildingBase>(ABuildingBase::StaticClass(), Transform.GetLocation(), Transform.Rotator(), SpawnInfo);
				NewActor->BuildingTypeStruct = SelectedBuildingType;
				NewActor->StaticMesh->SetStaticMesh(SelectedBuildingType.StaticMesh);
			}
			else
			{
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("Not enough resources")));
				}
			}
		}
		else
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("Cant place")));
			}
		}
	}
}

// Detecting any overlapping actors on the static mesh
void APlayerControlled::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<ABuildingBase>(OtherActor))
	{
		doOncecheckResources = true;
	}
}

void APlayerControlled::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	if (Cast<ABuildingBase>(OtherActor))
	{
		doOncecheckResources = true;
	}
}
