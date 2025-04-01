// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"

#include "Perception/AIPerceptionStimuliSourceComponent.h"


AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SetUpStimulusSource();

	
	// PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	//
	// SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	// SightConfig->SightRadius = 1000.0f;
	// SightConfig->LoseSightRadius = 1200.0f;
	// SightConfig->PeripheralVisionAngleDegrees = 90.0f;
	// SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	// SightConfig->DetectionByAffiliation.bDetectNeutrals = false;
	// SightConfig->DetectionByAffiliation.bDetectFriendlies = false;
	//
	//
	// HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	//
	// PerceptionComponent->ConfigureSense(*SightConfig);
	// PerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
	//
	//
	// PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyCharacter::OnTargetDetected);
}

UBehaviorTree* AEnemyCharacter::GetBehaviorTree() const
{
	return BehaviorTree;
}


void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyCharacter::SetUpStimulusSource()
{
	StimulusSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("StimulusSource"));
	if (StimulusSource)
	{
		StimulusSource->RegisterForSense(TSubclassOf<UAISense_Sight>());
		StimulusSource->RegisterWithPerceptionSystem();
	}
}

// void AEnemyCharacter::OnTargetDetected(AActor* Actor, FAIStimulus Stimulus)
// {
// 	if(Actor)
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("Target Detected"));
// 	}
// }
