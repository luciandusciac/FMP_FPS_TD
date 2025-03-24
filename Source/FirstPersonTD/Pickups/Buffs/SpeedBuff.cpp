// Fill out your copyright notice in the Description page of Project Settings.


#include "SpeedBuff.h"

#include "FirstPersonTD/Characters/MyFPSCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Math/UnitConversion.h"

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

	AMyFPSCharacter* Ch = Cast<AMyFPSCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	Ch->GetCharacterMovement()->MaxWalkSpeed = 150.f;
	GetWorldTimerManager().ClearTimer(TimerHandle);
}

void ASpeedBuff::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (AMyFPSCharacter* Ch = Cast<AMyFPSCharacter>(OtherActor))
	{
		this->Destroy();

		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "SpeedBuff Applied");
		Ch->GetCharacterMovement()->MaxWalkSpeed = 1000.f;

		GetWorldTimerManager().SetTimer(Ch->AnimationTimerHandle, [Ch]() { Ch->ResetWalkingSpeed(); }, 15.f, false);
	}
}
