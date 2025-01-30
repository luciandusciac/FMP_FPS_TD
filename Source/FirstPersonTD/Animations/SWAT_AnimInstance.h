// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SWAT_AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPERSONTD_API USWAT_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, Category = "Bending")
	float HorizontalBend {0.0f};

	UPROPERTY(BlueprintReadWrite, Category = "Bending")
	float VerticalBend {0.0f};

	UPROPERTY(BlueprintReadWrite, Category = "Walking")
	float HorizontalWalk {0.0f};

	UPROPERTY(BlueprintReadWrite, Category = "Walking")
	float VerticalWalk {0.0f};

	UPROPERTY(BlueprintReadWrite, Category = "Action Booleans")
	bool bIsCrouching {false};

	UPROPERTY(BlueprintReadWrite, Category = "Action Booleans")
	bool bIsReloading {false};

	UPROPERTY(BlueprintReadWrite, Category = "Action Booleans")
	bool bIsShooting {false};

	UPROPERTY(BlueprintReadWrite, Category = "Action Booleans")
	bool bDead {false};

	UPROPERTY(BlueprintReadWrite, Category = "Action Booleans")
	bool bIsThrowingGrenade {false};
};
