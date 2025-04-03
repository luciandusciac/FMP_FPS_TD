// Fill out your copyright notice in the Description page of Project Settings.

#include "BTT_ChasePlayer.h"
#include "../EnemyKeys.h"
#include "BTT_FindPlayerLocation.h"
#include "./FirstPersonTD/Controller/EnemyController.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/GameplayStatics.h"

UBTT_ChasePlayer::UBTT_ChasePlayer(FObjectInitializer const& ObjectInitializer) : UBTTask_BlackboardBase(ObjectInitializer)
{
	NodeName = TEXT("Chase Player");
}

EBTNodeResult::Type UBTT_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto* AIController = Cast<AEnemyController>(OwnerComp.GetAIOwner()))
	{
		auto const PlayerLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());

		FVector const AICharacterLocation = AIController->GetPawn()->GetActorLocation();
		FRotator const LookAtRotation = (PlayerLocation - AICharacterLocation).Rotation();
		AIController->GetPawn()->SetActorRotation(LookAtRotation);
		
		
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(AIController, PlayerLocation / 3);
		

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;



	
	//return Super::ExecuteTask(OwnerComp, NodeMemory);
	
	// auto const AIController = Cast<AEnemyController>(OwnerComp.GetAIOwner());
	//
	// UNavigationSystemV1* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	//
	// if(NavSys)
	// {
	// 	//get player location
	// 	FVector PlayerLocation = AIController->GetBlackboardComponent()->GetValueAsVector(EnemyKeys::PlayerLocation);
	//
	// 	//move to player location
	// 	AIController->MoveToLocation(PlayerLocation, 10.0f);
	// }
	//
	// FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	// return EBTNodeResult::Succeeded;

}
