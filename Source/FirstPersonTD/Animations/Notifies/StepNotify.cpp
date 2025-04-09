// Fill out your copyright notice in the Description page of Project Settings.


#include "StepNotify.h"

#include "Kismet/GameplayStatics.h"

// void UStepNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
// {
// 	//Super::Notify(MeshComp, Animation);
//
// 	if (!MeshComp) return;
//
// 	AActor* Owner = MeshComp->GetOwner();
// 	if (!Owner) return;
//
// 	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "StepNotify::Notify");
//
// 	FVector SocketLocation = MeshComp->GetSocketLocation(FootSocketName);
// 	FHitResult Hit;
// 	FCollisionQueryParams Params;
// 	Params.AddIgnoredActor(Owner);
//
// 	if (Owner->GetWorld()->LineTraceSingleByChannel(Hit, SocketLocation, SocketLocation - FVector(0, 0, 100.f), ECC_Visibility, Params))
// 	{
// 		EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());
// 		if (SurfaceSounds.Contains(SurfaceType))
// 		{
// 			UGameplayStatics::PlaySoundAtLocation(Owner, SurfaceSounds[SurfaceType], Hit.Location);
// 			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Footstep sound played!"));
// 		}
// 	}
// 	
// }

void UStepNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    // Check if the mesh component is valid
    if (!MeshComp) return;

    AActor* Owner = MeshComp->GetOwner();
    if (!Owner) return;

    // Debugging step to ensure the notify is triggered
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("StepNotify::Notify Triggered"));

    FVector SocketLocation = MeshComp->GetSocketLocation(FootSocketName);
    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.bReturnPhysicalMaterial = true;
    Params.AddIgnoredActor(Owner); // Ignore the actor itself

    // Visualizing the line trace in the editor (blue line for debugging)
    DrawDebugLine(Owner->GetWorld(), SocketLocation, SocketLocation - FVector(0, 0, 1000.f), FColor::Blue, false, 2.f, 0, 1.f);

    // Perform the line trace to detect the ground surface
    bool bHit = Owner->GetWorld()->LineTraceSingleByChannel(Hit, SocketLocation, SocketLocation - FVector(0, 0, 1000.f), ECC_Visibility, Params);

    // Check if the trace hit something
    if (bHit)
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("Hit something!"));

        // Ensure the hit result has a valid physical material
        if (Hit.PhysMaterial.IsValid())
        {
            // Determine the surface type from the physical material
            EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());

            // Check if the surface type exists in our SurfaceSounds map
            if (SurfaceSounds.Contains(SurfaceType))
            {
                // Play the corresponding sound for the surface type
                UGameplayStatics::PlaySoundAtLocation(Owner, SurfaceSounds[SurfaceType], Hit.Location);
                GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Footstep sound played!"));
            }
            else
            {
                GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("No surface sound for this surface type."));
            }
        }
        else
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Hit does not have a valid physical material!"));
            //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("PhysMaterial: %s"), *Hit.PhysMaterial->GetName()));
        }
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("Did not hit anything."));
    }
}
