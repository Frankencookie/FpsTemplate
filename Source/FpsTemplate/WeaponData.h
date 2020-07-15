// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/StaticMesh.h"
#include "Engine/SkeletalMesh.h"
#include "Curves/CurveVector.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundBase.h"
#include "Camera/CameraShake.h"
#include "WeaponData.generated.h"


UENUM(Blueprintable)
enum EAmmoType
{
	A_NONE		UMETA(DisplayName = "NONE"),
	A_9MM		UMETA(DisplayName = "9mm"),
	A_45ACP		UMETA(DisplayName = ".45 acp"),
	A_303		UMETA(DisplayName = ".303 British"),

	A_LENGTH
};

UENUM(Blueprintable)
enum EWeaponType
{
	W_NONE		UMETA(DisplayName = "NONE"),
	W_LUGER		UMETA(DisplayName = "Luger"),
	W_COLT		UMETA(DisplayName = "Colt M1911"),
	W_THOMPSON	UMETA(DisplayName = "Thompson M1A1"),
	W_ENFIELD	UMETA(DisplayName = "Lee Enfield"),

	W_LENGTH
};

UENUM(Blueprintable)
enum EWeaponCategory
{
	C_NONE		UMETA(DisplayName = "NONE"),
	C_PISTOL	UMETA(DisplayName = "Pistol"),
	C_RIFLE		UMETA(DisplayName = "Rifle"),
	C_SMG		UMETA(DisplayName = "Submachinegun"),

	C_LENGTH
};

UCLASS(Blueprintable)
class FPSTEMPLATE_API UWeaponData : public UObject
{
	GENERATED_BODY()
	
public:
	//BASICS
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TEnumAsByte<EWeaponType> WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TEnumAsByte<EWeaponCategory> WeaponCategory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TEnumAsByte<EAmmoType> AmmoType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString WeaponName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int MagSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Auto = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Cooldown;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMesh* WeaponModel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USkeletalMesh* SkeletalModel;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector WeaponOffset = FVector(26, 15, -5);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Range = 6000;


	//RECOIL

	//How much the weapon should rotate Horizontally
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
		float HorizontalRecoil;

	//How much the weapon should rotate Vertically
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
		float VerticalRecoil;

	//How far the weapon should move back upon firing
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
		float BackRecoil;

	//How far the weapon should move up upon firing
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
		float UpRecoil;

	//How quickly the weapon should recoil, and recover from recoil
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
		float RecoilSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
		TSubclassOf<UCameraShake> CameraShake;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCurveVector* IdleSwayCurve;



	//This will use socket locations from static mesh instead of skeletal
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle Systems")
		bool UseStaticSocket = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle Systems")
		UParticleSystem* ShellEjectParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle Systems")
		UParticleSystem* MuzzleFlashParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
		USoundBase* GunshotSound;

	//ANIMATION
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		UAnimationAsset* ReloadAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		float ReloadValuesTime = 1;

	
};