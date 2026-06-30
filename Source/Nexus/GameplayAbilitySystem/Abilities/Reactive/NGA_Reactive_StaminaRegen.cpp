// Fill out your copyright notice in the Description page of Project Settings.


#include "Nexus/GameplayAbilitySystem/Abilities/Reactive/NGA_Reactive_StaminaRegen.h"
#include "Abilities/Tasks/AbilityTask_WaitAttributeChange.h"
#include "Nexus/GameplayAbilitySystem/AttributeSets/NBasicAttributeSets.h"

UNGA_Reactive_StaminaRegen::UNGA_Reactive_StaminaRegen()
{
}

void UNGA_Reactive_StaminaRegen::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                 const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                 const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (ASC)
	{
		OldStaminaValue = ASC->GetNumericAttribute(UNBasicAttributeSets::GetStaminaAttribute());
	}
	
	UAbilityTask_WaitAttributeChange* StaminaChangeTask =
	UAbilityTask_WaitAttributeChange::WaitForAttributeChange(
		this,
		UNBasicAttributeSets::GetStaminaAttribute(),
		FGameplayTag(),
		FGameplayTag(),
		false
	);

	if (StaminaChangeTask)
	{
		StaminaChangeTask->OnChange.AddDynamic(
			this,
			&UNGA_Reactive_StaminaRegen::OnStaminaChanged
		);

		StaminaChangeTask->ReadyForActivation();
	}
}

void UNGA_Reactive_StaminaRegen::OnStaminaChanged()
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (ASC)
	{
		NewStaminaValue = ASC->GetNumericAttribute(UNBasicAttributeSets::GetStaminaAttribute());
	}

	if (NewStaminaValue < OldStaminaValue)
	{
		ASC->RemoveActiveGameplayEffect(StaminaRegenHandle);

		GetWorld()->GetTimerManager().ClearTimer(RegenDelayTimer);
		GetWorld()->GetTimerManager().SetTimer(
			RegenDelayTimer,
			this,
			&UNGA_Reactive_StaminaRegen::OnDelayFinished,
			1.0f,
			false
		);
	}
	else if (NewStaminaValue >= ASC->GetNumericAttribute(UNBasicAttributeSets::GetMaxStaminaAttribute()))
	{
		ASC->RemoveActiveGameplayEffect(StaminaRegenHandle);
	}

	OldStaminaValue=NewStaminaValue;
}

void UNGA_Reactive_StaminaRegen::OnDelayFinished()
{
	FGameplayEffectSpecHandle SpecHandle =
	MakeOutgoingGameplayEffectSpec(
		StaminaRegenEffectClass,
		GetAbilityLevel()
	);
	
	StaminaRegenHandle =ApplyGameplayEffectSpecToOwner(
		CurrentSpecHandle,
		CurrentActorInfo,
		CurrentActivationInfo,
		SpecHandle
	);
}




