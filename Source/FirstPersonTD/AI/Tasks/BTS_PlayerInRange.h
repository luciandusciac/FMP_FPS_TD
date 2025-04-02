// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_PlayerInRange.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPERSONTD_API UBTS_PlayerInRange : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTS_PlayerInRange();
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, Category = "AI", meta = (AllowPrivateAccess = "true"))
	float ShootingRange = 200.f;
};
