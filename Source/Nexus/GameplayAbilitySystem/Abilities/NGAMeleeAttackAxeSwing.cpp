// Fill out your copyright notice in the Description page of Project Settings.


#include "Nexus/GameplayAbilitySystem/Abilities/NGAMeleeAttackAxeSwing.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameFramework/Character.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Nexus/Component/NWeaponsManagerComponent.h"
#include "Nexus/GameplayAbilitySystem/NGameplayTagContainer.h"
#include "Nexus/Component/NWeaponsManagerComponent.h"
#include"Nexus/GameplayAbilitySystem/Weapon/NWeapon_Base.h"

UNGAMeleeAttackAxeSwing::UNGAMeleeAttackAxeSwing()
	:AttackMontage(nullptr)
	,WeaponBase(nullptr)
{
}

void UNGAMeleeAttackAxeSwing::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                              const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                              const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UNWeaponsManagerComponent* WeaponsManagerComponent = Cast<UNWeaponsManagerComponent>(GetAvatarActorFromActorInfo()->GetComponentByClass(UNWeaponsManagerComponent::StaticClass()));
	if (!IsValid(WeaponsManagerComponent))	
	{
		return EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}

	WeaponBase = WeaponsManagerComponent->GetEquippedWeapon();
	if (!IsValid(WeaponBase))
	{
		return EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
	
	UAbilityTask_PlayMontageAndWait* Task =
	   UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		   this,
		   NAME_None,
		   AttackMontage
	   );
	
	if (Task)
	{
		Task->OnCompleted.AddDynamic(
			this,
			&UNGAMeleeAttackAxeSwing::OnMontageCompleted);

		Task->OnInterrupted.AddDynamic(
			this,
			&UNGAMeleeAttackAxeSwing::OnMontageCompleted);

		Task->OnCancelled.AddDynamic(
			this,
			&UNGAMeleeAttackAxeSwing::OnMontageCompleted);

		Task->ReadyForActivation();
	}

	if (ActorInfo->AvatarActor->HasAuthority())
	{
		UAbilityTask_WaitGameplayEvent* HitScanStartTask =UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this,TAG_Event_HitScan_Start);
		if (HitScanStartTask)
		{
			HitScanStartTask->EventReceived.AddDynamic(this,&UNGAMeleeAttackAxeSwing::OnHitScanStartEventReceived);
			HitScanStartTask->ReadyForActivation();
		}
		
		UAbilityTask_WaitGameplayEvent* HitScanEndTask =UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this,TAG_Event_HitScan_End);
		if (HitScanEndTask)
		{
			HitScanEndTask->EventReceived.AddDynamic(this,&UNGAMeleeAttackAxeSwing::OnHitScanEndEventReceived);
			HitScanEndTask->ReadyForActivation();
		}
	}
}

void UNGAMeleeAttackAxeSwing::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if (IsValid(WeaponBase))
	{
		WeaponBase->HitScanEnd();
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UNGAMeleeAttackAxeSwing::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UNGAMeleeAttackAxeSwing::OnHitScanStartEventReceived(FGameplayEventData Payload)
{
	if (!IsValid(WeaponBase))
	{
		return;
	}

	FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass,1);
	// SetByCaller 값 넣기
	SpecHandle.Data->SetSetByCallerMagnitude(
		TAG_Data_Damage,
		-34.f
	);
	WeaponBase->HitScanStart(SpecHandle);
}

void UNGAMeleeAttackAxeSwing::OnHitScanEndEventReceived(FGameplayEventData Payload)
{
	if (!IsValid(WeaponBase))
	{
		return;
	}

	WeaponBase->HitScanEnd();
}
