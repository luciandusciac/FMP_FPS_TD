// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoPickup.h"

#include "FirstPersonTD/Characters/MyFPSCharacter.h"

AAmmoPickup::AAmmoPickup()
{
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->InitSphereRadius(50.0f);
	SphereComponent->SetCollisionProfileName(TEXT("OverlapAll"));

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AAmmoPickup::OnComponentBeginOverlap);
}

void AAmmoPickup::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AMyFPSCharacter* Ch = Cast<AMyFPSCharacter>(OtherActor))
	{
		this->Destroy();
		
		// Increase the player's ammo
		
	}
}
