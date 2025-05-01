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

	bIsConsumable = true;
	//GetWorldTimerManager().SetTimer(ExplosionTimerHandle, this, &ABaseGrenade::Explode, ExplosionTime, false);
}

// Called every frame
void ABaseGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseGrenade::Explode()
{
}

void ABaseGrenade::OnExplode()
{
}

void ABaseGrenade::Use()
{
}


