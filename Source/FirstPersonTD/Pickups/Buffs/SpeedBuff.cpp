// Fill out your copyright notice in the Description page of Project Settings.


#include "SpeedBuff.h"

#include "FirstPersonTD/Characters/MyFPSCharacter.h"

ASpeedBuff::ASpeedBuff()
{
	//SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASpeedBuff::OnComponentBeginOverlap);
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->InitSphereRadius(50.0f);
	SphereComponent->SetCollisionProfileName(TEXT("OverlapAll"));

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASpeedBuff::OnComponentBeginOverlap);
}

void ASpeedBuff::OnExpire()
{
	//Super::OnExpire();
}

void ASpeedBuff::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (AMyFPSCharacter* Ch = Cast<AMyFPSCharacter>(OtherActor))
	{
		this->Destroy();
		
	}
}
