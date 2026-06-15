// Fill out your copyright notice in the Description page of Project Settings.


#include "Nexus/GameplayAbilitySystem/Abilities/NGA_Death.h"

UNGA_Death::UNGA_Death()
{
}

void UNGA_Death::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, DeathEffectSpecHandle, 1);
	BP_ApplyGameplayEffectToOwner(DeathGameplayEffectClass);

	//UE_LOG(LogTemp, Warning, TEXT("Death Ability Activated"));
}
