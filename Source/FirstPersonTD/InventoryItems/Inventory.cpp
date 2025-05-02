// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"

#include "FirstPersonTD/Animations/SWAT_AnimInstance.h"
#include "FirstPersonTD/Characters/MyFPSCharacter.h"
#include "FirstPersonTD/Controller/FPSPlayerController.h"
#include "Interfaces/FlashbangInterface.h"
#include "Interfaces/FragGrenadeInterface.h"
#include "Interfaces/KnifeInterface.h"
#include "Interfaces/PrimaryWeapon.h"
#include "Interfaces/SecondaryWeapon.h"
#include "Interfaces/SmokeGrenadeInterface.h"

UInventory::UInventory()
{
	CurrentItem = nullptr;
	CurrentInventorySlot = NULL;

	if(AMyFPSCharacter* C = Cast<AMyFPSCharacter>(GetOuter()))
	{
		Controller = Cast<AFPSPlayerController>(C->GetController());
		
	}

	InventorySlots = TMap<int, AInventoryItem*>();
}

void UInventory::BeginPlay()
{
}

bool UInventory::AddItem(AInventoryItem* Item)
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
		return false;
	}

	if(Item->Implements<UPrimaryWeapon>() && !InventorySlots.Contains(0))
	{
		//CheckEmptyInventory(0);
		InventorySlots.Add(0, Item);
		//InventoryItems.Add(Item);
		//CurrentItem = Item;
		//Item->Destroy();  //commented out because it needs adding to hands
		UE_LOG(LogTemp, Warning, TEXT("Item added to inventory: %s"), *Item->GetName());
		//CurrentInventorySlot = 0; //???
		CheckEmptyInventory(0);
	}
	else if(Item->Implements<USecondaryWeapon>() && !InventorySlots.Contains(1))
	{
		//CheckEmptyInventory(1);
		InventorySlots.Add(1, Item);
		//CurrentItem = Item;
		//Item->Destroy();  //commented out because it needs adding to hands
		UE_LOG(LogTemp, Warning, TEXT("Item added to inventory: %s"), *Item->GetName());
		//CurrentInventorySlot = 1; //???
		CheckEmptyInventory(1);
		
	}
	else if (Item->Implements<UFragGrenadeInterface>() && !InventorySlots.Contains(2))
	{
		//CheckEmptyInventory(2);
		InventorySlots.Add(2, Item);
		//CurrentItem = Item;
		//Item->Destroy();  //commented out because it needs adding to hands
		UE_LOG(LogTemp, Warning, TEXT("Item added to inventory: %s"), *Item->GetName());
		//CurrentInventorySlot = 2; //???
		CheckEmptyInventory(2);
		
	}
	else if (Item->Implements<USmokeGrenadeInterface>() && !InventorySlots.Contains(3))
	{
		//CheckEmptyInventory(3);
		InventorySlots.Add(3, Item);
		//CurrentItem = Item;
		//Item->Destroy();  //commented out because it needs adding to hands
		UE_LOG(LogTemp, Warning, TEXT("Item added to inventory: %s"), *Item->GetName());
		//CurrentInventorySlot = 3; //???
		CheckEmptyInventory(3);
	}
	else if (Item->Implements<UFlashbangInterface>() && !InventorySlots.Contains(4))
	{
		//CheckEmptyInventory(4);
		InventorySlots.Add(4, Item);
		//CurrentItem = Item;
		//Item->Destroy();  //commented out because it needs adding to hands
		UE_LOG(LogTemp, Warning, TEXT("Item added to inventory: %s"), *Item->GetName());
		//CurrentInventorySlot = 4; //???
		CheckEmptyInventory(4);
	}
	else if (Item->Implements<UKnifeInterface>() && !InventorySlots.Contains(5))
	{
		//CheckEmptyInventory(5);
		InventorySlots.Add(5, Item);
		//CurrentItem = Item;
		//Item->Destroy();  //commented out because it needs adding to hands
		UE_LOG(LogTemp, Warning, TEXT("Item added to inventory: %s"), *Item->GetName());
		//CurrentInventorySlot = 5; //???
		CheckEmptyInventory(5);
	}
	else
	{
		return false;
	}
	SortInventoryItems();
	
	//SetCurrentItemInHands();
	return true;
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
		UE_LOG(LogTemp, Warning, TEXT("Item thrown: %s"), *CurrentItem->GetName());
		InventorySlots.Remove(CurrentInventorySlot);
		UE_LOG(LogTemp, Warning, TEXT("Inventory slot empty: %d"), InventorySlots.IsEmpty());

		NextItem();
	}
}

