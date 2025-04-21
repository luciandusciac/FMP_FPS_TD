// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "PrimaryWeaponWidget.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPERSONTD_API UPrimaryWeaponWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetWeaponImage(UTexture2D* NewTexture);

protected:
	UPROPERTY(EditAnywhere)
	UImage* WeaponImage;
};
