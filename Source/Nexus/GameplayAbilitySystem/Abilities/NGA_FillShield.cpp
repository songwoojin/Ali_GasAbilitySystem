// Fill out your copyright notice in the Description page of Project Settings.


#include "Nexus/GameplayAbilitySystem/Abilities/NGA_FillShield.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

void UNGA_FillShield::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                      const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilityTask_PlayMontageAndWait* MontageTask=UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		FName("None"),
		FillShieldMontage
		);
	if (MontageTask)
	{
		MontageTask->OnCompleted.AddDynamic(this,&UNGA_FillShield::OnMontageCompleted);
		MontageTask->OnInterrupted.AddDynamic(this,&UNGA_FillShield::OnMontageCompleted);
		MontageTask->OnCancelled.AddDynamic(this,&UNGA_FillShield::OnMontageCompleted);
	}

	MontageTask->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* MontageTriggerEvent=UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,FGameplayTag::RequestGameplayTag("Event.Montage.Trigger"));
	if (MontageTriggerEvent)
	{
		MontageTriggerEvent->EventReceived.AddDynamic(this,&UNGA_FillShield::OnMontageTriggerEventReceived);
	}

	MontageTriggerEvent->ReadyForActivation();
}

void UNGA_FillShield::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle,CurrentActorInfo,CurrentActivationInfo,true,false);
}

void UNGA_FillShield::OnMontageTriggerEventReceived(FGameplayEventData Payload)
{
	CommitAbility(CurrentSpecHandle,CurrentActorInfo,CurrentActivationInfo);

	BP_ApplyGameplayEffectToOwner(FillShieldEffectClass);
}
