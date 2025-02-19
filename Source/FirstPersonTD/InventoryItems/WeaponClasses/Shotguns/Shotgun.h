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

private:
	int PelletCount = 8;
	float Spread = 10;
};
