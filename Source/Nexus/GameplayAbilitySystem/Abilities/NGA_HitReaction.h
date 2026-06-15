// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Nexus/GameplayAbilitySystem/Abilities/NGameplayAbilty.h"
#include "NGA_HitReaction.generated.h"

/**
 * 
 */
UCLASS()
class NEXUS_API UNGA_HitReaction : public UNGameplayAbilty
{
	GENERATED_BODY()

public:
	UNGA_HitReaction();

protected:
	UPROPERTY(EditAnywhere)
	UAnimMontage* HitReactionMontage;
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void OnMontageCompleted();
};
