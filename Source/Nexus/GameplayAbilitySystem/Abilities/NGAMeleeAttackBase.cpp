// Fill out your copyright notice in the Description page of Project Settings.


#include "Nexus/GameplayAbilitySystem/Abilities/NGAMeleeAttackBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameFramework/Character.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Nexus/Component/NWeaponsManagerComponent.h"
#include "Nexus/GameplayAbilitySystem/NGameplayTagContainer.h"
#include"Nexus/GameplayAbilitySystem/Weapon/NWeapon_Base.h"

UNGAMeleeAttackBase::UNGAMeleeAttackBase()
	:AttackMontage(nullptr)
	,WeaponBase(nullptr)
	,DamageEffectClass(nullptr)
	,DamageMagnitude(0.0f)
{
}

void UNGAMeleeAttackBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
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
			&UNGAMeleeAttackBase::OnMontageCompleted);

		Task->OnInterrupted.AddDynamic(
			this,
			&UNGAMeleeAttackBase::OnMontageCompleted);

		Task->OnCancelled.AddDynamic(
			this,
			&UNGAMeleeAttackBase::OnMontageCompleted);

		Task->ReadyForActivation();
		MontageStart();
	}

	if (ActorInfo->AvatarActor->HasAuthority())
	{
		UAbilityTask_WaitGameplayEvent* HitScanStartTask =UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this,TAG_Event_HitScan_Start);
		if (HitScanStartTask)
		{
			HitScanStartTask->EventReceived.AddDynamic(this,&UNGAMeleeAttackBase::OnHitScanStartEventReceived);
			HitScanStartTask->ReadyForActivation();
		}
		
		UAbilityTask_WaitGameplayEvent* HitScanEndTask =UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this,TAG_Event_HitScan_End);
		if (HitScanEndTask)
		{
			HitScanEndTask->EventReceived.AddDynamic(this,&UNGAMeleeAttackBase::OnHitScanEndEventReceived);
			HitScanEndTask->ReadyForActivation();
		}
	}
}

void UNGAMeleeAttackBase::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	HandleHitScanEnd();
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UNGAMeleeAttackBase::MontageStart()
{
}

void UNGAMeleeAttackBase::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UNGAMeleeAttackBase::OnHitScanStartEventReceived(FGameplayEventData Payload)
{
	HandleHitScanStart();
}

void UNGAMeleeAttackBase::OnHitScanEndEventReceived(FGameplayEventData Payload)
{
	HandleHitScanEnd();
}

void UNGAMeleeAttackBase::HandleHitScanStart()
{
	if (!IsValid(WeaponBase))
	{
		return;
	}
	
	FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass,1);
	// SetByCaller 값 넣기
	SpecHandle.Data->SetSetByCallerMagnitude(
		TAG_Data_Damage,
		DamageMagnitude
	);
	WeaponBase->HitScanStart(SpecHandle);
}

void UNGAMeleeAttackBase::HandleHitScanEnd()
{
	if (!IsValid(WeaponBase))
	{
		return;
	}

	WeaponBase->HitScanEnd();
}
