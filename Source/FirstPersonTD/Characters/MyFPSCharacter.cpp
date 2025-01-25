// Fill out your copyright notice in the Description page of Project Settings.
#include "MyFPSCharacter.h"

#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/Controller.h"


// Sets default values
AMyFPSCharacter::AMyFPSCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->bUsePawnControlRotation = true;
	Camera->SetupAttachment(GetMesh(), "Head");
}

// Called when the game starts or when spawned
void AMyFPSCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// void AMyFPSCharacter::MoveForward(const float Value)
// {
// 	const FVector Direction = FRotationMatrix(FRotator(0.f, GetControlRotation().Yaw, 0.f)).GetUnitAxis(EAxis::X);
// 	AddMovementInput(Direction, Value);
// }
//
// void AMyFPSCharacter::MoveBackwards(const float Value)
// {
// 	const FVector Direction = FRotationMatrix(FRotator(0.f, GetControlRotation().Yaw, 0.f)).GetUnitAxis(EAxis::X);
// 	AddMovementInput(Direction, -Value);
// }
//
// void AMyFPSCharacter::MoveLeft(const float Value)
// {
// 	const FVector Direction = FRotationMatrix(FRotator(0.f, GetControlRotation().Yaw, 0.f)).GetUnitAxis(EAxis::Y);
// 	AddMovementInput(Direction, -Value);
// }
//
// void AMyFPSCharacter::MoveRight(const float Value)
// {
// 	const FVector Direction = FRotationMatrix(FRotator(0.f, GetControlRotation().Yaw, 0.f)).GetUnitAxis(EAxis::Y);
// 	AddMovementInput(Direction, Value);
// }
//
// void AMyFPSCharacter::LookAround(const float Value)
// {
// 	const FRotator NewRotation = FRotator(0.f, Value, 0.f);
// 	AddControllerYawInput(Value);
// }

// Called every frame
// void AMyFPSCharacter::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);
// }

// Called to bind functionality to input
void AMyFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	//
	// EnhancedInputComponent->BindAction(WalkForwardAction, ETriggerEvent::Triggered, this, GetController()->GetClass() MoveForward);
	// EnhancedInputComponent->BindAction(WalkBackwardsAction, ETriggerEvent::Triggered, this, &AMyFPSCharacter::MoveBackwards);
	// EnhancedInputComponent->BindAction(WalkLeftAction, ETriggerEvent::Triggered, this, &AMyFPSCharacter::MoveLeft);
	// EnhancedInputComponent->BindAction(WalkRightAction, ETriggerEvent::Triggered, this, &AMyFPSCharacter::MoveRight);
	// EnhancedInputComponent->BindAction(LookAroundAction, ETriggerEvent::Triggered, this, &AMyFPSCharacter::LookAround);
}

