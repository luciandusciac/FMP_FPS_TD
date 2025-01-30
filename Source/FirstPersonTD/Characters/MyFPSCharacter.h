// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"

#include "MyFPSCharacter.generated.h"

UCLASS()
class FIRSTPERSONTD_API AMyFPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMyFPSCharacter();

protected:
	virtual void BeginPlay() override;

	// virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	//
	// UPROPERTY(EditDefaultsOnly, Category = "Config")
	// TArray<TSubclassOf<class ABaseWeapon>> DefaultWeapons;
	//
	// UFUNCTION()
	// virtual void OnRep_CurrentWeapon(const class ABaseWeapon* LastWeapon);
	//
	// UFUNCTION(Server, Reliable)
	// void Server_SetCurrentWeapon(class ABaseWeapon* Weapon);
	// virtual void Server_SetCurrentWeapon_Implementation(class ABaseWeapon* NewWeapon);
	
public:
	
	//virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FTimerHandle AnimationTimerHandle;
	
	void ThrowGrenade();
	float GrenadeThrowTime;
	void OnGrenadeThrown();

	void Shoot();
	float ShootingTime;
	void OnShoot();

	void Aim();

	void Die();
	float DeathTime;
	void OnDeath();

	void Reload();
	float ReloadingTime;
	void OnReload();

	

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimationAsset* ShootingAnimation;

	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimationAsset* ReloadingAnimation;

	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimationAsset* DeathAnimation;

	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimationAsset* GrenadeThrowAnimation;

	// UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Replicated, Category = "State")
	// TArray<class ABaseWeapon*> Weapons;
	//
	// UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Replicated, Category = "State")
	// int32 CurrentWeaponIndex = 0;
	//
	// UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, ReplicatedUsing = OnRep_CurrentWeapon, Category = "State")
	// class ABaseWeapon* CurrentWeapon;
	//
	// UFUNCTION(BlueprintCallable)
	// virtual void SwapWeapon();
	//
	// UFUNCTION(BlueprintCallable)
	// void EquipWeapon(const int32 Index);
};
