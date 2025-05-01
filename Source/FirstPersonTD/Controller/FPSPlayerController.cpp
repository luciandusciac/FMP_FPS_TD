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
#include "Kismet/GameplayStatics.h"


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

	AnimationInstance = Cast<USWAT_AnimInstance>(GetCharacter()->GetMesh()->GetAnimInstance());

	//EquipWeapon(AnimationIndex);
	
	// AnimationStates.Add(0, AnimationInstance->bHasPrimary);
	//AnimationInstance->bHasPrimary = true;
	// AnimationStates.Add(1, AnimationInstance->bHasPistol);
	// //AnimationInstance->bHasPistol = false;
	// AnimationStates.Add(2, AnimationInstance->bHasGrenade);
	// //AnimationInstance->bHasGrenade = false;
	// AnimationStates.Add(3, AnimationInstance->bHasKnife);
	//AnimationInstance->bHasKnife = false;

	//AnimationStates.GenerateValueArray(AnimationBooleans);
}

void AFPSPlayerController::MoveForward(const FInputActionValue& Value)
{
	if (ACharacter* PlayerCharacter = Cast<ACharacter>(GetPawn()))
	{
		const FVector Direction = FRotationMatrix(FRotator(0.f, GetControlRotation().Yaw, 0.f)).GetUnitAxis(EAxis::X);
		PlayerCharacter->AddMovementInput(Direction, Value.Get<float>());
	}

	bIsWalking = true;
	bStoppedWalkingVert = false;
	
	//if(GetCharacter()->GetMesh())
	//{
		//if(USWAT_AnimInstance* AnimInstance = Cast<USWAT_AnimInstance>(GetCharacter()->GetMesh()->GetAnimInstance()))
		//{
			AnimationInstance->VerticalWalk = FMath::Lerp(AnimationInstance->VerticalWalk, Value.Get<float>() * 100.f,  DeltaT * 2.f);
			AnimationInstance->VerticalWalk = FMath::Clamp(AnimationInstance->VerticalWalk, 0.f, 100.f);
			//LookValue.Y = FMath::Clamp(LookValue.Y, -50.f, 50.f);
			
		//}
	//}
}

void AFPSPlayerController::MoveBackwards(const FInputActionValue& Value)
{
	if (ACharacter* PlayerCharacter = Cast<ACharacter>(GetPawn()))
	{
		const FVector Direction = FRotationMatrix(FRotator(0.f, GetControlRotation().Yaw, 0.f)).GetUnitAxis(EAxis::X);
		PlayerCharacter->AddMovementInput(Direction, -Value.Get<float>());
	}

	bIsWalking = true;
	bStoppedWalkingVert = false;
	
	//if(GetCharacter()->GetMesh())
	//{
		//if(USWAT_AnimInstance* AnimInstance = Cast<USWAT_AnimInstance>(GetCharacter()->GetMesh()->GetAnimInstance()))
		//{
			AnimationInstance->VerticalWalk = FMath::Lerp(AnimationInstance->VerticalWalk, Value.Get<float>() * -100.f,  DeltaT * 2.f);
			AnimationInstance->VerticalWalk = FMath::Clamp(AnimationInstance->VerticalWalk, -100.f, 0.f);
			//LookValue.Y = FMath::Clamp(LookValue.Y, -50.f, 50.f);
			
		//}
	//}
}

void AFPSPlayerController::MoveLeft(const FInputActionValue& Value)
{
	if (ACharacter* PlayerCharacter = Cast<ACharacter>(GetPawn()))
	{
		const FVector Direction = FRotationMatrix(FRotator(0.f, GetControlRotation().Yaw, 0.f)).GetUnitAxis(EAxis::Y);
		PlayerCharacter->AddMovementInput(Direction, -Value.Get<float>());
	}

	bIsWalking = true;
	bStoppedWalkingHoriz = false;
	
	//if(GetCharacter()->GetMesh())
	//{
		//if(USWAT_AnimInstance* AnimInstance = Cast<USWAT_AnimInstance>(GetCharacter()->GetMesh()->GetAnimInstance()))
		//{
			AnimationInstance->HorizontalWalk = FMath::Lerp(AnimationInstance->HorizontalWalk, Value.Get<float>() * 100.f,  DeltaT * 2.f);
			AnimationInstance->HorizontalWalk = FMath::Clamp(AnimationInstance->HorizontalWalk, 0.f, 100.f);
			//LookValue.Y = FMath::Clamp(LookValue.Y, -50.f, 50.f);
			
		//}
	//}
}

