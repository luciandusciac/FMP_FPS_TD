// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_ReturnToPatrol.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPERSONTD_API UBTT_ReturnToPatrol : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	UBTT_ReturnToPatrol(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
