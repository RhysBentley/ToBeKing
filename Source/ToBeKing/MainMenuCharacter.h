// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MainMenuCharacter.generated.h"

UCLASS()
class TOBEKING_API AMainMenuCharacter : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMainMenuCharacter();

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* VRCameraRoot;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* VRCamera;

	UPROPERTY(VisibleAnywhere)
	class UMotionControllerComponent* MC_Left;

	UPROPERTY(VisibleAnywhere)
	class UMotionControllerComponent* MC_Right;

	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* HandMesh_Left;

	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* HandMesh_Right;

	UPROPERTY(VisibleAnywhere)
	class UWidgetInteractionComponent* Interaction_Left;

	UPROPERTY(VisibleAnywhere)
	class UWidgetInteractionComponent* Interaction_Right;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
