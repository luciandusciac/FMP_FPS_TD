// Fill out your copyright notice in the Description page of Project Settings.
#include "MyFPSCharacter.h"

#include "EnemyCharacter.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/Controller.h"
#include "Net/UnrealNetwork.h"
#include "../Source/FirstPersonTD/InventoryItems/WeaponClasses/BaseWeapon.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CapsuleComponent.h"
#include "Components/ProgressBar.h"
#include "EntitySystem/MovieSceneEntitySystemRunner.h"
#include "FirstPersonTD/Animations/SWAT_AnimInstance.h"
#include "FirstPersonTD/InventoryItems/Interfaces/FlashbangInterface.h"
#include "FirstPersonTD/InventoryItems/Interfaces/KnifeInterface.h"
#include "FirstPersonTD/InventoryItems/Interfaces/PrimaryWeapon.h"
#include "FirstPersonTD/InventoryItems/Interfaces/SecondaryWeapon.h"
#include "FirstPersonTD/InventoryItems/Interfaces/SmokeGrenadeInterface.h"
#include "FirstPersonTD/InventoryItems/ThrowableItems/Grenades/BaseGrenade.h"
#include "FirstPersonTD/InventoryItems/ThrowableItems/Grenades/FlashbangGrenade.h"
#include "FirstPersonTD/InventoryItems/ThrowableItems/Grenades/FragGrenade.h"
#include "FirstPersonTD/InventoryItems/ThrowableItems/Grenades/SmokeGrenade.h"
#include "FirstPersonTD/InventoryItems/ThrowableItems/Knives/Knife.h"
#include "FirstPersonTD/InventoryItems/WeaponClasses/Pistols/Pistol.h"
#include "FirstPersonTD/InventoryItems/WeaponClasses/Shotguns/Shotgun.h"
#include "FirstPersonTD/InventoryItems/WeaponClasses/Snipers/Sniper.h"
#include "FirstPersonTD/Pickups/BasePickup.h"
#include "FirstPersonTD/Pickups/HealthPickup.h"
#include "FirstPersonTD/Projectiles/RifleBullet.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavMesh/NavMeshBoundsVolume.h"


class ABaseWeapon;

// Sets default values
AMyFPSCharacter::AMyFPSCharacter()
{
	
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->bUsePawnControlRotation = true;
	Camera->SetupAttachment(GetMesh(), "Head");

	ADSPosComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ADS"));
	ADSPosComponent->SetupAttachment(Camera);
	
	this->GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMyFPSCharacter::OnComponentBeginOverlap);

	HUDClass = nullptr;
	HUD = nullptr;

	CurrentItemInHands = nullptr;

	CurrentHealth = MaxHealth;
}

