// Fill out your copyright notice in the Description page of Project Settings.

#include "BTT_FindPlayerLocation.h"
#include "../EnemyKeys.h"
#include "./FirstPersonTD/Controller/EnemyController.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UBTT_FindPlayerLocation::UBTT_FindPlayerLocation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Find Player Location");
}

EBTNodeResult::Type UBTT_FindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//return Super::ExecuteTask(OwnerComp, NodeMemory);

	auto const AIController = Cast<AEnemyController>(OwnerComp.GetAIOwner());

	UNavigationSystemV1* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld());

	APlayerController* const Player = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	APawn* const PlayerPawn = Player->GetPawn();

	if(NavSys)
	{
		AIController->GetBlackboardComponent()->SetValueAsVector(EnemyKeys::PlayerLocation, PlayerPawn->GetActorLocation());
		
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
