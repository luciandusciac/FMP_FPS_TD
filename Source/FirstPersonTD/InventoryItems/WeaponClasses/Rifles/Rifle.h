// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseWeapon.h"

#include "Rifle.generated.h"

UCLASS()
class FIRSTPERSONTD_API ARifle : public ABaseWeapon
{
	GENERATED_BODY()

public:
	
	ARifle();

protected:
	
	virtual void BeginPlay() override;

public:
	
	virtual void Tick(float DeltaTime) override;

	virtual void Shoot() override;

	
};
