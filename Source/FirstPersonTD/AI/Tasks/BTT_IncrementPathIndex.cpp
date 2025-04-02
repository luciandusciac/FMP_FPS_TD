// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_IncrementPathIndex.h"

#include "FirstPersonTD/Characters/EnemyCharacter.h"
#include "FirstPersonTD/Controller/EnemyController.h"

UBTT_IncrementPathIndex::UBTT_IncrementPathIndex(FObjectInitializer const& ObjectInitializer) : UBTTask_BlackboardBase(ObjectInitializer)
{
	NodeName = TEXT("Increment Path Index");
}

EBTNodeResult::Type UBTT_IncrementPathIndex::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto* const AIController = Cast<AEnemyController>(OwnerComp.GetAIOwner()))
	{
		if (auto* const Char = Cast<AEnemyCharacter>(AIController->GetPawn()))
		{
			if (auto* const B = OwnerComp.GetBlackboardComponent())
			{
				auto const NumberOfPoints = Char->GetPatrolPath()->Num();
				auto const Min = 0;
				auto const Max = NumberOfPoints - 1;
				auto  CurrentIndex = B->GetValueAsInt(GetSelectedBlackboardKey());

				if (bBidirectional)
				{
					if (CurrentIndex >= Max && Direction == EDirectionType::Forward)
					{
						Direction = EDirectionType::Backward;
					}
					else if (CurrentIndex == Min && Direction == EDirectionType::Backward)
					{
						Direction = EDirectionType::Forward;
					}
				}
				B->SetValueAsInt(GetSelectedBlackboardKey(), (Direction == EDirectionType::Forward ? ++CurrentIndex : --CurrentIndex) % NumberOfPoints);

				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}
