// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"


#include "Perception/AIPerceptionStimuliSourceComponent.h"


AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SetUpStimulusSource();
	
	// Weapon = CreateDefaultSubobject<USceneComponent>(TEXT("Weapon"));
	// Weapon->SetupAttachment(GetMesh(), TEXT("RightHand"));
	// Weapon->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	// Weapon->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	// Weapon->SetRelativeScale3D(FVector(1.f, 1.f, 1.f));
	// Weapon->SetMobility(EComponentMobility::Movable);
	
}

UBehaviorTree* AEnemyCharacter::GetBehaviorTree() const
{
	return BehaviorTree;
}

APatrolPath* AEnemyCharacter::GetPatrolPath() const
{
	return PatrolPath;
}


void AEnemyCharacter::Shoot() const
{
	Weapon->Shoot();
	//Cast<ABaseWeapon>(Weapon)->Shoot();
}

void AEnemyCharacter::BeginPlay()
{

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	Weapon = GetWorld()->SpawnActor<ABaseWeapon>(Weapon.GetClass(), GetActorLocation(), GetActorRotation(), SpawnParams);
	
	if (Weapon)
	{
		Weapon->Mesh->SetSimulatePhysics(false);
		//Weapon->SetOwner(this);

		const FAttachmentTransformRules CustomAttachmentRules(EAttachmentRule::KeepWorld, EAttachmentRule::SnapToTarget,
														  EAttachmentRule::KeepWorld, true);

		bool bAttached = Weapon->AttachToActor(this, CustomAttachmentRules);
		//Weapon->SetActorRelativeLocation(FVector(10, 0, 0)); // Adjust this as needed
		//Weapon->SetActorRelativeRotation(FRotator(0, 90, 0));

		if (bAttached)
		{
			UE_LOG(LogTemp, Warning, TEXT("Weapon attached successfully!"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Weapon attachment FAILED!"));
		}
		Weapon->SetCurrentAmmo(100000);  // infinite ammo for npc
		//Weapon->Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
		
		
		
		//Weapon->SetActorScale3D(FVector(2.f, 2.f, 2.f)); // Adjust the scale as needed
		
		UE_LOG(LogTemp, Warning, TEXT("Weapon spawned successfully!"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn weapon!"));
	}
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
