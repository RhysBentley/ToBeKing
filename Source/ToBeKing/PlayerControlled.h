// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerControlled.generated.h"

UCLASS()
class TOBEKING_API APlayerControlled : public ACharacter
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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Input functions for movement
	void Move_XAxis(float AxisValue);
	void Move_YAxis(float AxisValue);

	// Input functions for faster movement
	void StartFasterMovement();
	void StopFasterMovement();

	// Input function for zooming in and out
	void Zoom(float AxisValue);

	// Input Variables for movement and zoom
	FVector CurrentVelocity;
	float CurrentZoomAmount;
};
