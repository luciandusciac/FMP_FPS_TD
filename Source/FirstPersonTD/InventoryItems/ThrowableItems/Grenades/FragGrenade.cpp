// Fill out your copyright notice in the Description page of Project Settings.


#include "FragGrenade.h"

#include "Components/SphereComponent.h"
#include "Engine/OverlapResult.h"
#include "FirstPersonTD/Characters/EnemyCharacter.h"
#include "FirstPersonTD/Characters/MyFPSCharacter.h"
#include "Kismet/GameplayStatics.h"


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
	if (UStaticMeshComponent* MeshComp = this->FindComponentByClass<UStaticMeshComponent>())
	{
		TArray<FOverlapResult> OverlapResults;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);

		bool bHasHit = GetWorld()->OverlapMultiByChannel(
			OverlapResults,
			MeshComp->GetComponentLocation(),
			FQuat::Identity,
			ECC_WorldDynamic,
			FCollisionShape::MakeSphere(300.f), // radius of the explosion, may need some tweaking
			CollisionParams
		);
	
	
		if (bHasHit)
		{
			// for (FOverlapResult Result : OverlapResults)
			// {
			// 	AActor* OverlappingActor = Result.GetActor();
			// 	if (OverlappingActor && OverlappingActor != this)
			// 	{
			// 		UE_LOG(LogTemp, Warning, TEXT("Overlapping Actor: %s"), *OverlappingActor->GetName());
			// 		if (AMyFPSCharacter* C = Cast<AMyFPSCharacter>(OverlappingActor))
			// 		{
			// 			C->TakeDamage(60.f);
			// 		}
			// 		else if (AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(OverlappingActor))
			// 		{
			// 			Enemy->TakeDamage(60.f);
			// 		}
			// 	}
			// }

			TSet<AActor*> DamagedActors;

			for (FOverlapResult Result : OverlapResults)
			{
				AActor* OverlappingActor = Result.GetActor();
				if (OverlappingActor && OverlappingActor != this && !DamagedActors.Contains(OverlappingActor))
				{
					DamagedActors.Add(OverlappingActor);

					UE_LOG(LogTemp, Warning, TEXT("Overlapping Actor: %s"), *OverlappingActor->GetName());

					if (AMyFPSCharacter* C = Cast<AMyFPSCharacter>(OverlappingActor))
					{
						C->TakeDamage(60.f);
					}
					else if (AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(OverlappingActor))
					{
						Enemy->TakeDamage(60.f);
					}
				}
			}
		}
	
		GetWorldTimerManager().ClearTimer(ExplosionTimerHandle);

		//if (UStaticMeshComponent* MeshComp = this->FindComponentByClass<UStaticMeshComponent>())
		//{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionVFX->GetAsset(), MeshComp->GetComponentLocation());
		//}
		UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Frag grenade exploded!"));
		this->Destroy();
	}
}

void AFragGrenade::Use()
{
	Super::Use();
	//bCanExplode = true;
}

