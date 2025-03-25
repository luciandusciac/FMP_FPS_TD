// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseProjectile.h"

#include "Components/SphereComponent.h"
//#include "FirstPersonTD/InventoryItems/WeaponClasses/BaseWeapon.h"
#include "Engine/World.h"
#include "FirstPersonTD/Characters/MyFPSCharacter.h"
#include "FirstPersonTD/InventoryItems/WeaponClasses/BaseWeapon.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"


ABaseProjectile::ABaseProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	RootComponent = SphereComponent;
	SphereComponent->InitSphereRadius(5.0f);
	//SphereComponent->SetCollisionProfileName("OverlapAll");
	
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        SphereComponent->SetCollisionObjectType(ECC_PhysicsBody);
        SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
        SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));

	BulletTrail = CreateDefaultSubobject<UNiagaraComponent>(TEXT("BulletTrail"));
	BulletTrail->SetupAttachment(SphereComponent);

	//BulletHoleDecal = LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/FMP_FPS_TD/Content/VFX/Bullet/BulletImpact/M_BulletHole.uasset'"));
	
	
	// SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
 //    SphereComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
 //    SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	// ABaseWeapon* WeaponOwner = Cast<ABaseWeapon>(GetOwner());
	// if(WeaponOwner)
	// {
	// 	SphereComponent->IgnoreActorWhenMoving(WeaponOwner, true);
	// }
	// else
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("No Weapon Owner"));
	// }
	// if(GetOwner())
	// {
	// 	SphereComponent->IgnoreActorWhenMoving(GetOwner(), true);
	// }
}


void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMovementComponent->InitialSpeed = Speed;
	ProjectileMovementComponent->MaxSpeed = Speed;
	ProjectileMovementComponent->Velocity = GetActorForwardVector() * Speed;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->ProjectileGravityScale = 0.1f;

	//UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BulletTrail->GetAsset(), GetActorLocation());
	UNiagaraFunctionLibrary::SpawnSystemAttached(
		   BulletTrail->GetAsset(),
		   RootComponent,
		   NAME_None,
		   FVector::ZeroVector,
		   FRotator(0, 180.f, 0),
		   EAttachLocation::Type::KeepRelativeOffset,
		   true
	   );

	
	// if (GetOwner())
	// {
	// 	//UE_LOG(LogTemp, Warning, TEXT("Projectile owner is: %s"), *GetOwner()->GetName());
	// 	//SphereComponent->IgnoreActorWhenMoving(Cast<AActor>(GetOwner()), true);
	// 	SphereComponent->MoveIgnoreActors.Add(GetOwner());
	// }
	// else
	// {
	// 	UE_LOG(LogTemp, Error, TEXT("Projectile has NO owner!"));
	// }
	
	//SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ABaseProjectile::OnComponentBeginOverlap);

	SphereComponent->OnComponentHit.AddDynamic(this, &ABaseProjectile::OnComponentHit);
	// if(GetOwner())
	// {
	// 	SphereComponent->IgnoreActorWhenMoving(GetOwner(), true);
	// }
}


void ABaseProjectile::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	
	//this->Destroy();
}

void ABaseProjectile::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if(OtherActor != GetOwner())
	{
		if (AMyFPSCharacter* Ch = Cast<AMyFPSCharacter>(OtherActor))
		{
			Ch->TakeDamage(DamageAmount);
		}
		
		this->Destroy();
		UGameplayStatics::SpawnDecalAtLocation(GetWorld(), BulletHoleDecal, FVector(10, 10, 10), Hit.ImpactPoint, Hit.ImpactNormal.Rotation(), 10.f);
	}
}

void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//BulletTrail->Activate();
}

