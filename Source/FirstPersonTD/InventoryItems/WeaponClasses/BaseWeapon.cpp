// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"
#include "Components/StaticMeshComponent.h"
#include "FirstPersonTD/Characters/MyFPSCharacter.h"
#include "FirstPersonTD/Controller/FPSPlayerController.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABaseWeapon::ABaseWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// //FSetActorReplicates Replicates = FSetActorReplicates(cast<AActor>(this), true);
	// bReplicates = true;
	// bNetLoadOnClient = true;
	// SetReplicates(true);
	
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

	BulletOrigin = CreateDefaultSubobject<USceneComponent>(TEXT("BulletOrigin"));
	BulletOrigin->SetupAttachment(Mesh);

	AimOrigin = CreateDefaultSubobject<USceneComponent>(TEXT("AimOrigin"));
	AimOrigin->SetupAttachment(Mesh);

	bIsShooting = false;
}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	CurrentAmmo = ClipSize;
	
	
	// if(!CurrentOwner)
	// 	Mesh->SetVisibility(false);
	//Mesh->IgnoreActorWhenMoving(Cast<AActor>(WeaponBullet), true);
}

// Called every frame
void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseWeapon::Shoot()
{
	if(AFPSPlayerController* controller = Cast<AFPSPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		if (AMyFPSCharacter* C = Cast<AMyFPSCharacter>(controller->GetCharacter()))
		{
			//C->AnimationInstance->bIsReloading = false;
			C->UpdateAmmoUI();
		}
	}
	// if(AMyFPSCharacter* C = Cast<AMyFPSCharacter>(GetOuter()))
	// {
	// 	//C->AnimationInstance->AnimationIndex = CurrentInventorySlot;
	// 	if(AFPSPlayerController* controller = Cast<AFPSPlayerController>(C->GetController()))
	// 	{
	// 		controller->Shoot();
	// 		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Character is shooting"));
	// 		//C->Shoot();
	// 	}
	// }
	// if (CurrentAmmo > 0)
 //    {
 //     	CurrentAmmo--;
 //    }
 //    else if (CurrentAmmo == 0 && ReserveAmmo > 0)
 //    {
 //     	Reload();
 //    }
 //    else
 //    {
 //     	//TODO: Play error sound
 //     	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No ammo"));
 //    	
 //    }
}

void ABaseWeapon::OnShoot()
{
	bIsShooting = false;
	GetWorldTimerManager().ClearTimer(ShootingTimerHandle);
}

void ABaseWeapon::Reload()
{
	//if(AMyFPSCharacter* C = Cast<AMyFPSCharacter>(GetOwner()))
	//{
		//C->AnimationInstance->AnimationIndex = CurrentInventorySlot;
		if(AFPSPlayerController* controller = Cast<AFPSPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
		{
			//C->Reload();
			// if(AMyFPSCharacter* C = Cast<AMyFPSCharacter>(GetOwner()))
			// {
			// 	C->UpdateAmmoUI();
			// }
			
			
			if(ReserveAmmo > 0)
			{
				GetWorldTimerManager().SetTimer(ReloadTimerHandle, this, &ABaseWeapon::OnReload, ReloadTime, false);
				//CurrentAmmo = ClipSize;
				ReserveAmmo--;
				if (AMyFPSCharacter* C = Cast<AMyFPSCharacter>(controller->GetCharacter()))
				{
					C->AnimationInstance->bIsReloading = true;
					//C->UpdateAmmoUI();
				}
				//controller->Reload();
			}
			else
			{
				//TODO: Play error sound
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No reserve ammo"));
			}
		}
	//}
	
}

void ABaseWeapon::OnReload()
{
	CurrentAmmo = ClipSize;
	if(AFPSPlayerController* controller = Cast<AFPSPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		if (AMyFPSCharacter* C = Cast<AMyFPSCharacter>(controller->GetCharacter()))
		{
			C->AnimationInstance->bIsReloading = false;
			C->UpdateAmmoUI();
		}
	}
	GetWorldTimerManager().ClearTimer(ReloadTimerHandle);
	//ReserveAmmo--;
}

void ABaseWeapon::Aim()
{
}

void ABaseWeapon::Use()
{
	Super::Use();

	Shoot();
}

int ABaseWeapon::GetCurrentAmmo()
{
	return CurrentAmmo;
}

void ABaseWeapon::SetCurrentAmmo(const int NewCurrentAmmo)
{
	CurrentAmmo = NewCurrentAmmo;
}

int ABaseWeapon::GetReserveAmmo()
{
	return ReserveAmmo;
}

void ABaseWeapon::SetReserveAmmo(const int NewReserveAmmo)
{
	ReserveAmmo = NewReserveAmmo;
}

