// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Nexus/GameplayAbilitySystem/Abilities/NGameplayAbilty.h"
#include "NGAMeleeAttackBase.generated.h"

class ANWeapon_Base;
/**
 * 
 */
UCLASS()
class NEXUS_API UNGAMeleeAttackBase : public UNGameplayAbilty
{
	GENERATED_BODY()

public:
	UNGAMeleeAttackBase();

protected:
	UPROPERTY(EditAnywhere,Category="Ability")
	UAnimMontage* AttackMontage;

	UPROPERTY()
	ANWeapon_Base* WeaponBase;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float DamageMagnitude;
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual void MontageStart();
	
	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	virtual void OnHitScanStartEventReceived(FGameplayEventData Payload);

	UFUNCTION()
	void OnHitScanEndEventReceived(FGameplayEventData Payload);

	void HandleHitScanStart();
	void HandleHitScanEnd();
	
};
