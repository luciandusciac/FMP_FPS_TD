// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGrenade.h"

#include "TimerManager.h"
#include "FirstPersonTD/Characters/MyFPSCharacter.h"


// Sets default values
ABaseGrenade::ABaseGrenade()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	ExplosionVFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ExplosionVFX"));
	ExplosionVFX->SetupAttachment(Root);

	bCanExplode = false;
	bMeshDisabled = false;

	
}

// Called when the game starts or when spawned
void ABaseGrenade::BeginPlay()
{
	Super::BeginPlay();

	if (UStaticMeshComponent* MeshComp = this->FindComponentByClass<UStaticMeshComponent>())
	{
		MeshComp->SetSimulatePhysics(true);
		MeshComp->SetEnableGravity(true);
		MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		MeshComp->BodyInstance.SetUseCCD(true);
		
	}
	
	//GetWorldTimerManager().SetTimer(ExplosionTimerHandle, this, &ABaseGrenade::Explode, ExplosionTime, false);
}

// Called every frame
void ABaseGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// if (bMeshDisabled)
	// {
	// 	if (UStaticMeshComponent* MeshComp = this->FindComponentByClass<UStaticMeshComponent>())
	// 	{
	// 		MeshComp->SetSimulatePhysics(false);
	// 		MeshComp->SetEnableGravity(false);
	// 		
	// 	}
	// 	bMeshDisabled = false;
	// }
}

void ABaseGrenade::Explode()
{
	//GetWorldTimerManager().SetTimer(ExplosionTimerHandle, this, &ABaseGrenade::OnExplode, ExplosionTime, false);
	
	//GetWorldTimerManager().ClearTimer(ExplosionTimerHandle);
	//UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionVFX->GetAsset(), GetActorLocation());
	
}

void ABaseGrenade::OnExplode()
{
	//GetWorldTimerManager().ClearTimer(ExplosionTimerHandle);
	//UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionVFX->GetAsset(), GetActorLocation());
}

void ABaseGrenade::Use()
{
	//Super::Use();
	//bCanExplode = true;
	//Explode();
}

// void ABaseGrenade::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
// 	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
// {
// 	if (Cast<AMyFPSCharacter>(OtherActor))
// 	{
// 		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Grenade added"));
// 		if (UStaticMeshComponent* MeshComp = this->FindComponentByClass<UStaticMeshComponent>())
// 		{
// 			MeshComp->SetSimulatePhysics(false);
// 			MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
// 			MeshComp->SetMassScale(NAME_None, 0.0f);
// 			MeshComp->SetEnableGravity(false);
// 			MeshComp->WakeRigidBody();
// 			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Grenade mesh disabled"));
// 		}
// 	}
// }

