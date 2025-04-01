// // // Fill out your copyright notice in the Description page of Project Settings.
// //
// //
// // #include "EnemyController.h"
// //
// // #include "BehaviorTree/BehaviorTree.h"
// //
// //
// // // Sets default values
// // AEnemyController::AEnemyController(FObjectInitializer const& ObjectInitializer)
// // {
// // 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
// // 	PrimaryActorTick.bCanEverTick = true;
// // 	
// // 	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviorTreeAsset(TEXT("BehaviorTree'/Game/AI/BT_EnemyBehaviour.BT_EnemyBehaviour'"));
// //
// // 	if (BehaviorTreeAsset.Succeeded())
// // 	{
// // 		BehaviorTree = BehaviorTreeAsset.Object;
// // 	}
// //
// // 	BehaviorTreeComponent = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorTreeComponent"));
// // 	BlackboardComponent = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComponent"));
// // }
// //
// // // Called when the game starts or when spawned
// // void AEnemyController::BeginPlay()
// // {
// // 	Super::BeginPlay();
// //
// // 	RunBehaviorTree(BehaviorTree);
// // 	BehaviorTreeComponent->StartTree(*BehaviorTree);
// // 	
// // }
// //
// //
// // // Called every frame
// // void AEnemyController::Tick(float DeltaTime)
// // {
// // 	Super::Tick(DeltaTime);
// // }
// //
// // void AEnemyController::OnPossess(APawn* InPawn)
// // {
// // 	Super::OnPossess(InPawn);
// // 	
// // 	if (BlackboardComponent)
// // 	{
// // 		BlackboardComponent->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
// // 	}
// // }
// //
//
//
// #include "EnemyController.h"
//
// #include "BehaviorTree/BehaviorTree.h"
// #include "Kismet/GameplayStatics.h"
//
// AEnemyController::AEnemyController(FObjectInitializer const& ObjectInitializer)
// {
//     PrimaryActorTick.bCanEverTick = true;
//
//     static ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviorTreeAsset(TEXT("BehaviorTree'/Game/AI/BT_EnemyBehaviour.BT_EnemyBehaviour'"));
//     if (BehaviorTreeAsset.Succeeded())
//     {
//         BehaviorTree = BehaviorTreeAsset.Object;
//     }
//
//     BehaviorTreeComponent = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorTreeComponent"));
//     BlackboardComponent = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComponent"));
//     
//     AIPerceptionComponent = ObjectInitializer.CreateDefaultSubobject<UAIPerceptionComponent>(this, TEXT("AIPerceptionComponent"));
//     SightConfig = ObjectInitializer.CreateDefaultSubobject<UAISenseConfig_Sight>(this, TEXT("SightConfig"));
//
//     AIPerceptionComponent->ConfigureSense(*SightConfig);
//     AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
//     AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyController::OnTargetPerceptionUpdated);
// }
//
// void AEnemyController::BeginPlay()
// {
//     Super::BeginPlay();
//     if (BehaviorTree)
//     {
//         RunBehaviorTree(BehaviorTree);
//         BehaviorTreeComponent->StartTree(*BehaviorTree);
//     }
//
//     UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("PatrolPoint"), PatrolPoints);
// }
//
// void AEnemyController::Tick(float DeltaTime)
// {
//     Super::Tick(DeltaTime);
//     
//     if (BlackboardComponent)
//     {
//         AActor* PatrolPoint = Cast<AActor>(BlackboardComponent->GetValueAsObject("NextPatrolPoint"));
//         if (PatrolPoint)
//         {
//             if (GetPawn() && FVector::Dist(GetPawn()->GetActorLocation(), PatrolPoint->GetActorLocation()) < 10000.0f)
//             {
//                 BlackboardComponent->SetValueAsObject("NextPatrolPoint", GetNextPatrolPoint());
//             }
//             else
//             {
//                 MoveToActor(PatrolPoint, 5.0f);
//             }
//         }
//     }
// }
//
// void AEnemyController::OnPossess(APawn* InPawn)
// {
//     Super::OnPossess(InPawn);
//     if (BlackboardComponent && BehaviorTree)
//     {
//         BlackboardComponent->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
//         BlackboardComponent->SetValueAsObject("NextPatrolPoint", GetNextPatrolPoint());
//     }
// }
//
// void AEnemyController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
// {
//     if (!BlackboardComponent) return;
//
//     if (Actor->ActorHasTag("Player"))
//     {
//         if (Stimulus.WasSuccessfullySensed())
//         {
//             BlackboardComponent->SetValueAsBool("PlayerDetected", true);
//             BlackboardComponent->SetValueAsObject("TargetActor", Actor);
//         }
//         else
//         {
//             BlackboardComponent->SetValueAsBool("PlayerDetected", false);
//             BlackboardComponent->ClearValue("TargetActor");
//         }
//     }
// }
//
// AActor* AEnemyController::GetNextPatrolPoint()
// {
//     if (PatrolPoints.Num() == 0)
//     {
//         return nullptr;
//     }
//
//     AActor* NextPoint = PatrolPoints[CurrentPatrolIndex];
//     CurrentPatrolIndex = (CurrentPatrolIndex + 1) % PatrolPoints.Num();
//     return NextPoint;
// }


