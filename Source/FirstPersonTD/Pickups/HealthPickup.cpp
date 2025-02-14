// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPickup.h"

#include "FirstPersonTD/Characters/MyFPSCharacter.h"

AHealthPickup::AHealthPickup()
{
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->InitSphereRadius(50.0f);
	SphereComponent->SetCollisionProfileName(TEXT("BlockAll"));
}

void AHealthPickup::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Super::OnComponentBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	// Check if the overlapped actor is the player
	if (AMyFPSCharacter* Ch = Cast<AMyFPSCharacter>(OtherActor))
	{
		this->ConditionalBeginDestroy();
		
		// Increase the player's health
		// Cast the overlapped actor to the player
		// Call the IncreaseHealth function on the player
	}
}
