// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItem.h"
#include "Inventory.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPERSONTD_API UInventory : public UObject
{
	GENERATED_BODY()

public:

	UInventory();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	AInventoryItem* CurrentItem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	int CurrentInventorySlot;
	
	void AddItem(AInventoryItem* Item);
	void ThrowItem();
	void UseItem(AInventoryItem* Item);
	void SwapItem();

private:

	UPROPERTY()
	TMap<int, AInventoryItem*> InventorySlots;


};
