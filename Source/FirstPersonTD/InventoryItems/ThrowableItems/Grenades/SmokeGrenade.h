// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseGrenade.h"
#include "FirstPersonTD/InventoryItems/Interfaces/SmokeGrenadeInterface.h"
#include "SmokeGrenade.generated.h"

UCLASS()
class FIRSTPERSONTD_API ASmokeGrenade : public ABaseGrenade, public ISmokeGrenadeInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASmokeGrenade();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Explode() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UParticleSystemComponent* SmokeParticle;
};
