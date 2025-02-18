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

	if(CurrentFireRate>FireRate)
	{
		CurrentFireRate = 0;
		Shoot();
	}
	else
	{
		CurrentFireRate += DeltaTime;
	}
}

void APistol::Shoot()
{
	Super::Shoot();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	GetWorld()->SpawnActor<ABaseProjectile>(WeaponBullet, BulletOrigin->GetComponentLocation(), BulletOrigin->GetComponentRotation(), SpawnParams);

	UE_LOG(LogTemp, Warning, TEXT("Pistol Shot"));
}

