// Fill out your copyright notice in the Description page of Project Settings.
#include "MyFPSCharacter.h"

#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/Controller.h"
#include "Net/UnrealNetwork.h"
#include "../Source/FirstPersonTD/InventoryItems/WeaponClasses/BaseWeapon.h"
#include "Components/CapsuleComponent.h"
#include "FirstPersonTD/Animations/SWAT_AnimInstance.h"

class ABaseWeapon;

// Sets default values
AMyFPSCharacter::AMyFPSCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->bUsePawnControlRotation = true;
	Camera->SetupAttachment(GetMesh(), "Head");
	
	this->GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMyFPSCharacter::OnComponentBeginOverlap);

	AnimationInstance = Cast<USWAT_AnimInstance>(GetMesh()->GetAnimInstance());
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
}


void AMyFPSCharacter::ThrowGrenade()
{
	//GetMesh()->PlayAnimation(GrenadeThrowAnimation, false);
	GetWorldTimerManager().SetTimer(AnimationTimerHandle, this, &AMyFPSCharacter::OnGrenadeThrown, GrenadeThrowTime, false);
	
	if(USWAT_AnimInstance* AnimInstance = Cast<USWAT_AnimInstance>(GetMesh()->GetAnimInstance()))
	{
		AnimInstance->bIsThrowingGrenade = true;
	}
}

void AMyFPSCharacter::OnGrenadeThrown()
{
	GetWorldTimerManager().ClearTimer(AnimationTimerHandle);
	
	if(USWAT_AnimInstance* AnimInstance = Cast<USWAT_AnimInstance>(GetMesh()->GetAnimInstance()))
	{
		AnimInstance->bIsThrowingGrenade = false;
		AnimInstance->bHasGrenade = false;
	}
}

void AMyFPSCharacter::Shoot()
{
	//GetMesh()->PlayAnimation(ShootingAnimation, false);
	if(USWAT_AnimInstance* AnimInstance = Cast<USWAT_AnimInstance>(GetMesh()->GetAnimInstance()))
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
	if(Inventory->CurrentItem != nullptr)
	{
		//                                                                                                                   tweak the values here for camera position when aiming
		Camera->SetRelativeLocation(FMath::VInterpTo(Camera->GetRelativeLocation(), AimTransform.GetLocation() + FVector(-8, 6, 0), GetWorld()->GetDeltaSeconds(), 10.0f));
	
		Camera->SetFieldOfView(50.f);
	}
}

void AMyFPSCharacter::StopAiming()
{
	Camera->SetRelativeLocation(FMath::VInterpTo(Camera->GetRelativeLocation(), FVector(0.f, 0.f, 0.f), GetWorld()->GetDeltaSeconds(), 50.0f));

	Camera->SetFieldOfView(90.f);
}

void AMyFPSCharacter::Die()
{
	GetMesh()->PlayAnimation(DeathAnimation, false);
}

void AMyFPSCharacter::OnDeath()
{
}

void AMyFPSCharacter::Reload()
{
	//GetMesh()->PlayAnimation(ReloadingAnimation, false);
	GetWorldTimerManager().SetTimer(AnimationTimerHandle, this, &AMyFPSCharacter::OnReload, ReloadingTime, false);
	
	if(USWAT_AnimInstance* AnimInstance = Cast<USWAT_AnimInstance>(GetMesh()->GetAnimInstance()))
	{
		AnimInstance->bIsReloading = true;
	}
}

void AMyFPSCharacter::OnReload()
{
	GetWorldTimerManager().ClearTimer(AnimationTimerHandle);
	
	if(USWAT_AnimInstance* AnimInstance = Cast<USWAT_AnimInstance>(GetMesh()->GetAnimInstance()))
	{
		AnimInstance->bIsReloading = false;
	}
}

void AMyFPSCharacter::NextWeapon()
{
	Inventory->NextItem();
}

void AMyFPSCharacter::PreviousWeapon()
{
	Inventory->PreviousItem();
	
}

void AMyFPSCharacter::ThrowWeapon()
{
	if (!Inventory)
	{
		UE_LOG(LogTemp, Error, TEXT("Inventory is NULL!"));
		return;
	}

	AInventoryItem* ItemToThrow = Inventory->CurrentItem;
	Inventory->ThrowItem();

	if (ItemToThrow)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		
		AActor* SpawnedWeapon = GetWorld()->SpawnActor<AActor>(ItemToThrow->GetClass(), GetActorLocation() + GetActorForwardVector() * 250.f, GetActorRotation(), SpawnParams);
		UStaticMeshComponent* MeshComp = SpawnedWeapon->FindComponentByClass<UStaticMeshComponent>();
		if (MeshComp)
		{
			MeshComp->SetSimulatePhysics(true);
			MeshComp->AddImpulse(GetActorForwardVector() * 5000.f + FVector(0.f, 0.f, 4000.f));
			//TODO: Destroy item in hands
			CurrentItemInHands->Destroy();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to get mesh component!"));
		}

		if (!SpawnedWeapon)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to spawn weapon!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No weapon to throw!"));
	}
	
}

void AMyFPSCharacter::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!Inventory)
	{
		UE_LOG(LogTemp, Error, TEXT("Inventory is NULL! Cannot add item."));
		return;
	}
	if(Cast<ABaseWeapon>(OtherActor))
	{
		//INFO: Add weapon to inventory
		if(Inventory->AddItem(Cast<AInventoryItem>(OtherActor)))
		{
			//INFO: Attach weapon to player hand
			OtherActor->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "WeaponSocket");
			//make these into a uproperty
			OtherActor->SetActorRelativeLocation(FVector(0.f, 0.f, 0.f));
			OtherActor->SetActorRelativeRotation(FRotator(-90.f, -90.f, 0.f)); 
			OtherActor->SetActorScale3D(FVector(0.5f, 0.5f, 0.5f));
			CurrentItemInHands = OtherActor;
		}

		
	}
	
}

