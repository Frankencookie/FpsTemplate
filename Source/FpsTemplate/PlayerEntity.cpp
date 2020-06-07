// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerEntity.h"

APlayerEntity::APlayerEntity()
{
	MainCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Main Camera"));
	MainCamera->SetupAttachment(RootComponent);
	MainCamera->SetRelativeLocation(FVector(0, 0, 50.0f));
	MainCamera->bUsePawnControlRotation = true;

	ViewModel->SetupAttachment(MainCamera);
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

	//MovementBoi = GetCharacterMovement();
	//MovementBoi->MaxAcceleration = 4096;
}

//Forward
void APlayerEntity::MoveForward(float value)
{
	AddMovementInput(GetActorForwardVector(), value);
}

//Right
void APlayerEntity::MoveRight(float value)
{
	AddMovementInput(GetActorRightVector(), value);
}

/*
void APlayerEntity::Shoot()
{
	if (Magazine[CurrentWeaponInfo->WeaponType] > 0)
	{
		ShootRaycast();
		PlayEffects();
		Magazine[CurrentWeaponInfo->WeaponType] = Magazine[CurrentWeaponInfo->WeaponType] - 1;
	}
}
*/

void APlayerEntity::ADS()
{
	Aiming = true;
	SwayBase = 0.1;
	SwayMultiplier = 10;
	RotMultiplier = 1;
	LookSensitivity = AdsSensitivity;
}

void APlayerEntity::UnADS()
{
	Aiming = false;
	SwayBase = 0.7;
	SwayMultiplier = 3;
	RotMultiplier = 3;
	LookSensitivity = 1;
}

void APlayerEntity::LookUp(float Value)
{
	AddControllerPitchInput(Value * LookSensitivity);
}

void APlayerEntity::LookRight(float Value)
{
	AddControllerYawInput(Value * LookSensitivity);
}

void APlayerEntity::SwapWeaponUp()
{

}

void APlayerEntity::SwapWeaponDown()
{

}

void APlayerEntity::SwapWeaponNumber(float value)
{

}



// Called every frame
void APlayerEntity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float WalkDropValue = -GetVelocity().GetAbs().Size() / 50;

	//Dont do anything if data has not been assigned
	if (!CurrentWeapon)
	{
		GLog->Log(ELogVerbosity::Error, "NO DATA");
		return;
	}

	//Throw an error if Curve has not been set
	if (CurrentWeaponInfo->IdleSwayCurve == NULL)
	{
		GLog->Log(ELogVerbosity::Error, "NO CURVE DATA");
		return;
	}

	float CurveFinalTime;
	float CurveStartTime;
	CurrentWeaponInfo->IdleSwayCurve->GetTimeRange(CurveStartTime, CurveFinalTime);

	AnimTime += (DeltaTime + DeltaTime * -WalkDropValue / 5);

	if (AnimTime > CurveFinalTime)
	{
		AnimTime = CurveStartTime;
	}

	FVector SwayValue = CurrentWeaponInfo->IdleSwayCurve->GetVectorValue(AnimTime);
	SwayValue.Z *= (1 + -WalkDropValue / 15);

	FVector WeaponOffsetTarget = CurrentWeaponInfo->WeaponOffset;

	//If aiming, move weapon to aim position and reduce sway
	if (!Aiming)
	{
		WeaponOffsetTarget.Z += WalkDropValue / 2;
		HorizontalOffsetMultiplier = 2;
	}
	else
	{
		WeaponOffsetTarget = FVector(CurrentWeaponInfo->WeaponOffset.X, 0, 0);
		HorizontalOffsetMultiplier = 0;
	}

	//Add Side Input
	WeaponOffsetTarget.Y += GetInputAxisValue("MoveRight") * HorizontalOffsetMultiplier;

	WeaponOffsetFinal = CurrentWeaponInfo->WeaponOffset;
	WeaponOffsetBlend = FMath::VInterpTo(WeaponOffsetBlend, WeaponOffsetTarget, DeltaTime, OffsetSpeed);

	//Final Offset Value Addition
	WeaponOffsetFinal = WeaponOffsetBlend;
	WeaponOffsetFinal += SwayValue * (SwayBase + -WalkDropValue / SwayMultiplier);

	//Recoil

	//Set Position
	ViewModel->SetRelativeLocation(WeaponOffsetFinal);

	//Rotation
	FRotator RotationOffsetTarget = FRotator(0, 0, 0);
	RotationOffsetTarget.Pitch = -GetInputAxisValue("LookUp") * RotMultiplier;
	RotationOffsetTarget.Yaw = GetInputAxisValue("LookRight") * RotMultiplier;
	RotationOffset = FMath::RInterpTo(RotationOffset, RotationOffsetTarget, DeltaTime, RotInterpSpeed);

	ViewModel->SetRelativeRotation(RotationOffset);
}

void APlayerEntity::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Directional Movement
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerEntity::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerEntity::MoveRight);

	//Mouse Look
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerEntity::LookUp);
	PlayerInputComponent->BindAxis("LookRight", this, &APlayerEntity::LookRight);

	//Jump
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	//Crouch
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AGameEntity::StartCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AGameEntity::EndCrouch);

	//Shoot
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &AGameEntity::Shoot);
	PlayerInputComponent->BindAction("Shoot", IE_Released, this, &AGameEntity::UnShoot);

	//Aim and release aim
	PlayerInputComponent->BindAction("ADS", IE_Pressed, this, &APlayerEntity::ADS);
	PlayerInputComponent->BindAction("ADS", IE_Released, this, &APlayerEntity::UnADS);

	//Swap Weapon
	PlayerInputComponent->BindAction("SwapWeaponUp", IE_Pressed, this, &APlayerEntity::SwapWeaponUp);
	PlayerInputComponent->BindAction("SwapWeaponDown", IE_Pressed, this, &APlayerEntity::SwapWeaponDown);

	PlayerInputComponent->BindAxis("SwapWeaponValue", this, &APlayerEntity::SwapWeaponNumber);
}
