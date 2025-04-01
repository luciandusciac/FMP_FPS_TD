#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Navigation/PathFollowingComponent.h"
#include "FirstPersonTD/Controller/EnemyController.h"

#include "BTT_Patrol.generated.h"

UCLASS()
class FIRSTPERSONTD_API UBTT_Patrol : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTT_Patrol(FObjectInitializer const& ObjectInitializer);

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	// Function to handle when the move is complete
	void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result);

	// Function to get a random location within a navigable area
	FVector GetRandomLocationInNavigableRadius(FVector Origin, float Radius);

	UPROPERTY()
	UBehaviorTreeComponent* OwnerComponent;

	void OnWaitTimeComplete();

	FTimerHandle WaitTimerHandle;
};