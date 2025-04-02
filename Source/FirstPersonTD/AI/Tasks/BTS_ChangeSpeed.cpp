// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_ChangeSpeed.h"

#include "FirstPersonTD/Characters/EnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTS_ChangeSpeed::UBTS_ChangeSpeed()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Change Speed");
}

void UBTS_ChangeSpeed::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto* const AIController = Cast<AEnemyController>(OwnerComp.GetAIOwner()))
	{
		if (auto* const Char = Cast<AEnemyCharacter>(AIController->GetPawn()))
		{
			if (auto* const B = OwnerComp.GetBlackboardComponent())
			{
				Char->GetCharacterMovement()->MaxWalkSpeed = Speed;
			}
		}
	}
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
}
