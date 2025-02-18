// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseWeapon.h"

#include "Sniper.generated.h"

UCLASS()
class FIRSTPERSONTD_API ASniper : public ABaseWeapon
{
	GENERATED_BODY()

public:
	ASniper();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void Shoot() override;
};
