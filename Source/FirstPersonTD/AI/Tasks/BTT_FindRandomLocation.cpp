// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_FindRandomLocation.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_FindRandomLocation::UBTT_FindRandomLocation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "FindRandomLocation";
}

EBTNodeResult::Type UBTT_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAIController* const Controller = Cast<AAIController>(OwnerComp.GetAIOwner()))
	{
		if (auto* const NPC = Controller->GetPawn())
		{
			auto const Origin = NPC->GetActorLocation();

			if (auto* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
			{
				FNavLocation RandomLocation;
				if (NavSys->GetRandomPointInNavigableRadius(Origin, SearchRadius, RandomLocation))
				{
					FNavLocation NewLocation;
					if (NavSys->GetRandomPointInNavigableRadius(Origin, SearchRadius, NewLocation))
					{
						OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), NewLocation.Location);
					}
					
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
					return EBTNodeResult::Succeeded;
				}
			}
		}
	}
	
	return EBTNodeResult::Failed;
}
