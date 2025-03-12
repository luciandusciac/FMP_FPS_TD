// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGrenade.h"

#include "TimerManager.h"


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
}

// Called when the game starts or when spawned
void ABaseGrenade::BeginPlay()
{
	Super::BeginPlay();

	//GetWorldTimerManager().SetTimer(ExplosionTimerHandle, this, &ABaseGrenade::Explode, ExplosionTime, false);
}

// Called every frame
void ABaseGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

void ABaseGrenade::Explode()
{
	//GetWorldTimerManager().SetTimer(ExplosionTimerHandle, this, &ABaseGrenade::OnExplode, ExplosionTime, false);
	
	//GetWorldTimerManager().ClearTimer(ExplosionTimerHandle);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionVFX->GetAsset(), GetActorLocation());
	
}

void ABaseGrenade::OnExplode()
{
	//GetWorldTimerManager().ClearTimer(ExplosionTimerHandle);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionVFX->GetAsset(), GetActorLocation());
}

void ABaseGrenade::Use()
{
	//Super::Use();
	//bCanExplode = true;
	//Explode();
}

