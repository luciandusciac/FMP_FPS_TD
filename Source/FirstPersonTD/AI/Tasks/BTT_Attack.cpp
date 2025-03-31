#include "BTT_Attack.h"
#include "AIController.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_Attack::UBTT_Attack(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Attack Player";
}

EBTNodeResult::Type UBTT_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard) return EBTNodeResult::Failed;

	AActor* Player = Cast<AActor>(Blackboard->GetValueAsObject("TargetActor"));
	if (!Player) return EBTNodeResult::Failed;

	FVector Start = AIController->GetPawn()->GetActorLocation();
	FVector End = Player->GetActorLocation();

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(AIController->GetPawn());

	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
	{
		if (Hit.GetActor() == Player)
		{
			UGameplayStatics::ApplyDamage(Player, 20.0f, AIController, AIController->GetPawn(), nullptr);
		}
	}

	return EBTNodeResult::Succeeded;
}