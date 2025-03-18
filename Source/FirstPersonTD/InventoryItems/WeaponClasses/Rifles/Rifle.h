// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseWeapon.h"
#include "FirstPersonTD/InventoryItems/Interfaces/PrimaryWeapon.h"

#include "Rifle.generated.h"

UCLASS()
class FIRSTPERSONTD_API ARifle : public ABaseWeapon, public IPrimaryWeapon
{
	GENERATED_BODY()

public:
	
	ARifle();

protected:
	
	virtual void BeginPlay() override;

public:
	
	virtual void Tick(float DeltaTime) override;

	virtual void Shoot() override;
	virtual void Reload() override;

	
};