void AFPSPlayerController::MoveRight(const FInputActionValue& Value)
{
	if (ACharacter* PlayerCharacter = Cast<ACharacter>(GetPawn()))
	{
		const FVector Direction = FRotationMatrix(FRotator(0.f, GetControlRotation().Yaw, 0.f)).GetUnitAxis(EAxis::Y);
		PlayerCharacter->AddMovementInput(Direction, Value.Get<float>());
	}

	bIsWalking = true;
	bStoppedWalkingHoriz = false;
	
	//if(GetCharacter()->GetMesh())
	//{
		//if(USWAT_AnimInstance* AnimInstance = Cast<USWAT_AnimInstance>(GetCharacter()->GetMesh()->GetAnimInstance()))
		//{
			AnimationInstance->HorizontalWalk = FMath::Lerp(AnimationInstance->HorizontalWalk, Value.Get<float>() * -100.f,  DeltaT * 2.f);
			AnimationInstance->HorizontalWalk = FMath::Clamp(AnimationInstance->HorizontalWalk, -100.f, 0.f);
			//LookValue.Y = FMath::Clamp(LookValue.Y, -50.f, 50.f);
			
		//}
	//}
}

// void AFPSPlayerController::LookAround(const FInputActionValue& Value)
// {
// 	if (ACharacter* PlayerCharacter = Cast<ACharacter>(GetPawn()))
// 	{
// 		const FVector2D LookValue = Value.Get<FVector2D>();
// 		PlayerCharacter->AddControllerYawInput(LookValue.X);
// 		PlayerCharacter->AddControllerPitchInput(-LookValue.Y);
//
//
// 		//if(GetCharacter()->GetMesh())
// 		//{
// 			//if(USWAT_AnimInstance* AnimInstance = Cast<USWAT_AnimInstance>(GetCharacter()->GetMesh()->GetAnimInstance()))
// 			//{
// 				AnimationInstance->VerticalBend = FMath::Lerp(AnimationInstance->VerticalBend, LookValue.Y * 50.f,  DeltaT * 2.f);
// 				AnimationInstance->VerticalBend = FMath::Clamp(AnimationInstance->VerticalBend, -50.f, 50.f);
// 				//LookValue.Y = FMath::Clamp(LookValue.Y, -50.f, 50.f);
// 			
// 			//}
// 		//}
// 	}
// }

