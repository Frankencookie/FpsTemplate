// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerEntity.h"

APlayerEntity::APlayerEntity()
{
	MainCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Main Camera"));
	MainCamera->SetupAttachment(RootComponent);
	MainCamera->SetRelativeLocation(FVector(0, 0, 50.0f));
	MainCamera->bUsePawnControlRotation = true;

	ViewModel->SetupAttachment(MainCamera);
	//SkeletalViewModel->SetupAttachment(MainCamera);
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

	//MovementBoi = GetCharacterMovement();
	//MovementBoi->MaxAcceleration = 4096;
	SkeletalViewModel->SetRelativeLocation(FVector(0, 0, 0));
}

void APlayerEntity::BeginPlay()
{
	Super::BeginPlay();
	CreateUI();
}

void APlayerEntity::CreateUI()
{

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


void APlayerEntity::Fire()
{
	Super::Fire();
	UpdateUI_Fire(Magazine[CurrentWeaponInfo->WeaponCategory]);
}

void APlayerEntity::ADS()
{
	Aiming = true;
	SwayBase = 0.1;
	SwayMultiplier = 10;
	RotMultiplier = 1;
	LookSensitivity = AdsSensitivity;
	RecoilMultiplier = 1;
}

void APlayerEntity::UnADS()
{
	Aiming = false;
	SwayBase = 0.7;
	SwayMultiplier = 3;
	RotMultiplier = 3;
	LookSensitivity = 1;
	RecoilMultiplier = 3;
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
	int currentWeapon = CurrentWeaponInfo->WeaponType;
}

void APlayerEntity::SwapWeaponDown()
{

}

void APlayerEntity::SwapWeaponNumber()
{
	int weaponSwitch = GetInputAxisValue("SwapWeaponValue");
	if (weaponSwitch < WeaponInventory.Num())
	{
		if (WeaponInventory[weaponSwitch] != NULL)
		{
			EquipWeapon(weaponSwitch);
			UpdateUI_Reload(Magazine[CurrentWeaponInfo->WeaponCategory], AmmoPool[CurrentWeaponInfo->WeaponType]);
			return;
		}
	}

}

void APlayerEntity::Reload()
{
	if (Reloading)
	{
		GLog->Log("Already Reloading");
		return;
	}
	if (AmmoPool[CurrentWeaponInfo->WeaponType] < 1)
	{
		GLog->Log("Cannot Reload, Not enough Ammo");
		return;
	}
	else
	{
		GLog->Log("Reloading");
		Reloading = true;
		SkeletalViewModel->PlayAnimation(CurrentWeaponInfo->ReloadAnimation, false);
		ReloadDoneTime = CurrentShotTime + CurrentWeaponInfo->ReloadValuesTime;
		UpdateUI_Reload(Magazine[CurrentWeaponInfo->WeaponCategory], AmmoPool[CurrentWeaponInfo->WeaponType]);
		//ReloadValues();
		return;
	}

}

void APlayerEntity::hehehhe(float value)
{
}


void APlayerEntity::ReloadValues()
{
	Super::ReloadValues();

	UpdateUI_Reload(Magazine[CurrentWeaponInfo->WeaponCategory], AmmoPool[CurrentWeaponInfo->WeaponType]);
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

	//If aiming, move weapon to aim position
	if (!Aiming || Reloading)
	{
		WeaponOffsetTarget.Z += WalkDropValue / 2;
		HorizontalOffsetMultiplier = 2;
	}
	else
	{
		WeaponOffsetTarget = FVector(CurrentWeaponInfo->WeaponOffset.X, 0, 0);
		HorizontalOffsetMultiplier = 0;
	}

	//Jumping
	if (!GetCharacterMovement()->IsMovingOnGround())
	{
		WeaponOffsetTarget.Z += -JumpWeaponOffset;
		SwayValue.Z = 0;
		SwayValue.Y = 0;
	}

	//Add Side Input
	WeaponOffsetTarget.Y += GetInputAxisValue("MoveRight") * HorizontalOffsetMultiplier;

	WeaponOffsetFinal = CurrentWeaponInfo->WeaponOffset;
	WeaponOffsetBlend = FMath::VInterpTo(WeaponOffsetBlend, WeaponOffsetTarget, DeltaTime, OffsetSpeed);

	//Offset Value Addition
	WeaponOffsetFinal = WeaponOffsetBlend;
	WeaponOffsetFinal += SwayValue * (SwayBase + -WalkDropValue / SwayMultiplier);

	//Blend X axis recoil
	RecoilXTarget = FMath::VInterpTo(RecoilXTarget, FVector(0, 0, 0), DeltaTime, CurrentWeaponInfo->RecoilSpeed);
	//Recoil Z Blend
	RecoilZTarget = FMath::VInterpTo(RecoilZTarget, FVector(0, 0, 0), DeltaTime, CurrentWeaponInfo->RecoilSpeed / 4);
	RecoilZBlend = FMath::VInterpTo(RecoilZBlend, RecoilZTarget, DeltaTime, CurrentWeaponInfo->RecoilSpeed / 2);

	//Add to OffsetValue
	WeaponOffsetFinal += RecoilXTarget;
	WeaponOffsetFinal += RecoilZBlend;

	//Set Position
	ViewModel->SetRelativeLocation(WeaponOffsetFinal);
	//SkeletalViewModel->SetRelativeLocation(WeaponOffsetFinal);

	//Rotation
	FRotator RotationOffsetTarget = FRotator(0, 0, 0);
	RotationOffsetTarget.Pitch = -GetInputAxisValue("LookUp") * RotMultiplier;
	RotationOffsetTarget.Yaw = GetInputAxisValue("LookRight") * RotMultiplier;
	RotationOffset = FMath::RInterpTo(RotationOffset, RotationOffsetTarget, DeltaTime, RotInterpSpeed);

	//Recoil Rotation
	RecoilRotationTarget = FMath::RInterpTo(RecoilRotationTarget, FRotator(0, 0, 0), DeltaTime, CurrentWeaponInfo->RecoilSpeed);
	FRotator RecoilRotBlend = FRotator(0, 0, 0);
	RecoilRotBlend = FMath::RInterpTo(RecoilRotBlend, RecoilRotationTarget, DeltaTime, CurrentWeaponInfo->RecoilSpeed);

	RotationOffset += RecoilRotBlend;

	//Set Rotation
	ViewModel->SetRelativeRotation(RotationOffset);
	//SkeletalViewModel->SetRelativeRotation(RotationOffset);
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

	PlayerInputComponent->BindAction("SwapWeaponKey", IE_Pressed, this, &APlayerEntity::SwapWeaponNumber);
	PlayerInputComponent->BindAxis("SwapWeaponValue", this, &APlayerEntity::hehehhe);

	//Reload
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &APlayerEntity::Reload);
}

void APlayerEntity::PickupAmmo(EWeaponType AmmoType, int Amount)
{
	Super::PickupAmmo(AmmoType, Amount);

	UpdateUI_Reload(Magazine[CurrentWeaponInfo->WeaponCategory], AmmoPool[CurrentWeaponInfo->WeaponType]);
}
