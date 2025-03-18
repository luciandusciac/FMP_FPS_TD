// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../InventoryItem.h"
#include "../Source/FirstPersonTD/Projectiles/BaseProjectile.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

#include "BaseWeapon.generated.h"

UCLASS(Abstract)
class FIRSTPERSONTD_API ABaseWeapon : public AInventoryItem
{
	GENERATED_BODY()

public:
	ABaseWeapon();

protected:
	virtual void BeginPlay() override;

#pragma region WeaponStats

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	int CurrentAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	int ClipSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	int ReserveAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	float ReloadTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	float FireRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	float CurrentFireRate;

#pragma endregion

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet")
	TSubclassOf<ABaseProjectile> WeaponBullet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet")
	USceneComponent* BulletOrigin;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* Mesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aiming")
	USceneComponent* AimOrigin;
	
	//INFO: Variable to keep track if the weapon has a scope
	bool bHasScope;

#pragma region WeaponFunctions

	virtual void Shoot();
	virtual void OnShoot();
	virtual void Reload();
	void Aim();
	virtual void Use() override;

	int GetCurrentAmmo();
	void SetCurrentAmmo(const int NewCurrentAmmo);
	int GetReserveAmmo();
	void SetReserveAmmo(const int NewReserveAmmo);
	
#pragma endregion	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	UNiagaraComponent* MuzzleFlash;

	bool bIsShooting;
	float CurrentTime;
	FTimerHandle ShootingTimerHandle;
	
	// UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "State")
	// class AMyFPSCharacter* CurrentOwner;
	//
	// UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Config")
	// FIKProperties IKProperties;
	//
	// UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Config")
	// FTransform PlacementTransform;
	
	
	//niagara for shot and muzzle flash?
};
