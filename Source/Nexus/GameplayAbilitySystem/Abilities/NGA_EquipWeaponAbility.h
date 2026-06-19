// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Nexus/GameplayAbilitySystem/Abilities/NGameplayAbilty.h"
#include "NGA_EquipWeaponAbility.generated.h"

class ANWeapon_Base;

/**
 * 
 */
UCLASS()
class NEXUS_API UNGA_EquipWeaponAbility : public UNGameplayAbilty
{
	GENERATED_BODY()
public:
	UNGA_EquipWeaponAbility();
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnAttachWeaponEvent(FGameplayEventData EventData);
	
protected:
	UPROPERTY(EditAnywhere,Category="Ability")
	TSubclassOf<ANWeapon_Base> StaffWeaponClass;

	UPROPERTY(EditAnywhere,Category="Ability")
	TSubclassOf<ANWeapon_Base> AxeWeaponClass;

	UPROPERTY(EditAnywhere,Category="Ability")
	UAnimMontage* EquipMontage;

	FGameplayTagContainer WeaponTags;
};
