// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Nexus/GameplayAbilitySystem/Abilities/NGameplayAbilty.h"
#include "NGA_FillShield.generated.h"

/**
 * 
 */
UCLASS()
class NEXUS_API UNGA_FillShield : public UNGameplayAbilty
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	UAnimMontage* FillShieldMontage;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> FillShieldEffectClass;
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageTriggerEventReceived(FGameplayEventData Payload);
};
