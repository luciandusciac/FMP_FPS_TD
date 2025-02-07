// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "BaseProjectile.generated.h"

UCLASS()
class FIRSTPERSONTD_API ABaseProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseProjectile();

#pragma region BulletStats

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet|Stats")
	float DamageAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet|Stats")
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet|Stats")
	float LifeTime;
	
#pragma endregion

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#pragma region BulletComponents

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Bullet|Components")
	class USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Bullet|Components")
	class UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Bullet|Components")
	class UNiagaraComponent* BulletTrail;
	
#pragma endregion	

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
