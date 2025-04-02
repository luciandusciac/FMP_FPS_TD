// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_PlayerInRange.h"

#include "FirstPersonTD/Characters/EnemyCharacter.h"
#include "FirstPersonTD/Controller/EnemyController.h"
#include "Kismet/GameplayStatics.h"

UBTS_PlayerInRange::UBTS_PlayerInRange()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Player In Range");
}

void UBTS_PlayerInRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto const* const AIController = Cast<AEnemyController>(OwnerComp.GetAIOwner());
	auto const* const Char = Cast<AEnemyCharacter>(AIController->GetPawn());
	auto const* const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), Char->GetDistanceTo(Player) <= ShootingRange);

	
	// Super::OnBecomeRelevant(OwnerComp, NodeMemory);
	// if (auto* const AIController = Cast<AEnemyController>(OwnerComp.GetAIOwner()))
	// {
	// 	if (auto* const Char = Cast<AEnemyCharacter>(AIController->GetPawn()))
	// 	{
	// 		if (auto* const B = OwnerComp.GetBlackboardComponent())
	// 		{
	// 			auto* const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	// 			if (Player)
	// 			{
	// 				auto const Distance = FVector::Distance(Char->GetActorLocation(), Player->GetActorLocation());
	// 				B->SetValueAsBool(GetSelectedBlackboardKey(), Distance <= ShootingRange);
	// 			}
	// 		}
	// 	}
	// }
}
