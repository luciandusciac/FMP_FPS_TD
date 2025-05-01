// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseWeapon.h"
#include "FirstPersonTD/InventoryItems/Interfaces/PrimaryWeapon.h"

#include "Shotgun.generated.h"

UCLASS()
class FIRSTPERSONTD_API AShotgun : public ABaseWeapon, public IPrimaryWeapon
{
	GENERATED_BODY()

public:
	
	AShotgun();

protected:
	
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void Shoot() override;
	virtual void Reload() override;

private:
	int PelletCount = 8;
	float Spread = 10;

	FVector CameraLocation;
	FRotator CameraRotation;
	FVector ShotDirection;
	FVector TraceEnd;
	FVector TargetLocation;

	int32 PelletsFired = 0;
	
	float PelletSpawnInterval = 0.015f; // time between each pellet
	FTimerHandle PelletSpawnTimerHandle;

	void SpawnPellet();
};
