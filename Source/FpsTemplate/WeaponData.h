// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/StaticMesh.h"
#include "Curves/CurveVector.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundBase.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TEnumAsByte<EWeaponType> WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TEnumAsByte<EWeaponCategory> WeaponCategory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TEnumAsByte<EAmmoType> AmmoType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int MagSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Auto = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Cooldown;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMesh* WeaponModel;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector WeaponOffset = FVector(26, 15, -5);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Range = 6000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCurveVector* IdleSwayCurve;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle Systems")
		UParticleSystem* ShellEjectParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle Systems")
		UParticleSystem* MuzzleFlashParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USoundBase* GunshotSound;
	
};