// Fill out your copyright notice in the Description page of Project Settings.


#include "Rifle.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"


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
	Super::Shoot();


	if (!bIsShooting)
	{
		bIsShooting = true;
		GetWorldTimerManager().SetTimer(ShootingTimerHandle, this, &ABaseWeapon::OnShoot, FireRate, false);
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		GetWorld()->SpawnActor<ABaseProjectile>(WeaponBullet, BulletOrigin->GetComponentLocation(), BulletOrigin->GetComponentRotation(), SpawnParams);

		//UE_LOG(LogTemp, Warning, TEXT("Rifle Shot"));
	}
}

