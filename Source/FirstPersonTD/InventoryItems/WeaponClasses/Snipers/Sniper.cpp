// Fill out your copyright notice in the Description page of Project Settings.


#include "Sniper.h"

#include "Engine/World.h"


ASniper::ASniper()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASniper::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASniper::Tick(float DeltaTime)
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

void ASniper::Shoot()
{
	
	if (!bIsShooting)
	{
		if (CurrentAmmo > 0)
		{
			CurrentAmmo--;
			
			bIsShooting = true;
	
	
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			GetWorld()->SpawnActor<ABaseProjectile>(WeaponBullet, BulletOrigin->GetComponentLocation(), BulletOrigin->GetComponentRotation(), SpawnParams);
			Super::Shoot();

			
			if (CurrentAmmo == 0)
				Reload();

			
			GetWorldTimerManager().SetTimer(ShootingTimerHandle, this, &ABaseWeapon::OnShoot, FireRate, false);
		}
		// else if (CurrentAmmo == 0 && ReserveAmmo > 0)
		// {
		// 	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Character is reloading"));
		// 	Reload();
		// }
		// else
		// {
		// 	//TODO: Play error sound
		// 	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No ammo"));
		// }
		//UE_LOG(LogTemp, Warning, TEXT("Sniper Shot"));
	}
}

void ASniper::Reload()
{
	Super::Reload();
}

