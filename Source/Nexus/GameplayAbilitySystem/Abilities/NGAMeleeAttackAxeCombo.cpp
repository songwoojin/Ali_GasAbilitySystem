// Fill out your copyright notice in the Description page of Project Settings.


#include "Nexus/GameplayAbilitySystem/Abilities/NGAMeleeAttackAxeCombo.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Nexus/GameplayAbilitySystem/NGameplayTagContainer.h"

UNGAMeleeAttackAxeCombo::UNGAMeleeAttackAxeCombo()
	: bIsWithinComboWindow(false)
	,bIsReceivedInputAtRightTime(false)
	,ComboCount(0)
	,bAlwaysContinueCombo(false)
{
}

void UNGAMeleeAttackAxeCombo::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!HasPC())
	{
		bAlwaysContinueCombo = true;
	}
}

void UNGAMeleeAttackAxeCombo::MontageStart()
{
	ComboCount=1;
	
	UAbilityTask_WaitGameplayEvent* ContinueComboStartTask =UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this,TAG_Event_ContinueCombo_Start);
	if (ContinueComboStartTask)
	{
		ContinueComboStartTask->EventReceived.AddDynamic(this,&UNGAMeleeAttackAxeCombo::OnContinueComboStartEventReceived);
		ContinueComboStartTask->ReadyForActivation();
	}

	UAbilityTask_WaitGameplayEvent* ContinueComboEndTask =UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this,TAG_Event_ContinueCombo_End);
	if (ContinueComboEndTask)
	{
		ContinueComboEndTask->EventReceived.AddDynamic(this,&UNGAMeleeAttackAxeCombo::OnContinueComboEndEventReceived);
		ContinueComboEndTask->ReadyForActivation();
	}

	UAbilityTask_WaitGameplayEvent* ContinueComboInputTask =UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this,TAG_Event_ContinueCombo_Input);
	if (ContinueComboInputTask)
	{
		ContinueComboInputTask->EventReceived.AddDynamic(this,&UNGAMeleeAttackAxeCombo::OnContinueComboInputEventReceived);
		ContinueComboInputTask->ReadyForActivation();
	}
	
}

void UNGAMeleeAttackAxeCombo::OnContinueComboStartEventReceived(FGameplayEventData Payload)
{
	bIsWithinComboWindow=true;
	bIsReceivedInputAtRightTime=false;
}

void UNGAMeleeAttackAxeCombo::OnContinueComboEndEventReceived(FGameplayEventData Payload)
{
	bIsWithinComboWindow=false;
	if (bIsReceivedInputAtRightTime || bAlwaysContinueCombo)
	{
		ComboCount++;
	}
	else
	{
		EndAbility(CurrentSpecHandle,CurrentActorInfo,CurrentActivationInfo,true,false);
	}
}

void UNGAMeleeAttackAxeCombo::OnContinueComboInputEventReceived(FGameplayEventData Payload)
{
	UE_LOG(LogTemp, Warning,
		TEXT("Ability Combo Input Received / Authority: %d / Window: %d"),
		GetAvatarActorFromActorInfo()->HasAuthority(),
		bIsWithinComboWindow);
	
	bIsReceivedInputAtRightTime=bIsWithinComboWindow;
}

void UNGAMeleeAttackAxeCombo::OnHitScanStartEventReceived(FGameplayEventData Payload)
{
	DamageMagnitude=ComboCount*10.0f;

	UE_LOG(LogTemp, Warning,
		TEXT("AxeCombo HitScanStart / Authority: %d / ComboCount: %d / DamageMagnitude: %f"),
		GetAvatarActorFromActorInfo()->HasAuthority(),
		ComboCount,
		DamageMagnitude);
	
	Super::OnHitScanStartEventReceived(Payload);
}
