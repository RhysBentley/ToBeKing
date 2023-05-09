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
#include "Components/WidgetInteractionComponent.h"

// Function Libraries
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
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
	// Building Type DataTables
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

// Creating Bird Eye View Specific Component - PC Only
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

// Creating VR View Specific Component - VR Only
void APlayerControlled::CreateVRComponents()
{
	// VR Camera Root
	VRCameraRoot = CreateDefaultSubobject<USceneComponent>(TEXT("VR Camera Root"));
	VRCameraRoot->SetRelativeLocation(FVector(0.0f, 0.0f, 500.0f));
	VRCameraRoot->SetupAttachment(RootComponent);

	// VR Camera
	VRCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("VR Camera"));
	VRCamera->SetupAttachment(VRCameraRoot);

	// VR Motion Controllers
	CreateHandController(VRCameraRoot, "MC_Left", "Collision_Left", "Interaction_Left", FXRMotionControllerBase::LeftHandSourceId);
	CreateHandController(VRCameraRoot, "MC_Right", "Collision_Right", "Interaction_Right", FXRMotionControllerBase::RightHandSourceId);

	// Clipboard
	static ConstructorHelpers::FObjectFinder<UStaticMesh> TempMeshObject(TEXT("StaticMesh'/Game/Meshes/VR/SM_Clipboard.SM_Clipboard'"));
	if (!TempMeshObject.Object)
	{
		UE_LOG(LogTemp, Error, TEXT("Could not load the selected mesh for the hand"));
	}
	else
	{
		// Creating the Clipboard
		ClipboardRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Clipboard Root"));
		ClipboardRoot->SetupAttachment(VRCameraRoot);

		ClipboardMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Clipboard"));
		ClipboardMesh->SetStaticMesh(TempMeshObject.Object);
		ClipboardMesh->SetupAttachment(ClipboardRoot);
		ClipboardMesh->SetRelativeLocationAndRotation(FVector(0.0f, -10.0f, 40.0f),FRotator(0.0f, 90.0f, -90.0f));
		ClipboardMesh->SetRelativeScale3D(FVector(2.0f, 2.0f, 2.0f));
		ClipboardMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		ClipboardOriginPoint = ClipboardMesh->GetRelativeTransform();

		/// Setting the Widgets
		FRotator Rotation = FRotator(0.0f, -180.0f, 0.0f);
		FVector Scale = FVector(0.0175f);

		// Player's Resources
		ResourcesWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Resources Widget"));
		static ConstructorHelpers::FClassFinder<UResourcesWidget> TempResourcesWidgetObject(TEXT("/Game/Widgets/Widget_Resources"));
		ResourcesWidget->SetWidgetClass(TempResourcesWidgetObject.Class);
		ResourcesWidget->SetupAttachment(ClipboardMesh);
		ResourcesWidget->SetDrawSize(FVector2D(500.0f, 150.0f));
		ResourcesWidget->SetRelativeTransform(FTransform(Rotation, FVector(-0.3, 0.3f, 3.5f), Scale));

		// Building Selection
		BuildingSelectionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Building Selection Widget"));
		static ConstructorHelpers::FClassFinder<UBuildingWidget> TempBuildingWidgetObject(TEXT("/Game/Widgets/Widget_Building"));
		BuildingSelectionWidget->SetWidgetClass(TempBuildingWidgetObject.Class);
		BuildingSelectionWidget->SetupAttachment(ClipboardMesh);
		BuildingSelectionWidget->SetDrawSize(FVector2D(500.0f, 150.0f));
		BuildingSelectionWidget->SetRelativeTransform(FTransform(Rotation, FVector(-0.3, 0.0f, -0.5f), Scale));
	}
}

