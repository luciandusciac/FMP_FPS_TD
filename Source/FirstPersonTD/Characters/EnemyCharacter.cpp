// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"


#include "Perception/AIPerceptionStimuliSourceComponent.h"


AEnemyCharacter::AEnemyCharacter() : CurrentHealth(100.f)
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


void AEnemyCharacter::Shoot()
{
	if (bCanShoot)
	{
		Weapon->Shoot();
		bCanShoot = false;
		GetWorldTimerManager().SetTimer(ShootTimerHandle, this, &AEnemyCharacter::ResetShoot, 1.5f, false);
	}
	//Cast<ABaseWeapon>(Weapon)->Shoot();
}

void AEnemyCharacter::TakeDamage(float DamageAmount)
{
	CurrentHealth -= DamageAmount;
	if (CurrentHealth <= 0)
		Die();
}

void AEnemyCharacter::Die()
{
	this->Weapon->Destroy();
	this->Destroy();
}

void AEnemyCharacter::BeginPlay()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	Weapon = GetWorld()->SpawnActor<ABaseWeapon>(Weapon.GetClass(), GetActorLocation(), GetActorRotation(), SpawnParams);
	
	
	Weapon->Mesh->SetSimulatePhysics(false);
	Weapon->Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Weapon->Mesh->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("RightHand"));
	Weapon->SetOwner(this);
	
	Weapon->Mesh->SetRelativeRotation(FRotator(0, 180, 90));
	Weapon->Mesh->SetRelativeLocation(WeaponOffset);
	Weapon->Mesh->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.3f));
	
	Weapon->SetCurrentAmmo(100000);  // infinite ammo for npc
	
	UE_LOG(LogTemp, Warning, TEXT("Weapon spawned successfully!"));
	
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

void AEnemyCharacter::ResetShoot()
{
	bCanShoot = true;
	GetWorldTimerManager().ClearTimer(ShootTimerHandle);
}

// void AEnemyCharacter::OnTargetDetected(AActor* Actor, FAIStimulus Stimulus)
// {
// 	if(Actor)
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("Target Detected"));
// 	}
// }
