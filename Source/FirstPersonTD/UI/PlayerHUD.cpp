// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"

#include "Components/ProgressBar.h"

void UPlayerHUD::SetHealth(float Health, float MaxHealth)
{
	if (HealthBar)
		HealthBar->SetPercent(Health/MaxHealth);
}

void UPlayerHUD::SetKnifeThrowProgress(float Time, float MaxTime)
{
	if (KnifeThrowProgressBar)
		KnifeThrowProgressBar->SetPercent(Time/MaxTime);
}

void UPlayerHUD::UpdateAmmoValues(int CurrentAmmo, int ReserveAmmo)
{
	//if (CurrentAmmo && ReserveAmmo)
	//{
		CurrentAmmoText->SetText(FText::FromString(FString::FromInt(CurrentAmmo)));
		ReserveAmmoText->SetText(FText::FromString(FString::FromInt(ReserveAmmo)));
	//}
	// else
	// {
	// 	CurrentAmmoText->SetText(FText::FromString("0"));
	// 	ReserveAmmoText->SetText(FText::FromString("0"));
	// }
}