// #include "EnemyController.h"
// #include "NavigationSystem.h"
// #include "BehaviorTree/BehaviorTree.h"
// #include "Kismet/GameplayStatics.h"
// #include "Navigation/PathFollowingComponent.h"
// #include "BehaviorTree/BlackboardComponent.h"
// #include "GameFramework/Actor.h"
// #include "Engine/World.h"
// #include "Perception/AIPerceptionComponent.h"
// #include "TimerManager.h"  // For delay handling
//
// AEnemyController::AEnemyController(FObjectInitializer const& ObjectInitializer)
// {
// 	PrimaryActorTick.bCanEverTick = true;
//
// 	// Load the behavior tree asset
// 	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviorTreeAsset(TEXT("BehaviorTree'/Game/AI/BT_EnemyBehaviour.BT_EnemyBehaviour'"));
// 	if (BehaviorTreeAsset.Succeeded())
// 	{
// 		BehaviorTree = BehaviorTreeAsset.Object;
// 	}
//
// 	// Create components
// 	BehaviorTreeComponent = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorTreeComponent"));
// 	BlackboardComponent = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComponent"));
//
// 	// Perception components
// 	AIPerceptionComponent = ObjectInitializer.CreateDefaultSubobject<UAIPerceptionComponent>(this, TEXT("AIPerceptionComponent"));
// 	SightConfig = ObjectInitializer.CreateDefaultSubobject<UAISenseConfig_Sight>(this, TEXT("SightConfig"));
//
// 	AIPerceptionComponent->ConfigureSense(*SightConfig);
// 	AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
// 	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyController::OnTargetPerceptionUpdated);
//
// 	// Initial variables
// 	bIsPatrolling = false;
// 	PatrolWaitTime = 2.0f; // You can adjust this wait time between patrol points
// }
//
// void AEnemyController::BeginPlay()
// {
// 	Super::BeginPlay();
// 	if (BehaviorTree)
// 	{
// 		RunBehaviorTree(BehaviorTree);
// 		BehaviorTreeComponent->StartTree(*BehaviorTree);
// 	}
// }
//
// void AEnemyController::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);
//
// 	// Only find a new location when we're not patrolling
// 	if (!bIsPatrolling)
// 	{
// 		// Find a random location in the navigable area
// 		FVector RandomLocation = GetRandomLocationInNavigableRadius(GetPawn()->GetActorLocation(), 20000.f);  // 2000.f is the radius you can adjust
//
// 		if (RandomLocation != FVector::ZeroVector)  // Make sure a valid location was found
// 		{
// 			// Move to the random location
// 			MoveToLocation(RandomLocation);
//
// 			// Start the patrolling timer (the AI will wait at the location)
// 			bIsPatrolling = true;
// 			GetWorld()->GetTimerManager().SetTimer(PatrolTimerHandle, this, &AEnemyController::OnPatrolComplete, PatrolWaitTime, false);
// 		}
// 	}
// }
//
// void AEnemyController::OnPossess(APawn* InPawn)
// {
// 	Super::OnPossess(InPawn);
// 	if (BlackboardComponent && BehaviorTree)
// 	{
// 		// Initialize the blackboard
// 		BlackboardComponent->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
//
// 		//BlackboardComponent->SetValueAsObject("NextPatrolPoint", Cast<UObject>(GetRandomLocationInNavigableRadius(GetPawn()->GetActorLocation(), 20000.f)));
// 		BlackboardComponent->SetValueAsBool("PlayerDetected", false);
// 		BlackboardComponent->SetValueAsObject("TargetActor", nullptr);
// 		BlackboardComponent->SetValueAsFloat("PatrolWaitTime", 2.0f);  // Example value
// 		BlackboardComponent->SetValueAsBool("bIsPatrolling", true);
// 	}
// }
//
// void AEnemyController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
// {
// 	if (!BlackboardComponent) return;
//
// 	if (Actor->ActorHasTag("Player"))
// 	{
// 		if (Stimulus.WasSuccessfullySensed())
// 		{
// 			BlackboardComponent->SetValueAsBool("PlayerDetected", true);
// 			BlackboardComponent->SetValueAsObject("TargetActor", Actor);
// 		}
// 		else
// 		{
// 			BlackboardComponent->SetValueAsBool("PlayerDetected", false);
// 			BlackboardComponent->ClearValue("TargetActor");
// 		}
// 	}
// }
//
// FVector AEnemyController::GetRandomLocationInNavigableRadius(FVector Origin, float Radius)
// {
// 	FNavLocation RandomNavLocation;
//
// 	// Get a random point within a radius, on the navigation mesh
// 	if (UNavigationSystemV1::GetNavigationSystem(GetWorld())->GetRandomPointInNavigableRadius(Origin, Radius, RandomNavLocation))
// 	{
// 		return RandomNavLocation.Location;  // Return the found random location
// 	}
// 	return FVector::ZeroVector;  // Return an invalid vector if no valid location was found
// }
//
// void AEnemyController::OnPatrolComplete()
// {
// 	// Reset the patrolling state and allow the AI to pick a new patrol point
// 	bIsPatrolling = false;
// }

