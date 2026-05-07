// Fill out your copyright notice in the Description page of Project Settings.


#include "NWeapon_Base.h"

ANWeapon_Base::ANWeapon_Base()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	RootComponent=WeaponMesh;
	WeaponMesh->SetCollisionProfileName(TEXT("NoCollision"));

	bReplicates=true;

}

void ANWeapon_Base::BeginPlay()
{
	Super::BeginPlay();
	
}

void ANWeapon_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

