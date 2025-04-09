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

	//TODO: Add more item types
	//TODO: Add more inventory slots
	//TODO: If weapon is dropped and another one equipped, make it the current item and swap to it

	if (!Item)
	{
		UE_LOG(LogTemp, Error, TEXT("Trying to add a NULL item to inventory!"));
		return false;
	}

	if(Item->Implements<UPrimaryWeapon>() && !InventorySlots.Contains(0) /*&& !InventoryItems.Contains(Item)*/)
	{
		//CheckEmptyInventory(0);
		InventorySlots.Add(0, Item);
		//InventoryItems.Add(Item);
		//CurrentItem = Item;
		//Item->Destroy();  //commented out because it needs adding to hands
		UE_LOG(LogTemp, Warning, TEXT("Item added to inventory: %s"), *Item->GetName());
		//CurrentInventorySlot = 0; //???
		CheckEmptyInventory(0);
		
		
		// if(AMyFPSCharacter* C = Cast<AMyFPSCharacter>(GetOuter()))
		// {
		// 	//C->AnimationInstance->AnimationIndex = CurrentInventorySlot;
		// 	if(AFPSPlayerController* controller = Cast<AFPSPlayerController>(C->GetController()))
		// 	{
		// 		controller->AnimationIndex = CurrentInventorySlot;  //Animation is lined up with inventory slot
		// 		controller->EquipWeapon(CurrentInventorySlot);  //Play the animation related to the weapon
		// 		C->CurrentItemInHands = InventorySlots[CurrentInventorySlot];
		// 		SortInventoryItems();
		// 		
		// 	}
		// }
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
		//CurrentItem->Destroy();
		//InventorySlots[CurrentInventorySlot] = nullptr;
		UE_LOG(LogTemp, Warning, TEXT("Item thrown: %s"), *CurrentItem->GetName());
		InventorySlots.Remove(CurrentInventorySlot);
		UE_LOG(LogTemp, Warning, TEXT("Inventory slot empty: %d"), InventorySlots.IsEmpty());
		//CurrentItem = nullptr;
	

		//if(GetNumberOfItems() > 0)
			NextItem();
		// else
		// {
		// 	CurrentItem->Destroy();
		// 	CurrentItem = nullptr;
		// 	//CurrentInventorySlot = NULL;
		// 	
		// 	if(AMyFPSCharacter* C = Cast<AMyFPSCharacter>(GetOuter()))
		// 	{
		// 		//C->AnimationInstance->AnimationIndex = CurrentInventorySlot;
		// 		if(AFPSPlayerController* controller = Cast<AFPSPlayerController>(C->GetController()))
		// 		{
		// 			controller->AnimationIndex = 10;  //Animation is lined up with inventory slot
		// 			controller->EquipWeapon(controller->AnimationIndex);  //Play the animation related to the weapon
		// 			
		// 			//C->CurrentItemInHands = InventorySlots[CurrentInventorySlot];
		// 		}
		// 	}
		// }
		// if(CurrentItem)
		// 	UE_LOG(LogTemp, Warning, TEXT("Current item: %s"), *CurrentItem->GetName());
	}
}

void UInventory::UseItem(AInventoryItem* Item)
{
	//Item->Use();

	// If item is consumable, remove from inventory
	if(Item->bIsConsumable)
	{
		InventorySlots.Remove(CurrentInventorySlot);
		//if(AMyFPSCharacter* C = Cast<AMyFPSCharacter>(GetOuter()))
		//{
		//	C->NextWeapon();
		//}
		//NextItem();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Item used! Items left: %d"), InventorySlots.Num()));

		NextItem();
		
		
		//if(AMyFPSCharacter* C = Cast<AMyFPSCharacter>(GetOuter()))
		//{
			//if (C->CurrentItemInHands)
			//	C->CurrentItemInHands->Destroy();
		
			//if (InventorySlots.Num() == 1 && !NextItem())
			//{
			//	//C->SpawnCurrentWeaponInHands();
			//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Next item equipped!"));
			//	//return;
			//}
			//else
			//	NextItem();
			//if (NextItem())
			//{
			//	
			//}
		//}	
	}
	
}

bool UInventory::NextItem()
{
	if(InventorySlots.Num() == 0)
	{
		//CurrentItem = nullptr;
		if (CurrentItem)
			CurrentItem->Destroy();

		if (Controller)
			Controller->EquipWeapon(10);  //no weapon


		if (AMyFPSCharacter* C = Cast<AMyFPSCharacter>(GetOuter()))
		{
			if (C->HUD->CrosshairWidget->IsVisible())
				C->HUD->CrosshairWidget->SetVisibility(ESlateVisibility::Hidden);
		}
		// if (AMyFPSCharacter* C = Cast<AMyFPSCharacter>(Controller->GetCharacter()))
		// {
		// 	C->CurrentItemInHands->Destroy();
		// 	C->CurrentItemInHands = nullptr;
		// 	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No weapons in inventory!"));
		// }
		//
		// CurrentInventorySlot = 10;
		
		return false;
	}
	if(InventorySlots.Num() == 1)
	{
		CurrentInventorySlot = InventorySlots.CreateConstIterator()->Key;
		//CurrentItem = InventorySlots[CurrentInventorySlot];
		SetCurrentItemInHands();
		// if (AMyFPSCharacter* C = Cast<AMyFPSCharacter>(Controller->GetCharacter()))
		// {
		// 	C->SpawnCurrentWeaponInHands();
		// }
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
		//CurrentItem = nullptr;
		if (CurrentItem)
			CurrentItem->Destroy();

		if (Controller)
			Controller->EquipWeapon(10);  //no weapon

		return false;
	}

	if(InventorySlots.Num() == 1)
	{
		CurrentInventorySlot = InventorySlots.CreateConstIterator()->Key;
		//CurrentItem = InventorySlots[CurrentInventorySlot];
		
		SetCurrentItemInHands();
		return false;
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
	//else if (InventorySlots.Num() == 1)
	//{
		//SetCurrentItemInHands();
	//}
}

void UInventory::SetCurrentItemInHands()
{
	if(AMyFPSCharacter* C = Cast<AMyFPSCharacter>(GetOuter()))
	{
		//C->AnimationInstance->AnimationIndex = CurrentInventorySlot;
		if(AFPSPlayerController* controller = Cast<AFPSPlayerController>(C->GetController()))
		{
			//if (InventorySlots.Contains(CurrentInventorySlot))
			//{
				controller->AnimationIndex = CurrentInventorySlot;  // Animation is lined up with inventory slot
				controller->EquipWeapon(CurrentInventorySlot);  // Play the animation related to the weapon
				C->CurrentItemInHands = InventorySlots[CurrentInventorySlot];
				CurrentItem = InventorySlots[CurrentInventorySlot];


				//C->SpawnCurrentWeaponInHands();
				//SortInventoryItems();
			// }
			// else
			// {
			// 	UE_LOG(LogTemp, Error, TEXT("Invalid inventory slot: %d"), CurrentInventorySlot);
			// 	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Invalid inventory slot!"));
			// }
				
		}
	}
}

int UInventory::GetNumberOfItems()
{
	return InventorySlots.Num();
}
