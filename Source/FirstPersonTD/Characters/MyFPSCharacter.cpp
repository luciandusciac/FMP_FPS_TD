// Fill out your copyright notice in the Description page of Project Settings.
#include "MyFPSCharacter.h"

#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/Controller.h"
#include "Net/UnrealNetwork.h"
#include "../Source/FirstPersonTD/InventoryItems/WeaponClasses/BaseWeapon.h"
#include "Components/CapsuleComponent.h"
#include "FirstPersonTD/Animations/SWAT_AnimInstance.h"

class ABaseWeapon;

// Sets default values
AMyFPSCharacter::AMyFPSCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->bUsePawnControlRotation = true;
	Camera->SetupAttachment(GetMesh(), "Head");

	//Inventory = NewObject<UInventory>(this, UInventory::StaticClass());

	//Inventory = CreateDefaultSubobject<UInventory>(TEXT("Inventory"));
	
	this->GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMyFPSCharacter::OnComponentBeginOverlap);
}

// Called when the game starts or when spawned
void AMyFPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	ReloadingTime = ReloadingAnimation->GetPlayLength();
	ShootingTime = ShootingAnimation->GetPlayLength();
	PistolShootingTime = PistolShootingAnimation->GetPlayLength();
	//DeathTime = DeathAnimation->GetPlayLength();
	GrenadeThrowTime = GrenadeThrowAnimation->GetPlayLength();

	Inventory = NewObject<UInventory>(this);
	
	// if(HasAuthority())
	// {
	// 	for(const TSubclassOf<ABaseWeapon>& WeaponClass : DefaultWeapons)
	// 	{
	// 		if(!WeaponClass) continue;
	// 		FActorSpawnParameters SpawnParams;
	// 		//SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	// 		SpawnParams.Owner = this;
	// 		//SpawnParams.Instigator = this;
	// 		ABaseWeapon* Weapon;
	// 		
	// 		if(WeaponClass && GetWorld())
	// 		{
	// 			Weapon = GetWorld()->SpawnActor<ABaseWeapon>(WeaponClass, SpawnParams);
	// 			
	// 		}
	//
	// 		const int32 index = Weapons.Add(Weapon);
	// 		if(index == CurrentWeaponIndex)
	// 		{
	// 			CurrentWeapon = Weapon;
	// 			OnRep_CurrentWeapon(nullptr);
	// 		}
	// 	}
	// }
}

