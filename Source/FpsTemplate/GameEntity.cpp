// Fill out your copyright notice in the Description page of Project Settings.

#include "GameEntity.h"


// Sets default values
AGameEntity::AGameEntity()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ViewModel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("View Model"));
	//ViewModel->AddRelativeLocation(FVector(26, 15, 60));
	ViewModel->SetupAttachment(RootComponent);
	ViewModel->bCastDynamicShadow = false;
	//ViewModel->SetRenderCustomDepth(true);
	//ViewModel->SetCustomDepthStencilValue(255);
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

		ViewModel->SetStaticMesh(CurrentWeaponInfo->ViewModel);
		ViewModel->SetRelativeLocation(CurrentWeaponInfo->WeaponOffset);
	}


}

void AGameEntity::Shoot()
{

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
