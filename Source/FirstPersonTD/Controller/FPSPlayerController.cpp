// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FrameTypes.h"
#include "InputActionValue.h"
#include "Components/SkeletalMeshComponent.h"
#include "FirstPersonTD/Characters/MyFPSCharacter.h"
#include "GameFramework/Character.h"
#include "Engine/LocalPlayer.h"
#include "FirstPersonTD/Animations/SWAT_AnimInstance.h"


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
		PlayerCharacter->AddMovementInput(Direction, Value.Get<float>());
	}
}

void AFPSPlayerController::MoveBackwards(const FInputActionValue& Value)
{
	if (ACharacter* PlayerCharacter = Cast<ACharacter>(GetPawn()))
	{
		const FVector Direction = FRotationMatrix(FRotator(0.f, GetControlRotation().Yaw, 0.f)).GetUnitAxis(EAxis::X);
		PlayerCharacter->AddMovementInput(Direction, -Value.Get<float>());
	}
}

void AFPSPlayerController::MoveLeft(const FInputActionValue& Value)
{
	if (ACharacter* PlayerCharacter = Cast<ACharacter>(GetPawn()))
	{
		const FVector Direction = FRotationMatrix(FRotator(0.f, GetControlRotation().Yaw, 0.f)).GetUnitAxis(EAxis::Y);
		PlayerCharacter->AddMovementInput(Direction, -Value.Get<float>());
	}
}

void AFPSPlayerController::MoveRight(const FInputActionValue& Value)
{
	if (ACharacter* PlayerCharacter = Cast<ACharacter>(GetPawn()))
	{
		const FVector Direction = FRotationMatrix(FRotator(0.f, GetControlRotation().Yaw, 0.f)).GetUnitAxis(EAxis::Y);
		PlayerCharacter->AddMovementInput(Direction, Value.Get<float>());
	}
}

void AFPSPlayerController::LookAround(const FInputActionValue& Value)
{
	if (ACharacter* PlayerCharacter = Cast<ACharacter>(GetPawn()))
	{
		const FVector2D LookValue = Value.Get<FVector2D>();
		PlayerCharacter->AddControllerYawInput(LookValue.X);
		PlayerCharacter->AddControllerPitchInput(-LookValue.Y);


		if(GetCharacter()->GetMesh())
		{
			if(USWAT_AnimInstance* AnimInstance = Cast<USWAT_AnimInstance>(GetCharacter()->GetMesh()->GetAnimInstance()))
			{
				AnimInstance->VerticalBend = FMath::Lerp(AnimInstance->VerticalBend, LookValue.Y * 50.f,  DeltaT * 2.f);
				AnimInstance->VerticalBend = FMath::Clamp(AnimInstance->VerticalBend, -50.f, 50.f);
				//LookValue.Y = FMath::Clamp(LookValue.Y, -50.f, 50.f);
			
			}
		}
	}
}

void AFPSPlayerController::SwapWeapon()
{
	UE_LOG(LogTemp, Warning, TEXT("Swapping weapon"));
	
	// if (AMyFPSCharacter* PlayerCharacter = Cast<AMyFPSCharacter>(GetPawn()))
	// {
	// 	PlayerCharacter->SwapWeapon();
	// }
}

void AFPSPlayerController::Shoot()
{
	UE_LOG(LogTemp, Warning, TEXT("Shooting"));
}

void AFPSPlayerController::Reload()
{
	UE_LOG(LogTemp, Warning, TEXT("Reloading"));
}

void AFPSPlayerController::PeekRight(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Peeking right"));

	bIsPeeking = true;
	bPeekingCompleted = false;
	
	if(GetCharacter()->GetMesh())
	{
		if(USWAT_AnimInstance* AnimInstance = Cast<USWAT_AnimInstance>(GetCharacter()->GetMesh()->GetAnimInstance()))
		{
			AnimInstance->HorizontalBend = FMath::Lerp(AnimInstance->HorizontalBend, Value.Get<float>() * -50.f,  DeltaT * 10.f);
			AnimInstance->HorizontalBend = FMath::Clamp(AnimInstance->HorizontalBend, -50.f, 0.f);
			
		}
	}
}

void AFPSPlayerController::PeekLeft(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Peeking left"));
	
	bIsPeeking = true;
	bPeekingCompleted = false;
	
	if(GetCharacter()->GetMesh())
	{
		if(USWAT_AnimInstance* AnimInstance = Cast<USWAT_AnimInstance>(GetCharacter()->GetMesh()->GetAnimInstance()))
		{
			AnimInstance->HorizontalBend = FMath::Lerp(AnimInstance->HorizontalBend, Value.Get<float>() * 50.f,  DeltaT * 10.f);
			AnimInstance->HorizontalBend = FMath::Clamp(AnimInstance->HorizontalBend, 0.f, 50.f);
		}
	}
}

void AFPSPlayerController::Crouch()
{
	UE_LOG(LogTemp, Warning, TEXT("Crouching"));
}

void AFPSPlayerController::Aim()
{
	UE_LOG(LogTemp, Warning, TEXT("Aiming"));
}