// #include "EnemyController.h"
//
// #include "BehaviorTree/BehaviorTree.h"
// #include "BehaviorTree/BlackboardComponent.h"
// #include "FirstPersonTD/Characters/MyFPSCharacter.h"
// #include "Perception/AIPerceptionComponent.h"
// #include "Perception/AISenseConfig_Sight.h"
// #include "Navigation/PathFollowingComponent.h"
//
//
//
// AEnemyController::AEnemyController()
// {
// 	SetupPerceptionSystem();
// }
//
// void AEnemyController::BeginPlay()
// {
// 	Super::BeginPlay();
//
// 	
// }
//
// void AEnemyController::OnPossess(APawn* InPawn)
// {
// 	Super::OnPossess(InPawn);
//
// 	
// 	
// 		// Setup perception values
// 		SightConfig->SightRadius = 5000.f;
// 		SightConfig->LoseSightRadius = 5000.0f;
// 		SightConfig->PeripheralVisionAngleDegrees = 90.0f;
// 		SightConfig->SetMaxAge(5.0f);
//
// 		if (UBehaviorTree* BehaviorTree = Cast<UBehaviorTree>(StaticLoadObject(UBehaviorTree::StaticClass(), nullptr, TEXT("BehaviorTree'/Game/AI/BT_EnemyBehaviour.BT_EnemyBehaviour'"))))
// 		{
// 			// INFO: Initialize the blackboard
// 			if (Blackboard)
// 			{
// 				Blackboard->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
// 			}
// 			
// 			// INFO: Run the behavior tree
// 			{
// 				UBlackboardComponent* BlackboardComponent = nullptr;
// 				UseBlackboard(BehaviorTree->BlackboardAsset, BlackboardComponent);
// 				Blackboard = BlackboardComponent;
// 				RunBehaviorTree(BehaviorTree);
// 				Blackboard->SetValueAsBool("PlayerDetected", false);
// 			}
// 		}
// }
//
// void AEnemyController::SetupPerceptionSystem()
// {
// 	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
// 	SightConfig->SightRadius = 1000.0f;
// 	SightConfig->LoseSightRadius = SightConfig->SightRadius + 100.0f;
// 	SightConfig->PeripheralVisionAngleDegrees = 180.0f;
// 	SightConfig->SetMaxAge(4.0f);
// 	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
// 	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
// 	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
//
// 	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
// 	SetPerceptionComponent(*PerceptionComponent);
// 	PerceptionComponent->ConfigureSense(*SightConfig);
// 	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyController::OnTargetDetected);
// }
//
// void AEnemyController::OnTargetDetected(AActor* Actor, const FAIStimulus Stimulus)
// {
// 	
// 	if (AMyFPSCharacter* Ch = Cast<AMyFPSCharacter>(Actor))
// 	{
// 		if (Ch->CurrentHealth > 0)
// 		{
// 			Blackboard->SetValueAsBool("PlayerDetected", false);
// 			return;
// 		}
// 		Blackboard->SetValueAsBool("PlayerDetected", Stimulus.WasSuccessfullySensed());
// 		//Blackboard->SetValueAsObject("TargetActor", Actor);
// 	}
// }

#include "EnemyController.h"

#include "FirstPersonTD/Characters/EnemyCharacter.h"

AEnemyController::AEnemyController(FObjectInitializer const& ObjectInitializer)
{
	SetupPerceptionSystem();
}

void AEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (AEnemyCharacter* const EnemyCharacter = Cast<AEnemyCharacter>(InPawn))
	{
		if (UBehaviorTree* BehaviorTree = EnemyCharacter->GetBehaviorTree())
		{
			UBlackboardComponent* b;
			UseBlackboard(BehaviorTree->BlackboardAsset, b);
			Blackboard = b;
			RunBehaviorTree(BehaviorTree);
		}
	}
}

void AEnemyController::SetupPerceptionSystem()
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	if (SightConfig)
	{
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent")));
		SightConfig->SightRadius = 500.0f;
		SightConfig->LoseSightRadius = SightConfig->SightRadius + 25.f;
		SightConfig->PeripheralVisionAngleDegrees = 90.0f;
		SightConfig->SetMaxAge(5.0f);
		SightConfig->AutoSuccessRangeFromLastSeenLocation = 520.f;
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

		GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyController::OnTargetDetected);
		GetPerceptionComponent()->ConfigureSense(*SightConfig);
	}
}

void AEnemyController::OnTargetDetected(AActor* Actor, const FAIStimulus Stimulus)
{
	if (auto* const PlayerCharacter = Cast<AMyFPSCharacter>(Actor))
	{
		GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", Stimulus.WasSuccessfullySensed());
	}
}

