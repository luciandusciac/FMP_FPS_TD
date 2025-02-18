// Fill out your copyright notice in the Description page of Project Settings.


#include "Shotgun.h"

#include "Engine/World.h"


// Sets default values
AShotgun::AShotgun()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AShotgun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShotgun::Tick(float DeltaTime)
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

void AShotgun::Shoot()
{
	Super::Shoot();
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	for(int i = 0; i<=PelletCount;  ++i)
	{
		
		FRotator RandomRotation = BulletOrigin->GetComponentRotation();
		RandomRotation.Yaw += FMath::RandRange(-Spread, Spread);
		RandomRotation.Pitch += FMath::RandRange(-Spread, Spread);

		GetWorld()->SpawnActor<ABaseProjectile>(WeaponBullet, BulletOrigin->GetComponentLocation(), RandomRotation, SpawnParams);
	
	}

	UE_LOG(LogTemp, Warning, TEXT("Shotgun Shot"));
}

