// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"

UInventory::UInventory()
{
}

void UInventory::AddItem(AInventoryItem* Item)
{
	/*
	 * If item implements primary weapon interface, add with key 0
	 * If item implements secondary weapon interface, add with key 1
	 * If item implements grenade interface, add with key 2
	 * If item implements knife interface, add with key 3
	 */
}

void UInventory::ThrowItem()
{
}

void UInventory::UseItem(AInventoryItem* Item)
{
}

void UInventory::SwapItem()
{
}
