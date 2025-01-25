// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"

#include "MyFPSCharacter.generated.h"

UCLASS()
class FIRSTPERSONTD_API AMyFPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMyFPSCharacter();

protected:
	virtual void BeginPlay() override;

// #pragma region Movement
// 	void MoveForward(const float Value);
// 	void MoveBackwards(const float Value);
// 	void MoveLeft(const float Value);
// 	void MoveRight(const float Value);
// 	void LookAround(const float Value);
//
// 	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Input")
// 	UInputAction* WalkForwardAction;
// 	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Input")
// 	UInputAction* WalkBackwardsAction;
// 	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Input")
// 	UInputAction* WalkLeftAction;
// 	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Input")
// 	UInputAction* WalkRightAction;
// 	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Input")
// 	UInputAction* LookAroundAction;
// #pragma endregion
	
public:
	
	//virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UCameraComponent* Camera;
};
