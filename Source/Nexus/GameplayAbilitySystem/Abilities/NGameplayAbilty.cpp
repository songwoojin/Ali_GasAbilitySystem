// Fill out your copyright notice in the Description page of Project Settings.


#include "NGameplayAbilty.h"

UNGameplayAbilty::UNGameplayAbilty()
{
	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("GameplayAbility.Active")));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Dead")));
}
