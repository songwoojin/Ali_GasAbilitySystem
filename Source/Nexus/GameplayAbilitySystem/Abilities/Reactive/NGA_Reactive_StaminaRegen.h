// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Nexus/GameplayAbilitySystem/Abilities/NGameplayAbilty.h"
#include "NGA_Reactive_StaminaRegen.generated.h"

/**
 * 
 */

UCLASS()
class NEXUS_API UNGA_Reactive_StaminaRegen : public UNGameplayAbilty
{
	GENERATED_BODY()

public:
	UNGA_Reactive_StaminaRegen();
	
protected:
	float OldStaminaValue;
	float NewStaminaValue;

	FTimerHandle RegenDelayTimer;

	UPROPERTY()
	FActiveGameplayEffectHandle StaminaRegenHandle;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> StaminaRegenEffectClass;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void OnStaminaChanged();

	UFUNCTION()
	void OnDelayFinished();
};
