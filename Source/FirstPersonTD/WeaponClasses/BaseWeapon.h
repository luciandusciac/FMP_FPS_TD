// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "BaseWeapon.generated.h"

USTRUCT(BlueprintType)
struct FIKProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimSequence* AnimPose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AimOffset = 15;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform AimTransform;
};


UCLASS(Abstract)
class FIRSTPERSONTD_API ABaseWeapon : public AActor
{
	GENERATED_BODY()

public:
	ABaseWeapon();

protected:
	virtual void BeginPlay() override;

	//variable to keep track if the weapon has a scope
	bool bHasScope;

	int CurrentAmmo;
	int ClipSize;
	int ReserveAmmo;

	float ReloadTime;

	float FireRate;
	

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* Mesh;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "State")
	class AMyFPSCharacter* CurrentOwner;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Config")
	FIKProperties IKProperties;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Config")
	FTransform PlacementTransform;
	
	
	//niagara for shot and muzzle flash?
};
