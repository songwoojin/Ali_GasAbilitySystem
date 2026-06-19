// Fill out your copyright notice in the Description page of Project Settings.


#include "NGA_EquipWeaponAbility.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Nexus/Component/NWeaponsManagerComponent.h"
#include "Nexus/GameplayAbilitySystem/NGameplayTagContainer.h"

UNGA_EquipWeaponAbility::UNGA_EquipWeaponAbility()
{
}

void UNGA_EquipWeaponAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}
	
	UAbilityTask_PlayMontageAndWait* MontageTask=UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		EquipMontage
	);

	MontageTask->OnCompleted.AddDynamic(this, &UNGA_EquipWeaponAbility::OnMontageCompleted);
	MontageTask->ReadyForActivation();
	
	UAbilityTask_WaitGameplayEvent* WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FGameplayTag::RequestGameplayTag(TEXT("Event.AttachWeapon")));
	WaitEventTask->EventReceived.AddDynamic(this, &UNGA_EquipWeaponAbility::OnAttachWeaponEvent);
	WaitEventTask->ReadyForActivation();
	
	// if (!TriggerEventData)
	// {
	// 	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	// 	return;
	// }
	WeaponTags = TriggerEventData->TargetTags;
}

void UNGA_EquipWeaponAbility::OnMontageCompleted()
{
	UNWeaponsManagerComponent* WeaponManagerComponent =
		Cast<UNWeaponsManagerComponent>(
			GetAvatarActorFromActorInfo()
			->GetComponentByClass(UNWeaponsManagerComponent::StaticClass()));

	if (WeaponManagerComponent)
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(
			WeaponManagerComponent,
			&UNWeaponsManagerComponent::ApplyWeaponState
		);
	}

	//WeaponManagerComponent->ApplyWeaponState();
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UNGA_EquipWeaponAbility::OnAttachWeaponEvent(FGameplayEventData EventData)
{
	UNWeaponsManagerComponent* WeaponManagerComponent = Cast<UNWeaponsManagerComponent>(GetAvatarActorFromActorInfo()->GetComponentByClass(UNWeaponsManagerComponent::StaticClass()));
	
	if (WeaponTags.HasTag(TAG_Weapon_Ranged_Staff))
	{
		if (WeaponManagerComponent)
		{
			WeaponManagerComponent->EquipWeapon(StaffWeaponClass);
		}
	}
	else if (WeaponTags.HasTag(TAG_Weapon_Melee_Axe))
	{
		if (WeaponManagerComponent)
		{
			WeaponManagerComponent->EquipWeapon(AxeWeaponClass);
		}
	}
	else
	{
		if (WeaponManagerComponent)
		{
			WeaponManagerComponent->UnEquipWeapon();
		}
	}
}
