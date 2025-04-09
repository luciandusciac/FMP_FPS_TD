#include "BTT_Attack.h"
#include "AIController.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "FirstPersonTD/Characters/EnemyCharacter.h"
#include "Slate/SGameLayerManager.h"

UBTT_Attack::UBTT_Attack(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Attack Player";
}

EBTNodeResult::Type UBTT_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//works too
	// auto const OutOfRange = !OwnerComp.GetBlackboardComponent()->GetValueAsBool(GetSelectedBlackboardKey());
	// if (OutOfRange)
	// {
	// 	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	// 	return EBTNodeResult::Succeeded;
	// }
	//
	// auto const* const AIController = Cast<AEnemyController>(OwnerComp.GetAIOwner());
	// auto* Char = Cast<AEnemyCharacter>(AIController->GetPawn());
	//
	// auto* Player = Cast<AMyFPSCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	//
	// if (!Player)
	// 	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Player not found!"));
	//
	// if (AIController && Char && Char->Weapon && !(Player->CurrentHealth > 0))
	// 	Char->Shoot();
	//
	// FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	// return EBTNodeResult::Type();


	auto const bOutOfRange = !OwnerComp.GetBlackboardComponent()->GetValueAsBool(GetSelectedBlackboardKey());
	if (bOutOfRange)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	AEnemyController* AIController = Cast<AEnemyController>(OwnerComp.GetAIOwner());
	if (!AIController)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return EBTNodeResult::Failed;
	}

	AEnemyCharacter* Char = Cast<AEnemyCharacter>(AIController->GetPawn());
	if (!Char || !Char->Weapon)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return EBTNodeResult::Failed;
	}

	AMyFPSCharacter* Player = Cast<AMyFPSCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!Player)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Player not found!"));
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return EBTNodeResult::Failed;
	}
	
	if (Player->CurrentHealth > 0)
	{
		Char->Shoot();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Enemy shot at player!"));
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
	
	// AAIController* AIController = OwnerComp.GetAIOwner();
	// if (!AIController) return EBTNodeResult::Failed;
	//
	// UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	// if (!Blackboard) return EBTNodeResult::Failed;
	//
	// AActor* Player = Cast<AActor>(Blackboard->GetValueAsObject("TargetActor"));
	// if (!Player) return EBTNodeResult::Failed;
	//
	// FVector Start = AIController->GetPawn()->GetActorLocation();
	// FVector End = Player->GetActorLocation();
	//
	// FHitResult Hit;
	// FCollisionQueryParams Params;
	// Params.AddIgnoredActor(AIController->GetPawn());
	//
	// if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
	// {
	// 	if (Hit.GetActor() == Player)
	// 	{
	// 		UGameplayStatics::ApplyDamage(Player, 20.0f, AIController, AIController->GetPawn(), nullptr);
	// 	}
	// }
	//
	// return EBTNodeResult::Succeeded;
}