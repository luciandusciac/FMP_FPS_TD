// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FirstPersonTD/InventoryItems/InventoryItem.h"
#include "GameFramework/Actor.h"

#include "BaseThrowable.generated.h"

UCLASS(Abstract)
class FIRSTPERSONTD_API ABaseThrowable : public AInventoryItem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseThrowable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Use() override;
};
