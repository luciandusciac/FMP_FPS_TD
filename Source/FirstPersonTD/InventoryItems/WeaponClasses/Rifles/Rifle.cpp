// Fill out your copyright notice in the Description page of Project Settings.


#include "Rifle.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "FirstPersonTD/Characters/MyFPSCharacter.h"


// Sets default values
ARifle::ARifle()
{
	PrimaryActorTick.bCanEverTick = true;

	
}

void ARifle::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARifle::Tick(float DeltaTime)
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

void ARifle::Shoot()
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
			{
				Reload();
			}
			GetWorldTimerManager().SetTimer(ShootingTimerHandle, this, &ABaseWeapon::OnShoot, FireRate, false);
		}
	}
}

void ARifle::Reload()
{
	Super::Reload();
}

