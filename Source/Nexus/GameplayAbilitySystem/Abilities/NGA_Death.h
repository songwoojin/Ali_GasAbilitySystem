// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Nexus/GameplayAbilitySystem/Abilities/NGameplayAbilty.h"
#include "NGA_Death.generated.h"

/**
 * 
 */
UCLASS()
class NEXUS_API UNGA_Death : public UNGameplayAbilty
{
	GENERATED_BODY()

public:
	UNGA_Death();

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> DeathGameplayEffectClass;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
};
