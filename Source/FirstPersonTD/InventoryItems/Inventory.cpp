// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"

#include "FirstPersonTD/Animations/SWAT_AnimInstance.h"
#include "FirstPersonTD/Characters/MyFPSCharacter.h"
#include "FirstPersonTD/Controller/FPSPlayerController.h"
#include "Interfaces/PrimaryWeapon.h"
#include "Interfaces/SecondaryWeapon.h"

UInventory::UInventory()
{
	CurrentItem = nullptr;
	CurrentInventorySlot = NULL;
}

bool UInventory::AddItem(AInventoryItem* Item)
{
	/*
	 * If item implements primary weapon interface, add with key 0
	 * If item implements secondary weapon interface, add with key 1
	 * If item implements grenade interface, add with key 2
	 * If item implements knife interface, add with key 3
	 */

	//TODO: Add more item types
	//TODO: Add more inventory slots
	//TODO: If weapon is dropped and another one equipped, make it the current item and swap to it

	if (!Item)
	{
		UE_LOG(LogTemp, Error, TEXT("Trying to add a NULL item to inventory!"));
		return false;
	}

	if(Item->Implements<UPrimaryWeapon>() && !InventorySlots.Contains(0))
	{
		InventorySlots.Add(0, Item);
		CurrentItem = Item;
		//Item->Destroy();  //commented out because it needs adding to hands
		UE_LOG(LogTemp, Warning, TEXT("Item added to inventory: %s"), *Item->GetName());
		CurrentInventorySlot = 0; //???

		if(AMyFPSCharacter* C = Cast<AMyFPSCharacter>(GetOuter()))
		{
			//C->AnimationInstance->AnimationIndex = CurrentInventorySlot;
			if(AFPSPlayerController* controller = Cast<AFPSPlayerController>(C->GetController()))
			{
				controller->AnimationIndex = CurrentInventorySlot;  //Animation is lined up with inventory slot
				controller->EquipWeapon(CurrentInventorySlot);  //Play the animation related to the weapon
				return true;
			}
		}
	}
	else if(Item->Implements<USecondaryWeapon>() && !InventorySlots.Contains(1))
	{
		InventorySlots.Add(1, Item);
		CurrentItem = Item;
		//Item->Destroy();  //commented out because it needs adding to hands
		UE_LOG(LogTemp, Warning, TEXT("Item added to inventory: %s"), *Item->GetName());
		CurrentInventorySlot = 1; //???

		if(AMyFPSCharacter* C = Cast<AMyFPSCharacter>(GetOuter()))
		{
			//C->AnimationInstance->AnimationIndex = CurrentInventorySlot;
			if(AFPSPlayerController* controller = Cast<AFPSPlayerController>(C->GetController()))
			{
				controller->AnimationIndex = CurrentInventorySlot;  //Animation is lined up with inventory slot
				controller->EquipWeapon(CurrentInventorySlot);  //Play the animation related to the weapon
				return true;
			}
		}
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

	return false;
}

void UInventory::ThrowItem()
{
	if(!CurrentItem)
	{
		UE_LOG(LogTemp, Error, TEXT("Current item is NULL!"));
		return;
	}

	if(InventorySlots.Num() > 0)
	{
		//CurrentItem = nullptr;
		CurrentItem->Destroy();
		//InventorySlots[CurrentInventorySlot] = nullptr;
		InventorySlots.Remove(CurrentInventorySlot);
		UE_LOG(LogTemp, Warning, TEXT("Inventory slot empty: %d"), InventorySlots.IsEmpty());
	
		UE_LOG(LogTemp, Warning, TEXT("Item thrown: %s"), *CurrentItem->GetName());

		if(InventorySlots.Num()>0)
			NextItem();
		else
		{
			CurrentItem = nullptr;

			//CurrentInventorySlot = NULL;
			
			if(AMyFPSCharacter* C = Cast<AMyFPSCharacter>(GetOuter()))
			{
				//C->AnimationInstance->AnimationIndex = CurrentInventorySlot;
				if(AFPSPlayerController* controller = Cast<AFPSPlayerController>(C->GetController()))
				{
					controller->AnimationIndex = 10;  //Animation is lined up with inventory slot
					controller->EquipWeapon(controller->AnimationIndex);  //Play the animation related to the weapon
					//C->CurrentItemInHands = InventorySlots[CurrentInventorySlot];
				}
			}
		}
		if(CurrentItem)
			UE_LOG(LogTemp, Warning, TEXT("Current item: %s"), *CurrentItem->GetName());
	}
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
	if(InventorySlots.Num() == 0)
	{
		CurrentItem = nullptr;
		return;
	}

	if(InventorySlots.Num() == 1)
	{
		CurrentInventorySlot = InventorySlots.CreateConstIterator()->Key;
		CurrentItem = InventorySlots[CurrentInventorySlot];
		
		if(AMyFPSCharacter* C = Cast<AMyFPSCharacter>(GetOuter()))
		{
			//C->AnimationInstance->AnimationIndex = CurrentInventorySlot;
			if(AFPSPlayerController* controller = Cast<AFPSPlayerController>(C->GetController()))
			{
				controller->AnimationIndex = CurrentInventorySlot;  //Animation is lined up with inventory slot
				controller->EquipWeapon(CurrentInventorySlot);  //Play the animation related to the weapon
				C->CurrentItemInHands = InventorySlots[CurrentInventorySlot];
			}
		}
	}
	
	if(InventorySlots.Num() > 1)
	{
		CurrentInventorySlot++;
		if(CurrentInventorySlot >= InventorySlots.Num())
		{
			CurrentInventorySlot = 0;
		}


		if(AMyFPSCharacter* C = Cast<AMyFPSCharacter>(GetOuter()))
		{
			//C->AnimationInstance->AnimationIndex = CurrentInventorySlot;
			if(AFPSPlayerController* controller = Cast<AFPSPlayerController>(C->GetController()))
			{
				controller->AnimationIndex = CurrentInventorySlot;  //Animation is lined up with inventory slot
				controller->EquipWeapon(CurrentInventorySlot);  //Play the animation related to the weapon
				C->CurrentItemInHands = InventorySlots[CurrentInventorySlot];
			}
		}
		
		if (InventorySlots.Contains(CurrentInventorySlot)) 
		{
			CurrentItem = InventorySlots[CurrentInventorySlot];

			if (CurrentItem)
			{
				UE_LOG(LogTemp, Warning, TEXT("Switched to item: %s"), *CurrentItem->GetName());
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Item at slot %d is NULL!"), CurrentInventorySlot);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Invalid index %d when switching inventory item!"), CurrentInventorySlot);
			CurrentItem = nullptr;
		}
	}
	
}

