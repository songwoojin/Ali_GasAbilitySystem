// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Nexus/GameplayAbilitySystem/Abilities/NGameplayAbilty.h"
#include "NGAMeleeAttackAxeSwing.generated.h"

class ANWeapon_Base;
/**
 * 
 */
UCLASS()
class NEXUS_API UNGAMeleeAttackAxeSwing : public UNGameplayAbilty
{
	GENERATED_BODY()

public:
	UNGAMeleeAttackAxeSwing();

protected:
	UPROPERTY(EditAnywhere,Category="Ability")
	UAnimMontage* AttackMontage;

	UPROPERTY()
	ANWeapon_Base* WeaponBase;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnHitScanStartEventReceived(FGameplayEventData Payload);

	UFUNCTION()
	void OnHitScanEndEventReceived(FGameplayEventData Payload);
	
};
