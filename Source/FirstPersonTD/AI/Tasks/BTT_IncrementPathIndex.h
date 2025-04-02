// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_IncrementPathIndex.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPERSONTD_API UBTT_IncrementPathIndex : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	explicit UBTT_IncrementPathIndex(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	enum class EDirectionType {Forward, Backward};

	EDirectionType Direction = EDirectionType::Forward;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI", meta = (AllowPrivateAccess = "true"))
	bool bBidirectional = false;
};
