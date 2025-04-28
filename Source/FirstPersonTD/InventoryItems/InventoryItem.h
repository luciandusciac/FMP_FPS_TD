// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "InventoryItem.generated.h"

UCLASS(Abstract)
class FIRSTPERSONTD_API AInventoryItem : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInventoryItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Use();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsConsumable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	FTransform AttachmentTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	FTransform AimingTransform;
	
};
