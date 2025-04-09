// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "StepNotify.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPERSONTD_API UStepNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY(EditAnywhere, Category = "Footstep")
	FName FootSocketName = "LeftFoot";
	
	UPROPERTY(EditAnywhere, Category = "Footstep")
	TMap<TEnumAsByte<EPhysicalSurface>, USoundBase*> SurfaceSounds;
};
