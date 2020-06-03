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
}

// Called when the game starts or when spawned
void AGameEntity::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGameEntity::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (CurrentWeapon)
	{
		CurrentWeaponInfo = CurrentWeapon.GetDefaultObject();

		ViewModel->SetStaticMesh(CurrentWeaponInfo->WeaponModel);
		ViewModel->SetRelativeLocation(CurrentWeaponInfo->WeaponOffset);

		//Weapon Data Set
		WeaponInventory[CurrentWeaponInfo->WeaponType] = true;
		Magazine[CurrentWeaponInfo->WeaponType] = CurrentWeaponInfo->MagSize;
		
		if (CurrentWeaponInfo->ShellEjectParticle)
		{
			if (ViewModel->DoesSocketExist("ShellParticle"))
			{
				ShellParticleBoi->SetTemplate(CurrentWeaponInfo->ShellEjectParticle);
				ShellParticleBoi->SetWorldLocation(ViewModel->GetSocketLocation("ShellParticle"));
				ShellParticleBoi->SetWorldRotation(ViewModel->GetSocketRotation("ShellParticle"));
				ShellParticleBoi->SetActive(false);
				GLog->Log("Shell Socket Found");
			}
		}
		if (CurrentWeaponInfo->MuzzleFlashParticle)
		{
			if (ViewModel->DoesSocketExist("Muzzle"))
			{
				MuzzleParticleBoi->SetTemplate(CurrentWeaponInfo->MuzzleFlashParticle);
				MuzzleParticleBoi->SetWorldLocation(ViewModel->GetSocketLocation("Muzzle"));
				MuzzleParticleBoi->SetWorldRotation(ViewModel->GetSocketRotation("Muzzle"));
				MuzzleParticleBoi->SetActive(false);
				GLog->Log("Muzzle Socket Found");
			}
		}

		//Audio
		AudioSource->SetSound(CurrentWeaponInfo->GunshotSound);
		
	}


}

void AGameEntity::Shoot()
{
	if (Magazine[CurrentWeaponInfo->WeaponType] > 0)
	{
		ShootRaycast();
		PlayEffects();
		Magazine[CurrentWeaponInfo->WeaponType] = Magazine[CurrentWeaponInfo->WeaponType] - 1;
	}

}

void AGameEntity::UnShoot()
{
}

void AGameEntity::ShootRaycast()
{
	//Set up Variables
	FHitResult hitBoi;
	FCollisionQueryParams CollisionParameters;
	FVector Start = ViewModel->GetSocketLocation("Muzzle");
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

// Called every frame
void AGameEntity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGameEntity::PickupAmmo(EAmmoType AmmoType, int Amount)
{
	AmmoPool[AmmoType] += Amount;
}

void AGameEntity::DamageEntity(int amount)
{
	health -= amount;
	if (health < 1)
	{
		Destroy();
	}
}
