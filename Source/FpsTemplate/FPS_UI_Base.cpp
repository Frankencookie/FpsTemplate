// Fill out your copyright notice in the Description page of Project Settings.

#include "FPS_UI_Base.h"
#include "Components/TextBlock.h"
#include "Internationalization/Text.h"

void UFPS_UI_Base::SetCurrentAmmo(int newAmmo)
{
	CurrentAmmo = newAmmo;
	FString toPrint = FString::FromInt(newAmmo);

	if (CurrentAmmoText)
	{
		CurrentAmmoText->SetText(FText::FromString(toPrint));
	}

}

void UFPS_UI_Base::SetAmmoReserve(int newReserve)
{
	AmmoReserve = newReserve;
	if (ReserveAmmoText)
	{
		FString toPrint = FString::FromInt(AmmoReserve);
		ReserveAmmoText->SetText(FText::FromString(toPrint));
	}
	else
	{
		GLog->Log("No Text");
	}
}

void UFPS_UI_Base::SetWeaponName(FString newName)
{
	WeaponName = newName;

	if (WeaponNameText)
	{
		WeaponNameText->SetText(FText::FromString(WeaponName));
	}
}

void UFPS_UI_Base::NativeConstruct()
{
	Super::NativeConstruct();

	if (CurrentAmmoText)
	{
		FString toPrint = FString::FromInt(CurrentAmmo);
		CurrentAmmoText->SetText(FText::FromString(toPrint));
	}
	if (ReserveAmmoText)
	{
		FString toPrint = FString::FromInt(AmmoReserve);
		CurrentAmmoText->SetText(FText::FromString(toPrint));
	}
	if (WeaponNameText)
	{
		WeaponNameText->SetText(FText::FromString(WeaponName));
	}
}
