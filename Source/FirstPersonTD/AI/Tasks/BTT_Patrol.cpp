#include "BTT_Patrol.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Actor.h"

UBTT_Patrol::UBTT_Patrol(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Patrol";
}

EBTNodeResult::Type UBTT_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard) return EBTNodeResult::Failed;

	AActor* PatrolPoint = Cast<AActor>(Blackboard->GetValueAsObject("NextPatrolPoint"));
	if (!PatrolPoint) return EBTNodeResult::Failed;

	AIController->MoveToActor(PatrolPoint);

	return EBTNodeResult::Succeeded;
}