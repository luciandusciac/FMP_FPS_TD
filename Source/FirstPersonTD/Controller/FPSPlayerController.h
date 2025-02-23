// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Controller.h"

#include "InputAction.h"
#include "InputMappingContext.h"
#include "FirstPersonTD/Animations/SWAT_AnimInstance.h"
#include "GameFramework/PlayerController.h"

#include "FPSPlayerController.generated.h"



UENUM()
enum class EInputActionKey : uint8
{
	IAK_None				UMETA(DisplayName = "None"),
	
	IAK_MoveForward			UMETA(DisplayName = "MoveForward"),
	IAK_MoveBackwards		UMETA(DisplayName = "MoveBackwards"),
	IAK_Look				UMETA(DisplayName = "Look"),
	IAK_MoveRight			UMETA(DisplayName = "MoveRight"),
	IAK_MoveLeft			UMETA(DisplayName = "MoveLeft"),
	IAK_NextWeapon			UMETA(DisplayName = "NextWeapon"),
	IAK_PreviousWeapon		UMETA(DisplayName = "PreviousWeapon"),
	IAK_Shoot				UMETA(DisplayName = "Shoot"),
	IAK_Reload				UMETA(DisplayName = "Reload"),
	IAK_PeekRight			UMETA(DisplayName = "PeekRight"),
	IAK_PeekLeft			UMETA(DisplayName = "PeekLeft"),
	IAK_Crouch				UMETA(DisplayName = "Crouch"),
	IAK_Aim					UMETA(DisplayName = "Aim"),
	IAK_AimGrenade			UMETA(DisplayName = "AimGrenade"),
	IAK_ThrowGrenade		UMETA(DisplayName = "ThrowGrenade"),
	IAK_ThrowItem			UMETA(DisplayName = "ThrowItem"),
};



UCLASS()
class FIRSTPERSONTD_API AFPSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFPSPlayerController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float DeltaT;
	
#pragma region Movement and Actions
	void MoveForward(const FInputActionValue& Value);
	void MoveBackwards(const FInputActionValue& Value);
	void MoveLeft(const FInputActionValue& Value);
	void MoveRight(const FInputActionValue& Value);
	void LookAround(const FInputActionValue& Value);
	void NextWeapon();
	void PreviousWeapon();
	void ThrowItem();

	void Shoot();
	void Reload();
	void PeekRight(const FInputActionValue& Value);
	void PeekLeft(const FInputActionValue& Value);
	void Crouch();
	void Aim();
	void StopAiming();
	void AimGrenade();
	void ThrowGrenade();

	void ResetPeeking();
	bool bIsPeeking = false;
	bool bPeekingCompleted = false;

	void StopWalking();
	bool bIsWalking = false;
	bool bStoppedWalkingVert = false;
	bool bStoppedWalkingHoriz = false;

	void StopCrouching();
	bool bIsCrouching = false;
	bool bCrouchingCompleted = false;
	
	float MovementSpeed = 100.0f;
	float RotationSpeed = 100.0f;
#pragma endregion

	//TMap<int, bool> AnimationStates;
	//TArray<bool> AnimationBooleans;
	UPROPERTY()
	USWAT_AnimInstance* AnimationInstance;

#pragma region InputProperties
	UPROPERTY(EditAnywhere, Category= "Player|Input")
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere, Category = "Player|Input")
	TMap<EInputActionKey, TObjectPtr<UInputAction>> InputActions;
#pragma endregion InputProperties

public:
	virtual void Tick(float DeltaSeconds) override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void SetupInputComponent() override;

	int AnimationIndex = NULL;
	void EquipWeapon(int Index);
};
