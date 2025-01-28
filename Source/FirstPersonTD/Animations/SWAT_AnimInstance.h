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
};
