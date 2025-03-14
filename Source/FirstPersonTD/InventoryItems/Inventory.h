// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItem.h"
#include "FirstPersonTD/Controller/FPSPlayerController.h"
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

	virtual void BeginPlay();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	AInventoryItem* CurrentItem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	int CurrentInventorySlot;

	UPROPERTY()
	AFPSPlayerController* Controller;
	
	bool AddItem(AInventoryItem* Item);
	void ThrowItem();
	void UseItem(AInventoryItem* Item);
	//void SwapItem();

	bool NextItem();
	bool PreviousItem();

	int GetNumberOfItems();

private:

	UPROPERTY()
	TMap<int, AInventoryItem*> InventorySlots;

	UPROPERTY()
	TArray<AInventoryItem*> InventoryItems;

	void SortInventoryItems();
	void CheckEmptyInventory(int Index);
	void SetCurrentItemInHands();

};


