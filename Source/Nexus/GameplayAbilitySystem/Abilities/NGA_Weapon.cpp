// Fill out your copyright notice in the Description page of Project Settings.


#include "Nexus/GameplayAbilitySystem/Abilities/NGA_Weapon.h"
#include "Nexus/Component/NWeaponsManagerComponent.h"
#include "Nexus/GameplayAbilitySystem/Weapon/NWeapon_Base.h"

bool UNGA_Weapon::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                     const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
                                     const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	UNWeaponsManagerComponent* WeaponsManager = ActorInfo->AvatarActor->FindComponentByClass<UNWeaponsManagerComponent>();
	if (!WeaponsManager)
	{
		return false;
	}

	if (!WeaponsManager->GetEquippedWeapon() || WeaponsManager->GetEquippedWeapon()->GetClass() != RequiredEquippedWeapon)
	{
		return false;
	}
	
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}
