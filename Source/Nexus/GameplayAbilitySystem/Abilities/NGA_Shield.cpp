// Fill out your copyright notice in the Description page of Project Settings.


#include "Nexus/GameplayAbilitySystem/Abilities/NGA_Shield.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

UNGA_Shield::UNGA_Shield()
{
}

void UNGA_Shield::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                  const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilityTask_PlayMontageAndWait* Task =
	   UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		   this,
		   NAME_None,
		   ShieldMontage
	   );

	if (Task)
	{
		Task->OnCompleted.AddDynamic(
			this,
			&UNGA_Shield::OnMontageCompleted);

		Task->OnInterrupted.AddDynamic(
			this,
			&UNGA_Shield::OnMontageCompleted);

		Task->OnCancelled.AddDynamic(
			this,
			&UNGA_Shield::OnMontageCompleted);

		Task->ReadyForActivation();
	}

	UAbilityTask_WaitGameplayEvent* MontageTriggerEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this,FGameplayTag::RequestGameplayTag(FName("Event.Montage.Trigger")));
	if (MontageTriggerEvent)
	{
		MontageTriggerEvent->EventReceived.AddDynamic(this,&UNGA_Shield::OnShootProjectileEventReceived);
		MontageTriggerEvent->ReadyForActivation();
	}
}

void UNGA_Shield::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UNGA_Shield::OnShootProjectileEventReceived(FGameplayEventData Payload)
{
	CommitAbility(CurrentSpecHandle,CurrentActorInfo,CurrentActivationInfo);

	BP_ApplyGameplayEffectToOwner(ShieldEffectClass,10);
	
}
