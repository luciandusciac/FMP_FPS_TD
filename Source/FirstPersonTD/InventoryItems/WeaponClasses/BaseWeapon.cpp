// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"
#include "Components/StaticMeshComponent.h"
#include "FirstPersonTD/Characters/MyFPSCharacter.h"
#include "FirstPersonTD/Controller/FPSPlayerController.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABaseWeapon::ABaseWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// //FSetActorReplicates Replicates = FSetActorReplicates(cast<AActor>(this), true);
	// bReplicates = true;
	// bNetLoadOnClient = true;
	// SetReplicates(true);
	
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

	BulletOrigin = CreateDefaultSubobject<USceneComponent>(TEXT("BulletOrigin"));
	BulletOrigin->SetupAttachment(Mesh);

	AimOrigin = CreateDefaultSubobject<USceneComponent>(TEXT("AimOrigin"));
	AimOrigin->SetupAttachment(Mesh);

	ShellOrigin = CreateDefaultSubobject<USceneComponent>(TEXT("ShellOrigin"));
	ShellOrigin->SetupAttachment(Mesh);

	bIsShooting = false;
}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	CurrentAmmo = ClipSize;
	
	
	// if(!CurrentOwner)
	// 	Mesh->SetVisibility(false);
	//Mesh->IgnoreActorWhenMoving(Cast<AActor>(WeaponBullet), true);
}

// Called every frame
void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseWeapon::Shoot()
{
	if(AFPSPlayerController* controller = Cast<AFPSPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		if (AMyFPSCharacter* C = Cast<AMyFPSCharacter>(controller->GetCharacter()))
		{
			C->UpdateAmmoUI();
		}
	}

	// INFO: Instantiate bullet shell
	 if (BulletShell && ShellOrigin)
	 {
	 	FActorSpawnParameters SpawnParams;
	 	SpawnParams.Owner = this;
	 	SpawnParams.Instigator = GetInstigator();
	
	 	FVector SpawnLocation = ShellOrigin->GetComponentLocation();
	 	FRotator SpawnRotation = ShellOrigin->GetComponentRotation();
	 	AActor* SpawnedBulletShell = GetWorld()->SpawnActor<AActor>(BulletShell, SpawnLocation, SpawnRotation, SpawnParams);
	
	 	if (SpawnedBulletShell)
	 	{
	 		if (UStaticMeshComponent* MeshComponent = SpawnedBulletShell->FindComponentByClass<UStaticMeshComponent>())
	 		{
	 			//MeshComponent->SetSimulatePhysics(true);
	 			//MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	 			//MeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	 			//MeshComponent->SetCollisionProfileName(TEXT("PhysicsActor"));
	
	 			FVector LocalImpulse = FVector(200.f, 50.0f, 600.0f);
	 			
	 			FVector WorldImpulse = ShellOrigin->GetComponentTransform().TransformVector(LocalImpulse * 2);
	 			
	 			MeshComponent->AddImpulse(WorldImpulse);
	
	 			
	 			//MeshComponent->AddImpulse(FVector(150.f, 0.0f, 100.0f));
	 		}
	 	}
	}

	
	//TODO: Play shoot sound
}

void ABaseWeapon::OnShoot()
{
	bIsShooting = false;
	GetWorldTimerManager().ClearTimer(ShootingTimerHandle);
}

void ABaseWeapon::Reload()
{
	//if(AMyFPSCharacter* C = Cast<AMyFPSCharacter>(GetOwner()))
	//{
		//C->AnimationInstance->AnimationIndex = CurrentInventorySlot;
		if(AFPSPlayerController* controller = Cast<AFPSPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
		{
			//C->Reload();
			// if(AMyFPSCharacter* C = Cast<AMyFPSCharacter>(GetOwner()))
			// {
			// 	C->UpdateAmmoUI();
			// }
			
			
			if(ReserveAmmo > 0)
			{
				GetWorldTimerManager().SetTimer(ReloadTimerHandle, this, &ABaseWeapon::OnReload, ReloadTime, false);
				//CurrentAmmo = ClipSize;
				ReserveAmmo--;
				if (AMyFPSCharacter* C = Cast<AMyFPSCharacter>(controller->GetCharacter()))
				{
					C->AnimationInstance->bIsReloading = true;
					//C->UpdateAmmoUI();
				}
				//controller->Reload();
			}
			else
			{
				//TODO: Play error sound
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No reserve ammo"));
			}
		}
	//}
	
}

void ABaseWeapon::OnReload()
{
	CurrentAmmo = ClipSize;
	if(AFPSPlayerController* controller = Cast<AFPSPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		if (AMyFPSCharacter* C = Cast<AMyFPSCharacter>(controller->GetCharacter()))
		{
			C->AnimationInstance->bIsReloading = false;
			C->UpdateAmmoUI();
		}
	}
	GetWorldTimerManager().ClearTimer(ReloadTimerHandle);
}

void ABaseWeapon::Aim()
{
}

void ABaseWeapon::Use()
{
	Super::Use();

	Shoot();
}

int ABaseWeapon::GetCurrentAmmo()
{
	return CurrentAmmo;
}

void ABaseWeapon::SetCurrentAmmo(const int NewCurrentAmmo)
{
	CurrentAmmo = NewCurrentAmmo;
}

int ABaseWeapon::GetReserveAmmo()
{
	return ReserveAmmo;
}

void ABaseWeapon::SetReserveAmmo(const int NewReserveAmmo)
{
	ReserveAmmo = NewReserveAmmo;
}

