// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerEntity.h"

APlayerEntity::APlayerEntity()
{
	MainCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Main Camera"));
	MainCamera->SetupAttachment(RootComponent);
	MainCamera->SetRelativeLocation(FVector(0, 0, 50.0f));
	MainCamera->bUsePawnControlRotation = true;

	ViewModel->SetupAttachment(MainCamera);

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

void APlayerEntity::Shoot()
{

}

void APlayerEntity::ADS()
{
	Aiming = true;
}

void APlayerEntity::UnADS()
{
	Aiming = false;
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
		RotMultiplier = 3;
		SwayBase = 0.7;
		SwayMultiplier = 3;
		HorizontalOffsetMultiplier = 2;
	}
	else
	{
		WeaponOffsetTarget = FVector(CurrentWeaponInfo->WeaponOffset.X, 0, 0);
		RotMultiplier = 1;
		SwayBase = 0.1;
		SwayMultiplier = 10;
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
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookRight", this, &APawn::AddControllerYawInput);

	//Jump
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	//Shoot
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &APlayerEntity::Shoot);

	//Aim and release aim
	PlayerInputComponent->BindAction("ADS", IE_Pressed, this, &APlayerEntity::ADS);
	PlayerInputComponent->BindAction("ADS", IE_Released, this, &APlayerEntity::UnADS);

	//Swap Weapon
	PlayerInputComponent->BindAction("SwapWeaponUp", IE_Pressed, this, &APlayerEntity::SwapWeaponUp);
	PlayerInputComponent->BindAction("SwapWeaponDown", IE_Pressed, this, &APlayerEntity::SwapWeaponDown);

	PlayerInputComponent->BindAxis("SwapWeaponValue", this, &APlayerEntity::SwapWeaponNumber);
}
