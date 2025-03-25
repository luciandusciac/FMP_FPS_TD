// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPickup.h"

#include "FirstPersonTD/Characters/MyFPSCharacter.h"

AHealthPickup::AHealthPickup()
{
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->InitSphereRadius(50.0f);
	SphereComponent->SetCollisionProfileName(TEXT("OverlapAll"));

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AHealthPickup::OnComponentBeginOverlap);
}

void AHealthPickup::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AMyFPSCharacter* Ch = Cast<AMyFPSCharacter>(OtherActor))
	{
		if (Ch->HealingEffectWidgetClass)
		{
			Ch->HealingEffectWidget = CreateWidget<UUserWidget>(GetWorld(), Ch->HealingEffectWidgetClass);
			if (Ch->HealingEffectWidget)
			{
				Ch->HealingEffectWidget->AddToViewport();
				Ch->Heal();
				GetWorldTimerManager().SetTimer(Ch->WidgetTimerHandle, [Ch]() { Ch->DestroyWidget(Ch->HealingEffectWidget); }, 0.5f, false);
			}
		}
		
		this->Destroy();
	}
}
