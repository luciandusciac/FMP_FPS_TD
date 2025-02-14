// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FirstPersonTD/Pickups/BasePickup.h"
#include "BuffPickup.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPERSONTD_API ABuffPickup : public ABasePickup
{
	GENERATED_BODY()

protected:
	float LifeTime;

public:
	
	virtual void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void OnExpire();

	FTimerHandle TimerHandle;
};
