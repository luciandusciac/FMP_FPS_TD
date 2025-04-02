// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_FindPathPoint.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "FirstPersonTD/Characters/EnemyCharacter.h"
#include "FirstPersonTD/Controller/EnemyController.h"

UBTT_FindPathPoint::UBTT_FindPathPoint(FObjectInitializer const& ObjectInitializer) : UBTTask_BlackboardBase(ObjectInitializer)
{
	NodeName = TEXT("Find Path Point");
}

EBTNodeResult::Type UBTT_FindPathPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto* const AIController = Cast<AEnemyController>(OwnerComp.GetAIOwner()))
	{
		if (auto* const B = OwnerComp.GetBlackboardComponent())
		{
			auto const index = B->GetValueAsInt(GetSelectedBlackboardKey());

			if (auto* const Char = Cast<AEnemyCharacter>(AIController->GetPawn()))
			{
				auto const Point = Char->GetPatrolPath()->GetPatrolPoint(index);

				auto const GlobalPoint = Char->GetPatrolPath()->GetActorTransform().TransformPosition(Point);
				B->SetValueAsVector(PatrolPathVectorKey.SelectedKeyName, GlobalPoint);

				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
			
		}
	}
	return EBTNodeResult::Failed;
}