// Creating the Hand Controllers - VR Only
void APlayerControlled::CreateHandController(USceneComponent* Parent, FName DisplayName, FName CollisionName, FName InteractionName, FName HandType)
{
	UMotionControllerComponent* MotionController = CreateDefaultSubobject<UMotionControllerComponent>(DisplayName);
	MotionController->MotionSource = HandType;
	MotionController->SetupAttachment(Parent);

	USphereComponent* HandCollision = CreateDefaultSubobject<USphereComponent>(CollisionName);
	HandCollision->SetupAttachment(MotionController);
	HandCollision->SetSphereRadius(8.0f);

	// Creating the hand mesh
	FName MeshDisplayName = HandType == FXRMotionControllerBase::LeftHandSourceId ? FName(TEXT("Hand_Left")) : FName(TEXT("Hand_Right"));
	CreateHandMesh(MotionController, MeshDisplayName, HandType);

	// Widget Interaction
	UWidgetInteractionComponent* Interaction = CreateDefaultSubobject<UWidgetInteractionComponent>(InteractionName);
	USceneComponent* WidgetInteraction = HandType == FXRMotionControllerBase::LeftHandSourceId ? HandMesh_Left : HandMesh_Right;
	Interaction->SetupAttachment(WidgetInteraction, "WidgetInteraction");
	Interaction->DebugLineThickness = 0.5f;
	Interaction->DebugSphereLineThickness = 0.6f;

	// Setting Variables
	if (HandType == FXRMotionControllerBase::LeftHandSourceId)
	{
		MC_Left = MotionController;
		Collision_Left = HandCollision;
		Interaction_Left = Interaction;
	}
	else
	{
		MC_Right = MotionController;
		Collision_Right = HandCollision;
		Interaction_Right = Interaction;
	}
}

// Creating the Hand Mesh - VR Only
USkeletalMeshComponent* APlayerControlled::CreateHandMesh(UMotionControllerComponent* Parent, FName DisplayName, FName HandType)
{
	USkeletalMeshComponent* ComponentHand = NULL;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMeshObject(TEXT("SkeletalMesh'/Game/Meshes/VR/Hands/SM_HandR.SM_HandR'"));
	if (!TempMeshObject.Object)
	{
		UE_LOG(LogTemp, Error, TEXT("Could not load the selected mesh for the hand"));
		return NULL;
	}

	// Set the defaults
	ComponentHand = CreateDefaultSubobject<USkeletalMeshComponent>(DisplayName);
	ComponentHand->SetSkeletalMesh(TempMeshObject.Object);
	
	// Setting the Scale of the Hands
	FVector Scale = FVector(2.5f, 2.5f, 2.5f);
	FQuat Rotation;
	if (HandType == FXRMotionControllerBase::LeftHandSourceId)
	{
		Scale.Y = Scale.Y * -1.0f;
		Rotation = FQuat(FVector(1, 0, 0), FMath::DegreesToRadians(90));
	}
	else
	{
		Rotation = FQuat(FVector(1, 0, 0), FMath::DegreesToRadians(270));
	}
	ComponentHand->SetRelativeRotation(Rotation);
	ComponentHand->SetRelativeScale3D(Scale);
	ComponentHand->SetupAttachment(Parent);

	// Setting Variables
	if (HandType == FXRMotionControllerBase::LeftHandSourceId)
	{
		HandMesh_Left = ComponentHand;
	}
	else
	{
		HandMesh_Right = ComponentHand;
	}

	return ComponentHand;
}

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

	// Setting the camera to go over the landscape - PC Only
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
		if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled() == false)
		{
			PlayerController->GetHitResultUnderCursorByChannel(TraceChannel, true, InteractHitResult);
		}
		else
		{
			InteractHitResult = ActiveInteraction->GetLastHitResult();
		}

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

	// Setting the rotation of the clipboard on the player's hip when using VR
	/*if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled() == true)
	{
		if (ClipboardMesh)
		{
			ClipboardRoot->SetWorldLocation(FVector(0.0f, 0.0f, 0.0f));
		}
	}*/
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

	PlayerInputComponent->BindAction("GrabLeft", IE_Pressed, this, &APlayerControlled::GrabLeftPressed);
	PlayerInputComponent->BindAction("GrabLeft", IE_Released, this, &APlayerControlled::GrabLeftReleased);

	PlayerInputComponent->BindAction("GrabRight", IE_Pressed, this, &APlayerControlled::GrabRightPressed);
	PlayerInputComponent->BindAction("GrabRight", IE_Released, this, &APlayerControlled::GrabRightReleased);
}