void AFPSPlayerController::LookAround(const FInputActionValue& Value)
{
	if (ACharacter* PlayerCharacter = Cast<ACharacter>(GetPawn()))
	{
		const FVector2D LookValue = Value.Get<FVector2D>();
		// PlayerCharacter->AddControllerYawInput(LookValue.X * 1.5f);
		// PlayerCharacter->AddControllerPitchInput(-LookValue.Y * 1.5f);
		
		// if (UCameraComponent* Camera = PlayerCharacter->FindComponentByClass<UCameraComponent>())
		// {
		// 	float PitchAngle = PlayerCharacter->GetControlRotation().Pitch;
		// 	float Offset = FMath::Clamp(PitchAngle * 0.1f, 0.f, 10.f);
		//
		// 	FVector NewLocation = Camera->GetRelativeLocation();
		// 	NewLocation.Z = Offset;
		// 	Camera->SetRelativeLocation(NewLocation);
		// }
		float Pitch = 0.f;

		if (AMyFPSCharacter* Pl = Cast<AMyFPSCharacter>(PlayerCharacter))
		{
			if (Pl->HUD->SniperScopeWidget->IsVisible())
			{
				PlayerCharacter->AddControllerYawInput(LookValue.X * 2.f);
				PlayerCharacter->AddControllerPitchInput(-LookValue.Y * 2.f);
			}
			else
			{
				PlayerCharacter->AddControllerYawInput(LookValue.X * 1.5f);
				PlayerCharacter->AddControllerPitchInput(-LookValue.Y * 1.5f);
				
			}
			
			if (Pl->Inventory->GetNumberOfItems() == 0)
			{
				if (UCameraComponent* Camera = PlayerCharacter->FindComponentByClass<UCameraComponent>())
				{
					Pitch = PlayerCharacter->GetControlRotation().Pitch;
					float NormalizedPitch = FMath::Clamp(Pitch / 80.f, -1.f, 1.f);  
			
					float DownOffset = FMath::Lerp(0.f, 10.f, NormalizedPitch);
					float UpOffset = FMath::Lerp(0.f, 80.f, NormalizedPitch);

					// INFO: Use the correct offset depending on the direction
					float Offset = Pitch > 89 ? DownOffset : UpOffset;

					
					//float Offset = FMath::Lerp(0.f, 10.f, (NormalizedPitch + 1.f) / 2.f); // Map NormalizedPitch to -20.f to 20.f

					FVector NewLocation = Camera->GetRelativeLocation();
					NewLocation.Z = Offset;
					Camera->SetRelativeLocation(NewLocation);
				}
				if (Pitch <= 90)
				{
					Pl->GetMesh()->SetVisibility(false);
				}
				else
				{
					Pl->GetMesh()->SetVisibility(true);
				}
			}
			
		}

		AnimationInstance->VerticalBend = FMath::Lerp(AnimationInstance->VerticalBend, LookValue.Y * 50.f,  DeltaT * 2.f);
		//AnimationInstance->VerticalBend = FMath::Clamp(AnimationInstance->VerticalBend, -50.f, 50.f);
	}
}

void AFPSPlayerController::NextWeapon()
{
	
	UE_LOG(LogTemp, Warning, TEXT("Swapping to next weapon"));

	if(AMyFPSCharacter* C = Cast<AMyFPSCharacter>(GetCharacter()))
	{
		C->NextWeapon();
	}
}

void AFPSPlayerController::PreviousWeapon()
{
	UE_LOG(LogTemp, Warning, TEXT("Swapping to previous weapon"));

	if(AMyFPSCharacter* C = Cast<AMyFPSCharacter>(GetCharacter()))
	{
		C->PreviousWeapon();
	}
}

void AFPSPlayerController::EquipWeapon(int Index)
{

	AnimationInstance->bHasPrimary = false;
	AnimationInstance->bHasPistol = false;
	AnimationInstance->bHasGrenade = false;
	AnimationInstance->bHasKnife = false;
	
	switch (Index)
	{
	case 0: AnimationInstance->bHasPrimary = true; break;
	case 1: AnimationInstance->bHasPistol = true; break;
	case 2:
	case 3:
	case 4:
		AnimationInstance->bHasGrenade = true; break;
	case 5: AnimationInstance->bHasKnife = true; break;
	case 10: AnimationInstance->bHasPrimary = false; AnimationInstance->bHasPistol = false; AnimationInstance->bHasGrenade = false; AnimationInstance->bHasKnife = false; break; 
	default:
		UE_LOG(LogTemp, Error, TEXT("Invalid weapon index: %d"), Index);
		return;
	}

	AnimationInstance->Modify();

	UE_LOG(LogTemp, Warning, TEXT("Weapon index is %d"), Index);
}

void AFPSPlayerController::ThrowItem()
{
	if(AMyFPSCharacter* C = Cast<AMyFPSCharacter>(GetCharacter()))
	{
		C->ThrowWeapon();
	}
}

void AFPSPlayerController::Shoot()
{
	//UE_LOG(LogTemp, Warning, TEXT("Shooting"));

	//if(USWAT_AnimInstance* AnimInstance = Cast<USWAT_AnimInstance>(GetCharacter()->GetMesh()->GetAnimInstance()))
	//{
		//if(AnimationInstance->bIsShooting)
		//{
		//	return;
		//}
	//}
	
	if(AMyFPSCharacter* PlayerCharacter = Cast<AMyFPSCharacter>(GetCharacter()))
	{
		PlayerCharacter->Shoot();
	}
}

