// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePickup.h"
#include "AmmoPickup.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPERSONTD_API AAmmoPickup : public ABasePickup
{
	GENERATED_BODY()

public:

	AAmmoPickup();
	
	int AmmoIncreaseAmount;

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
