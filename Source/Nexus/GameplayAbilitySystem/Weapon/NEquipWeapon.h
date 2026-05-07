// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "NEquipWeapon.generated.h"

class ANWeapon_Base;

/**
 * 
 */
UCLASS()
class NEXUS_API UNEquipWeapon : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UNEquipWeapon();
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UPROPERTY(EditAnywhere,Category="Ability")
	TSubclassOf<ANWeapon_Base> StaffWeaponClass;

	UPROPERTY(EditAnywhere,Category="Ability")
	TSubclassOf<ANWeapon_Base> AxeWeaponClass;
};
