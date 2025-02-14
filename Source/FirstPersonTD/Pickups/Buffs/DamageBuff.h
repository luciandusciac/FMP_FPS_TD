// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuffPickup.h"
#include "DamageBuff.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPERSONTD_API ADamageBuff : public ABuffPickup
{
	GENERATED_BODY()

public:
	float DamageIncreaseAmount;

	virtual void OnExpire() override;

	virtual void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
