// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"

#include "Components/StaticMeshComponent.h"



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
}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();

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
	//TODO: Generate bullet object
}

void ABaseWeapon::Reload()
{
	if(ClipSize > 0)
	{
		CurrentAmmo = ClipSize;
		ClipSize--;
	}
	else
	{
		//TODO: Play error sound
	}
}

void ABaseWeapon::Aim()
{
}

