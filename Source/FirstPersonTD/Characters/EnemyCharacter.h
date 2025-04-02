// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyFPSCharacter.h"
#include "Runtime/AIModule/Classes/Perception/AIPerceptionComponent.h"
#include "Runtime/AIModule/Classes/Perception/AISenseConfig_Sight.h"
#include "Runtime/AIModule/Classes/Perception/AISense_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BehaviorTree.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "../Controller/EnemyController.h"
#include "FirstPersonTD/AI/PatrolPath.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Runtime/AIModule/Classes/Perception/AIPerceptionTypes.h"
#include "EnemyCharacter.generated.h"


UCLASS()
class FIRSTPERSONTD_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

	UBehaviorTree* GetBehaviorTree() const;

	APatrolPath* GetPatrolPath() const;
	
protected:
	void BeginPlay() override;
	
	

private:
	UPROPERTY()
	UAIPerceptionStimuliSourceComponent* StimulusSource;

	void SetUpStimulusSource();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy AI", meta= (AllowPrivateAccess = "true"))
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy AI", meta= (AllowPrivateAccess = "true"))
	APatrolPath* PatrolPath;
	
	
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy AI")
	// UBehaviorTree* BehaviorTree;
	//
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy AI")
	// UAIPerceptionComponent* PerceptionComponent;
	//
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy AI")
	// UAISenseConfig_Sight* SightConfig;
	//
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy AI")
	// UAISenseConfig_Hearing* HearingConfig;
	//
	// UFUNCTION()
	// void OnTargetDetected(AActor* Actor, FAIStimulus Stimulus);
};
