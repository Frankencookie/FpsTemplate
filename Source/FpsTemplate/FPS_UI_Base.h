// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FPS_UI_Base.generated.h"

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UFPS_UI_Base : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic, Category = "Super Important Bois")
	void UpdateUI(int currentAmmo, int reserveAmmo);

	UFUNCTION(BlueprintCallable)
	void SetCurrentAmmo(int newAmmo);
	UFUNCTION(BlueprintCallable)
	void SetAmmoReserve(int newReserve);
	UFUNCTION(BlueprintCallable)
	void SetWeaponName(FString newName);

	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* CurrentAmmoText = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* ReserveAmmoText = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* WeaponNameText = nullptr;

	
	
protected:
	UPROPERTY(BlueprintReadWrite)
	int CurrentAmmo = 0;
	UPROPERTY(BlueprintReadOnly)
	int AmmoReserve = 0;
	UPROPERTY(BlueprintReadWrite)
	FString WeaponName = "Sample Text";
	
};
