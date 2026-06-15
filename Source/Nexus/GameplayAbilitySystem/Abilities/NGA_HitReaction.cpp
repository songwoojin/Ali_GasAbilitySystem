// Fill out your copyright notice in the Description page of Project Settings.


#include "Nexus/GameplayAbilitySystem/Abilities/NGA_HitReaction.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UNGA_HitReaction::UNGA_HitReaction()
{
}

void UNGA_HitReaction::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                       const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                       const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, NAME_None, HitReactionMontage);

	MontageTask->OnCompleted.AddDynamic(this, &UNGA_HitReaction::OnMontageCompleted);
	MontageTask->OnCancelled.AddDynamic(this, &UNGA_HitReaction::OnMontageCompleted);
	MontageTask->OnInterrupted.AddDynamic(this, &UNGA_HitReaction::OnMontageCompleted);
	
	MontageTask->ReadyForActivation();
}

void UNGA_HitReaction::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
