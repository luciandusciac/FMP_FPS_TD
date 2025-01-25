// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"



class UEnhancedInputLocalPlayerSubsystem;


// Sets default values
AFPSPlayerController::AFPSPlayerController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}



// Called when the game starts or when spawned
void AFPSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetupInputComponent();
}

void AFPSPlayerController::MoveForward(const FInputActionValue& Value)
{
	if (ACharacter* PlayerCharacter = Cast<ACharacter>(GetPawn()))
	{
		const FVector Direction = FRotationMatrix(FRotator(0.f, GetControlRotation().Yaw, 0.f)).GetUnitAxis(EAxis::X);
		PlayerCharacter->AddMovementInput(Direction, Value.Get<float>() * MovementSpeed);
	}
}

void AFPSPlayerController::MoveBackwards(const FInputActionValue& Value)
{
	if (ACharacter* PlayerCharacter = Cast<ACharacter>(GetPawn()))
	{
		const FVector Direction = FRotationMatrix(FRotator(0.f, GetControlRotation().Yaw, 0.f)).GetUnitAxis(EAxis::X);
		PlayerCharacter->AddMovementInput(Direction, -Value.Get<float>() * MovementSpeed);
	}
}

void AFPSPlayerController::MoveLeft(const FInputActionValue& Value)
{
	if (ACharacter* PlayerCharacter = Cast<ACharacter>(GetPawn()))
	{
		const FVector Direction = FRotationMatrix(FRotator(0.f, GetControlRotation().Yaw, 0.f)).GetUnitAxis(EAxis::Y);
		PlayerCharacter->AddMovementInput(Direction, Value.Get<float>() * -MovementSpeed);
	}
}

void AFPSPlayerController::MoveRight(const FInputActionValue& Value)
{
	if (ACharacter* PlayerCharacter = Cast<ACharacter>(GetPawn()))
	{
		const FVector Direction = FRotationMatrix(FRotator(0.f, GetControlRotation().Yaw, 0.f)).GetUnitAxis(EAxis::Y);
		PlayerCharacter->AddMovementInput(Direction, Value.Get<float>() * MovementSpeed);
	}
}

void AFPSPlayerController::LookAround(const FInputActionValue& Value)
{
	if (ACharacter* PlayerCharacter = Cast<ACharacter>(GetPawn()))
	{
		const FVector2D LookValue = Value.Get<FVector2D>();
		PlayerCharacter->AddControllerYawInput(LookValue.X);
		PlayerCharacter->AddControllerPitchInput(-LookValue.Y);
	}
}

// Called every frame
void AFPSPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFPSPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}
}

void AFPSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent.Get()))
	{
		EnhancedInputComponent->BindAction(*InputActions.Find(EInputActionKey::IAK_MoveForward), ETriggerEvent::Triggered, this, &AFPSPlayerController::MoveForward);
		EnhancedInputComponent->BindAction(*InputActions.Find(EInputActionKey::IAK_MoveBackwards), ETriggerEvent::Triggered, this, &AFPSPlayerController::MoveBackwards);
		EnhancedInputComponent->BindAction(*InputActions.Find(EInputActionKey::IAK_MoveLeft), ETriggerEvent::Triggered, this, &AFPSPlayerController::MoveLeft);
		EnhancedInputComponent->BindAction(*InputActions.Find(EInputActionKey::IAK_MoveRight), ETriggerEvent::Triggered, this, &AFPSPlayerController::MoveRight);
		EnhancedInputComponent->BindAction(*InputActions.Find(EInputActionKey::IAK_Look), ETriggerEvent::Triggered, this, &AFPSPlayerController::LookAround);
		
	}
}

