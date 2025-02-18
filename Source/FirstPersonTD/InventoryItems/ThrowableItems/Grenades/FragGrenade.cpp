// Fill out your copyright notice in the Description page of Project Settings.


#include "FragGrenade.h"

#include "SWarningOrErrorBox.h"
#include "Components/SphereComponent.h"
#include "FirstPersonTD/Characters/MyFPSCharacter.h"


// Sets default values
AFragGrenade::AFragGrenade()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(RootComponent);

	//SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//SphereComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	//SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereComponent->InitSphereRadius(10.f);

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
}

void AFragGrenade::Explode()
{
	Super::Explode();

	TArray<AActor*> OverlappingActors;
	SphereComponent->GetOverlappingActors(OverlappingActors);

	UE_LOG(LogTemp, Warning, TEXT("Frag Explosion"));

	for (AActor* Actor : OverlappingActors)
	{
		if(Cast<AMyFPSCharacter>(Actor))
			UE_LOG(LogTemp, Warning, TEXT("Actor: %s"), *Actor->GetName());
	}

	this->Destroy();
}

