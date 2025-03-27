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
		if (!Ch->CurrentItemInHands)
			return;
		
		if (ABaseWeapon* W = Cast<ABaseWeapon>(Ch->CurrentItemInHands))
		{
			if (W->WeaponBullet)
			{
				ABaseProjectile* DefaultBullet = W->WeaponBullet->GetDefaultObject<ABaseProjectile>();
				if (DefaultBullet)
				{
					DefaultBullet->DamageAmount *= 1.1f;  // Increase base damage by 10%
                    
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Bullet damage buff applied!"));

					if (Ch->HUD->DamageBuffWidgetClass)
					{
						Ch->HUD->DamageBuffWidget = CreateWidget<UUserWidget>(GetWorld(), Ch->HUD->DamageBuffWidgetClass);
						if (Ch->HUD->DamageBuffWidget)
						{
							Ch->HUD->DamageBuffWidget->AddToViewport();
							GetWorldTimerManager().SetTimer(Ch->WidgetTimerHandle, [Ch]
							{
								Ch->HUD->DamageBuffWidget->RemoveFromParent();
							}, 30.f, false);
						}
					}

					// Reset after 30 seconds
					GetWorldTimerManager().SetTimer(Ch->AnimationTimerHandle, [DefaultBullet]
					{
						//Ch->ResetBulletDamage();
						DefaultBullet->DamageAmount /= 1.1f;
						//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Bullet damage buff removed!"));
					}, 30.f, false);
				}
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("WeaponBullet is NULL!"));
			}
		}
		this->Destroy();
	}
}
