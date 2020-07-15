// Fill out your copyright notice in the Description page of Project Settings.

#include "GameEntity.h"
#include "DrawDebugHelpers.h"

// Sets default values
AGameEntity::AGameEntity()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ViewModel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("View Model"));
	//ViewModel->AddRelativeLocation(FVector(26, 15, 60));
	ViewModel->SetupAttachment(RootComponent);
	ViewModel->bCastDynamicShadow = false;
	ViewModel->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SkeletalViewModel = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Animated ViewModel"));
	SkeletalViewModel->SetupAttachment(ViewModel);
	SkeletalViewModel->SetRelativeLocation(FVector(0, 0, 0));
	SkeletalViewModel->bCastDynamicShadow = false;
	SkeletalViewModel->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SkeletalViewModel->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	SkeletalViewModel->AnimationData.bSavedLooping = false;
	SkeletalViewModel->AnimationData.bSavedPlaying = true;

	//Shell eject particle system
	ShellParticleBoi = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Shell Eject Particle System"));
	ShellParticleBoi->SetupAttachment(ViewModel);
	ShellParticleBoi->bAutoActivate = false;

	//Muzzle flash particle system
	MuzzleParticleBoi = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Muzzle Flash Particle System"));
	MuzzleParticleBoi->SetupAttachment(ViewModel);
	MuzzleParticleBoi->bAutoActivate = false;

	//Sound source
	AudioSource = CreateDefaultSubobject<UAudioComponent>("Audio Source");
	AudioSource->SetupAttachment(ViewModel);
	AudioSource->bAutoActivate = false;
	AudioSource->bOverrideAttenuation = true;

	//Weapon Inventory Init
	WeaponInventory.SetNum(C_LENGTH);

}

// Called when the game starts or when spawned
void AGameEntity::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<AFpsTemplateGameModeBase>(GetWorld()->GetAuthGameMode());
	AmmoMode = GameMode->GetWeaponAmmoMode();
	if (!GameMode)
	{
		GLog->Log("Gamemode not found, please cry");
	}
	else
	{
		GLog->Log("Gamemode found");

		//Set ammo array to size of ammo mode
		switch (AmmoMode)
		{
		case WAM_CALIBRE:
			AmmoPool.SetNum(A_LENGTH);
			break;
		case WAM_CATEGORY:
			AmmoPool.SetNum(C_LENGTH);
			break;
		case WAM_WEAPONEXCLUSIVE:
			AmmoPool.SetNum(W_LENGTH);
			for (int i = 0; i < W_LENGTH; i++)
			{
				AmmoPool[i] = 30;
			}
			break;
		}

		GLog->Log(FString::FromInt(AmmoPool.Num()));
	}

}

