// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseWeapon.h"
#include "FirstPersonTD/InventoryItems/Interfaces/SecondaryWeapon.h"

#include "Pistol.generated.h"

UCLASS()
class FIRSTPERSONTD_API APistol : public ABaseWeapon, public ISecondaryWeapon
{
	GENERATED_BODY()

public:
	APistol();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void Shoot() override;
	virtual void Reload() override;
};
