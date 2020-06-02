// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/StaticMesh.h"
#include "WeaponData.h"
#include "FpsGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UFpsGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	
public:
	UPROPERTY(EditAnywhere)
	TArray<UWeaponData*> WeaponList;
	
};
