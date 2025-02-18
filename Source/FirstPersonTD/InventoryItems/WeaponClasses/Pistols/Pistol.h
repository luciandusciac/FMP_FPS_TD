// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseWeapon.h"

#include "Pistol.generated.h"

UCLASS()
class FIRSTPERSONTD_API APistol : public ABaseWeapon
{
	GENERATED_BODY()

public:
	APistol();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void Shoot() override;
};
