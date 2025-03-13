// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseGrenade.h"
#include "FirstPersonTD/InventoryItems/Interfaces/FlashbangInterface.h"
#include "FlashbangGrenade.generated.h"

UCLASS()
class FIRSTPERSONTD_API AFlashbangGrenade : public ABaseGrenade, public IFlashbangInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFlashbangGrenade();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Explode() override;
	virtual void OnExplode() override;
	
};
