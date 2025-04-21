// Fill out your copyright notice in the Description page of Project Settings.


#include "PrimaryWeaponWidget.h"

void UPrimaryWeaponWidget::SetWeaponImage(UTexture2D* NewTexture)
{
	if (WeaponImage && NewTexture)
	{
		WeaponImage->SetBrushFromTexture(NewTexture);
	}
}
