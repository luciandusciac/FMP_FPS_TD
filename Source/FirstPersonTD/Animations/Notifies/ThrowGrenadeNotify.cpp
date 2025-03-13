// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrowGrenadeNotify.h"

#include "FirstPersonTD/Characters/MyFPSCharacter.h"

void UThrowGrenadeNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	//Super::Notify(MeshComp, Animation);

	AMyFPSCharacter* Character = Cast<AMyFPSCharacter>(MeshComp->GetOwner());
	if (!Character)
	{
		UE_LOG(LogTemp, Error, TEXT("UAnimNotify_SpawnGrenade: Character is NULL!"));
		return;
	}
	
	Character->ThrowGrenade();
}
