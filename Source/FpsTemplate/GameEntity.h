// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WeaponData.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "FpsGameInstance.h"
#include "Components/AudioComponent.h"
#include "ShootableObject.h"
#include "FpsTemplateGameModeBase.h"
#include "GameEntity.generated.h"

UCLASS()
class FPSTEMPLATE_API AGameEntity : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGameEntity();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	virtual void ShootRaycast();
	virtual void PlayEffects();
	virtual void Fire();
	UFUNCTION(BlueprintCallable)
	virtual void ReloadValues();

	bool Aiming = false;
	bool Firing = false;
	bool Reloading = false;

	float ReloadDoneTime = 0;
	float NextShotTime = 0;
	float CurrentShotTime = 0;
	FRotator RecoilRotationTarget = FRotator(0, 0, 0);
	FVector RecoilXTarget = FVector(0, 0, 0);
	FVector RecoilZTarget = FVector(0, 0, 0);
	FVector RecoilZBlend = FVector(0, 0, 0);
	float RecoilMultiplier = 3;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LookSensitivity = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AdsSensitivity = 0.5f;


	AFpsTemplateGameModeBase* GameMode;
	EWeaponAmmoMode AmmoMode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int> AmmoPool;
	//int AmmoPool[A_LENGTH];
	int Magazine[C_LENGTH];
	//bool WeaponInventory[W_LENGTH];
	//TArray<TSubclassOf<UWeaponData>> WeaponInventory;

	//Stats
	int health = 100;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void StartCrouch();
	virtual void EndCrouch();
	virtual void Shoot();
	virtual void UnShoot();
	virtual void EquipWeapon(int numberToLoad);

	UFUNCTION(BlueprintCallable)
	virtual void PickupAmmo(EWeaponType AmmoType, int Amount);

	UFUNCTION(BlueprintCallable)
	int GetCurrentAmmo();

	UFUNCTION(BlueprintCallable)
	int GetReserveAmmo();

	UFUNCTION(BlueprintCallable)
	FString GetWeaponName();

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//TSubclassOf<UWeaponData> CurrentWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EWeaponCategory> CurrentWeaponCategory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UWeaponData> CurrentWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<UWeaponData>> WeaponInventory;

	UWeaponData* CurrentWeaponInfo;

	//Static Mesh Viewmodel. Could be used for world model
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* ViewModel;

	//Skeletal Mesh Viewmodel
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* SkeletalViewModel;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystemComponent* ShellParticleBoi;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystemComponent* MuzzleParticleBoi;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAudioComponent* AudioSource;

	void DamageEntity(int amount);
};
