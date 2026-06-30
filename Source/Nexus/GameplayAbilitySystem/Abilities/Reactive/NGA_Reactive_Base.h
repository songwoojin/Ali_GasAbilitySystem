// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Nexus/GameplayAbilitySystem/Abilities/NGameplayAbilty.h"
#include "NGA_Reactive_Base.generated.h"

class UNStatusEffectDataAsset;
/**
 * 
 */
UCLASS()
class NEXUS_API UNGA_Reactive_Base : public UNGameplayAbilty
{
	GENERATED_BODY()

public:
	UNGA_Reactive_Base();

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNStatusEffectDataAsset> StatusEffectData;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void OnGameplayEffectApplied(
		AActor* Target,
		FGameplayEffectSpecHandle SpecHandle,
		FActiveGameplayEffectHandle ActiveHandle
	);

	void ModifyEffectSpecBeforeApplication(FGameplayEffectSpecHandle& SpecIn);
};
