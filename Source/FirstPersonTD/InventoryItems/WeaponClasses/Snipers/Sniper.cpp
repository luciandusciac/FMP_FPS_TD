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
		bIsShooting = true;
		GetWorldTimerManager().SetTimer(ShootingTimerHandle, this, &ABaseWeapon::OnShoot, FireRate, false);
	
	
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		GetWorld()->SpawnActor<ABaseProjectile>(WeaponBullet, BulletOrigin->GetComponentLocation(), BulletOrigin->GetComponentRotation(), SpawnParams);

		//UE_LOG(LogTemp, Warning, TEXT("Sniper Shot"));
	}
}

