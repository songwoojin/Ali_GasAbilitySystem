// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Nexus/GameplayAbilitySystem/Abilities/NGameplayAbilty.h"
#include "NDashAbility.generated.h"

/**
 * 
 */
UCLASS()
class NEXUS_API UNDashAbility : public UNGameplayAbilty
{
	GENERATED_BODY()
public:
	UNDashAbility();

protected:
	UPROPERTY(EditAnywhere,Category="Ability")
	float Strength;

	UPROPERTY(EditAnywhere,Category="Ability")
	float Duration;
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	//FVector GetDashDirection();
	float GetMaxSpeed();
	
	UFUNCTION()
	void OnDashFinished();

	FVector GetDashDirection();

	virtual bool CommitAbilityCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FGameplayTagContainer* OptionalRelevantTags = nullptr) override;
	virtual bool CommitAbilityCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const bool ForceCooldown, FGameplayTagContainer* OptionalRelevantTags = nullptr) override;
};