// Used as a delay for setting references of both the 'Player Controlled' and 'HUD'
void APlayerControlled::DelayBeginPlay()
{
	APlayerHUD* HUDClassReference = Cast<APlayerHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	HUDReference = HUDClassReference->HUDReference;

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &APlayerControlled::Victory, 0.01f, false, 180.0f);
}

// Move this class on the X Axis - PC Only
void APlayerControlled::Move_XAxis(float AxisValue)
{
	CurrentVelocity.X = FMath::Clamp(AxisValue, -1.0f, 1.0) * Speed;
}

// Move this class on the Y Axis - PC Only
void APlayerControlled::Move_YAxis(float AxisValue)
{
	CurrentVelocity.Y = FMath::Clamp(AxisValue, -1.0f, 1.0) * Speed;
}

// Zoom in and out using the length of the spring arm - PC Only
void APlayerControlled::Zoom(float AxisValue)
{
	CurrentZoomAmount = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 100.0f;
	if (CurrentZoomAmount != 0.0f)
	{
		float NewSpringArmLength = SpringArm->TargetArmLength + (FMath::Clamp(CurrentZoomAmount, -1.0f, 1.0f) * 20.0f);
		SpringArm->TargetArmLength = FMath::Clamp(NewSpringArmLength, 300.0f, 1400.0f);
	}
}

//Change the speed of the camera - PC Only
void APlayerControlled::StartFasterMovement()
{
	Speed = 1200.0f;
}

void APlayerControlled::StopFasterMovement()
{
	Speed = 600.0f;
}

// Enabling and disabling Building Mode - PC Only
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


// Interact - Left Mouse Button, Oculus Touch Triggers
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
		if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled() == false)
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

// VR Based Inputs - VR Only
void APlayerControlled::GrabLeftPressed()
{
	GrabClipboard(Collision_Left, MC_Left);
}

void APlayerControlled::GrabLeftReleased()
{
	ReleaseClipboard(MC_Left);
}

void APlayerControlled::GrabRightPressed()
{
	GrabClipboard(Collision_Right, MC_Right);
}

void APlayerControlled::GrabRightReleased()
{
	ReleaseClipboard(MC_Right);
}

void APlayerControlled::GrabClipboard(USphereComponent* HandCollision, UMotionControllerComponent* MotionController)
{
	if (HandCollision->IsOverlappingComponent(ClipboardMesh))
	{
		const FAttachmentTransformRules Attachment = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true);
		ClipboardMesh->AttachToComponent(MotionController, Attachment);
		FVector Location;
		FRotator Rotation;
		if (HandCollision->GetName() == "Collision_Left")
		{
			Location = ClipboardMesh->GetRelativeLocation() + FVector(-1.25f, 1.75f, -10.0f);
			Rotation = FRotator(0.0f, -70.0f, 90.0f);
			Interaction_Right->bShowDebug = true;
			ActiveInteraction = Interaction_Left;
		}
		else
		{
			Location = ClipboardMesh->GetRelativeLocation() + FVector(-1.25f, -1.75f, -10.0f);
			Rotation = FRotator(0.0f, 70.0f, -90.0f);
			Interaction_Left->bShowDebug = true;
			ActiveInteraction = Interaction_Right;
		}
		ClipboardMesh->SetRelativeLocationAndRotation(Location, Rotation);
		isBuildingMode = true;
	}
}

void APlayerControlled::ReleaseClipboard(UMotionControllerComponent* MotionController)
{
	if (ClipboardMesh->IsAttachedTo(MotionController))
	{
		const FDetachmentTransformRules Detachment = FDetachmentTransformRules(EDetachmentRule::KeepRelative, true);
		ClipboardMesh->DetachFromComponent(Detachment);
		const FAttachmentTransformRules Attachment = FAttachmentTransformRules(EAttachmentRule::KeepRelative, true);
		ClipboardMesh->AttachToComponent(ClipboardRoot, Attachment);
		ClipboardMesh->SetRelativeTransform(ClipboardOriginPoint);
		Interaction_Left->bShowDebug = false;
		Interaction_Right->bShowDebug = false;
		isBuildingMode = false;
	}
}