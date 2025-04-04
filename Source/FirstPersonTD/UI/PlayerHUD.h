// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPERSONTD_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UProgressBar* HealthBar;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UProgressBar* KnifeThrowProgressBar;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UTextBlock* CurrentAmmoText;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UTextBlock* ReserveAmmoText;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> CrosshairWidgetClass;

	UPROPERTY()
	UUserWidget* CrosshairWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> SpedUpWidgetClass;

	UPROPERTY()
	UUserWidget* SpedUpWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> DamageBuffWidgetClass;

	UPROPERTY()
	UUserWidget* DamageBuffWidget;

	//UPROPERTY(EditAnywhere)
	//TSubclassOf<UUserWidget> SniperScopeWidgetClass;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UImage* SniperScopeWidget;

	void SetHealth(float Health, float MaxHealth);
	void SetKnifeThrowProgress(float Time, float MaxTime);
	void UpdateAmmoValues(int CurrentAmmo, int ReserveAmmo);
	
};
