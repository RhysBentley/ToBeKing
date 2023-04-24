// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "HUDWidget.h"
#include "Templates/SubclassOf.h"
#include "PlayerControlled.generated.h"

UENUM(BlueprintType)
enum EBuildingType
{
	LumberMill UMETA(DisplayName = "Lumber Mill"),
	Mine UMETA(DisplayName = "Mine"),
	Farm UMETA(DisplayName = "Farm"),
	Market UMETA(DisplayName = "Market"),
	TownHall UMETA(DisplayName = "Town Hall"),
	ArcherTower UMETA(DisplayName = "Archer Tower")
};

USTRUCT(BlueprintType)
struct FResourceList
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float Wood;
	UPROPERTY(EditAnywhere)
	float Stone;
	UPROPERTY(EditAnywhere)
	float Wheat;
	UPROPERTY(EditAnywhere)
	float Coins;

};

USTRUCT(BlueprintType)
struct FEnemySpawning : public FTableRowBase
{
	GENERATED_BODY()

	// Temp - Enemy Spawning Variables
	UPROPERTY(EditAnywhere, Category = "Enemy Spawning")
	float gracePeriod;

	UPROPERTY(EditAnywhere, Category = "Enemy Spawning")
	float spawningInterval;

	UPROPERTY(EditAnywhere, Category = "Enemy Spawning")
	int spawnAmount;
};

USTRUCT(BlueprintType)
struct FBuildingTypeStruct : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Building Information")
	FString Name;

	UPROPERTY(EditAnywhere, Category = "Building Information")
	UStaticMesh* StaticMesh;

	UPROPERTY(EditAnywhere, Category = "Building Information")
	TEnumAsByte<EBuildingType> BuildingTypeByte;

	UPROPERTY(EditAnywhere, Category = "Building Information")
	float ProductionDuration = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Building Information")
	float ProductionAmount = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Building Information")
	float DistanceFromPivot;

	UPROPERTY(EditAnywhere, Category = "Building Information")
	FResourceList BuildingCost;

	UPROPERTY(EditAnywhere, Category = "Building Information")
	TSubclassOf<AActor> BuildingClass;

	UPROPERTY(EditAnywhere, Category = "Building Information|Health")
	float health;

	UPROPERTY(EditAnywhere, Category = "Building Information|Health")
	float maxHealth;
};

UCLASS()
class TOBEKING_API APlayerControlled : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerControlled();


	// Adding components to this class
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
	class UBillboardComponent* StaticMeshRoot;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* Collision;


	// References
	UPROPERTY()
	APlayerController* PlayerController;

	UPROPERTY()
	UHUDWidget* HUDReference;

	UPROPERTY(EditAnywhere, Category= "References")
	UDataTable* BuildingTypeDT;


	// Materials
	UPROPERTY(EditAnywhere, Category = "References|Materials")
	UMaterialInstance* GreenMat;

	UPROPERTY(EditAnywhere, Category = "References|Materials")
	UMaterialInstance* RedMat;

	UPROPERTY(EditAnywhere, Category = "References|Materials")
	UMaterialInstance* YellowMat;

	// Building functions and variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FResourceList ResourceList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBuildingTypeStruct SelectedBuildingType;

	UPROPERTY()
	FResourceList TempResourceList;

	UPROPERTY()
	TArray<AActor*> OverlappingActors;
	TSubclassOf<AActor> ClassFilter;


	// Grid System Variables
	UPROPERTY(EditAnywhere, Category = "Grid System")
	int gridSize = 10;

	UPROPERTY(EditAnywhere, Category = "Grid System")
	bool gridEnabled = true;


	// Material Change Variables
	UPROPERTY()
	bool hasEnoughResources = false;

	UPROPERTY()
	bool canBePlaced = false;

	// doOnce Variable
	UPROPERTY()
	bool doOncecheckResources = true;

	// Temp - Wave Spawning
	UPROPERTY(EditAnywhere)
	TArray<FEnemySpawning> EnemySpawningWaves;

	UPROPERTY(EditAnywhere)
	int WaveNumber = 1;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	// Delayed Begin Play of 0.01 Seconds
	void DelayBeginPlay();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);


	/// Input functions and variables
	// Input functions for movement
	void Move_XAxis(float AxisValue);
	void Move_YAxis(float AxisValue);

	// Input function for zooming in and out
	void Zoom(float AxisValue);

	// Input functions for faster movement
	void StartFasterMovement();
	void StopFasterMovement();

	// Input Variables for movement and zoom
	FVector CurrentVelocity;
	float CurrentZoomAmount;
	float Speed = 600.0f;

	// Input function and variables for the Building Mode
	void BuildingMode();
	bool isBuildingMode = false;

	bool checkResources(FResourceList CostInput);
	void VariableBaseSetMaterial();

	// Input function for interacting
	void Interact();

	UPROPERTY()
	FHitResult InteractHitResult;

	// Pause Game Function
	void PauseGame();

	void Defeat();

	void Victory();
};