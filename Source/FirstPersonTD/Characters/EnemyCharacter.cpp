// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"


#include "Components/CapsuleComponent.h"
#include "FirstPersonTD/InventoryItems/ThrowableItems/Knives/Knife.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"


AEnemyCharacter::AEnemyCharacter() : CurrentHealth(100.f)
{
	PrimaryActorTick.bCanEverTick = true;

	SetUpStimulusSource();

	this->GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::OnComponentBeginOverlap);
	
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
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);
	
	FVector Impulse = FVector(0, 0, 1000); 
	GetMesh()->AddImpulse(Impulse, NAME_None, true);
	
	SetLifeSpan(10.0f);

	this->Weapon->Mesh->SetSimulatePhysics(true);
	this->Weapon->Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	this->Weapon->Mesh->SetCollisionProfileName(TEXT("Ragdoll"));
	
	this->Weapon->SetOwner(nullptr);

	FTimerHandle RespawnTimer;
	GetWorldTimerManager().SetTimer(RespawnTimer, this, &AEnemyCharacter::Respawn, 3.0f, false);
	
	//this->Weapon->Destroy();
	//this->Destroy();
}

void AEnemyCharacter::Respawn()
{
	FName CurrentLevel = *UGameplayStatics::GetCurrentLevelName(GetWorld());
	UGameplayStatics::OpenLevel(this, CurrentLevel);
}

void AEnemyCharacter::BeginPlay()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	Weapon = GetWorld()->SpawnActor<ABaseWeapon>(Weapon.GetClass(), GetActorLocation(), GetActorRotation(), SpawnParams);
	
	if (!Weapon)
		return;
	
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

void AEnemyCharacter::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(AKnife::StaticClass()))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Knife hit!"));
		Die();
		OtherActor->Destroy();
	}
}

