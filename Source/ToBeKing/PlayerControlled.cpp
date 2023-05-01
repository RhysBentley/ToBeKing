// Fill out your copyright notice in the Description page of Project Settings.

// My own classes
#include "PlayerControlled.h"
#include "PlayerHUD.h"
#include "ResourcesWidget.h"
#include "BuildingBase.h"

// Components
#include "BuildingInformationWidget.h"
#include "BuildingWidget.h"
#include "VictoryWidget.h"
#include "DefeatWidget.h"
#include "PauseWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/BillboardComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/BoxComponent.h"

// VR-Based Headers
#include "HeadMountedDisplay.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h"

// Function Libraries
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

// Sets default values
APlayerControlled::APlayerControlled()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Root Component
	RootComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("Root Component"));

	// Building Root
	StaticMeshRoot = CreateDefaultSubobject<UBillboardComponent>(TEXT("Static Mesh Root"));
	StaticMeshRoot->SetupAttachment(RootComponent);

	// Building Mesh
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	StaticMesh->SetupAttachment(StaticMeshRoot);
	StaticMesh->SetWorldScale3D(FVector(0.25f, 0.25f, 0.25f));
	StaticMesh->SetCollisionProfileName(TEXT("Custom"));
	StaticMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);

	// Building Collision
	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	Collision->SetupAttachment(StaticMeshRoot);
	Collision->SetBoxExtent(FVector (65.0f, 65.0f, 65.0f));
	Collision->SetCollisionProfileName(TEXT("Custom"));
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Collision->SetCollisionResponseToAllChannels(ECR_Ignore);
	Collision->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);

	// Creating the Components for either views
	/*if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled() == false)
	{
		CreateBirdEyeComponents();
	}
	else
	{*/
		CreateVRComponents();
	//}

	/// Basic Settings on the Player Pawn
	// Setting the player to possess this to control this class
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Setting Default Resource Values
	ResourceList.Wood = 10;

	/// Setting references
	// Building Type Datatables
	ConstructorHelpers::FObjectFinder<UDataTable> BuildingTypeDTAsset(TEXT("DataTable'/Game/DataTables/DT_BuildingTypes.DT_BuildingTypes'"));
	BuildingTypeDT = BuildingTypeDTAsset.Object;

	// Material Types for the Building System
	ConstructorHelpers::FObjectFinder<UMaterialInstance> GreenMatAsset(TEXT("MaterialInstanceConstant'/Game/Materials/MI_Green.MI_Green'"));
	GreenMat = GreenMatAsset.Object;

	ConstructorHelpers::FObjectFinder<UMaterialInstance> RedMatAsset(TEXT("MaterialInstanceConstant'/Game/Materials/MI_Red.MI_Red'"));
	RedMat = RedMatAsset.Object;

	ConstructorHelpers::FObjectFinder<UMaterialInstance> YellowMatAsset(TEXT("MaterialInstanceConstant'/Game/Materials/MI_Yellow.MI_Yellow'"));
	YellowMat = YellowMatAsset.Object;
}

// Creating Bird Eye View Specific Component
void APlayerControlled::CreateBirdEyeComponents()
{
	// Bird-Eyes Spring Arm
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 20.0f), FRotator(-60.0f, 0.0f, 0.0f));
	SpringArm->TargetArmLength = 1000.0f;

	// Bird-Eyes Camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

// Creating VR View Specific Component
void APlayerControlled::CreateVRComponents()
{
	// VR Camera Root
	VRCameraRoot = CreateDefaultSubobject<USceneComponent>(TEXT("VR Camera Root"));
	VRCameraRoot->SetupAttachment(RootComponent);

	// VR Camera
	VRCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("VR Camera"));
	VRCamera->SetupAttachment(VRCameraRoot);

	// VR Motion Controllers
	CreateHandController(VRCameraRoot, "MC_Left", FXRMotionControllerBase::LeftHandSourceId);
	CreateHandController(VRCameraRoot, "MC_Right", FXRMotionControllerBase::RightHandSourceId);
}

void APlayerControlled::CreateHandController(USceneComponent* Parent, FName DisplayName, FName HandType)
{
	UMotionControllerComponent* MotionController = CreateDefaultSubobject<UMotionControllerComponent>(DisplayName);
	MotionController->MotionSource = HandType;
	MotionController->SetupAttachment(Parent);
}

/*USkeletalMeshComponent* APlayerControlled::CreateHandMesh(UMotionControllerComponent* Parent, FName DisplayName, FName HandType)
{
	USkeletalMeshComponent* ComponentHand = NULL;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMeshObject(TEXT("StaticMesh'/Game/Meshes/SM_Foliage_Plant.SM_Foliage_Plant'"));
	if (!TempMeshObject)
	{
		UE_LOG(LogTemp, Error, TEXT("Could not load the selected mesh for hand mesh"));
		return NULL;
	}

	// Set the defaults
	ComponentHand = CreateDefaultSubobject<USkeletalMeshComponent>(DisplayName);
	ComponentHand->SetSkeletalMesh(TempMeshObject.Object);
	ComponentHand->SetupAttachment(Parent);

	return ComponentHand;
}*/

