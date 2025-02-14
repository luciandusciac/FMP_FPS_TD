// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageBuff.h"

#include "FirstPersonTD/Characters/MyFPSCharacter.h"

void ADamageBuff::OnExpire()
{
	Super::OnExpire();
}

void ADamageBuff::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnComponentBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (AMyFPSCharacter* Ch = Cast<AMyFPSCharacter>(OtherActor))
	{
		this->ConditionalBeginDestroy();
		
		// Increase the player's health
		// Cast the overlapped actor to the player
		// Call the IncreaseHealth function on the player
	}
}
