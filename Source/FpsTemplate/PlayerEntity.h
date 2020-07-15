// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameEntity.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "FPS_UI_Base.h"
#include "PlayerEntity.generated.h"

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API APlayerEntity : public AGameEntity
{
	GENERATED_BODY()
	
public:
	APlayerEntity();
	
protected:

	virtual void BeginPlay() override;
	UCharacterMovementComponent* MovementBoi;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UFPS_UI_Base> PlayerUI;

	UFPS_UI_Base* PlayerUIRef;

	void CreateUI();

	void MoveForward(float value);
	void MoveRight(float value);

	//virtual void Shoot() override;
	virtual void Fire() override;
	void ADS();
	void UnADS();
	void LookUp(float Value);
	void LookRight(float Value);

	void SwapWeaponUp();
	void SwapWeaponDown();
	void SwapWeaponNumber();

	void Reload();

	//Weapon switch value getter
	void hehehhe(float value);

	UCameraComponent* MainCamera;

	//Smooth Position blend
	FVector WeaponOffsetBlend = FVector(0, 0, 0);
	FVector WeaponOffsetFinal = FVector(0, 0, 0);
	float AnimTime = 0;
	float OffsetSpeed = 10;
	float SwayBase = 1;
	float SwayMultiplier = 3;

	float HorizontalOffsetMultiplier = 3;

	//Recoil
	//FVector RecoilClimbTarget = FVector(0, 0, 0);
	//FRotator RecoilRotationTarget = FRotator(0, 0, 0);
	float RecoilSpeed = 15;

	//Smooth Rotation
	FRotator RotationOffset = FRotator(0, 0, 0);
	float RotInterpSpeed = 5;
	float RotMultiplier = 3;

	UFUNCTION(BlueprintCallable)
	virtual void ReloadValues();

	UPROPERTY(EditAnywhere)
	float JumpWeaponOffset = 8;

public:

	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic, Category = "Super Important UI Bois")
	void UpdateUI_Fire(int newAmmo);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic, Category = "Super Important UI Bois")
	void UpdateUI_Reload(int newAmmo, int newReserve);

	UFUNCTION(BlueprintCallable)
	virtual void PickupAmmo(EWeaponType AmmoType, int Amount);
};
