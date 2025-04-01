// // // Fill out your copyright notice in the Description page of Project Settings.
// //
// // #pragma once
// //
// // #include "CoreMinimal.h"
// // #include "Runtime/AIModule/Classes/AIController.h"
// // #include "Runtime/AIModule/Classes/Perception/AIPerceptionComponent.h"
// // #include "Runtime/AIModule/Classes/BehaviorTree/BehaviorTreeComponent.h"
// // #include "Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h"
// // #include "EnemyController.generated.h"
// //
// // UCLASS()
// // class FIRSTPERSONTD_API AEnemyController : public AAIController
// // {
// // 	GENERATED_BODY()
// //
// // public:
// // 	// Sets default values for this actor's properties
// // 	AEnemyController(FObjectInitializer const& ObjectInitializer);
// //
// // protected:
// // 	// Called when the game starts or when spawned
// // 	virtual void BeginPlay() override;
// //
// // public:
// // 	// Called every frame
// // 	virtual void Tick(float DeltaTime) override;
// //
// // 	void OnPossess(APawn* InPawn) override;
// //
// // 	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI")
// // 	UBlackboardComponent* BlackboardComponent;
// // 	
// // 	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI")
// // 	UBehaviorTreeComponent* BehaviorTreeComponent;
// //
// // 	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI")
// // 	UBehaviorTree* BehaviorTree;
// // 	
// // };
// //
// //
// //
// //
// //
//
// #pragma once
//
// #include "CoreMinimal.h"
// #include "AIController.h"
// #include "Perception/AIPerceptionComponent.h"
// #include "Perception/AISenseConfig_Sight.h"
// #include "BehaviorTree/BlackboardComponent.h"
// #include "BehaviorTree/BehaviorTreeComponent.h"
// #include "EnemyController.generated.h"
//
// UCLASS()
// class FIRSTPERSONTD_API AEnemyController : public AAIController
// {
// 	GENERATED_BODY()
//
// public:
// 	AEnemyController(FObjectInitializer const& ObjectInitializer);
// 	AActor* GetNextPatrolPoint();
//
// protected:
// 	virtual void BeginPlay() override;
// 	virtual void Tick(float DeltaTime) override;
// 	virtual void OnPossess(APawn* InPawn) override;
// 	UFUNCTION()
// 	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
//
// private:
// 	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
// 	UBlackboardComponent* BlackboardComponent;
//     
// 	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
// 	UBehaviorTreeComponent* BehaviorTreeComponent;
//     
// 	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
// 	UBehaviorTree* BehaviorTree;
//
// 	UPROPERTY(VisibleAnywhere, Category = "AI")
// 	UAIPerceptionComponent* AIPerceptionComponent;
//     
// 	UPROPERTY(VisibleAnywhere, Category = "AI")
// 	UAISenseConfig_Sight* SightConfig;
//
// 	UPROPERTY(EditAnywhere, Category = "AI")
// 	TArray<AActor*> PatrolPoints;
// 	int32 CurrentPatrolIndex = 0;
// };



// #pragma once
//
// #include "CoreMinimal.h"
// #include "AIController.h"
// #include "BehaviorTree/BehaviorTreeComponent.h"
// #include "BehaviorTree/BlackboardComponent.h"
// #include "Perception/AIPerceptionComponent.h"
// #include "GameFramework/Actor.h"
// #include "TimerManager.h"  // For delay handling
// #include "Perception/AISenseConfig_Sight.h"
// #include "EnemyController.generated.h"
//
// UCLASS()
// class FIRSTPERSONTD_API AEnemyController : public AAIController
// {
// 	GENERATED_BODY()
//
// public:
// 	AEnemyController(FObjectInitializer const& ObjectInitializer);
//
// protected:
// 	virtual void BeginPlay() override;
// 	virtual void Tick(float DeltaTime) override;
//
// 	// Function to handle when the AI has completed patrolling
// 	void OnPatrolComplete();
//
// 	// Function to get a random location within a navigable area
// 	FVector GetRandomLocationInNavigableRadius(FVector Origin, float Radius);
//
// 	// Handle target perception updates
// 	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
//
// 	// Patrol state
// 	bool bIsPatrolling;
//
// 	// Timer handle for patrolling wait time
// 	FTimerHandle PatrolTimerHandle;
//
// 	// Patrol wait time
// 	float PatrolWaitTime;
//
// public:
// 	// Components
// 	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
// 	UBehaviorTreeComponent* BehaviorTreeComponent;
//
// 	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
// 	UBlackboardComponent* BlackboardComponent;
//
// 	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
// 	UAIPerceptionComponent* AIPerceptionComponent;
//
// 	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
// 	UAISenseConfig_Sight* SightConfig;
//
// 	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI")
// 	UBehaviorTree* BehaviorTree;
//
// 	void OnPossess(APawn* InPawn) override;
// };


// #pragma once
//
// #include "CoreMinimal.h"
// #include "AIController.h"
// #include "Perception/AIPerceptionTypes.h"
// #include "Perception/AIPerceptionComponent.h"
// #include "Perception/AISenseConfig_Sight.h"
// #include "BehaviorTree/BehaviorTreeComponent.h"
// #include "BehaviorTree/BlackboardComponent.h"
// #include "EnemyController.generated.h"
//
// class UAISenseConfig_Sight;
// class UBehaviorTree;
// class UBlackboardComponent;
//
//
// UCLASS()
// class FIRSTPERSONTD_API AEnemyController : public AAIController
// {
// 	GENERATED_BODY()
//
// public:
// 	AEnemyController();
//
// protected:
// 	virtual void OnPossess(APawn* InPawn) override;
// 	virtual void BeginPlay() override;
//
// private:
// 	void SetupPerceptionSystem();
//
// 	UFUNCTION()
// 	void OnTargetDetected(AActor* Actor, const FAIStimulus Stimulus);
//
// 	// AI Perception
// 	
// 	TObjectPtr<UAISenseConfig_Sight> SightConfig;
//
// 	TObjectPtr<UAIPerceptionComponent> PerceptionComponent;
//
// 	// Blackboard
// 	
// 	TObjectPtr<UBlackboardComponent> Blackboard;
//
// 	bool bAlwaysSeePlayer = false;
// };


#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyController.generated.h"

UCLASS()
class AEnemyController : public AAIController
{
private:
	GENERATED_BODY()

public:
	explicit AEnemyController(FObjectInitializer const& ObjectInitializer);

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY()
	class UAISenseConfig_Sight* SightConfig;

	void SetupPerceptionSystem();

	UFUNCTION()
	void OnTargetDetected(AActor* Actor, const FAIStimulus Stimulus);
		
};