// void AMyFPSCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
// {
// 	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//
// 	DOREPLIFETIME(AMyFPSCharacter, Weapons);
// 	DOREPLIFETIME(AMyFPSCharacter, CurrentWeapon);
// 	DOREPLIFETIME(AMyFPSCharacter, CurrentWeaponIndex);
// }
//
// void AMyFPSCharacter::OnRep_CurrentWeapon(const class ABaseWeapon* LastWeapon)
// {
// 	// if(CurrentWeapon)
// 	// {
// 	// 	if(!CurrentWeapon->CurrentOwner)
// 	// 	{
// 	// 		const FTransform SocketTransform = CurrentWeapon->PlacementTransform * GetMesh()->GetSocketTransform(FName("WeaponSocket"));
// 	// 		CurrentWeapon->SetActorTransform(SocketTransform, false, nullptr, ETeleportType::TeleportPhysics);
// 	// 		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform, "WeaponSocket");
// 	//
// 	// 		CurrentWeapon->CurrentOwner = this;
// 	// 		CurrentWeapon->SetOwner(this);
// 	// 	}
// 	// 	CurrentWeapon->Mesh->SetVisibility(true);
// 	// 	wprintf(TEXT("%d : Visible"), GetLocalRole());
// 	// }
// 	//
// 	// if(LastWeapon)
// 	// {
// 	// 	LastWeapon->Mesh->SetVisibility(false);
// 	// }
// }
//
// void AMyFPSCharacter::EquipWeapon(const int32 Index)
// {
// 	// if(!Weapons.IsValidIndex(Index) || CurrentWeapon == Weapons[Index]) return;
// 	//
// 	// if(IsLocallyControlled())
// 	// {
// 	// 	CurrentWeaponIndex = Index;
// 	// 	
// 	// 	const ABaseWeapon* LastWeapon = CurrentWeapon;
// 	// 	CurrentWeapon = Weapons[Index];
// 	// 	OnRep_CurrentWeapon(LastWeapon);
// 	// 	Server_SetCurrentWeapon(Weapons[Index]);
// 	// }
// 	// else if(!HasAuthority())
// 	// {
// 	// 	Server_SetCurrentWeapon(Weapons[Index]);	
// 	// }
//
// 	
// 	// if (HasAuthority()) // Server
// 	// {
// 	// 	const ABaseWeapon* LastWeapon = CurrentWeapon;
// 	// 	CurrentWeapon = Weapons[Index];
// 	// 	CurrentWeaponIndex = Index;
// 	// 	OnRep_CurrentWeapon(LastWeapon); // Update state
// 	// }
// 	// else if (IsLocallyControlled()) // Client
// 	// {
// 	// 	Server_SetCurrentWeapon(Weapons[Index]); // Notify server
// 	// }
// 	
// }
//
// void AMyFPSCharacter::SwapWeapon()
// {
// 	// const int32 Index = Weapons.IsValidIndex(CurrentWeaponIndex + 1) ? CurrentWeaponIndex + 1 : 0;
// 	// EquipWeapon(Index);
// }
//
// void AMyFPSCharacter::Server_SetCurrentWeapon_Implementation(ABaseWeapon* NewWeapon)
// {
// 	// const ABaseWeapon* LastWeapon = CurrentWeapon;
// 	// CurrentWeapon = NewWeapon;
// 	// OnRep_CurrentWeapon(LastWeapon);
// 	
// 	// if (Weapons.Contains(NewWeapon)) // Validate the weapon
// 	// {
// 	// 	const ABaseWeapon* LastWeapon = CurrentWeapon;
// 	// 	CurrentWeapon = NewWeapon;
// 	//
// 	// 	// Update index for replication
// 	// 	CurrentWeaponIndex = Weapons.IndexOfByKey(NewWeapon);
// 	//
// 	// 	OnRep_CurrentWeapon(LastWeapon);
// 	// }
// }

// Called to bind functionality to input
// void AMyFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
// {
// 	Super::SetupPlayerInputComponent(PlayerInputComponent);
// 	
// }

void AMyFPSCharacter::ThrowGrenade()
{
	//GetMesh()->PlayAnimation(GrenadeThrowAnimation, false);
	GetWorldTimerManager().SetTimer(AnimationTimerHandle, this, &AMyFPSCharacter::OnGrenadeThrown, GrenadeThrowTime, false);
	
	if(USWAT_AnimInstance* AnimInstance = Cast<USWAT_AnimInstance>(GetMesh()->GetAnimInstance()))
	{
		AnimInstance->bIsThrowingGrenade = true;
	}
}

void AMyFPSCharacter::OnGrenadeThrown()
{
	GetWorldTimerManager().ClearTimer(AnimationTimerHandle);
	
	if(USWAT_AnimInstance* AnimInstance = Cast<USWAT_AnimInstance>(GetMesh()->GetAnimInstance()))
	{
		AnimInstance->bIsThrowingGrenade = false;
		AnimInstance->bHasGrenade = false;
	}
}

