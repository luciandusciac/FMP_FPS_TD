// // Fill out your copyright notice in the Description page of Project Settings.
//
//
// #include "EnemyController.h"
//
// #include "BehaviorTree/BehaviorTree.h"
//
//
// // Sets default values
// AEnemyController::AEnemyController(FObjectInitializer const& ObjectInitializer)
// {
// 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
// 	PrimaryActorTick.bCanEverTick = true;
// 	
// 	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviorTreeAsset(TEXT("BehaviorTree'/Game/AI/BT_EnemyBehaviour.BT_EnemyBehaviour'"));
//
// 	if (BehaviorTreeAsset.Succeeded())
// 	{
// 		BehaviorTree = BehaviorTreeAsset.Object;
// 	}
//
// 	BehaviorTreeComponent = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorTreeComponent"));
// 	BlackboardComponent = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComponent"));
// }
//
// // Called when the game starts or when spawned
// void AEnemyController::BeginPlay()
// {
// 	Super::BeginPlay();
//
// 	RunBehaviorTree(BehaviorTree);
// 	BehaviorTreeComponent->StartTree(*BehaviorTree);
// 	
// }
//
//
// // Called every frame
// void AEnemyController::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);
// }
//
// void AEnemyController::OnPossess(APawn* InPawn)
// {
// 	Super::OnPossess(InPawn);
// 	
// 	if (BlackboardComponent)
// 	{
// 		BlackboardComponent->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
// 	}
// }
//


#include "EnemyController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "Kismet/GameplayStatics.h"

AEnemyController::AEnemyController(FObjectInitializer const& ObjectInitializer)
{
    PrimaryActorTick.bCanEverTick = true;

    static ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviorTreeAsset(TEXT("BehaviorTree'/Game/AI/BT_EnemyBehaviour.BT_EnemyBehaviour'"));
    if (BehaviorTreeAsset.Succeeded())
    {
        BehaviorTree = BehaviorTreeAsset.Object;
    }

    BehaviorTreeComponent = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorTreeComponent"));
    BlackboardComponent = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComponent"));
    
    AIPerceptionComponent = ObjectInitializer.CreateDefaultSubobject<UAIPerceptionComponent>(this, TEXT("AIPerceptionComponent"));
    SightConfig = ObjectInitializer.CreateDefaultSubobject<UAISenseConfig_Sight>(this, TEXT("SightConfig"));

    AIPerceptionComponent->ConfigureSense(*SightConfig);
    AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
    AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyController::OnTargetPerceptionUpdated);
}

void AEnemyController::BeginPlay()
{
    Super::BeginPlay();
    if (BehaviorTree)
    {
        RunBehaviorTree(BehaviorTree);
        BehaviorTreeComponent->StartTree(*BehaviorTree);
    }

    UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("PatrolPoint"), PatrolPoints);
}

void AEnemyController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    if (BlackboardComponent)
    {
        AActor* PatrolPoint = Cast<AActor>(BlackboardComponent->GetValueAsObject("NextPatrolPoint"));
        if (PatrolPoint)
        {
            if (GetPawn() && FVector::Dist(GetPawn()->GetActorLocation(), PatrolPoint->GetActorLocation()) < 100.0f)
            {
                BlackboardComponent->SetValueAsObject("NextPatrolPoint", GetNextPatrolPoint());
            }
            else
            {
                MoveToActor(PatrolPoint, 5.0f);
            }
        }
    }
}

void AEnemyController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    if (BlackboardComponent && BehaviorTree)
    {
        BlackboardComponent->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
        BlackboardComponent->SetValueAsObject("NextPatrolPoint", GetNextPatrolPoint());
    }
}

void AEnemyController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    if (!BlackboardComponent) return;

    if (Actor->ActorHasTag("Player"))
    {
        if (Stimulus.WasSuccessfullySensed())
        {
            BlackboardComponent->SetValueAsBool("PlayerDetected", true);
            BlackboardComponent->SetValueAsObject("TargetActor", Actor);
        }
        else
        {
            BlackboardComponent->SetValueAsBool("PlayerDetected", false);
            BlackboardComponent->ClearValue("TargetActor");
        }
    }
}

AActor* AEnemyController::GetNextPatrolPoint()
{
    if (PatrolPoints.Num() == 0)
    {
        return nullptr;
    }

    AActor* NextPoint = PatrolPoints[CurrentPatrolIndex];
    CurrentPatrolIndex = (CurrentPatrolIndex + 1) % PatrolPoints.Num();
    return NextPoint;
}
