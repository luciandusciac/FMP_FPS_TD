// Fill out your copyright notice in the Description page of Project Settings.


#include "Shotgun.h"

#include "Engine/World.h"
#include "FirstPersonTD/Controller/FPSPlayerController.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AShotgun::AShotgun()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bHasMagazine = false;
}

// Called when the game starts or when spawned
void AShotgun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShotgun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AShotgun::Shoot()
{
	if (!bIsShooting)
	{
		if (CurrentAmmo > 0)
		{
			CurrentAmmo--;
			bIsShooting = true;

			Super::Shoot();
			
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
			if (!PlayerController) return;

			FVector CameraLocation;
			FRotator CameraRotation;
			PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);  // Get camera location & rotation

			FVector ShotDirection = CameraRotation.Vector();
			FVector TraceEnd = CameraLocation + (ShotDirection * 10000.0f); // Long-range trace

			FHitResult Hit;
			FVector TargetLocation = TraceEnd;

			// Perform line trace to determine where the crosshair is aiming
			if (GetWorld()->LineTraceSingleByChannel(Hit, CameraLocation, TraceEnd, ECC_Visibility))
			{
				TargetLocation = Hit.ImpactPoint; // Set target to the actual hit location
			}

			for (int i = 0; i < PelletCount; ++i)
			{
				// Calculate spread by randomly adjusting the direction
				FVector ShotSpread = ShotDirection;
				ShotSpread = FMath::VRandCone(ShotSpread, FMath::DegreesToRadians(Spread));

				FRotator FinalShotRotation = ShotSpread.Rotation();
				FVector MuzzleLocation = BulletOrigin->GetComponentLocation();

				GetWorld()->SpawnActor<ABaseProjectile>(WeaponBullet, MuzzleLocation, FinalShotRotation, SpawnParams);
			}

			

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

void AShotgun::Reload()
{
	Super::Reload();
}

