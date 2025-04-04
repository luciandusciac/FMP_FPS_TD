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
	}
	

	// if (!bIsShooting)
	// {
	// 	if (CurrentAmmo > 0)
	// 	{
	// 		CurrentAmmo--;
	// 		
	// 		bIsShooting = true;
	//
	// 		
	// 		// if(AFPSPlayerController* PC = Cast<AFPSPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	// 		// {
	// 		// 	if (!PC) return;
	// 		// 	FVector Start, Direction;
	// 		// 	FVector2D ViewportSize;
	// 		// 	int32 ViewportSizeX, ViewportSizeY;
	// 		// 	PC->GetViewportSize(ViewportSizeX, ViewportSizeY);
	// 		// 	ViewportSize.X = static_cast<float>(ViewportSizeX);
	// 		// 	ViewportSize.Y = static_cast<float>(ViewportSizeY);
	// 		// 	PC->DeprojectScreenPositionToWorld(
	// 		// 		ViewportSize.X * 0.5f,
	// 		// 		ViewportSize.Y * 0.5f,
	// 		// 		Start,
	// 		// 		Direction
	// 		// 	);
	// 		// 	FVector End = Start + (Direction * 10000.f); // Target position far ahead
	// 		//
	// 		// 	// Spawn Projectile
	// 		// 	if (WeaponBullet)
	// 		// 	{
	// 		// 		FVector MuzzleLocation = BulletOrigin->GetComponentLocation(); // Get Gun's Muzzle Position
	// 		// 		FRotator AimDirection = (End - MuzzleLocation).Rotation(); // Aim at target
	// 		//
	// 		// 		for(int i = 0; i<=PelletCount;  ++i)
	// 		// 		{
	// 		// 			FRotator RandomRotation = BulletOrigin->GetComponentRotation();
	// 		// 			RandomRotation.Yaw += FMath::RandRange(-Spread, Spread);
	// 		// 			RandomRotation.Pitch += FMath::RandRange(-Spread, Spread);
	// 		//
	// 		// 			ABaseProjectile* Projectile = GetWorld()->SpawnActor<ABaseProjectile>(WeaponBullet, BulletOrigin->GetComponentLocation(), AimDirection);
	// 		// 		
	// 		// 		
	// 		// 			if (Projectile)
	// 		// 			{
	// 		// 				Projectile->SetOwner(this);
	// 		// 			}
	// 		// 		}
	// 		// 	}
	// 		// }
	// 		
	//
	// 		FActorSpawnParameters SpawnParams;
	// 		SpawnParams.Owner = this;
	// 		SpawnParams.Instigator = GetInstigator();
	//
	// 		for(int i = 0; i<=PelletCount;  ++i)
	// 		{
	// 		
	// 			FRotator RandomRotation = BulletOrigin->GetComponentRotation();
	// 			RandomRotation.Yaw += FMath::RandRange(-Spread, Spread);
	// 			RandomRotation.Pitch += FMath::RandRange(-Spread, Spread);
	// 		
	// 			GetWorld()->SpawnActor<ABaseProjectile>(WeaponBullet, BulletOrigin->GetComponentLocation(), RandomRotation, SpawnParams);
	// 		}
	//
	// 		Super::Shoot();
	// 		
	// 		if (CurrentAmmo == 0)
	// 			Reload();
	//
	// 		GetWorldTimerManager().SetTimer(ShootingTimerHandle, this, &ABaseWeapon::OnShoot, FireRate, false);
	// 		
	// 	}
	// 	// else if (CurrentAmmo == 0 && ReserveAmmo > 0)
	// 	// {
	// 	// 	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Character is reloading"));
	// 	// 	Reload();
	// 	// }
	// 	// else
	// 	// {
	// 	// 	//TODO: Play error sound
	// 	// 	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No ammo"));
	// 	// }
	//
	// 	//UE_LOG(LogTemp, Warning, TEXT("Shotgun Shot"));
	// }
}

void AShotgun::Reload()
{
	Super::Reload();
}