void AFPSPlayerController::Reload()
{
	UE_LOG(LogTemp, Warning, TEXT("Reloading"));

	//if(USWAT_AnimInstance* AnimInstance = Cast<USWAT_AnimInstance>(GetCharacter()->GetMesh()->GetAnimInstance()))
	//{
		if(AnimationInstance->bIsReloading)
		{
			return;
		}
	//}
	
	if(AMyFPSCharacter* PlayerCharacter = Cast<AMyFPSCharacter>(GetCharacter()))
	{
		PlayerCharacter->Reload();
	}
}

void AFPSPlayerController::PeekRight(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Peeking right"));

	bIsPeeking = true;
	bPeekingCompleted = false;
	
	//if(GetCharacter()->GetMesh())
	//{
		//if(USWAT_AnimInstance* AnimInstance = Cast<USWAT_AnimInstance>(GetCharacter()->GetMesh()->GetAnimInstance()))
		//{
			AnimationInstance->HorizontalBend = FMath::Lerp(AnimationInstance->HorizontalBend, Value.Get<float>() * -50.f,  DeltaT * 10.f);
			AnimationInstance->HorizontalBend = FMath::Clamp(AnimationInstance->HorizontalBend, -50.f, 0.f);
			
		//}
	//}
}

void AFPSPlayerController::PeekLeft(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Peeking left"));
	
	bIsPeeking = true;
	bPeekingCompleted = false;
	
	//if(GetCharacter()->GetMesh())
	//{
		//if(USWAT_AnimInstance* AnimInstance = Cast<USWAT_AnimInstance>(GetCharacter()->GetMesh()->GetAnimInstance()))
		//{
			AnimationInstance->HorizontalBend = FMath::Lerp(AnimationInstance->HorizontalBend, Value.Get<float>() * 50.f,  DeltaT * 10.f);
			AnimationInstance->HorizontalBend = FMath::Clamp(AnimationInstance->HorizontalBend, 0.f, 50.f);
		//}
	//}
}

void AFPSPlayerController::Crouch()
{
	
	if(GetCharacter())
	{
		GetCharacter()->Crouch();
		//if(USWAT_AnimInstance* AnimInstance = Cast<USWAT_AnimInstance>(GetCharacter()->GetMesh()->GetAnimInstance()))
		//{
			AnimationInstance->bIsCrouching = true;
			bIsCrouching = true;
			UE_LOG(LogTemp, Warning, TEXT("Crouching"));
		//}
	}
	

	
}

void AFPSPlayerController::Aim()
{
	if(AMyFPSCharacter* PlayerCharacter = Cast<AMyFPSCharacter>(GetCharacter()))
	{
		PlayerCharacter->Aim();
	}
}

void AFPSPlayerController::StopAiming()
{
	if(AMyFPSCharacter* PlayerCharacter = Cast<AMyFPSCharacter>(GetCharacter()))
	{
		PlayerCharacter->StopAiming();
	}
}

void AFPSPlayerController::AimGrenade()
{
	UE_LOG(LogTemp, Warning, TEXT("Aiming grenade"));
}

void AFPSPlayerController::ThrowGrenade()
{
	UE_LOG(LogTemp, Warning, TEXT("Throwing grenade"));

	//if(USWAT_AnimInstance* AnimInstance = Cast<USWAT_AnimInstance>(GetCharacter()->GetMesh()->GetAnimInstance()))
	//{
		if(AnimationInstance->bIsThrowingGrenade)
		{
			return;
		}
	//}
	
	if(AMyFPSCharacter* PlayerCharacter = Cast<AMyFPSCharacter>(GetCharacter()))
	{
		PlayerCharacter->ThrowGrenade();
	}
}

void AFPSPlayerController::ResetPeeking()
{
	bIsPeeking = false;
}

void AFPSPlayerController::StopWalking()
{
	bIsWalking = false;
}