void AMyFPSCharacter::Shoot()
{
	//GetMesh()->PlayAnimation(ShootingAnimation, false);
	if(USWAT_AnimInstance* AnimInstance = Cast<USWAT_AnimInstance>(GetMesh()->GetAnimInstance()))
	{
		if(AnimInstance->bHasPistol)
		{
			GetWorldTimerManager().SetTimer(AnimationTimerHandle, this, &AMyFPSCharacter::OnShoot, PistolShootingTime, false);
			
		}
		else
		{
			GetWorldTimerManager().SetTimer(AnimationTimerHandle, this, &AMyFPSCharacter::OnShoot, ShootingTime, false);
			
		}
		AnimInstance->bIsShooting = true;
	}
	// GetWorldTimerManager().SetTimer(AnimationTimerHandle, this, &AMyFPSCharacter::OnShoot, ShootingTime, false);
	//
	// if(USWAT_AnimInstance* AnimInstance = Cast<USWAT_AnimInstance>(GetMesh()->GetAnimInstance()))
	// {
	// 	AnimInstance->bIsShooting = true;
	// }
}

void AMyFPSCharacter::OnShoot()
{
	GetWorldTimerManager().ClearTimer(AnimationTimerHandle);
	
	if(USWAT_AnimInstance* AnimInstance = Cast<USWAT_AnimInstance>(GetMesh()->GetAnimInstance()))
	{
		AnimInstance->bIsShooting = false;

		// if(AnimInstance->bHasPistol)
		// {
		// 	//GetMesh()->PlayAnimation(PistolShootingAnimation, false);
		// 	AnimInstance->PlaySlotAnimationAsDynamicMontage(PistolShootingAnimation, FName("Spine"));
		// }
		// else
		// {
		// 	//GetMesh()->PlayAnimation(ShootingAnimation, false);
		// 	AnimInstance->PlaySlotAnimationAsDynamicMontage(ShootingAnimation, FName("Spine"));
		// }
	}
}

void AMyFPSCharacter::Aim()
{
}

void AMyFPSCharacter::Die()
{
	GetMesh()->PlayAnimation(DeathAnimation, false);
}

void AMyFPSCharacter::OnDeath()
{
}

void AMyFPSCharacter::Reload()
{
	//GetMesh()->PlayAnimation(ReloadingAnimation, false);
	GetWorldTimerManager().SetTimer(AnimationTimerHandle, this, &AMyFPSCharacter::OnReload, ReloadingTime, false);
	
	if(USWAT_AnimInstance* AnimInstance = Cast<USWAT_AnimInstance>(GetMesh()->GetAnimInstance()))
	{
		AnimInstance->bIsReloading = true;

		// if(AnimInstance->bHasPistol)
		// {
		// 	//GetMesh()->PlayAnimation(PistolReloadingAnimation, false);
		// 	AnimInstance->PlaySlotAnimationAsDynamicMontage(PistolReloadingAnimation, FName("Spine"));
		// }
		// else
		// {
		// 	//GetMesh()->PlayAnimation(ReloadingAnimation, false);
		// 	AnimInstance->PlaySlotAnimationAsDynamicMontage(ReloadingAnimation, FName("Spine"));
		// }
	}
}

void AMyFPSCharacter::OnReload()
{
	GetWorldTimerManager().ClearTimer(AnimationTimerHandle);
	
	if(USWAT_AnimInstance* AnimInstance = Cast<USWAT_AnimInstance>(GetMesh()->GetAnimInstance()))
	{
		AnimInstance->bIsReloading = false;
	}
}

void AMyFPSCharacter::NextWeapon()
{
	Inventory->NextItem();
}

void AMyFPSCharacter::PreviousWeapon()
{
	Inventory->PreviousItem();
}

void AMyFPSCharacter::ThrowWeapon()
{
	Inventory->ThrowItem();

	//TODO: Istantiate item and throw it
	
}

void AMyFPSCharacter::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!Inventory)
	{
		UE_LOG(LogTemp, Error, TEXT("Inventory is NULL! Cannot add item."));
		return;
	}
	if(Cast<ABaseWeapon>(OtherActor))
	{
		Inventory->AddItem(Cast<AInventoryItem>(OtherActor));
	}
	
}

