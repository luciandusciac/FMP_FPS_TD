#include "BTT_ReturnToPatrol.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_ReturnToPatrol::UBTT_ReturnToPatrol(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Return to Patrol";
}

EBTNodeResult::Type UBTT_ReturnToPatrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard) return EBTNodeResult::Failed;

	Blackboard->ClearValue("TargetActor");

	return EBTNodeResult::Succeeded;
}