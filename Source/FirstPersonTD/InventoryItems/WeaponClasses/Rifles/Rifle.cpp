// Fill out your copyright notice in the Description page of Project Settings.


#include "Rifle.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "FirstPersonTD/Characters/EnemyCharacter.h"
#include "FirstPersonTD/Characters/MyFPSCharacter.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ARifle::ARifle()
{
	PrimaryActorTick.bCanEverTick = true;

	bHasMagazine = true;
	
}

void ARifle::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARifle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// if(CurrentFireRate>FireRate)
	// {
	// 	CurrentFireRate = 0;
	// 	Shoot();
	// }
	// else
	// {
	// 	CurrentFireRate += DeltaTime;
	// }
}

void ARifle::Shoot()
{
	if (!bIsShooting)
	{
		
		if (CurrentAmmo > 0)
		{
			CurrentAmmo--;

			bIsShooting = true;

			//
			// AMyFPSCharacter* Player = Cast<AMyFPSCharacter>(GetOwner());
			// if (!Player || !Player->GetController()) return;

			
			if(AFPSPlayerController* PC = Cast<AFPSPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
			{
				if (!PC) return;

				if (this->GetOwner() == PC->GetCharacter())
				{
					FVector Start, Direction;
					FVector2D ViewportSize;
					int32 ViewportSizeX, ViewportSizeY;
					PC->GetViewportSize(ViewportSizeX, ViewportSizeY);
					ViewportSize.X = static_cast<float>(ViewportSizeX);
					ViewportSize.Y = static_cast<float>(ViewportSizeY);
					PC->DeprojectScreenPositionToWorld(
						ViewportSize.X * 0.5f,
						ViewportSize.Y * 0.5f,
						Start,
						Direction
					);
					FVector End = Start + (Direction * 10000.f); // Target position far ahead
			
					// Spawn Projectile
					if (WeaponBullet)
					{
						FVector MuzzleLocation = BulletOrigin->GetComponentLocation(); // Get Gun's Muzzle Position
						FRotator AimDirection = (End - MuzzleLocation).Rotation(); // Aim at target

						ABaseProjectile* Projectile = GetWorld()->SpawnActor<ABaseProjectile>(WeaponBullet, MuzzleLocation, AimDirection);
						if (Projectile)
						{
							Projectile->SetOwner(this);
						}
					}
				}
				else
				{
					FVector MuzzleLocation = BulletOrigin->GetComponentLocation(); // Get Gun's Muzzle Position
					FRotator AimDirection = BulletOrigin->GetComponentRotation(); // Aim at target
					ABaseProjectile* Projectile = GetWorld()->SpawnActor<ABaseProjectile>(WeaponBullet, MuzzleLocation, AimDirection);
				}
			}
			
			// FActorSpawnParameters SpawnParams;
			// SpawnParams.Owner = this;
			// SpawnParams.Instigator = GetInstigator();
			//
			// GetWorld()->SpawnActor<ABaseProjectile>(WeaponBullet, BulletOrigin->GetComponentLocation(), BulletOrigin->GetComponentRotation(), SpawnParams);
			Super::Shoot();
			
			if (CurrentAmmo == 0)
			{
				Reload();
			}
			GetWorldTimerManager().SetTimer(ShootingTimerHandle, this, &ABaseWeapon::OnShoot, FireRate, false);
		}
		else
		{
			UGameplayStatics::PlaySoundAtLocation(this, EmptySound, GetActorLocation());
		}
	}
}

void ARifle::Reload()
{
	Super::Reload();
}