// Called when the game starts or when spawned
void AMyFPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	ReloadingTime = ReloadingAnimation->GetPlayLength();
	ShootingTime = ShootingAnimation->GetPlayLength();
	PistolShootingTime = PistolShootingAnimation->GetPlayLength();
	//DeathTime = DeathAnimation->GetPlayLength();
	GrenadeThrowTime = GrenadeThrowAnimation->GetPlayLength();

	Inventory = NewObject<UInventory>(this);

	AnimationInstance = Cast<USWAT_AnimInstance>(GetMesh()->GetAnimInstance());

	// INFO: UI creation
	if (HUDClass)
	{
		AFPSPlayerController* PlayerController = Cast<AFPSPlayerController>(GetController());
		
		if (PlayerController)
		{
			HUD = CreateWidget<UPlayerHUD>(PlayerController, HUDClass);
			if (HUD)
			{
				HUD->AddToPlayerScreen(10);
				HUD->KnifeThrowProgressBar->SetVisibility(ESlateVisibility::Hidden);
				HUD->SniperScopeWidget->SetVisibility(ESlateVisibility::Hidden);
			}
		}

		if (HUD && HUD->CrosshairWidgetClass)
		{
			HUD->CrosshairWidget = CreateWidget<UUserWidget>(GetWorld(), HUD->CrosshairWidgetClass);
			if (HUD->CrosshairWidget)
			{
				HUD->CrosshairWidget->AddToPlayerScreen();
				HUD->CrosshairWidget->SetVisibility(ESlateVisibility::Hidden);
			}
		}
		// INFO: Create all weapon widgets
		if (HUD && HUD->PrimaryWeaponWidgetClass)
		{
			HUD->PrimaryWeaponWidget = CreateWidget<UPrimaryWeaponWidget>(GetWorld(), HUD->PrimaryWeaponWidgetClass);
			if (HUD->PrimaryWeaponWidget)
			{
				HUD->PrimaryWeaponWidget->AddToPlayerScreen();
				HUD->PrimaryWeaponWidget->SetVisibility(ESlateVisibility::Hidden);
			}
		}

		if (HUD && HUD->SecondaryWeaponWidgetClass)
		{
			HUD->SecondaryWeaponWidget = CreateWidget<UUserWidget>(GetWorld(), HUD->SecondaryWeaponWidgetClass);
			if (HUD->SecondaryWeaponWidget)
			{
				HUD->SecondaryWeaponWidget->AddToPlayerScreen();
				HUD->SecondaryWeaponWidget->SetVisibility(ESlateVisibility::Hidden);
			}
		}

		if (HUD && HUD->FragGrenadeWidgetClass)
		{
			HUD->FragGrenadeWidget = CreateWidget<UUserWidget>(GetWorld(), HUD->FragGrenadeWidgetClass);
			if (HUD->FragGrenadeWidget)
			{
				HUD->FragGrenadeWidget->AddToPlayerScreen();
				HUD->FragGrenadeWidget->SetVisibility(ESlateVisibility::Hidden);
			}
		}

		if (HUD && HUD->FlashbangGrenadeWidgetClass)
		{
			HUD->FlashbangGrenadeWidget = CreateWidget<UUserWidget>(GetWorld(), HUD->FlashbangGrenadeWidgetClass);
			if (HUD->FlashbangGrenadeWidget)
			{
				HUD->FlashbangGrenadeWidget->AddToPlayerScreen();
				HUD->FlashbangGrenadeWidget->SetVisibility(ESlateVisibility::Hidden);
			}
		}

		if (HUD && HUD->SmokeGrenadeWidgetClass)
		{
			HUD->SmokeGrenadeWidget = CreateWidget<UUserWidget>(GetWorld(), HUD->SmokeGrenadeWidgetClass);
			if (HUD->SmokeGrenadeWidget)
			{
				HUD->SmokeGrenadeWidget->AddToPlayerScreen();
				HUD->SmokeGrenadeWidget->SetVisibility(ESlateVisibility::Hidden);
			}
		}

		if (HUD && HUD->KnifeWidgetClass)
		{
			HUD->KnifeWidget = CreateWidget<UUserWidget>(GetWorld(), HUD->KnifeWidgetClass);
			if (HUD->KnifeWidget)
			{
				HUD->KnifeWidget->AddToPlayerScreen();
				HUD->KnifeWidget->SetVisibility(ESlateVisibility::Hidden);
			}
		}
		
	}

	HUD->SetHealth(CurrentHealth, MaxHealth);

	if (GEngine)
	{
		GEngine->Exec(GetWorld(), TEXT("r.SetNearClipPlane 1"));
	}

	PlayerStartLocation = GetActorLocation();


	// INFO: Draw navmesh bounds
	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANavMeshBoundsVolume::StaticClass(), FoundVolumes);

	for (AActor* Actor : FoundVolumes)
	{
		if (ANavMeshBoundsVolume* Volume = Cast<ANavMeshBoundsVolume>(Actor))
		{
			const FBoxSphereBounds Bounds = Volume->GetBounds();
			const FVector Origin = Bounds.Origin;
			const FVector Extent = Bounds.BoxExtent;

			DrawDebugBox(GetWorld(), Origin, Extent, FColor::Red, true, -1.f, 0, 5.f);
		}
	}


	if (InstructionsWidgetClass)
	{
		InstructionsWidget = CreateWidget<UUserWidget>(GetWorld(), InstructionsWidgetClass);
		if (InstructionsWidget)
		{
			InstructionsWidget->AddToViewport();
			InstructionsWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void AMyFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentItemInHands && CurrentItemInHands->IsA(AKnife::StaticClass()) && AnimationInstance->bIsShooting)
	{
		if (!HUD->KnifeThrowProgressBar->IsVisible())
		{
			HUD->KnifeThrowProgressBar->SetVisibility(ESlateVisibility::Visible);
		}
		
		float TotalTime = 1.4f;
		KnifeThrowElapsedTime += DeltaTime;

		//float Progress = FMath::Clamp(KnifeThrowElapsedTime / TotalTime, 0.f, 1.f);
		HUD->SetKnifeThrowProgress(KnifeThrowElapsedTime, TotalTime);

		// INFO: Hide progress bar when complete
		if (KnifeThrowElapsedTime >= TotalTime)
		{
			HUD->KnifeThrowProgressBar->SetVisibility(ESlateVisibility::Hidden);
			bIsThrowingKnife = false;

			if (HUD->KnifeWidget->IsVisible())
			{
				HUD->KnifeWidget->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
	else if (CurrentItemInHands && CurrentItemInHands->IsA(AKnife::StaticClass()) && Inventory->GetNumberOfItems() > 0)
	{
		if (HUD->KnifeThrowProgressBar->IsVisible())
		{
			HUD->KnifeThrowProgressBar->SetVisibility(ESlateVisibility::Hidden);
		}
		
		KnifeThrowElapsedTime = 0.f;
		HUD->SetKnifeThrowProgress(KnifeThrowElapsedTime, 1.4f);
	}


	if (bIsDeathCameraMoving)
	{
		CameraLerpAlpha += GetWorld()->GetDeltaSeconds() / 2.0f;
		CameraLerpAlpha = FMath::Clamp(CameraLerpAlpha, 0.f, 1.f);

		FVector NewLocation = FMath::Lerp(StartDeathCamLocation, EndDeathCamLocation, CameraLerpAlpha);
		FRotator NewRotation = FMath::Lerp(StartDeathCamRotation, EndDeathCamRotation, CameraLerpAlpha);

		Camera->SetWorldLocation(NewLocation);
		Camera->SetWorldRotation(NewRotation);

		if (CameraLerpAlpha >= 1.f)
		{
			bIsDeathCameraMoving = false;
		}
	}
}


void AMyFPSCharacter::ThrowGrenade()
{
	GetWorldTimerManager().SetTimer(AnimationTimerHandle, this, &AMyFPSCharacter::OnGrenadeThrown, GrenadeThrowTime, false);

	if(USWAT_AnimInstance* AnimInstance = Cast<USWAT_AnimInstance>(GetMesh()->GetAnimInstance()))
	{
		//AnimInstance->bIsThrowingGrenade = true;
		//AnimInstance->bHasGrenade = false;
	
	}
	
	if (CurrentItemInHands->GetClass()->ImplementsInterface(UFragGrenadeInterface::StaticClass()))
	{
		AFragGrenade* Gr = GetWorld()->SpawnActor<AFragGrenade>(CurrentItemInHands->GetClass(), GetActorLocation() + GetActorForwardVector() * 100.f, GetActorRotation());
		UStaticMeshComponent* MeshComp = Gr->FindComponentByClass<UStaticMeshComponent>();
		if (MeshComp)
		{
			MeshComp->SetSimulatePhysics(true);
			MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			MeshComp->BodyInstance.SetUseCCD(true);
			MeshComp->AddImpulse(GetActorForwardVector() * 1000.f + FVector(0.f, 0.f, 600.f));
		}
		Gr->bCanExplode = true;

		// INFO: Play grenade throw sound
		UGameplayStatics::PlaySoundAtLocation(this, Gr->ThrowSound, GetActorLocation());

		if (HUD->FragGrenadeWidget->IsVisible())
		{
			HUD->FragGrenadeWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	else if (CurrentItemInHands->GetClass()->ImplementsInterface(UFlashbangInterface::StaticClass()))
	{
		AFlashbangGrenade* Gr = GetWorld()->SpawnActor<AFlashbangGrenade>(CurrentItemInHands->GetClass(), GetActorLocation() + GetActorForwardVector() * 100.f, GetActorRotation());
		UStaticMeshComponent* MeshComp = Gr->FindComponentByClass<UStaticMeshComponent>();
		if (MeshComp)
		{
			MeshComp->SetSimulatePhysics(true);
			MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			MeshComp->BodyInstance.SetUseCCD(true);
			MeshComp->AddImpulse(GetActorForwardVector() * 1000.f + FVector(0.f, 0.f, 600.f));
		}
		Gr->bCanExplode = true;

		// INFO: Play grenade throw sound
		UGameplayStatics::PlaySoundAtLocation(this, Gr->ThrowSound, GetActorLocation());

		if (HUD->FlashbangGrenadeWidget->IsVisible())
		{
			HUD->FlashbangGrenadeWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	else if (CurrentItemInHands->GetClass()->ImplementsInterface(USmokeGrenadeInterface::StaticClass()))
	{
		ASmokeGrenade* Gr = GetWorld()->SpawnActor<ASmokeGrenade>(CurrentItemInHands->GetClass(), GetActorLocation() + GetActorForwardVector() * 100.f, GetActorRotation());
		UStaticMeshComponent* MeshComp = Gr->FindComponentByClass<UStaticMeshComponent>();
		if (MeshComp)
		{
			MeshComp->SetSimulatePhysics(true);
			MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			MeshComp->BodyInstance.SetUseCCD(true);
			MeshComp->AddImpulse(GetActorForwardVector() * 1000.f + FVector(0.f, 0.f, 600.f));
		}
		Gr->bCanExplode = true;
		
		// INFO: Play grenade throw sound
		UGameplayStatics::PlaySoundAtLocation(this, Gr->ThrowSound, GetActorLocation());

		if (HUD->SmokeGrenadeWidget->IsVisible())
		{
			HUD->SmokeGrenadeWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	
	CurrentItemInHands->Destroy();
	//CurrentItemInHands = nullptr;
	Inventory->UseItem(Inventory->CurrentItem);
}

void AMyFPSCharacter::OnGrenadeThrown()
{
	GetWorldTimerManager().ClearTimer(AnimationTimerHandle);
	
	if(USWAT_AnimInstance* AnimInstance = Cast<USWAT_AnimInstance>(GetMesh()->GetAnimInstance()))
	{
		AnimInstance->bIsThrowingGrenade = false;
		AnimInstance->bHasGrenade = false;
	}

	GetWorldTimerManager().SetTimer(AnimationTimerHandle, this, &AMyFPSCharacter::CheckGrenadeInHand, 0.1f, false);
}

void AMyFPSCharacter::CheckGrenadeInHand()
{
	if(USWAT_AnimInstance* AnimInstance = Cast<USWAT_AnimInstance>(GetMesh()->GetAnimInstance()))
	{
		if (CurrentItemInHands != nullptr && Inventory->GetNumberOfItems() > 0 &&
			(CurrentItemInHands->IsA(AFragGrenade::StaticClass()) || CurrentItemInHands->IsA(AFlashbangGrenade::StaticClass()) ||
				CurrentItemInHands->IsA(ASmokeGrenade::StaticClass())))
		{
			AnimInstance->bHasGrenade = true;
		}
		else
		{
			AnimInstance->bHasGrenade = false;
		}
	}

	GetWorldTimerManager().ClearTimer(AnimationTimerHandle);
}

void AMyFPSCharacter::ThrowKnife()
{
	if (!CurrentItemInHands) return;

	if (CurrentItemInHands->GetClass()->ImplementsInterface(UKnifeInterface::StaticClass()))
	{
		// INFO: Deproject screen center to world direction
		int32 ViewportX, ViewportY;

		if(AFPSPlayerController* PC = Cast<AFPSPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
		{
			PC->GetViewportSize(ViewportX, ViewportY);

			FVector WorldLocation;
			FVector WorldDirection;
			PC->DeprojectScreenPositionToWorld(ViewportX / 2.0f, ViewportY / 2.0f, WorldLocation, WorldDirection);

			// INFO: Spawn the knife slightly in front of the player
			FVector SpawnLocation = Camera->GetComponentLocation() + WorldDirection * 100.f;
			FRotator SpawnRotation = WorldDirection.Rotation();

			AKnife* Knife = GetWorld()->SpawnActor<AKnife>(CurrentItemInHands->GetClass(), SpawnLocation, SpawnRotation);
			if (Knife)
			{
				if (UStaticMeshComponent* MeshComp = Knife->FindComponentByClass<UStaticMeshComponent>())
				{
					// INFO: Launch toward the center of screen
					MeshComp->AddImpulse(WorldDirection * 5000.f);

					// INFO: Play knife throw sound
					UGameplayStatics::PlaySoundAtLocation(this, Knife->ThrowSound, GetActorLocation());
				}
			}
		}
	}
	
	if (HUD->KnifeWidget->IsVisible())
	{
		HUD->KnifeWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	
	CurrentItemInHands->Destroy();
	//CurrentItemInHands = nullptr;
	Inventory->UseItem(Inventory->CurrentItem);
}

void AMyFPSCharacter::UpdateKnifeThrowProgress()
{
	float ElapsedTime = GetWorldTimerManager().GetTimerElapsed(KnifeProgressBarTimer);
	float TotalTime = KnifeThrowAnimation->GetPlayLength();

	UE_LOG(LogTemp, Warning, TEXT("Progress: %f | Elapsed: %f | Total: %f"), ElapsedTime / TotalTime, ElapsedTime, TotalTime);

	float Progress = FMath::Clamp(ElapsedTime / TotalTime, 0.f, 1.f);
	HUD->SetKnifeThrowProgress(Progress, TotalTime);

	// INFO: If progress is complete, hide the progress bar
	if (Progress >= 1.f)
	{
		HUD->KnifeThrowProgressBar->SetVisibility(ESlateVisibility::Hidden);
		GetWorldTimerManager().ClearTimer(KnifeProgressBarTimer);
		UE_LOG(LogTemp, Warning, TEXT("Knife throw progress complete!"));
	}
}

void AMyFPSCharacter::OnKnifeThrown()
{
	if (AnimationInstance)
	{
		AnimationInstance->bHasKnife = false;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Knife animinstance changed!"));
	}

	

	GetWorldTimerManager().ClearTimer(AnimationTimerHandle);
}

void AMyFPSCharacter::Shoot()
{
	if (!Inventory->CurrentItem)
		return;
	
	if(USWAT_AnimInstance* AnimInstance = Cast<USWAT_AnimInstance>(GetMesh()->GetAnimInstance()))
	{
		if (!AnimInstance->bIsReloading)
		{
			if(AnimInstance->bHasPistol)
			{
				GetWorldTimerManager().SetTimer(AnimationTimerHandle, this, &AMyFPSCharacter::OnShoot, PistolShootingTime, false);
		
			}
			else
			{
				GetWorldTimerManager().SetTimer(AnimationTimerHandle, this, &AMyFPSCharacter::OnShoot, ShootingTime, false);
		
			}
	
			AnimInstance->bIsShooting = true;
	
			if (AInventoryItem* i =  Cast<AInventoryItem>(CurrentItemInHands))
			{
				i->Use();
			}
		}
	}
}

void AMyFPSCharacter::OnShoot()
{
	GetWorldTimerManager().ClearTimer(AnimationTimerHandle);
	
	if(USWAT_AnimInstance* AnimInstance = Cast<USWAT_AnimInstance>(GetMesh()->GetAnimInstance()))
	{
		AnimInstance->bIsShooting = false;
	}
	
}

void AMyFPSCharacter::Aim()
{
	if (Inventory->CurrentItem != nullptr)
	{
		if (ABaseWeapon* W = Cast<ABaseWeapon>(CurrentItemInHands))
		{
			bIsAiming = true;

			if (W->bHasScope)
			{
				if (HUD->SniperScopeWidget && !HUD->SniperScopeWidget->IsVisible())
				{
					// HUD->SniperScopeWidget->RemoveFromParent();
					// HUD->SniperScopeWidget->AddToViewport(-1);
					HUD->SniperScopeWidget->SetVisibility(ESlateVisibility::Visible);

					Camera->SetFieldOfView(50.f);
					

					Camera->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
					
					Camera->AttachToComponent(W->AimOrigin, FAttachmentTransformRules::SnapToTargetIncludingScale);
					
					Camera->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
					Camera->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
				}
			}
			else
			{
				// INFO: Aiming without scope
				//Camera->SetFieldOfView(FMath::FInterpTo(Camera->FieldOfView, 50.f, GetWorld()->GetDeltaSeconds(), 5.0f));
				Camera->SetFieldOfView(50.f);

				W->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
				W->Mesh->AttachToComponent(ADSPosComponent,
					FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true));
				W->Mesh->SetRelativeTransform(Inventory->CurrentItem->AimingTransform);


				if (W->IsA(APistol::StaticClass()))
				{
					GetMesh()->SetVisibility(false);
				}
			}
		}
	}
}

void AMyFPSCharacter::StopAiming()
{
	if (ABaseWeapon* W = Cast<ABaseWeapon>(CurrentItemInHands))
	{
		//W->Mesh->SetRelativeTransform(Inventory->CurrentItem->AttachmentTransform);

		W->Mesh->AttachToComponent(
			GetMesh(),
			FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true),
			FName("WeaponSocket")
		);
		W->Mesh->SetRelativeTransform(Inventory->CurrentItem->AttachmentTransform);
	}
	
	//Camera->SetFieldOfView(FMath::FInterpTo(Camera->FieldOfView, 100.f, GetWorld()->GetDeltaSeconds(), 5.0f));
	Camera->SetFieldOfView(90.f);

	bIsAiming = false;

	if (HUD->SniperScopeWidget && HUD->SniperScopeWidget->IsVisible())
	{
		HUD->SniperScopeWidget->SetVisibility(ESlateVisibility::Hidden);

		Camera->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		
		Camera->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Head"));
		Camera->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
		Camera->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	}

	if (!(GetMesh()->IsVisible()))
	{
		GetMesh()->SetVisibility(true);
	}
}

void AMyFPSCharacter::Die()
{
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	if (!bHasPlayed)
	{
		bHasPlayed = true;
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	}

	Camera->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	
	StartDeathCamLocation = Camera->GetComponentLocation();
	EndDeathCamLocation = StartDeathCamLocation - Camera->GetForwardVector() * 300.f + FVector(0.f, 0.f, 100.f);
	StartDeathCamRotation = Camera->GetComponentRotation();
	EndDeathCamRotation = (GetActorLocation() - EndDeathCamLocation).Rotation();
	CameraLerpAlpha = 0.f;
	bIsDeathCameraMoving = true;
	
	FTimerHandle RespawnTimer;
	GetWorldTimerManager().SetTimer(RespawnTimer, this, &AMyFPSCharacter::Respawn, 3.0f, false);
}

void AMyFPSCharacter::OnDeath()
{
}

void AMyFPSCharacter::Respawn()
{
	FName CurrentLevel = *UGameplayStatics::GetCurrentLevelName(GetWorld());
	UGameplayStatics::OpenLevel(this, CurrentLevel);
}

void AMyFPSCharacter::Reload()
{
	if (CurrentItemInHands)
	{
		if (ABaseWeapon* W = Cast<ABaseWeapon>(CurrentItemInHands))
		{
			W->Reload();
		}
	}
}

void AMyFPSCharacter::OnReload()
{
	
	if(USWAT_AnimInstance* AnimInstance = Cast<USWAT_AnimInstance>(GetMesh()->GetAnimInstance()))
	{
		AnimInstance->bIsReloading = false;
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Character has reloaded"));
	UpdateAmmoUI();
	GetWorldTimerManager().ClearTimer(AnimationTimerHandle);
}

void AMyFPSCharacter::NextWeapon()
{
	if (Inventory->GetNumberOfItems() == 1 || Inventory->GetNumberOfItems() == 0)
		return;

		
	if (CurrentItemInHands)
	{
		if (ABaseWeapon* W = Cast<ABaseWeapon>(CurrentItemInHands))
		{
			FAmmoData& CurrentWeaponData = AmmoDataMap.FindOrAdd(W->GetClass());
			CurrentWeaponData.CurrentAmmo = W->GetCurrentAmmo();
			CurrentWeaponData.ClipSize = W->GetReserveAmmo();
		}
		CurrentItemInHands->Destroy();
	}

	if (Inventory->NextItem())
	{
		
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Item destroyed, next weapon!"));

		SpawnCurrentWeaponInHands();

		if (CurrentItemInHands)
		{
			// INFO: Load up the new weapon with the saved data if it exists
			if (ABaseWeapon* W = Cast<ABaseWeapon>(CurrentItemInHands))
			{
				if (const FAmmoData* NewAmmoData = AmmoDataMap.Find(CurrentItemInHands->GetClass()))
				{
					W->SetCurrentAmmo(NewAmmoData->CurrentAmmo);
					W->SetReserveAmmo(NewAmmoData->ClipSize);
				}

				if (!W->IsA(ASniper::StaticClass()))
				{
					if (!HUD->CrosshairWidget->IsVisible())
					{
						HUD->CrosshairWidget->SetVisibility(ESlateVisibility::Visible);
					}
				}
				else
				{
					HUD->CrosshairWidget->SetVisibility(ESlateVisibility::Hidden);
				}

				if (W->Implements<UPrimaryWeapon>() && HUD->PrimaryWeaponWidget->IsVisible())
				{
					HUD->PrimaryWeaponWidget->SetRenderOpacity(1.f);
					
					HUD->SecondaryWeaponWidget->SetRenderOpacity(0.5f);
					HUD->FragGrenadeWidget->SetRenderOpacity(0.5f);
					HUD->FlashbangGrenadeWidget->SetRenderOpacity(0.5f);
					HUD->SmokeGrenadeWidget->SetRenderOpacity(0.5f);
					HUD->KnifeWidget->SetRenderOpacity(0.5f);
				}
				else if (W->Implements<USecondaryWeapon>() && HUD->SecondaryWeaponWidget->IsVisible())
				{
					HUD->SecondaryWeaponWidget->SetRenderOpacity(1.f);

					HUD->PrimaryWeaponWidget->SetRenderOpacity(0.5f);
					HUD->FragGrenadeWidget->SetRenderOpacity(0.5f);
					HUD->FlashbangGrenadeWidget->SetRenderOpacity(0.5f);
					HUD->SmokeGrenadeWidget->SetRenderOpacity(0.5f);
					HUD->KnifeWidget->SetRenderOpacity(0.5f);
				}
			}
			else if (ABaseGrenade* G = Cast<ABaseGrenade>(CurrentItemInHands))
			{
				HUD->CrosshairWidget->SetVisibility(ESlateVisibility::Visible);

				if (G->IsA(AFragGrenade::StaticClass()))
				{
					HUD->FragGrenadeWidget->SetRenderOpacity(1.f);
					
					HUD->FlashbangGrenadeWidget->SetRenderOpacity(0.5f);
					HUD->SmokeGrenadeWidget->SetRenderOpacity(0.5f);
				}
				else if (G->IsA(AFlashbangGrenade::StaticClass()))
				{
					HUD->FlashbangGrenadeWidget->SetRenderOpacity(1.f);

					HUD->FragGrenadeWidget->SetRenderOpacity(0.5f);
					HUD->SmokeGrenadeWidget->SetRenderOpacity(0.5f);
				}
				else if (G->IsA(ASmokeGrenade::StaticClass()))
				{
					HUD->SmokeGrenadeWidget->SetRenderOpacity(1.f);

					HUD->FragGrenadeWidget->SetRenderOpacity(0.5f);
					HUD->FlashbangGrenadeWidget->SetRenderOpacity(0.5f);
				}

				HUD->PrimaryWeaponWidget->SetRenderOpacity(0.5f);
				HUD->SecondaryWeaponWidget->SetRenderOpacity(0.5f);
				HUD->KnifeWidget->SetRenderOpacity(0.5f);
			}
			else if (Cast<AKnife>(CurrentItemInHands))
			{
				HUD->KnifeWidget->SetRenderOpacity(1.f);
				
				HUD->PrimaryWeaponWidget->SetRenderOpacity(0.5f);
				HUD->SecondaryWeaponWidget->SetRenderOpacity(0.5f);
				HUD->FragGrenadeWidget->SetRenderOpacity(0.5f);
				HUD->FlashbangGrenadeWidget->SetRenderOpacity(0.5f);
				HUD->SmokeGrenadeWidget->SetRenderOpacity(0.5f);
			}
		}
	}

	UpdateAmmoUI();
}

void AMyFPSCharacter::PreviousWeapon()
{
	if (Inventory->GetNumberOfItems() == 1 || Inventory->GetNumberOfItems() == 0)
		return;

		
	if (CurrentItemInHands)
	{
		if (ABaseWeapon* W = Cast<ABaseWeapon>(CurrentItemInHands))
		{
			FAmmoData& CurrentWeaponData = AmmoDataMap.FindOrAdd(W->GetClass());
			CurrentWeaponData.CurrentAmmo = W->GetCurrentAmmo();
			CurrentWeaponData.ClipSize = W->GetReserveAmmo();
		}
		CurrentItemInHands->Destroy();
	}

	if (Inventory->PreviousItem())
	{
		
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Item destroyed, next weapon!"));

		SpawnCurrentWeaponInHands();

		if (CurrentItemInHands)
		{
			// INFO: Load up the new weapon with the saved data if it exists
			if (ABaseWeapon* W = Cast<ABaseWeapon>(CurrentItemInHands))
			{
				if (const FAmmoData* NewAmmoData = AmmoDataMap.Find(CurrentItemInHands->GetClass()))
				{
					W->SetCurrentAmmo(NewAmmoData->CurrentAmmo);
					W->SetReserveAmmo(NewAmmoData->ClipSize);
				}
				else
				{
					HUD->CrosshairWidget->SetVisibility(ESlateVisibility::Hidden);
				}

				if (W->Implements<UPrimaryWeapon>() && HUD->PrimaryWeaponWidget->IsVisible())
				{
					HUD->PrimaryWeaponWidget->SetRenderOpacity(1.f);
					
					HUD->SecondaryWeaponWidget->SetRenderOpacity(0.5f);
					HUD->FragGrenadeWidget->SetRenderOpacity(0.5f);
					HUD->FlashbangGrenadeWidget->SetRenderOpacity(0.5f);
					HUD->SmokeGrenadeWidget->SetRenderOpacity(0.5f);
					HUD->KnifeWidget->SetRenderOpacity(0.5f);
				}
				else if (W->Implements<USecondaryWeapon>() && HUD->SecondaryWeaponWidget->IsVisible())
				{
					HUD->SecondaryWeaponWidget->SetRenderOpacity(1.f);

					HUD->PrimaryWeaponWidget->SetRenderOpacity(0.5f);
					HUD->FragGrenadeWidget->SetRenderOpacity(0.5f);
					HUD->FlashbangGrenadeWidget->SetRenderOpacity(0.5f);
					HUD->SmokeGrenadeWidget->SetRenderOpacity(0.5f);
					HUD->KnifeWidget->SetRenderOpacity(0.5f);
				}
			}
			else if (ABaseGrenade* G = Cast<ABaseGrenade>(CurrentItemInHands))
			{
				HUD->CrosshairWidget->SetVisibility(ESlateVisibility::Visible);

				if (G->IsA(AFragGrenade::StaticClass()))
				{
					HUD->FragGrenadeWidget->SetRenderOpacity(1.f);
					
					HUD->FlashbangGrenadeWidget->SetRenderOpacity(0.5f);
					HUD->SmokeGrenadeWidget->SetRenderOpacity(0.5f);
				}
				else if (G->IsA(AFlashbangGrenade::StaticClass()))
				{
					HUD->FlashbangGrenadeWidget->SetRenderOpacity(1.f);

					HUD->FragGrenadeWidget->SetRenderOpacity(0.5f);
					HUD->SmokeGrenadeWidget->SetRenderOpacity(0.5f);
				}
				else if (G->IsA(ASmokeGrenade::StaticClass()))
				{
					HUD->SmokeGrenadeWidget->SetRenderOpacity(1.f);

					HUD->FragGrenadeWidget->SetRenderOpacity(0.5f);
					HUD->FlashbangGrenadeWidget->SetRenderOpacity(0.5f);
				}

				HUD->PrimaryWeaponWidget->SetRenderOpacity(0.5f);
				HUD->SecondaryWeaponWidget->SetRenderOpacity(0.5f);
				HUD->KnifeWidget->SetRenderOpacity(0.5f);
			}
			else if (Cast<AKnife>(CurrentItemInHands))
			{
				HUD->KnifeWidget->SetRenderOpacity(1.f);
				
				HUD->PrimaryWeaponWidget->SetRenderOpacity(0.5f);
				HUD->SecondaryWeaponWidget->SetRenderOpacity(0.5f);
				HUD->FragGrenadeWidget->SetRenderOpacity(0.5f);
				HUD->FlashbangGrenadeWidget->SetRenderOpacity(0.5f);
				HUD->SmokeGrenadeWidget->SetRenderOpacity(0.5f);
			}
		
		}
	}

	UpdateAmmoUI();
}

void AMyFPSCharacter::ThrowWeapon()
{
	if (AnimationInstance)
	{
		if (AnimationInstance->bIsReloading)
			return;  // INFO: Can't throw weapon while reloading
		
	}
	
	if (!Inventory)
	{
		UE_LOG(LogTemp, Error, TEXT("Inventory is NULL!"));
		return;
	}

	if (Inventory->GetNumberOfItems() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("No items in inventory!"));
		return;
	}

	AInventoryItem* ItemToThrow = Inventory->CurrentItem;

	if (CurrentItemInHands)
	{
		// INFO: Save weapon data before throwing
		if (ABaseWeapon* W = Cast<ABaseWeapon>(CurrentItemInHands))
		{
			FAmmoData& CurrentWeaponData = AmmoDataMap.FindOrAdd(W->GetClass());
			CurrentWeaponData.CurrentAmmo = W->GetCurrentAmmo();
			CurrentWeaponData.ClipSize = W->GetReserveAmmo();
			// INFO: Hide crosshair when throwing a weapon
			if (HUD->CrosshairWidget->IsVisible())
			{
				HUD->CrosshairWidget->SetVisibility(ESlateVisibility::Hidden);
			}

			if (W->Implements<UPrimaryWeapon>())
			{
				HUD->PrimaryWeaponWidget->SetVisibility(ESlateVisibility::Hidden);
			}
			else if (W->Implements<USecondaryWeapon>())
			{
				HUD->SecondaryWeaponWidget->SetVisibility(ESlateVisibility::Hidden);
			}
		}

		if (ABaseGrenade* G = Cast<ABaseGrenade>(CurrentItemInHands))
		{
			if ( HUD->CrosshairWidget->IsVisible())
				HUD->CrosshairWidget->SetVisibility(ESlateVisibility::Hidden);

			if (G->IsA(AFragGrenade::StaticClass()))
			{
				HUD->FragGrenadeWidget->SetVisibility(ESlateVisibility::Hidden);
			}
			else if (G->IsA(AFlashbangGrenade::StaticClass()))
			{
				HUD->FlashbangGrenadeWidget->SetVisibility(ESlateVisibility::Hidden);
			}
			else if (G->IsA(ASmokeGrenade::StaticClass()))
			{
				HUD->SmokeGrenadeWidget->SetVisibility(ESlateVisibility::Hidden);
			}
		}

		if (Cast<AKnife>(CurrentItemInHands))
		{
			HUD->KnifeWidget->SetVisibility(ESlateVisibility::Hidden);
		}
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		
		AActor* SpawnedWeapon = GetWorld()->SpawnActor<AActor>(ItemToThrow->GetClass(), GetActorLocation() + GetActorForwardVector() * 250.f, GetActorRotation(), SpawnParams);

		UStaticMeshComponent* MeshComponent = SpawnedWeapon->FindComponentByClass<UStaticMeshComponent>();
		if (MeshComponent)
		{
			MeshComponent->SetSimulatePhysics(true);
			MeshComponent->AddImpulse(GetActorForwardVector() * 50.f + FVector(0.f, 0.f, 400.f));
			MeshComponent->SetWorldRotation(FRotator(0, 0, 0));
			//CurrentItemInHands = nullptr;
		}

		if (CurrentItemInHands->Destroy())
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Item destroyed!"));
		}
		CurrentItemInHands = nullptr;
		Inventory->ThrowItem();
		
		if (Inventory->GetNumberOfItems() > 0)
		{
			SpawnCurrentWeaponInHands();
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("One more item in inventory!"));

			
			if (ABaseWeapon* W = Cast<ABaseWeapon>(CurrentItemInHands))
			{
				if (!HUD->CrosshairWidget->IsVisible() && !CurrentItemInHands->IsA(ASniper::StaticClass()))
				{
					HUD->CrosshairWidget->SetVisibility(ESlateVisibility::Visible);
				}
				
				if (W->Implements<UPrimaryWeapon>())
				{
					HUD->PrimaryWeaponWidget->SetRenderOpacity(1.f);
				}
				else if (W->Implements<USecondaryWeapon>())
				{
					HUD->SecondaryWeaponWidget->SetRenderOpacity(1.f);
				}
			}
			else if (ABaseGrenade* G = Cast<ABaseGrenade>(CurrentItemInHands))
			{
				HUD->CrosshairWidget->SetVisibility(ESlateVisibility::Visible);

				if (G->IsA(AFragGrenade::StaticClass()))
				{
					HUD->FragGrenadeWidget->SetRenderOpacity(1.f);
				}
				else if (G->IsA(AFlashbangGrenade::StaticClass()))
				{
					HUD->FlashbangGrenadeWidget->SetRenderOpacity(1.f);
				}
				else if (G->IsA(ASmokeGrenade::StaticClass()))
				{
					HUD->SmokeGrenadeWidget->SetRenderOpacity(1.f);
				}
			}
			else if (Cast<AKnife>(CurrentItemInHands))
			{
				HUD->KnifeWidget->SetRenderOpacity(1.f);
			}
		}

		UpdateAmmoUI();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No weapon to throw!"));
	}
}

void AMyFPSCharacter::SpawnCurrentWeaponInHands()
{
	if (!Inventory || !Inventory->CurrentItem || !CurrentItemInHands)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Inventory or CurrentItem is null!"));
		return;
	}
	
	TSubclassOf<AActor> WeaponClass = CurrentItemInHands->GetClass();
	if (!WeaponClass)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Invalid weapon class!"));
		return;
	}

	// Spawn new weapon
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	AInventoryItem* SpawnedWeapon = GetWorld()->SpawnActor<AInventoryItem>(WeaponClass, GetActorLocation() + GetActorForwardVector() * 250.f, GetActorRotation(), SpawnParams);
	CurrentItemInHands = SpawnedWeapon;

	SpawnedWeapon->SetOwner(this);

	if (!SpawnedWeapon)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to spawn weapon!"));
		return;
	}

	// Attach weapon to character mesh
	UStaticMeshComponent* MeshComponent = SpawnedWeapon->FindComponentByClass<UStaticMeshComponent>();
	if (MeshComponent)
	{
		MeshComponent->SetSimulatePhysics(false);
		MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		MeshComponent->SetMassScale(NAME_None, 0.0f);
		MeshComponent->SetEnableGravity(false);
		MeshComponent->WakeRigidBody();

		// Attach to socket
		MeshComponent->AttachToComponent(
			GetMesh(),
			FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true),
			FName("WeaponSocket")
		);

		MeshComponent->SetRelativeTransform(Inventory->CurrentItem->AttachmentTransform);
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Weapon spawned and attached!"));
}

void AMyFPSCharacter::ResetWalkingSpeed()
{
	GetCharacterMovement()->MaxWalkSpeed /= 2.f;
	GetWorldTimerManager().ClearTimer(AnimationTimerHandle);
}

void AMyFPSCharacter::ResetBulletDamage()
{
	if (!CurrentItemInHands)
		return;

	if (ABaseWeapon* W = Cast<ABaseWeapon>(CurrentItemInHands))
	{
		if (ABaseProjectile* P = Cast<ABaseProjectile>(W->WeaponBullet))
		{
			P->DamageAmount /= 1.1f;
		}
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Bullet damage reset!"));
	GetWorldTimerManager().ClearTimer(AnimationTimerHandle);
}


void AMyFPSCharacter::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!Inventory)
	{
		UE_LOG(LogTemp, Error, TEXT("Inventory is NULL! Cannot add item."));
		return;
	}

	if (OtherActor->GetOwner() && OtherActor->GetOwner()->IsA(AEnemyCharacter::StaticClass()))
		return;
	
	if(AInventoryItem* It = Cast<AInventoryItem>(OtherActor))
	{
			//INFO: Add weapon to inventory
		if (Inventory->GetNumberOfItems() == 0 && Inventory->AddItem(It))
		{
			if (UStaticMeshComponent* MeshComp = OtherActor->FindComponentByClass<UStaticMeshComponent>())
			{
				MeshComp->SetSimulatePhysics(false);
				MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				MeshComp->SetMassScale(NAME_None, 0.0f);
				MeshComp->SetEnableGravity(false);
				MeshComp->WakeRigidBody();
			
				//MeshComp->SetWorldLocationAndRotation(WeaponLocation, WeaponRotation);
			
				//MeshComp->SetWorldScale3D(WeaponScale);
				MeshComp->AttachToComponent(
					GetMesh(),
					FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true),
					FName("WeaponSocket")
				);
			
				MeshComp->SetRelativeTransform(Inventory->CurrentItem->AttachmentTransform);
				OtherActor->SetOwner(this);

				if (Cast<ABaseGrenade>(OtherActor))
				{
					HUD->CrosshairWidget->SetVisibility(ESlateVisibility::Visible);
				}
			}
			
			if (ABaseWeapon* W = Cast<ABaseWeapon>(CurrentItemInHands))
			{
				if (const FAmmoData* NewAmmoData = AmmoDataMap.Find(CurrentItemInHands->GetClass()))
				{
					W->SetCurrentAmmo(NewAmmoData->CurrentAmmo);
					W->SetReserveAmmo(NewAmmoData->ClipSize);
				}
				UpdateAmmoUI();

				if (!W->IsA(ASniper::StaticClass()))
				{
					HUD->CrosshairWidget->SetVisibility(ESlateVisibility::Visible);
				}

				if (W->Implements<UPrimaryWeapon>())
				{
					HUD->PrimaryWeaponWidget->SetVisibility(ESlateVisibility::Visible);

					if (W->IsA(ASniper::StaticClass()))
					{
						HUD->PrimaryWeaponWidget->SetWeaponImage(HUD->Sniper);
					}
					else if (W->IsA(ARifle::StaticClass()))
					{
						HUD->PrimaryWeaponWidget->SetWeaponImage(HUD->AK47);
					}
					else if (W->IsA(AShotgun::StaticClass()))
					{
						HUD->PrimaryWeaponWidget->SetWeaponImage(HUD->Shotgun);
					}
				}
				else if (W->Implements<USecondaryWeapon>())
				{
					HUD->SecondaryWeaponWidget->SetVisibility(ESlateVisibility::Visible);
					//HUD->SecondaryWeaponWidget->SetRenderOpacity(0.5f);
				}
			}
			else if (ABaseGrenade* G = Cast<ABaseGrenade>(CurrentItemInHands))
			{
				if (G->IsA(AFragGrenade::StaticClass()))
				{
					HUD->FragGrenadeWidget->SetVisibility(ESlateVisibility::Visible);
				}
				else if (G->IsA(AFlashbangGrenade::StaticClass()))
				{
					HUD->FlashbangGrenadeWidget->SetVisibility(ESlateVisibility::Visible);
				}
				else if (G->IsA(ASmokeGrenade::StaticClass()))
				{
					HUD->SmokeGrenadeWidget->SetVisibility(ESlateVisibility::Visible);
				}
			}
			else if (Cast<AKnife>(CurrentItemInHands))
			{
				HUD->KnifeWidget->SetVisibility(ESlateVisibility::Visible);
			}
		}
		else if(Inventory->AddItem(It))
		{
			if (ABaseWeapon* W = Cast<ABaseWeapon>(It))
			{
				if (const FAmmoData* NewAmmoData = AmmoDataMap.Find(CurrentItemInHands->GetClass()))
				{
					W->SetCurrentAmmo(NewAmmoData->CurrentAmmo);
					W->SetReserveAmmo(NewAmmoData->ClipSize);
				}

				if (W->Implements<UPrimaryWeapon>())
				{
					HUD->PrimaryWeaponWidget->SetVisibility(ESlateVisibility::Visible);
					HUD->PrimaryWeaponWidget->SetRenderOpacity(0.5f);

					if (W->IsA(ASniper::StaticClass()))
					{
						HUD->PrimaryWeaponWidget->SetWeaponImage(HUD->Sniper);
					}
					else if (W->IsA(ARifle::StaticClass()))
					{
						HUD->PrimaryWeaponWidget->SetWeaponImage(HUD->AK47);
					}
					else if (W->IsA(AShotgun::StaticClass()))
					{
						HUD->PrimaryWeaponWidget->SetWeaponImage(HUD->Shotgun);
					}
				}
				else if (W->Implements<USecondaryWeapon>())
				{
					HUD->SecondaryWeaponWidget->SetVisibility(ESlateVisibility::Visible);
					HUD->SecondaryWeaponWidget->SetRenderOpacity(0.5f);
				}
			}
			else if (ABaseGrenade* G = Cast<ABaseGrenade>(It))
			{
				if (G->IsA(AFragGrenade::StaticClass()))
				{
					HUD->FragGrenadeWidget->SetVisibility(ESlateVisibility::Visible);
					HUD->FragGrenadeWidget->SetRenderOpacity(0.5f);
				}
				else if (G->IsA(AFlashbangGrenade::StaticClass()))
				{
					HUD->FlashbangGrenadeWidget->SetVisibility(ESlateVisibility::Visible);
					HUD->FlashbangGrenadeWidget->SetRenderOpacity(0.5f);
				}
				else if (G->IsA(ASmokeGrenade::StaticClass()))
				{
					HUD->SmokeGrenadeWidget->SetVisibility(ESlateVisibility::Visible);
					HUD->SmokeGrenadeWidget->SetRenderOpacity(0.5f);
				}
			}
			else if (Cast<AKnife>(It))
			{
				HUD->KnifeWidget->SetVisibility(ESlateVisibility::Visible);
				HUD->KnifeWidget->SetRenderOpacity(0.5f);
			}
			
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Item added to inventory!"));
			OtherActor->Destroy();
		}
		UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
	}
}

void AMyFPSCharacter::DestroyWidget(UUserWidget* Widget)
{
	Widget->Destruct();
}

void AMyFPSCharacter::UpdateAmmoUI()
{
	if (CurrentItemInHands)
	{
		if (ABaseWeapon* W = Cast<ABaseWeapon>(CurrentItemInHands))
		{
			HUD->CurrentAmmoText->SetVisibility(ESlateVisibility::Visible);
			HUD->ReserveAmmoText->SetVisibility(ESlateVisibility::Visible);
			HUD->UpdateAmmoValues(W->GetCurrentAmmo(), W->GetReserveAmmo());
		}
		else
		{
			HUD->CurrentAmmoText->SetVisibility(ESlateVisibility::Hidden);
			HUD->ReserveAmmoText->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	else
	{
		HUD->CurrentAmmoText->SetVisibility(ESlateVisibility::Hidden);
		HUD->ReserveAmmoText->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AMyFPSCharacter::Heal()
{
	CurrentHealth += 50;

	UGameplayStatics::PlaySoundAtLocation(this, HealingSound, GetActorLocation());
	
	if (CurrentHealth > MaxHealth)
	{
		CurrentHealth = MaxHealth;
	}
	HUD->SetHealth(CurrentHealth, MaxHealth);
}

void AMyFPSCharacter::TakeDamage(float Damage)
{
	CurrentHealth -= Damage;
	if (CurrentHealth <= 0)
	{
		Die();
	}
	else
	{
		UGameplayStatics::PlaySoundAtLocation(this, TakeDamageSound, GetActorLocation());
	}
	HUD->SetHealth(CurrentHealth, MaxHealth);

	if (DamageEffectWidgetClass)
	{
		DamageEffectWidget = CreateWidget<UUserWidget>(GetWorld(), DamageEffectWidgetClass);
		if (DamageEffectWidget)
		{
			DamageEffectWidget->AddToViewport();
			
			GetWorldTimerManager().SetTimer(WidgetTimerHandle, [this]{DestroyWidget(DamageEffectWidget);}, 0.5f, false);
		}
	}
}

