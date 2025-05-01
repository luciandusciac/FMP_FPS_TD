// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseProjectile.h"

#include "Components/SphereComponent.h"
//#include "FirstPersonTD/InventoryItems/WeaponClasses/BaseWeapon.h"
#include "Engine/World.h"
#include "FirstPersonTD/Characters/EnemyCharacter.h"
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
	
	UNiagaraFunctionLibrary::SpawnSystemAttached(
		   BulletTrail->GetAsset(),
		   RootComponent,
		   NAME_None,
		   FVector::ZeroVector,
		   FRotator(0, 180.f, 0),
		   EAttachLocation::Type::KeepRelativeOffset,
		   true);

	SphereComponent->OnComponentHit.AddDynamic(this, &ABaseProjectile::OnComponentHit);
}


void ABaseProjectile::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
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
		else if (AEnemyCharacter* En = Cast<AEnemyCharacter>(OtherActor))
		{
			En->TakeDamage(DamageAmount);
		}
		
		this->Destroy();
		UGameplayStatics::SpawnDecalAtLocation(GetWorld(), BulletHoleDecal, FVector(10, 10, 10), Hit.ImpactPoint, Hit.ImpactNormal.Rotation(), 10.f);
	}
}

void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

