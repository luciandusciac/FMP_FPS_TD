// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuffPickup.h"
#include "SpeedBuff.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPERSONTD_API ASpeedBuff : public ABuffPickup
{
	GENERATED_BODY()

public:
	float SpeedIncreaseAmount;

	virtual void OnExpire() override;

	virtual void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
