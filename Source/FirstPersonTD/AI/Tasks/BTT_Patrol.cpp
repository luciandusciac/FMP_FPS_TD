#include "BTT_Patrol.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "FirstPersonTD/Controller/EnemyController.h"
#include "Navigation/PathFollowingComponent.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"

UBTT_Patrol::UBTT_Patrol(FObjectInitializer const& ObjectInitializer)
{
    NodeName = "Patrol";
}

EBTNodeResult::Type UBTT_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // Get the AI controller associated with this task
    AEnemyController* EnemyController = Cast<AEnemyController>(OwnerComp.GetAIOwner());
    if (!EnemyController)
    {
        return EBTNodeResult::Failed;
    }

    // Get the blackboard component
    UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
    if (!Blackboard)
    {
        return EBTNodeResult::Failed;
    }

    // Get the current location of the AI
    FVector CurrentLocation = EnemyController->GetPawn()->GetActorLocation();

    // Get a random location in the navigable area (within a certain radius)
    FVector RandomLocation = GetRandomLocationInNavigableRadius(CurrentLocation, 1000.0f);

    // Move to that location
    FAIRequestID RequestID = EnemyController->MoveToLocation(RandomLocation);
    if (RequestID.IsValid())
    {
        // Bind the OnMoveCompleted function to the path following component's OnRequestFinished event
        EnemyController->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &UBTT_Patrol::OnMoveCompleted);
        
        // Task is in progress until the move is completed
        return EBTNodeResult::InProgress;
    }

    return EBTNodeResult::Failed;
}

FVector UBTT_Patrol::GetRandomLocationInNavigableRadius(FVector Origin, float Radius)
{
    // Set up the query parameters to get a valid random location
    FNavLocation NavLocation;
    UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
    if (NavSystem && NavSystem->GetRandomPointInNavigableRadius(Origin, Radius, NavLocation))
    {
        return NavLocation.Location; // Return the valid random location
    }
    return FVector::ZeroVector; // Return zero vector if no valid location found
}

void UBTT_Patrol::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
    // If movement is completed, move to the next patrol point or location
    if (OwnerComponent)
    {
        UBlackboardComponent* Blackboard = OwnerComponent->GetBlackboardComponent();
        if (Blackboard)
        {
            AEnemyController* EnemyController = Cast<AEnemyController>(OwnerComponent->GetAIOwner());
            if (EnemyController)
            {
                // You can update the blackboard with new patrol points here if needed
                // Example: Blackboard->SetValueAsObject("NextPatrolPoint", NewPatrolPoint);
            }
        }
        FinishLatentTask(*OwnerComponent, EBTNodeResult::Succeeded);
    }
}