// Called when the game starts or when spawned
void APlayerControlled::BeginPlay()
{
	Super::BeginPlay();
	
	// Setting Player Controller
	PlayerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled() == false)
	{
		PlayerController->SetShowMouseCursor(true);
		FInputModeGameAndUI InputMode;
		PlayerController->SetInputMode(InputMode);
	}
	
	// Starting a time for delayed BeginPlay (0.01 seconds)
	FTimerHandle DelayedBeginTimerHandle;
	GetWorldTimerManager().SetTimer(DelayedBeginTimerHandle, this, &APlayerControlled::DelayBeginPlay, 0.01f, false, 0.0f);

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
		// Block the user from going past 5000 and -5000
		NewLocation = FVector((FMath::Clamp(NewLocation.X, -5000.0f, 5000.0f)), (FMath::Clamp(NewLocation.Y, -5000.0f, 5000.0f)), NewLocation.Z);
		SetActorLocation(NewLocation);

		// Setting the camera to follow the landscape using a line trace to stay a certain distance from the landscape
		FHitResult OutHit;
		FVector Start = RootComponent->GetComponentLocation();
		FVector DownVector = RootComponent->GetUpVector() * -1.0f;
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
	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &APlayerControlled::PauseGame);
}

// Used as a delay for setting references of both the 'Player Controlled' and 'HUD'
void APlayerControlled::DelayBeginPlay()
{
	APlayerHUD* HUDClassReference = Cast<APlayerHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	HUDReference = HUDClassReference->HUDReference;

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &APlayerControlled::Victory, 0.01f, false, 180.0f);
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
		HUDReference->Widget_Building->Init();
		HUDReference->Widget_Building->SetVisibility(ESlateVisibility::Visible);
		isBuildingMode = true;
	}
	// Turning OFF Building Mode
	else
	{
		HUDReference->Widget_Building->SetVisibility(ESlateVisibility::Hidden);
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
		StaticMesh->SetMaterial(1, GreenMat);
		StaticMesh->SetMaterial(2, GreenMat);
		StaticMesh->SetMaterial(3, GreenMat);
		StaticMesh->SetMaterial(4, GreenMat);
		StaticMesh->SetMaterial(5, GreenMat);
	}
	else if (!hasEnoughResources && canBePlaced)
	{
		StaticMesh->SetMaterial(0, YellowMat);
		StaticMesh->SetMaterial(1, YellowMat);
		StaticMesh->SetMaterial(2, YellowMat);
		StaticMesh->SetMaterial(3, YellowMat);
		StaticMesh->SetMaterial(4, YellowMat);
		StaticMesh->SetMaterial(5, YellowMat);
	}
	else
	{
		StaticMesh->SetMaterial(0, RedMat);
		StaticMesh->SetMaterial(1, RedMat);
		StaticMesh->SetMaterial(2, RedMat);
		StaticMesh->SetMaterial(3, RedMat);
		StaticMesh->SetMaterial(4, RedMat);
		StaticMesh->SetMaterial(5, RedMat);
	}
}


// Interact - Left Mouse Button
void APlayerControlled::Interact()
{
	// Placing the building under the cursor
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
				HUDReference->Widget_Building->Init();
				doOncecheckResources = true;
				FTransform Transform = StaticMesh->GetComponentTransform();
				FActorSpawnParameters SpawnInfo;
				ABuildingBase* NewActor = GetWorld()->SpawnActor<ABuildingBase>(SelectedBuildingType.BuildingClass, Transform.GetLocation(), Transform.Rotator(), SpawnInfo);
				NewActor->BuildingTypeStruct = SelectedBuildingType;
				NewActor->StaticMesh->SetStaticMesh(SelectedBuildingType.StaticMesh);
				NewActor->BuildingTypeStruct.health = NewActor->BuildingTypeStruct.maxHealth;
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

	// Selecting a building and viewing it's information
	else
	{
		ETraceTypeQuery TraceChannel = TraceTypeQuery9;
		PlayerController->GetHitResultUnderCursorByChannel(TraceChannel, true, InteractHitResult);
		ABuildingBase* NewActor = Cast<ABuildingBase>(InteractHitResult.GetActor());
		if ((IsValid(NewActor)))
		{
			HUDReference->Widget_BuildingInformation->SetBuildingInformation(NewActor->BuildingTypeStruct);
			HUDReference->Widget_BuildingInformation->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			HUDReference->Widget_BuildingInformation->SetVisibility(ESlateVisibility::Hidden);
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

void APlayerControlled::PauseGame()
{
	HUDReference->Widget_Pause->SetVisibility(ESlateVisibility::Visible);
	PlayerController->SetPause(true);
}

void APlayerControlled::Victory()
{
	HUDReference->Widget_Victory->SetVisibility(ESlateVisibility::Visible);
	PlayerController->SetPause(true);
}

void APlayerControlled::Defeat()
{
	HUDReference->Widget_Defeat->SetVisibility(ESlateVisibility::Visible);
	PlayerController->SetPause(true);
}
