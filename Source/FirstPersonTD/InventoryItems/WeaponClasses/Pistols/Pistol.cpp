// Fill out your copyright notice in the Description page of Project Settings.


#include "Pistol.h"

#include "Engine/World.h"


// Sets default values
APistol::APistol()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APistol::BeginPlay()
{
	Super::BeginPlay();
	
}

void APistol::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// if(CurrentFireRate>FireRate)
	// {
	// 	CurrentFireRate = 0;
	// 	Shoot();
	// }
	// else
	// {
	// 	CurrentFireRate += DeltaTime;
	// }
}

void APistol::Shoot()
{
	//Super::Shoot();


	if (!bIsShooting)
	{
		if (CurrentAmmo > 0)
		{
			CurrentAmmo--;
			
			bIsShooting = true;
			GetWorldTimerManager().SetTimer(ShootingTimerHandle, this, &ABaseWeapon::OnShoot, FireRate, false);
	
	
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			GetWorld()->SpawnActor<ABaseProjectile>(WeaponBullet, BulletOrigin->GetComponentLocation(), BulletOrigin->GetComponentRotation(), SpawnParams);
		}
		else if (CurrentAmmo == 0 && ReserveAmmo > 0)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Character is reloading"));
			Reload();
		}
		else
		{
			//TODO: Play error sound
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No ammo"));
		}

		//UE_LOG(LogTemp, Warning, TEXT("Pistol Shot"));
	}

	
}

void APistol::Reload()
{
	Super::Reload();
}