void AGameEntity::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (CurrentWeapon)
	{

		CurrentWeaponInfo = CurrentWeapon.GetDefaultObject();
		if (CurrentWeaponInfo->SkeletalModel)
		{
			SkeletalViewModel->SetSkeletalMesh(CurrentWeaponInfo->SkeletalModel);
		}

		ViewModel->SetStaticMesh(CurrentWeaponInfo->WeaponModel);
		ViewModel->SetRelativeLocation(CurrentWeaponInfo->WeaponOffset);



		//Weapon Data Set
		//WeaponInventory[CurrentWeaponInfo->WeaponType] = ;
		//Magazine[CurrentWeaponInfo->WeaponType] = CurrentWeaponInfo->MagSize;
		for (int i = 0; i < EWeaponCategory::C_LENGTH; i++)
		{
			if (WeaponInventory[i] != NULL)
			{
				Magazine[i] = WeaponInventory[i].GetDefaultObject()->MagSize;
				GLog->Log("E");
			}
		}
		
		if (CurrentWeaponInfo->ShellEjectParticle)
		{
			//Skeletal or static, skeletal priority
			if (SkeletalViewModel->SkeletalMesh && !CurrentWeaponInfo->UseStaticSocket)
			{
				if (SkeletalViewModel->DoesSocketExist("ShellParticle"))
				{
					ShellParticleBoi->SetTemplate(CurrentWeaponInfo->ShellEjectParticle);
					ShellParticleBoi->SetWorldLocation(SkeletalViewModel->GetSocketLocation("ShellParticle"));
					ShellParticleBoi->SetWorldRotation(SkeletalViewModel->GetSocketRotation("ShellParticle"));
					ShellParticleBoi->SetActive(false);
					GLog->Log("SKELETAL - Shell Socket Found");
				}
			}
			else if (ViewModel->DoesSocketExist("ShellParticle"))
			{
				ShellParticleBoi->SetTemplate(CurrentWeaponInfo->ShellEjectParticle);
				ShellParticleBoi->SetWorldLocation(ViewModel->GetSocketLocation("ShellParticle"));
				ShellParticleBoi->SetWorldRotation(ViewModel->GetSocketRotation("ShellParticle"));
				ShellParticleBoi->SetActive(false);
				GLog->Log("STATIC - Shell Socket Found");
			}
		}
		if (CurrentWeaponInfo->MuzzleFlashParticle)
		{
			//Skeletal or static, skeletal priority
			if (SkeletalViewModel->SkeletalMesh && !CurrentWeaponInfo->UseStaticSocket)
			{
				if (SkeletalViewModel->DoesSocketExist("Muzzle"))
				{
					MuzzleParticleBoi->SetTemplate(CurrentWeaponInfo->MuzzleFlashParticle);
					MuzzleParticleBoi->SetWorldLocation(SkeletalViewModel->GetSocketLocation("Muzzle"));
					MuzzleParticleBoi->SetWorldRotation(SkeletalViewModel->GetSocketRotation("Muzzle"));
					MuzzleParticleBoi->SetActive(false);
					GLog->Log("SKELETAL - Muzzle Socket Found");
				}
			}
			else if (ViewModel->DoesSocketExist("Muzzle"))
			{
				MuzzleParticleBoi->SetTemplate(CurrentWeaponInfo->MuzzleFlashParticle);
				MuzzleParticleBoi->SetWorldLocation(ViewModel->GetSocketLocation("Muzzle"));
				MuzzleParticleBoi->SetWorldRotation(ViewModel->GetSocketRotation("Muzzle"));
				MuzzleParticleBoi->SetActive(false);
				GLog->Log("STATIC -	Muzzle Socket Found");
			}
		}

		if (SkeletalViewModel->SkeletalMesh && CurrentWeaponInfo->ReloadAnimation)
		{
			/*
			SkeletalViewModel->AnimationData.AnimToPlay = CurrentWeaponInfo->ReloadAnimation;
			SkeletalViewModel->AnimationData.bSavedPlaying = false;
			SkeletalViewModel->AnimationData.SavedPosition = 0;
			*/
			SkeletalViewModel->PlayAnimation(CurrentWeaponInfo->ReloadAnimation, false);
			SkeletalViewModel->Stop();
		}

		//Audio
		AudioSource->SetSound(CurrentWeaponInfo->GunshotSound);
		
		if (SkeletalViewModel->SkeletalMesh)
		{
			ViewModel->SetVisibility(false);
		}
	}

}

void AGameEntity::Shoot()
{
	Firing = true;
	if (!CurrentWeaponInfo->Auto)
	{
		Fire();
	}

}

void AGameEntity::UnShoot()
{
	Firing = false;
}