void AFPSPlayerController::StopCrouching()
{
	if(GetCharacter())
	{
		GetCharacter()->Crouch();
		//if(USWAT_AnimInstance* AnimInstance = Cast<USWAT_AnimInstance>(GetCharacter()->GetMesh()->GetAnimInstance()))
		//{
			AnimationInstance->bIsCrouching = false;
			bIsCrouching = false;
			UE_LOG(LogTemp, Warning, TEXT("Crouching"));
		//}
	}
	//bIsCrouching = false;
}

// Called every frame
void AFPSPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DeltaT = DeltaTime;

	if(!bIsPeeking && !bPeekingCompleted)
	{
		//if(GetCharacter()->GetMesh())
		//{
			//if(USWAT_AnimInstance* AnimInstance = Cast<USWAT_AnimInstance>(GetCharacter()->GetMesh()->GetAnimInstance()))
			//{
				AnimationInstance->HorizontalBend = FMath::Lerp(AnimationInstance->HorizontalBend,0.f,  DeltaT * 10.f);
				bPeekingCompleted = FMath::IsNearlyEqual(AnimationInstance->HorizontalBend, 0.f, 0.01f);
				//AnimInstance->HorizontalBend = FMath::Clamp(AnimInstance->HorizontalBend, 0.f, 50.f);
			//}
		//}
	}

	if(!bIsWalking && !bStoppedWalkingVert)
	{
		//if(GetCharacter()->GetMesh())
		//{
			//if(USWAT_AnimInstance* AnimInstance = Cast<USWAT_AnimInstance>(GetCharacter()->GetMesh()->GetAnimInstance()))
			//{
				AnimationInstance->VerticalWalk = FMath::Lerp(AnimationInstance->VerticalWalk, 0.f,  DeltaT * 10.f);
				bStoppedWalkingVert = FMath::IsNearlyEqual(AnimationInstance->VerticalWalk, 0.f, 0.1f);
				//AnimInstance->HorizontalWalk = 0.f;
				//AnimInstance->VerticalWalk = 0.f;
			//}
		//}
	}

	if(!bIsWalking && !bStoppedWalkingHoriz)
	{
		//if(GetCharacter()->GetMesh())
		//{
			//if(USWAT_AnimInstance* AnimInstance = Cast<USWAT_AnimInstance>(GetCharacter()->GetMesh()->GetAnimInstance()))
			//{
				AnimationInstance->HorizontalWalk = FMath::Lerp(AnimationInstance->HorizontalWalk, 0.f,  DeltaT * 10.f);
				bStoppedWalkingHoriz = FMath::IsNearlyEqual(AnimationInstance->HorizontalWalk, 0.f, 0.1f);
				//AnimInstance->HorizontalWalk = 0.f;
				//AnimInstance->VerticalWalk = 0.f;
			//}
		//}
	}

	if(!bIsCrouching && !bCrouchingCompleted)
	{
		//if(GetCharacter()->GetMesh())
		//{
			//if(USWAT_AnimInstance* AnimInstance = Cast<USWAT_AnimInstance>(GetCharacter()->GetMesh()->GetAnimInstance()))
			//{
				//AnimInstance->HorizontalWalk = FMath::Lerp(AnimInstance->HorizontalWalk, 0.f,  DeltaT * 10.f);
				bCrouchingCompleted = FMath::IsNearlyEqual(GetCharacter()->CrouchedEyeHeight, GetCharacter()->GetDefaultHalfHeight(), 0.1f);

				if(bCrouchingCompleted)
				{
					AnimationInstance->bIsCrouching = false;
				}
			//}
		//}
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
			EnhancedInputComponent->BindAction(*InputActions.Find(EInputActionKey::IAK_NextWeapon), ETriggerEvent::Started, this, &AFPSPlayerController::NextWeapon);
			EnhancedInputComponent->BindAction(*InputActions.Find(EInputActionKey::IAK_PreviousWeapon), ETriggerEvent::Started, this, &AFPSPlayerController::PreviousWeapon);
			EnhancedInputComponent->BindAction(*InputActions.Find(EInputActionKey::IAK_ThrowItem), ETriggerEvent::Triggered, this, &AFPSPlayerController::ThrowItem);

			EnhancedInputComponent->BindAction(*InputActions.Find(EInputActionKey::IAK_MoveForward), ETriggerEvent::Triggered, this, &AFPSPlayerController::MoveForward);
			EnhancedInputComponent->BindAction(*InputActions.Find(EInputActionKey::IAK_MoveBackwards), ETriggerEvent::Triggered, this, &AFPSPlayerController::MoveBackwards);
			EnhancedInputComponent->BindAction(*InputActions.Find(EInputActionKey::IAK_MoveLeft), ETriggerEvent::Triggered, this, &AFPSPlayerController::MoveLeft);
			EnhancedInputComponent->BindAction(*InputActions.Find(EInputActionKey::IAK_MoveRight), ETriggerEvent::Triggered, this, &AFPSPlayerController::MoveRight);
			
			EnhancedInputComponent->BindAction(*InputActions.Find(EInputActionKey::IAK_Look), ETriggerEvent::Triggered, this, &AFPSPlayerController::LookAround);

			EnhancedInputComponent->BindAction(*InputActions.Find(EInputActionKey::IAK_Shoot), ETriggerEvent::Triggered, this, &AFPSPlayerController::Shoot);
			EnhancedInputComponent->BindAction(*InputActions.Find(EInputActionKey::IAK_Reload), ETriggerEvent::Triggered, this, &AFPSPlayerController::Reload);

			EnhancedInputComponent->BindAction(*InputActions.Find(EInputActionKey::IAK_PeekRight), ETriggerEvent::Triggered, this, &AFPSPlayerController::PeekRight);
			EnhancedInputComponent->BindAction(*InputActions.Find(EInputActionKey::IAK_PeekLeft), ETriggerEvent::Triggered, this, &AFPSPlayerController::PeekLeft);
			EnhancedInputComponent->BindAction(*InputActions.Find(EInputActionKey::IAK_PeekRight), ETriggerEvent::Completed, this, &AFPSPlayerController::ResetPeeking);
			EnhancedInputComponent->BindAction(*InputActions.Find(EInputActionKey::IAK_PeekLeft), ETriggerEvent::Completed, this, &AFPSPlayerController::ResetPeeking);

			EnhancedInputComponent->BindAction(*InputActions.Find(EInputActionKey::IAK_Crouch), ETriggerEvent::Triggered, this, &AFPSPlayerController::Crouch);
			EnhancedInputComponent->BindAction(*InputActions.Find(EInputActionKey::IAK_Crouch), ETriggerEvent::Completed, this, &AFPSPlayerController::StopCrouching);

			EnhancedInputComponent->BindAction(*InputActions.Find(EInputActionKey::IAK_Aim), ETriggerEvent::Triggered, this, &AFPSPlayerController::Aim);
			EnhancedInputComponent->BindAction(*InputActions.Find(EInputActionKey::IAK_Aim), ETriggerEvent::Completed, this, &AFPSPlayerController::StopAiming);

			EnhancedInputComponent->BindAction(*InputActions.Find(EInputActionKey::IAK_AimGrenade), ETriggerEvent::Triggered, this, &AFPSPlayerController::AimGrenade);
			//EnhancedInputComponent->BindAction(*InputActions.Find(EInputActionKey::IAK_ThrowGrenade), ETriggerEvent::Triggered, this, &AFPSPlayerController::ThrowGrenade);

			
			EnhancedInputComponent->BindAction(*InputActions.Find(EInputActionKey::IAK_MoveForward), ETriggerEvent::Completed, this, &AFPSPlayerController::StopWalking);
			EnhancedInputComponent->BindAction(*InputActions.Find(EInputActionKey::IAK_MoveBackwards), ETriggerEvent::Completed, this, &AFPSPlayerController::StopWalking);
			EnhancedInputComponent->BindAction(*InputActions.Find(EInputActionKey::IAK_MoveLeft), ETriggerEvent::Completed, this, &AFPSPlayerController::StopWalking);
			EnhancedInputComponent->BindAction(*InputActions.Find(EInputActionKey::IAK_MoveRight), ETriggerEvent::Completed, this, &AFPSPlayerController::StopWalking);
		}
	}
}

