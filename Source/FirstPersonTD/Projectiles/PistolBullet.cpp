// Fill out your copyright notice in the Description page of Project Settings.


#include "PistolBullet.h"


// Sets default values
APistolBullet::APistolBullet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DamageAmount = 20.f;
}

// Called when the game starts or when spawned
void APistolBullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APistolBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