void UInventory::PreviousItem()
{
	if(InventorySlots.Num() == 0)
	{
		CurrentItem = nullptr;
		return;
	}
	
	if(InventorySlots.Num() > 1)
	{
		CurrentInventorySlot--;
		if(CurrentInventorySlot < 0)
		{
			CurrentInventorySlot = InventorySlots.Num()-1;
		}


		if(AMyFPSCharacter* C = Cast<AMyFPSCharacter>(GetOuter()))
		{
			//C->AnimationInstance->AnimationIndex = CurrentInventorySlot;
			if(AFPSPlayerController* controller = Cast<AFPSPlayerController>(C->GetController()))
			{
				controller->AnimationIndex = CurrentInventorySlot;  //Animation is lined up with inventory slot
				controller->EquipWeapon(CurrentInventorySlot);  //Play the animation related to the weapon
				C->CurrentItemInHands = InventorySlots[CurrentInventorySlot];
			}
		}
		
		if (InventorySlots.Contains(CurrentInventorySlot)) 
		{
			CurrentItem = InventorySlots[CurrentInventorySlot];

			if (CurrentItem)
			{
				UE_LOG(LogTemp, Warning, TEXT("Switched to item: %s"), *CurrentItem->GetName());
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Item at slot %d is NULL!"), CurrentInventorySlot);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Invalid index %d when switching inventory item!"), CurrentInventorySlot);
			CurrentItem = nullptr;
		}
	}
}