void AGameEntity::EquipWeapon(int numberToLoad)
{
	AudioSource->SetActive(false);
	CurrentWeaponInfo = WeaponInventory[numberToLoad].GetDefaultObject();
	if (CurrentWeaponInfo->SkeletalModel)
	{
		SkeletalViewModel->SetSkeletalMesh(CurrentWeaponInfo->SkeletalModel);
	}

	ViewModel->SetStaticMesh(CurrentWeaponInfo->WeaponModel);
	ViewModel->SetRelativeLocation(CurrentWeaponInfo->WeaponOffset);

	//Weapon Data Set
	//Magazine[CurrentWeaponInfo->WeaponType] = CurrentWeaponInfo->MagSize;

	if (CurrentWeaponInfo->ShellEjectParticle)
	{
		//Skeletal or static, skeletal priority
		if (SkeletalViewModel->SkeletalMesh && !CurrentWeaponInfo->UseStaticSocket)
		{
			if (SkeletalViewModel->DoesSocketExist("ShellParticle"))
			{
				ShellParticleBoi->SetTemplate(CurrentWeaponInfo->ShellEjectParticle);
				ShellParticleBoi->SetWorldLocation(SkeletalViewModel->GetSocketLocation("ShellParticle"));
				ShellParticleBoi->SetWorldRotation(SkeletalViewModel->GetSocketRotation("ShellParticle"));
				ShellParticleBoi->SetActive(false);
				GLog->Log("SKELETAL - Shell Socket Found");
			}
		}
		else if (ViewModel->DoesSocketExist("ShellParticle"))
		{
			ShellParticleBoi->SetTemplate(CurrentWeaponInfo->ShellEjectParticle);
			ShellParticleBoi->SetWorldLocation(ViewModel->GetSocketLocation("ShellParticle"));
			ShellParticleBoi->SetWorldRotation(ViewModel->GetSocketRotation("ShellParticle"));
			ShellParticleBoi->SetActive(false);
			GLog->Log("STATIC - Shell Socket Found");
		}
	}
	if (CurrentWeaponInfo->MuzzleFlashParticle)
	{
		//Skeletal or static, skeletal priority
		if (SkeletalViewModel->SkeletalMesh && !CurrentWeaponInfo->UseStaticSocket)
		{
			if (SkeletalViewModel->DoesSocketExist("Muzzle"))
			{
				MuzzleParticleBoi->SetTemplate(CurrentWeaponInfo->MuzzleFlashParticle);
				MuzzleParticleBoi->SetWorldLocation(SkeletalViewModel->GetSocketLocation("Muzzle"));
				MuzzleParticleBoi->SetWorldRotation(SkeletalViewModel->GetSocketRotation("Muzzle"));
				MuzzleParticleBoi->SetActive(false);
				GLog->Log("SKELETAL - Muzzle Socket Found");
			}
		}
		else if (ViewModel->DoesSocketExist("Muzzle"))
		{
			MuzzleParticleBoi->SetTemplate(CurrentWeaponInfo->MuzzleFlashParticle);
			MuzzleParticleBoi->SetWorldLocation(ViewModel->GetSocketLocation("Muzzle"));
			MuzzleParticleBoi->SetWorldRotation(ViewModel->GetSocketRotation("Muzzle"));
			MuzzleParticleBoi->SetActive(false);
			GLog->Log("STATIC -	Muzzle Socket Found");
		}
	}

	if (SkeletalViewModel->SkeletalMesh && CurrentWeaponInfo->ReloadAnimation)
	{
		SkeletalViewModel->AnimationData.AnimToPlay = CurrentWeaponInfo->ReloadAnimation;
	}

	//Audio
	AudioSource->SetSound(CurrentWeaponInfo->GunshotSound);

	if (SkeletalViewModel->SkeletalMesh)
	{
		ViewModel->SetStaticMesh(NULL);
	}
}

void AGameEntity::Fire()
{
	if (Magazine[CurrentWeaponInfo->WeaponCategory] > 0)
	{
		ShootRaycast();
		PlayEffects();
		Magazine[CurrentWeaponInfo->WeaponCategory] = Magazine[CurrentWeaponInfo->WeaponCategory] - 1;
		float RecoilUP = FMath::FRandRange(CurrentWeaponInfo->VerticalRecoil / 2, CurrentWeaponInfo->VerticalRecoil) * RecoilMultiplier;
		float RecoilSIDE = FMath::FRandRange(-CurrentWeaponInfo->HorizontalRecoil, CurrentWeaponInfo->HorizontalRecoil) * RecoilMultiplier;
		RecoilRotationTarget += FRotator(RecoilUP, RecoilSIDE, 0);

		RecoilXTarget += FVector(-CurrentWeaponInfo->BackRecoil, 0, CurrentWeaponInfo->UpRecoil);
		RecoilZTarget += FVector(0, 0, CurrentWeaponInfo->UpRecoil);
		//Camera Shake
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(CurrentWeaponInfo->CameraShake, 1.0f);
	}
}

