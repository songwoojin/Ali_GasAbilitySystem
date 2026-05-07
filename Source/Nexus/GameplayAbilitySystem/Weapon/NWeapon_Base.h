// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/NWeaponData.h"
#include "NWeapon_Base.generated.h"

UCLASS()
class NEXUS_API ANWeapon_Base : public AActor
{
	GENERATED_BODY()
	
public:	
	ANWeapon_Base();

protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Weapon")
	UStaticMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere,Category="Weapon",meta=(AllowPrivateAccess=true))
	FWeaponConfig WeaponConfig;
	
protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	FWeaponConfig GetWeaponConfig() const {return WeaponConfig;};

};
