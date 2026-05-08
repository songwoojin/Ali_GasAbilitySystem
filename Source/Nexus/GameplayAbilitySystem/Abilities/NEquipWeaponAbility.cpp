// Fill out your copyright notice in the Description page of Project Settings.


#include "NEquipWeaponAbility.h"
#include "Nexus/Component/NWeaponsManagerComponent.h"
#include "Nexus/GameplayAbilitySystem/NGameplayTagContainer.h"

UNEquipWeaponAbility::UNEquipWeaponAbility()
{
}

void UNEquipWeaponAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	if (!TriggerEventData)	return;

	const FGameplayTagContainer& Tags = TriggerEventData->TargetTags;

	UNWeaponsManagerComponent* WeaponManagerComponent = Cast<UNWeaponsManagerComponent>(GetAvatarActorFromActorInfo()->GetComponentByClass(UNWeaponsManagerComponent::StaticClass()));
	
	if (Tags.HasTag(TAG_Weapon_Ranged_Staff))
	{
		if (WeaponManagerComponent)
		{
			WeaponManagerComponent->EquipWeapon(StaffWeaponClass);
			
		}
	}
	else if (Tags.HasTag(TAG_Weapon_Melee_Axe))
	{
		if (WeaponManagerComponent)
		{
			WeaponManagerComponent->EquipWeapon(AxeWeaponClass);
		}
	}
	else
	{
		if (WeaponManagerComponent)
		{
			WeaponManagerComponent->UnEquipWeapon();
		}
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
