// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FpsTemplateGameModeBase.generated.h"


UENUM(Blueprintable)
enum EWeaponAmmoMode
{
	WAM_WEAPONEXCLUSIVE		UMETA(DisplayName = "Weapon Exclusive Ammo"),
	WAM_CALIBRE				UMETA(DisplayName = "Weapon Ammo By Calibre"),
	WAM_CATEGORY			UMETA(DisplayName = "Weapon Ammo By Category")
};

UCLASS()
class FPSTEMPLATE_API AFpsTemplateGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Modes")
	TEnumAsByte<EWeaponAmmoMode> WeaponAmmoMode = WAM_WEAPONEXCLUSIVE;
	
	UFUNCTION(BlueprintCallable)
	EWeaponAmmoMode GetWeaponAmmoMode() { return WeaponAmmoMode; }
};
