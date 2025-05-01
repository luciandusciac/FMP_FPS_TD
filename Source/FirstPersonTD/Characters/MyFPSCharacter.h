// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "../InventoryItems/Inventory.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Animation/AnimSequence.h"
#include "FirstPersonTD/Animations/SWAT_AnimInstance.h"
#include "FirstPersonTD/InventoryItems/ThrowableItems/Grenades/BaseGrenade.h"
#include "FirstPersonTD/InventoryItems/WeaponClasses/BaseWeapon.h"

#include "Blueprint/UserWidget.h"
#include "../UI/PlayerHUD.h"

#include "MyFPSCharacter.generated.h"

USTRUCT(BlueprintType)
struct FAmmoData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CurrentAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ClipSize;

	FAmmoData() : CurrentAmmo(0), ClipSize(0) {}
	FAmmoData(const int NewCurrentAmmo, const int NewClipSize) : CurrentAmmo(NewCurrentAmmo), ClipSize(NewClipSize) {}
};


UCLASS()
class FIRSTPERSONTD_API AMyFPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMyFPSCharacter();

protected:
	virtual void BeginPlay() override;
	
public:
	
	virtual void Tick(float DeltaTime) override;

	FTimerHandle AnimationTimerHandle;

#pragma region Movement and Actions
	void ThrowGrenade();
	float GrenadeThrowTime;
	void OnGrenadeThrown();
	void CheckGrenadeInHand();

	void ThrowKnife();
	void OnKnifeThrown();

	void Shoot();
	float ShootingTime;
	float PistolShootingTime;
	void OnShoot();

	void Aim();
	void StopAiming();
	bool bIsAiming = false;

	void Die();
	float DeathTime;
	void OnDeath();

	void Respawn();
	FVector StartDeathCamLocation;
	FVector EndDeathCamLocation;
	FRotator StartDeathCamRotation;
	FRotator EndDeathCamRotation;
	float CameraLerpAlpha;
	bool bIsDeathCameraMoving;
	FVector PlayerStartLocation;

	void Reload();
	float ReloadingTime;
	void OnReload();

	void NextWeapon();
	void PreviousWeapon();
	void ThrowWeapon();

	void SpawnCurrentWeaponInHands();

	void ResetWalkingSpeed();
	void ResetBulletDamage();

#pragma endregion

	TMap<TSubclassOf<ABaseWeapon>, FAmmoData> AmmoDataMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USceneComponent* ADSPosComponent;

#pragma region Animations
	
	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimSequence* ShootingAnimation;

	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimSequence* ReloadingAnimation;

	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimSequence* DeathAnimation;

	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimSequence* GrenadeThrowAnimation;

	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimSequence* KnifeThrowAnimation;
	
	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimSequence* PistolReloadingAnimation;

	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimSequence* PistolShootingAnimation;

	UPROPERTY()
	USWAT_AnimInstance* AnimationInstance;
	
#pragma endregion

#pragma region Inventory

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	UInventory* Inventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	AInventoryItem* CurrentItemInHands;

#pragma endregion

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	

#pragma region Weapons

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aiming")
	FVector AimTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Holding")
	FVector WeaponScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Holding")
	FRotator WeaponRotation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Holding")
	FVector WeaponLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Holding")
	FTransform WeaponTransform;

#pragma endregion

#pragma region Sounds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	class USoundBase* DeathSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	class USoundBase* TakeDamageSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	class USoundBase* PickupSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	class USoundBase* HealingSound;

	bool bHasPlayed = false;

#pragma endregion

#pragma region UI
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UPlayerHUD> HUDClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	class UPlayerHUD* HUD;

	UPROPERTY(EditAnywhere, Category="UI")
	TSubclassOf<UUserWidget> DamageEffectWidgetClass;

	UPROPERTY(EditAnywhere, Category="UI")
	TSubclassOf<UUserWidget> HealingEffectWidgetClass;

	UPROPERTY(EditAnywhere, Category="UI")
	TSubclassOf<UUserWidget> FlashbangWidgetClass;

	UPROPERTY(EditAnywhere, Category="UI")
	TSubclassOf<UUserWidget> InstructionsWidgetClass;
	
	UPROPERTY()
	UUserWidget* DamageEffectWidget;

	UPROPERTY()
	UUserWidget* HealingEffectWidget;

	UPROPERTY()
	UUserWidget* FlashbangWidget;

	UPROPERTY()
	UUserWidget* InstructionsWidget;

	void DestroyWidget(UUserWidget* Widget);

	FTimerHandle WidgetTimerHandle;

	void UpdateAmmoUI();
	void UpdateKnifeThrowProgress();
	FTimerHandle KnifeProgressBarTimer;
	
	float KnifeThrowElapsedTime = 0.f;
	bool bIsThrowingKnife = false;
	
#pragma endregion


#pragma region Health

	float CurrentHealth;
	float MaxHealth = 100.0f;
	void Heal();
	void TakeDamage(float Damage);
	
#pragma endregion 
	
};
