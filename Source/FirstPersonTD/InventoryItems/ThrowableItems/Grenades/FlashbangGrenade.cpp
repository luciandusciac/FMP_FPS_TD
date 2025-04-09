// Fill out your copyright notice in the Description page of Project Settings.


#include "FlashbangGrenade.h"

#include "Engine/World.h"
#include "FirstPersonTD/Characters/EnemyCharacter.h"
#include "FirstPersonTD/Characters/MyFPSCharacter.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AFlashbangGrenade::AFlashbangGrenade()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ExplosionTime = 3.0f;
}

// Called when the game starts or when spawned
void AFlashbangGrenade::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFlashbangGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bCanExplode)
	{
		Explode();
		bCanExplode = false;
	}
}

void AFlashbangGrenade::Explode()
{
	//Super::Explode();
 
	GetWorldTimerManager().SetTimer(ExplosionTimerHandle, this, &AFlashbangGrenade::OnExplode, ExplosionTime, false);
}

void AFlashbangGrenade::OnExplode()
{
	UE_LOG(LogTemp, Warning, TEXT("Flashbang Explosion"));

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacter::StaticClass(), FoundActors);

	UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());

	for (AActor* Actor : FoundActors)
	{
		FVector DirectionToExplosion = (GetActorLocation() - Actor->GetActorLocation()).GetSafeNormal();
		FVector ActorForwardVector = Actor->GetActorForwardVector();
		
		if (FVector::DotProduct(DirectionToExplosion, ActorForwardVector) > 0.5f)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s is facing the explosion"), *Actor->GetName());

			if (AMyFPSCharacter* Ch = Cast<AMyFPSCharacter>(Actor))
			{
				if (Ch->FlashbangWidgetClass)
				{
					Ch->FlashbangWidget = CreateWidget<UUserWidget>(GetWorld(), Ch->FlashbangWidgetClass);
					if (Ch->FlashbangWidget)
					{
						Ch->FlashbangWidget->AddToViewport();
						GetWorldTimerManager().SetTimer(Ch->WidgetTimerHandle, [Ch]{Ch->DestroyWidget(Ch->FlashbangWidget);}, 5.f, false);
					}
				}
			}
			else if (AEnemyCharacter* En = Cast<AEnemyCharacter>(Actor))
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Enemy blinded!"));
				if (AEnemyController* AIController = Cast<AEnemyController>(En->GetController()))
				{
					if (UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent())
					{
						AIController->GetBrainComponent()->StopLogic("Flashbang effect");
						
						GetWorldTimerManager().SetTimer(FlashbangTimer, [AIController, BlackboardComp]() 
						{
							GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Enemy can see player again!"));
							
							// INFO: Restart the AI behavior tree
							AIController->GetBrainComponent()->RestartLogic();
						}, 5.0f, false); 


						
						// BlackboardComp->SetValueAsBool(TEXT("CanSeePlayer"), false);
						// GetWorldTimerManager().SetTimer(FlashbangTimer, [En]() 
						// {
						// 	//En->ResetCanSeePlayer();
						// 	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Enemy can see player again!"));
						// 	// if (AEnemyController* AIController = Cast<AEnemyController>(En->GetController()))
						// 	// {
						// 	// 	if (UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent())
						// 	// 	{
						// 	// 		BlackboardComp->SetValueAsBool(TEXT("CanSeePlayer"), true);
						// 	// 	}
						// 	// }
						// }, 5.0f, false);
					}
				}
			}
		}
	}

	this->Destroy();
}

