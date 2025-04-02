// Fill out your copyright notice in the Description page of Project Settings.

#include "BTT_FindPlayerLocation.h"
#include "../EnemyKeys.h"
#include "./FirstPersonTD/Controller/EnemyController.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UBTT_FindPlayerLocation::UBTT_FindPlayerLocation(FObjectInitializer const& ObjectInitializer) : UBTTask_BlackboardBase(ObjectInitializer)
{
	NodeName = TEXT("Find Player Location");
}

EBTNodeResult::Type UBTT_FindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto* const Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		auto const PlayerLocation = Character->GetActorLocation();
		if (SearchRandom)
		{
			FNavLocation Location;
			if (auto* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
			{
				if (NavSys->GetRandomPointInNavigableRadius(PlayerLocation, SearchRadius, Location))
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), Location.Location);
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
					return EBTNodeResult::Succeeded;
				}
			}
		}
		else
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), PlayerLocation);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
		
	}
	return EBTNodeResult::Failed;
	
	// //return Super::ExecuteTask(OwnerComp, NodeMemory);
	//
	// auto const AIController = Cast<AEnemyController>(OwnerComp.GetAIOwner());
	//
	// UNavigationSystemV1* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	//
	// APlayerController* const Player = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	// APawn* const PlayerPawn = Player->GetPawn();
	//
	// if(NavSys)
	// {
	// 	AIController->GetBlackboardComponent()->SetValueAsVector(EnemyKeys::PlayerLocation, PlayerPawn->GetActorLocation());
	// 	
	// }
	//
	// FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	// return EBTNodeResult::Succeeded;
}
