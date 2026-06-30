// Fill out your copyright notice in the Description page of Project Settings.


#include "Nexus/GameplayAbilitySystem/Abilities/Reactive/NGA_Reactive_Base.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEffectApplied_Target.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Nexus/GameplayAbilitySystem/Data/NStatusEffectDataAsset.h"

UNGA_Reactive_Base::UNGA_Reactive_Base()
{
}

void UNGA_Reactive_Base::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                         const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilityTask_WaitGameplayEffectApplied_Target* Task =
	UAbilityTask_WaitGameplayEffectApplied_Target::WaitGameplayEffectAppliedToTarget(
		this,
		FGameplayTargetDataFilterHandle(),
		FGameplayTagRequirements(),
		FGameplayTagRequirements(),
		FGameplayTagRequirements(),
		FGameplayTagRequirements()
	);

	if (Task)
	{
		Task->OnApplied.AddDynamic(this,&UNGA_Reactive_Base::OnGameplayEffectApplied);
		Task->ReadyForActivation();
	}
}

void UNGA_Reactive_Base::OnGameplayEffectApplied(AActor* Target, FGameplayEffectSpecHandle SpecHandle,
	FActiveGameplayEffectHandle ActiveHandle)
{
	UAbilitySystemComponent* TargetASC =UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);

	if (!TargetASC)
	{
		return;
	}
	
	const FActiveGameplayEffect* ActiveGE =TargetASC->GetActiveGameplayEffect(ActiveHandle);
	if (!ActiveGE)
	{
		return;
	}

	FGameplayTagContainer GrantedTags;
	ActiveGE->Spec.GetAllGrantedTags(GrantedTags);
	UE_LOG(LogTemp, Warning, TEXT("GrantedTags: %s"),*GrantedTags.ToStringSimple());
	
	if (GrantedTags.HasTag(StatusEffectData->DebuffTag))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Handle Stack : %d"),ActiveGE->Spec.GetStackCount());
		UE_LOG(LogTemp, Warning, TEXT("ASC Stack : %d"),TargetASC->GetCurrentStackCount(ActiveHandle));
		
		//int32 StackCount = ActiveGE->Spec.GetStackCount();
		const int32 StackCount = TargetASC->GetCurrentStackCount(ActiveHandle);
		if (StackCount >= StatusEffectData->RequiredStackCount)
		{
			FGameplayAbilityTargetDataHandle TargetData;
			FGameplayAbilityTargetData_ActorArray* Data =new FGameplayAbilityTargetData_ActorArray();
			Data->TargetActorArray.Add(Target);
			TargetData.Add(Data);

			FGameplayEffectSpecHandle BurnSpec =
				MakeOutgoingGameplayEffectSpec(
					StatusEffectData->StatusEffectToApplyClass,
					1.0f
				);

			ModifyEffectSpecBeforeApplication(BurnSpec);
			
			ApplyGameplayEffectSpecToTarget(
				CurrentSpecHandle,
				CurrentActorInfo,
				CurrentActivationInfo,
				BurnSpec,
				TargetData
			);
		}
	}
}

void UNGA_Reactive_Base::ModifyEffectSpecBeforeApplication(FGameplayEffectSpecHandle& SpecIn)
{
	SpecIn.Data->SetDuration(StatusEffectData->StatusDuration,true);
	SpecIn.Data->SetSetByCallerMagnitude(
	FGameplayTag::RequestGameplayTag(TEXT("Data.Damage")),
	StatusEffectData->Damage
	);
	
}
