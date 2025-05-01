// Fill out your copyright notice in the Description page of Project Settings.


#include "Pistol.h"

#include "Engine/World.h"
#include "FirstPersonTD/Controller/FPSPlayerController.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
APistol::APistol()
{
	PrimaryActorTick.bCanEverTick = true;
	bHasMagazine = true;
}

void APistol::BeginPlay()
{
	Super::BeginPlay();
	
}

void APistol::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APistol::Shoot()
{
	if (!bIsShooting)
	{
		if (CurrentAmmo > 0)
		{
			CurrentAmmo--;
			
			bIsShooting = true;


			if(AFPSPlayerController* PC = Cast<AFPSPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
			{
				if (!PC) return;
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

			Super::Shoot();

			
			if (CurrentAmmo == 0)
				Reload();

			GetWorldTimerManager().SetTimer(ShootingTimerHandle, this, &ABaseWeapon::OnShoot, FireRate, false);
			
		}
		else
		{
			UGameplayStatics::PlaySoundAtLocation(this, EmptySound, GetActorLocation());
		}
	}

	
}

void APistol::Reload()
{
	Super::Reload();
}

