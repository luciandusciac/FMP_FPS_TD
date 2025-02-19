// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"

#include "Interfaces/PrimaryWeapon.h"
#include "Interfaces/SecondaryWeapon.h"

UInventory::UInventory()
{
	CurrentItem = nullptr;
	CurrentInventorySlot = NULL;
}

void UInventory::AddItem(AInventoryItem* Item)
{
	/*
	 * If item implements primary weapon interface, add with key 0
	 * If item implements secondary weapon interface, add with key 1
	 * If item implements grenade interface, add with key 2
	 * If item implements knife interface, add with key 3
	 */

	if (!Item)
	{
		UE_LOG(LogTemp, Error, TEXT("Trying to add a NULL item to inventory!"));
		return;
	}

	if(Item->Implements<UPrimaryWeapon>() && !InventorySlots.Contains(0))
	{
		InventorySlots.Add(0, Item);
		Item->Destroy();
		UE_LOG(LogTemp, Warning, TEXT("Item added to inventory: %s"), *Item->GetName());
	}
	else if(Item->Implements<USecondaryWeapon>() && !InventorySlots.Contains(1))
	{
		InventorySlots.Add(1, Item);
		Item->Destroy();
		UE_LOG(LogTemp, Warning, TEXT("Item added to inventory: %s"), *Item->GetName());
	}

	// else if(Item->Implements<UGrenade>())
	// {
	// 	InventorySlots.Add(2, Item);
	// }
	// else if(Item->Implements<UKnife>())
	// {
	// 	InventorySlots.Add(3, Item);
	// }
	// else
	// {
	// 	InventorySlots.Add(4, Item);
	// }

	
}

void UInventory::ThrowItem()
{
	CurrentItem = nullptr;
	InventorySlots.Remove(CurrentInventorySlot);

	//TODO: Instantiate item and throw it
	

	
	NextItem();
}

void UInventory::UseItem(AInventoryItem* Item)
{
	Item->Use();

	// If item is consumable, remove from inventory
	if(Item->bIsConsumable)
	{
		InventorySlots.Remove(CurrentInventorySlot);
		NextItem();
	}
	
}

void UInventory::NextItem()
{
	CurrentInventorySlot++;
	if(CurrentInventorySlot > InventorySlots.Num())
	{
		CurrentInventorySlot = 0;
	}

	CurrentItem = InventorySlots[CurrentInventorySlot];
}

void UInventory::PreviousItem()
{
	CurrentInventorySlot--;
	if(CurrentInventorySlot < 0)
	{
		CurrentInventorySlot = InventorySlots.Num();
	}

	CurrentItem = InventorySlots[CurrentInventorySlot];
}