//Update magazine ammo and subtract from pool
void AGameEntity::ReloadValues()
{
	int magSize = CurrentWeaponInfo->MagSize;
	switch (AmmoMode)
	{
	case WAM_WEAPONEXCLUSIVE:
	{
		EWeaponType type = CurrentWeaponInfo->WeaponType;
		EWeaponCategory cat = CurrentWeaponInfo->WeaponCategory;
		int x = CurrentWeaponInfo->MagSize - Magazine[type];
		if (x < AmmoPool[type])
		{
			GLog->Log("Enough ammo for full reload");
			Magazine[cat] = magSize;
			AmmoPool[type] -= x;
		}
		else
		{
			GLog->Log("Not enough for a full reload, doing a partial");
			Magazine[cat] += AmmoPool[type];
			AmmoPool[type] = 0;
		}
	}
		break;
	case WAM_CALIBRE:
		break;
	case WAM_CATEGORY:
		break;
	}
}

void AGameEntity::ShootRaycast()
{
	//Set up Variables
	FHitResult hitBoi;
	FCollisionQueryParams CollisionParameters;
	//FVector Start = ViewModel->GetSocketLocation("Muzzle");
	//FVector End = ((ViewModel->GetForwardVector() * CurrentWeaponInfo->Range) + Start);
	FVector Start = MuzzleParticleBoi->GetComponentLocation();
	FVector End = ((ViewModel->GetForwardVector() * CurrentWeaponInfo->Range) + Start);
	//DebugLine
	//DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 5, 0, 1);

	
	GLog->Log("Weapon Fired");

	//Perform Raycast
	if (GetWorld()->LineTraceSingleByChannel(hitBoi, Start, End, ECC_WorldDynamic, CollisionParameters))
	{
		GLog->Log("Hit Something");

		//Test if it is an entity
		AGameEntity* EntityHit = Cast<AGameEntity>(hitBoi.Actor);
		if (EntityHit)
		{
			GLog->Log("Hit Entity");
			EntityHit->DamageEntity(100);
			return;
		}
		//Test if it is a shootable object
		AShootableObject* ObjectHit = Cast<AShootableObject>(hitBoi.Actor);
		if (ObjectHit)
		{
			GLog->Log("Hit Object");
			ObjectHit->Shot(hitBoi);
			return;
		}
	}
}

void AGameEntity::PlayEffects()
{
	if (CurrentWeaponInfo->ShellEjectParticle && ViewModel->DoesSocketExist("ShellParticle"))
	{
		ShellParticleBoi->ResetParticles();
		ShellParticleBoi->Activate();
	}
	if (CurrentWeaponInfo->MuzzleFlashParticle && ViewModel->DoesSocketExist("Muzzle"))
	{
		MuzzleParticleBoi->ResetParticles();
		MuzzleParticleBoi->Activate();
	}

	AudioSource->Play();
}


void AGameEntity::StartCrouch()
{
	Crouch();
}

void AGameEntity::EndCrouch()
{
	UnCrouch();
}

// Called every frame
void AGameEntity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurrentShotTime += DeltaTime;

	if (Reloading)
	{
		if (ReloadDoneTime < CurrentShotTime)
		{
			Reloading = false;
			ReloadValues();
		}
		else
		{
			return;
		}
	}

	if (Firing && CurrentWeaponInfo->Auto && CurrentShotTime > NextShotTime)
	{
		Fire();
		NextShotTime = CurrentShotTime + CurrentWeaponInfo->Cooldown;
	}
}

void AGameEntity::PickupAmmo(EWeaponType AmmoType, int Amount)
{
	AmmoPool[AmmoType] += Amount;
}

int AGameEntity::GetCurrentAmmo()
{
	return 0;
}

int AGameEntity::GetReserveAmmo()
{
	return 0;
}

FString AGameEntity::GetWeaponName()
{
	return CurrentWeaponInfo->WeaponName;
}

void AGameEntity::DamageEntity(int amount)
{
	health -= amount;
	if (health < 1)
	{
		Destroy();
	}
}
