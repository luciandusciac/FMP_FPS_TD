// Fill out your copyright notice in the Description page of Project Settings.


#include "FragGrenade.h"

#include "SWarningOrErrorBox.h"
#include "Components/SphereComponent.h"
#include "Engine/OverlapResult.h"
#include "FirstPersonTD/Characters/EnemyCharacter.h"
#include "FirstPersonTD/Characters/MyFPSCharacter.h"


// Sets default values
AFragGrenade::AFragGrenade()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	//SphereComponent->SetupAttachment(RootComponent);
	//SphereComponent->Deactivate();

	//SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//SphereComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	//SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	//SphereComponent->InitSphereRadius(10.f);

	ExplosionTime = 3.0f;
}

// Called when the game starts or when spawned
void AFragGrenade::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFragGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bCanExplode)
	{
		Explode();
		bCanExplode = false;
	}
}

void AFragGrenade::Explode()
{
	//Super::Explode();
	//SphereComponent->SetActive(true);
	//SphereComponent->Activate();

	GetWorldTimerManager().SetTimer(ExplosionTimerHandle, this, &AFragGrenade::OnExplode, ExplosionTime, false);
	
	// TArray<AActor*> OverlappingActors;
	//
	// SphereComponent->GetOverlappingActors(OverlappingActors);
	//
	// UE_LOG(LogTemp, Warning, TEXT("Frag Explosion"));
	//
	// for (AActor* Actor : OverlappingActors)
	// {
	// 	if(Cast<AMyFPSCharacter>(Actor))
	// 		UE_LOG(LogTemp, Warning, TEXT("Actor: %s"), *Actor->GetName());
	// }

	
}

void AFragGrenade::OnExplode()
{

	//Super::OnExplode();

	
	
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	
	bool bHasHit = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		GetActorLocation(),
		FQuat::Identity,
		ECC_WorldDynamic,
		FCollisionShape::MakeSphere(100.f),
		CollisionParams
	);
	
	if (bHasHit)
	{
		for (FOverlapResult Result : OverlapResults)
		{
			AActor* OverlappingActor = Result.GetActor();
			if (OverlappingActor && OverlappingActor != this && (Cast<AMyFPSCharacter>(OverlappingActor) || Cast<AEnemyCharacter>(OverlappingActor)))
			{
				UE_LOG(LogTemp, Warning, TEXT("Overlapping Actor: %s"), *OverlappingActor->GetName());
			}
		}
	}
	
	GetWorldTimerManager().ClearTimer(ExplosionTimerHandle);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionVFX->GetAsset(), this->GetActorLocation());
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Frag grenade exploded!"));
	this->Destroy();
}

void AFragGrenade::Use()
{
	Super::Use();
	//bCanExplode = true;
}

