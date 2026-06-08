// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/NWeaponData.h"
#include "GameplayEffectTypes.h"
#include "NWeapon_Base.generated.h"

UCLASS()
class NEXUS_API ANWeapon_Base : public AActor
{
	GENERATED_BODY()
	
public:	
	ANWeapon_Base();

protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Weapon")
	USceneComponent* SceneRoot;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Weapon")
	UStaticMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere,Category="Weapon",meta=(AllowPrivateAccess=true))
	FWeaponConfig WeaponConfig;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Weapon")
	USceneComponent* TraceStart;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Weapon")
	USceneComponent* TraceEnd;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Weapon")
	USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(EditAnywhere,Category="HitScan",meta=(AllowPrivateAccess=true))
	float HitScanRadius;

	FTimerHandle HitScanTimer;

	UPROPERTY()
	TArray<TObjectPtr<AActor>> HitActors;
	
	FGameplayEffectSpecHandle CurrentHitScanEffectSpecHandle;

public:	
	virtual void Tick(float DeltaTime) override;

	FWeaponConfig GetWeaponConfig() const {return WeaponConfig;};

	void HitScanStart(FGameplayEffectSpecHandle HitScanEffectSpecHandle);
	void HitScanEnd();
	FVector GetSpawnPointLocation() const {return ProjectileSpawnPoint->GetComponentLocation();}
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void HitScan();
	
};
