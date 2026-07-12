// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Nexus/GameplayAbilitySystem/Abilities/NGA_Weapon.h"
#include "NGA_ShootProjectileBase.generated.h"

class ANProjectileBase;
/**
 * 
 */
UCLASS()
class NEXUS_API UNGA_ShootProjectileBase : public UNGA_Weapon
{
	GENERATED_BODY()

public:
	UNGA_ShootProjectileBase();

protected:
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* ShootMontage;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ANProjectileBase> ProjectileClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly)
	float DamageMagnitude;
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnTargetDataReady(const FGameplayAbilityTargetDataHandle& Data);

	UFUNCTION()
	void OnTargetDataCancelled(const FGameplayAbilityTargetDataHandle& Data);

	UFUNCTION()
	void OnShootProjectileEventReceived(FGameplayEventData Payload);

	FVector GetSpawnLocation();

	void ShootProjectile(FVector TargetLocation);
};
