// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseThrowable.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"

#include "BaseGrenade.generated.h"

UCLASS()
class FIRSTPERSONTD_API ABaseGrenade : public ABaseThrowable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseGrenade();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Explode();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class USceneComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade Stats")
	float ExplosionTime;

	FTimerHandle ExplosionTimerHandle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	UNiagaraComponent* ExplosionVFX;
	
};
