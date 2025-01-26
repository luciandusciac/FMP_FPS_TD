// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Controller.h"

#include "InputAction.h"
#include "InputMappingContext.h"
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
	IAK_SwitchWeapon		UMETA(DisplayName = "SwitchWeapon"),
	//IAK_SwitchWeapon		UMETA(DisplayName = "SwitchWeapon"),
	//IAK_SwitchExplosive 	UMETA(DisplayName = "SwitchExplosive"),
	//IAK_LayExplosive		UMETA(DisplayName = "LayExplosive"),
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

#pragma region Movement
	void MoveForward(const FInputActionValue& Value);
	void MoveBackwards(const FInputActionValue& Value);
	void MoveLeft(const FInputActionValue& Value);
	void MoveRight(const FInputActionValue& Value);
	void LookAround(const FInputActionValue& Value);
	virtual void SwapWeapon();

	float MovementSpeed = 100.0f;
	float RotationSpeed = 100.0f;
#pragma endregion

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
};
