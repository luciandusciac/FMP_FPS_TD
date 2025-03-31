// // Fill out your copyright notice in the Description page of Project Settings.
//
// #pragma once
//
// #include "CoreMinimal.h"
// #include "Runtime/AIModule/Classes/AIController.h"
// #include "Runtime/AIModule/Classes/Perception/AIPerceptionComponent.h"
// #include "Runtime/AIModule/Classes/BehaviorTree/BehaviorTreeComponent.h"
// #include "Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h"
// #include "EnemyController.generated.h"
//
// UCLASS()
// class FIRSTPERSONTD_API AEnemyController : public AAIController
// {
// 	GENERATED_BODY()
//
// public:
// 	// Sets default values for this actor's properties
// 	AEnemyController(FObjectInitializer const& ObjectInitializer);
//
// protected:
// 	// Called when the game starts or when spawned
// 	virtual void BeginPlay() override;
//
// public:
// 	// Called every frame
// 	virtual void Tick(float DeltaTime) override;
//
// 	void OnPossess(APawn* InPawn) override;
//
// 	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI")
// 	UBlackboardComponent* BlackboardComponent;
// 	
// 	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI")
// 	UBehaviorTreeComponent* BehaviorTreeComponent;
//
// 	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI")
// 	UBehaviorTree* BehaviorTree;
// 	
// };
//
//
//
//
//

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "EnemyController.generated.h"

UCLASS()
class FIRSTPERSONTD_API AEnemyController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyController(FObjectInitializer const& ObjectInitializer);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnPossess(APawn* InPawn) override;
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	AActor* GetNextPatrolPoint();

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UBlackboardComponent* BlackboardComponent;
    
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UBehaviorTreeComponent* BehaviorTreeComponent;
    
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* BehaviorTree;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	UAIPerceptionComponent* AIPerceptionComponent;
    
	UPROPERTY(VisibleAnywhere, Category = "AI")
	UAISenseConfig_Sight* SightConfig;

	UPROPERTY(EditAnywhere, Category = "AI")
	TArray<AActor*> PatrolPoints;
	int32 CurrentPatrolIndex = 0;
};
