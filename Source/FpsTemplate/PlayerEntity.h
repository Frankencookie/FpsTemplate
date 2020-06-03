// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameEntity.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
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
	UCharacterMovementComponent* MovementBoi;

	void MoveForward(float value);
	void MoveRight(float value);

	virtual void Shoot() override;
	void ADS();
	void UnADS();

	void SwapWeaponUp();
	void SwapWeaponDown();
	void SwapWeaponNumber(float value);

	UCameraComponent* MainCamera;

	//Smooth Position blend
	FVector WeaponOffsetBlend = FVector(0, 0, 0);
	FVector WeaponOffsetFinal = FVector(0, 0, 0);
	float AnimTime = 0;
	float OffsetSpeed = 10;
	float SwayBase = 1;
	float SwayMultiplier = 3;

	float HorizontalOffsetMultiplier = 2;

	//Recoil
	FVector RecoilClimbTarget = FVector(0, 0, 0);
	FRotator RecoilRotationTarget = FRotator(0, 0, 0);

	//Smooth Rotation
	FRotator RotationOffset = FRotator(0, 0, 0);
	float RotInterpSpeed = 5;
	float RotMultiplier = 3;

public:

	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