void UInventory::UseItem(AInventoryItem* Item)
{
	// If item is consumable, remove from inventory
	if(Item && Item->bIsConsumable)
	{
		InventorySlots.Remove(CurrentInventorySlot);

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Item used! Items left: %d"), InventorySlots.Num()));

		if (NextItem())
		{
			if(AMyFPSCharacter* C = Cast<AMyFPSCharacter>(GetOuter()))
			{
				if (C->CurrentItemInHands)
					C->SpawnCurrentWeaponInHands();
			}
		}
	}
	
}

bool UInventory::NextItem()
{
	if(InventorySlots.Num() == 0)
	{
		if (CurrentItem)
			CurrentItem->Destroy();

		if (Controller)
			Controller->EquipWeapon(10);  //no weapon


		if (AMyFPSCharacter* C = Cast<AMyFPSCharacter>(GetOuter()))
		{
			if (C->HUD->CrosshairWidget->IsVisible())
				C->HUD->CrosshairWidget->SetVisibility(ESlateVisibility::Hidden);
		}
		return false;
	}
	if(InventorySlots.Num() == 1)
	{
		CurrentInventorySlot = InventorySlots.CreateConstIterator()->Key;
		
		SetCurrentItemInHands();
		return true;
	}
	if(InventorySlots.Num() > 1)
	{
		TArray<int> GunValues;
		InventorySlots.GenerateKeyArray(GunValues);
		int CurrentIndex = GunValues.Find(CurrentInventorySlot);
		CurrentIndex = (CurrentIndex + 1) % GunValues.Num();
		CurrentInventorySlot = GunValues[CurrentIndex];
		
		SetCurrentItemInHands();
		
		return true;
	}
	return false;
}

bool UInventory::PreviousItem()
{
	if(InventorySlots.Num() == 0)
	{
		if (CurrentItem)
			CurrentItem->Destroy();

		if (Controller)
			Controller->EquipWeapon(10);  //no weapon

		return false;
	}

	if(InventorySlots.Num() == 1)
	{
		CurrentInventorySlot = InventorySlots.CreateConstIterator()->Key;
	
		SetCurrentItemInHands();
		return true;
	}


	if(InventorySlots.Num() > 1)
	{
		TArray<int> GunValues;
		InventorySlots.GenerateKeyArray(GunValues);
		int CurrentIndex = GunValues.Find(CurrentInventorySlot);
		CurrentIndex = (CurrentIndex - 1 + GunValues.Num()) % GunValues.Num();
		CurrentInventorySlot = GunValues[CurrentIndex];
		
		SetCurrentItemInHands();
		
		return true;
	}

	return false;
}

void UInventory::SortInventoryItems()
{
	TArray<int> Keys;
	InventorySlots.GenerateKeyArray(Keys);
	Keys.Sort([](int A, int B) { return A < B; });
	TMap<int, AInventoryItem*> SortedInventoryItems;
	for (int Key : Keys)
	{
		SortedInventoryItems.Add(Key, InventorySlots[Key]);
	}
	InventorySlots = SortedInventoryItems;
}

void UInventory::CheckEmptyInventory(int Index)
{
	if (InventorySlots.Num() == 1)
	{
		CurrentInventorySlot = Index;
		SetCurrentItemInHands();
	}
}

void UInventory::SetCurrentItemInHands()
{
	if(AMyFPSCharacter* C = Cast<AMyFPSCharacter>(GetOuter()))
	{
		if(AFPSPlayerController* controller = Cast<AFPSPlayerController>(C->GetController()))
		{
			controller->AnimationIndex = CurrentInventorySlot;  // Animation is lined up with inventory slot
			controller->EquipWeapon(CurrentInventorySlot);  // Play the animation related to the weapon
			C->CurrentItemInHands = InventorySlots[CurrentInventorySlot];
			CurrentItem = InventorySlots[CurrentInventorySlot];
		}
	}
}

int UInventory::GetNumberOfItems()
{
	return InventorySlots.Num();
}
