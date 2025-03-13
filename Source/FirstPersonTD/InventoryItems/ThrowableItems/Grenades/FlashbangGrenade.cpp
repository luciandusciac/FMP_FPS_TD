// Fill out your copyright notice in the Description page of Project Settings.


#include "FlashbangGrenade.h"

#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AFlashbangGrenade::AFlashbangGrenade()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ExplosionTime = 3.0f;
}

// Called when the game starts or when spawned
void AFlashbangGrenade::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFlashbangGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bCanExplode)
	{
		Explode();
		bCanExplode = false;
	}
}

void AFlashbangGrenade::Explode()
{
	//Super::Explode();
 
	GetWorldTimerManager().SetTimer(ExplosionTimerHandle, this, &AFlashbangGrenade::OnExplode, ExplosionTime, false);
}

void AFlashbangGrenade::OnExplode()
{
	UE_LOG(LogTemp, Warning, TEXT("Flashbang Explosion"));

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacter::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		FVector DirectionToExplosion = (GetActorLocation() - Actor->GetActorLocation()).GetSafeNormal();
		FVector ActorForwardVector = Actor->GetActorForwardVector();
		
		if (FVector::DotProduct(DirectionToExplosion, ActorForwardVector) > 0.5f)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s is facing the explosion"), *Actor->GetName());
		}
	}

	this->Destroy();
}

