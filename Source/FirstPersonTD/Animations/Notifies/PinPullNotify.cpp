// Fill out your copyright notice in the Description page of Project Settings.


#include "PinPullNotify.h"

#include "Kismet/GameplayStatics.h"

void UPinPullNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	//Super::Notify(MeshComp, Animation);

	UGameplayStatics::PlaySoundAtLocation(MeshComp->GetWorld(), PinSound, MeshComp->GetComponentLocation());
}