void AFPSPlayerController::AimGrenade()
{
	UE_LOG(LogTemp, Warning, TEXT("Aiming grenade"));
}

void AFPSPlayerController::ThrowGrenade()
{
	UE_LOG(LogTemp, Warning, TEXT("Throwing grenade"));
}

void AFPSPlayerController::ResetPeeking()
{
	// if(GetCharacter()->GetMesh())
	// {
	// 	if(USWAT_AnimInstance* AnimInstance = Cast<USWAT_AnimInstance>(GetCharacter()->GetMesh()->GetAnimInstance()))
	// 	{
	// 		AnimInstance->HorizontalBend = FMath::Lerp(AnimInstance->HorizontalBend,0.f,  DeltaT * 10.f);
	// 		//AnimInstance->HorizontalBend = FMath::Clamp(AnimInstance->HorizontalBend, 0.f, 50.f);
	// 	}
	// }
	bIsPeeking = false;
}

// Called every frame
void AFPSPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DeltaT = DeltaTime;

	if(!bIsPeeking && !bPeekingCompleted)
	{
		if(GetCharacter()->GetMesh())
		{
			if(USWAT_AnimInstance* AnimInstance = Cast<USWAT_AnimInstance>(GetCharacter()->GetMesh()->GetAnimInstance()))
			{
				AnimInstance->HorizontalBend = FMath::Lerp(AnimInstance->HorizontalBend,0.f,  DeltaT * 10.f);
				bPeekingCompleted = FMath::IsNearlyEqual(AnimInstance->HorizontalBend, 0.f, 0.01f);
				//AnimInstance->HorizontalBend = FMath::Clamp(AnimInstance->HorizontalBend, 0.f, 50.f);
			}
		}
	}
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

	if(InputComponent)
	{
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent.Get()))
		{
			EnhancedInputComponent->BindAction(*InputActions.Find(EInputActionKey::IAK_SwitchWeapon), ETriggerEvent::Triggered, this, &AFPSPlayerController::SwapWeapon);
			EnhancedInputComponent->BindAction(*InputActions.Find(EInputActionKey::IAK_MoveForward), ETriggerEvent::Triggered, this, &AFPSPlayerController::MoveForward);
			EnhancedInputComponent->BindAction(*InputActions.Find(EInputActionKey::IAK_MoveBackwards), ETriggerEvent::Triggered, this, &AFPSPlayerController::MoveBackwards);
			EnhancedInputComponent->BindAction(*InputActions.Find(EInputActionKey::IAK_MoveLeft), ETriggerEvent::Triggered, this, &AFPSPlayerController::MoveLeft);
			EnhancedInputComponent->BindAction(*InputActions.Find(EInputActionKey::IAK_MoveRight), ETriggerEvent::Triggered, this, &AFPSPlayerController::MoveRight);
			EnhancedInputComponent->BindAction(*InputActions.Find(EInputActionKey::IAK_Look), ETriggerEvent::Triggered, this, &AFPSPlayerController::LookAround);
			EnhancedInputComponent->BindAction(*InputActions.Find(EInputActionKey::IAK_Shoot), ETriggerEvent::Triggered, this, &AFPSPlayerController::Shoot);
			EnhancedInputComponent->BindAction(*InputActions.Find(EInputActionKey::IAK_Reload), ETriggerEvent::Triggered, this, &AFPSPlayerController::Reload);
			EnhancedInputComponent->BindAction(*InputActions.Find(EInputActionKey::IAK_PeekRight), ETriggerEvent::Triggered, this, &AFPSPlayerController::PeekRight);
			EnhancedInputComponent->BindAction(*InputActions.Find(EInputActionKey::IAK_PeekLeft), ETriggerEvent::Triggered, this, &AFPSPlayerController::PeekLeft);
			EnhancedInputComponent->BindAction(*InputActions.Find(EInputActionKey::IAK_Crouch), ETriggerEvent::Triggered, this, &AFPSPlayerController::Crouch);
			EnhancedInputComponent->BindAction(*InputActions.Find(EInputActionKey::IAK_Aim), ETriggerEvent::Triggered, this, &AFPSPlayerController::Aim);
			EnhancedInputComponent->BindAction(*InputActions.Find(EInputActionKey::IAK_AimGrenade), ETriggerEvent::Triggered, this, &AFPSPlayerController::AimGrenade);
			EnhancedInputComponent->BindAction(*InputActions.Find(EInputActionKey::IAK_ThrowGrenade), ETriggerEvent::Triggered, this, &AFPSPlayerController::ThrowGrenade);
			EnhancedInputComponent->BindAction(*InputActions.Find(EInputActionKey::IAK_PeekRight), ETriggerEvent::Completed, this, &AFPSPlayerController::ResetPeeking);
			EnhancedInputComponent->BindAction(*InputActions.Find(EInputActionKey::IAK_PeekLeft), ETriggerEvent::Completed, this, &AFPSPlayerController::ResetPeeking);
		}
	}
}

