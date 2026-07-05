// Fill out your copyright notice in the Description page of Project Settings.


#include "NGameplayAbilty.h"

UNGameplayAbilty::UNGameplayAbilty()
{
	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("GameplayAbility.Active")));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Dead")));
}

void UNGameplayAbilty::SetAbilityLevel(int32 NewLevel)
{
	if (FGameplayAbilitySpec* AbilitySpec=GetCurrentAbilitySpec())
	{
		AbilitySpec->Level = NewLevel;
	}
}

bool UNGameplayAbilty::HasPC()
{
	APawn* NPawn = Cast<APawn>(GetAvatarActorFromActorInfo());
	if (!NPawn)
	{
		return false;
	}

	return NPawn->IsPlayerControlled();
}
