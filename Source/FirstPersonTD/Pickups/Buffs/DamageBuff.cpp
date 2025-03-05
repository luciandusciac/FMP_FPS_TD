// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageBuff.h"

#include "FirstPersonTD/Characters/MyFPSCharacter.h"

ADamageBuff::ADamageBuff()
{
	//SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ADamageBuff::OnComponentBeginOverlap);
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->InitSphereRadius(50.0f);
	SphereComponent->SetCollisionProfileName(TEXT("OverlapAll"));

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ADamageBuff::OnComponentBeginOverlap);
}

void ADamageBuff::OnExpire()
{
	//Super::OnExpire();
}

void ADamageBuff::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AMyFPSCharacter* Ch = Cast<AMyFPSCharacter>(OtherActor))
	{
		this->Destroy();
		
	}
}
