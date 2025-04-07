// Fill out your copyright notice in the Description page of Project Settings.


#include "SmokeGrenade.h"


// Sets default values
ASmokeGrenade::ASmokeGrenade()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ExplosionTime = 3.0f;
}

// Called when the game starts or when spawned
void ASmokeGrenade::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASmokeGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bCanExplode)
	{
		Explode();
		bCanExplode = false;
	}
}

void ASmokeGrenade::Explode()
{
	GetWorldTimerManager().SetTimer(ExplosionTimerHandle, this, &ASmokeGrenade::OnExplode, ExplosionTime, false);

	
}

void ASmokeGrenade::OnExplode()
{
	UE_LOG(LogTemp, Warning, TEXT("Smoke Explosion"));

	if (UStaticMeshComponent* MeshComp = this->FindComponentByClass<UStaticMeshComponent>())
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionVFX->GetAsset(), MeshComp->GetComponentLocation());
		
	}

	this->Destroy();
}

