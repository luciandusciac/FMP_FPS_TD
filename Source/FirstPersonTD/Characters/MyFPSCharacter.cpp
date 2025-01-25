// Fill out your copyright notice in the Description page of Project Settings.
#include "MyFPSCharacter.h"

#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/Controller.h"
#include "Net/UnrealNetwork.h"
#include "./FirstPersonTD/WeaponClasses/BaseWeapon.h"

class ABaseWeapon;

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

	if(HasAuthority())
	{
		for(const TSubclassOf<ABaseWeapon>& WeaponClass : DefaultWeapons)
		{
			if(!WeaponClass) continue;
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			SpawnParams.Owner = this;
			//SpawnParams.Instigator = this;
			ABaseWeapon* Weapon;
			
			if(WeaponClass && GetWorld())
			{
				Weapon = GetWorld()->SpawnActor<ABaseWeapon>(WeaponClass, SpawnParams);
				
			}

			const int32 index = Weapons.Add(Weapon);
			if(index == CurrentWeaponIndex)
			{
				CurrentWeapon = Weapon;
				OnRep_CurrentWeapon(nullptr);
			}
		}
	}
}

void AMyFPSCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AMyFPSCharacter, Weapons, COND_None);
	DOREPLIFETIME_CONDITION(AMyFPSCharacter, CurrentWeapon, COND_None);
}

void AMyFPSCharacter::OnRep_CurrentWeapon(const class ABaseWeapon* LastWeapon)
{
	if(CurrentWeapon)
	{
		if(!CurrentWeapon->CurrentOwner)
		{
			const FTransform SocketTransform = CurrentWeapon->PlacementTransform * GetMesh()->GetSocketTransform(FName("WeaponSocket"));
			CurrentWeapon->CurrentOwner = this;
			CurrentWeapon->SetActorTransform(SocketTransform, false, nullptr, ETeleportType::TeleportPhysics);
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform, "WeaponSocket");
		}
		CurrentWeapon->Mesh->SetVisibility(true);
	}

	if(LastWeapon)
	{
		LastWeapon->Mesh->SetVisibility(false);
	}
}

void AMyFPSCharacter::EquipWeapon(const int32 Index)
{
	if(!Weapons.IsValidIndex(Index) || CurrentWeapon == Weapons[Index]) return;
	
	if(IsLocallyControlled())
	{
		CurrentWeaponIndex = Index;

		const ABaseWeapon* LastWeapon = CurrentWeapon;
		CurrentWeapon = Weapons[Index];
		OnRep_CurrentWeapon(LastWeapon);
		Server_SetCurrentWeapon(Weapons[Index]);
	}
	else if(!HasAuthority())
	{
		Server_SetCurrentWeapon(Weapons[Index]);	
	}
	
}

void AMyFPSCharacter::SwapWeapon()
{
	const int32 Index = Weapons.IsValidIndex(CurrentWeaponIndex + 1) ? CurrentWeaponIndex + 1 : 0;
	EquipWeapon(Index);
}

void AMyFPSCharacter::Server_SetCurrentWeapon_Implementation(ABaseWeapon* NewWeapon)
{
	const ABaseWeapon* LastWeapon = CurrentWeapon;
	CurrentWeapon = NewWeapon;
	OnRep_CurrentWeapon(LastWeapon);
}

// Called to bind functionality to input
void AMyFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}

