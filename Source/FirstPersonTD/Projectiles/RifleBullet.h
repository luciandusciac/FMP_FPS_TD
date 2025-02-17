// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseProjectile.h"
#include "RifleBullet.generated.h"

UCLASS()
class FIRSTPERSONTD_API ARifleBullet : public ABaseProjectile
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARifleBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Bullet|Components")
	// AActor* WeaponOwner;
};
