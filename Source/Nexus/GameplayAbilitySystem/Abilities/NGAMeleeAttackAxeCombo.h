// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Nexus/GameplayAbilitySystem/Abilities/NGAMeleeAttackBase.h"
#include "NGAMeleeAttackAxeCombo.generated.h"

/**
 * 
 */
UCLASS()
class NEXUS_API UNGAMeleeAttackAxeCombo : public UNGAMeleeAttackBase
{
	GENERATED_BODY()

public:
	UNGAMeleeAttackAxeCombo();

protected:
	bool bIsWithinComboWindow;
	bool bIsReceivedInputAtRightTime;
	int32 ComboCount;
	bool bAlwaysContinueCombo;
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void MontageStart() override;

	UFUNCTION()
	void OnContinueComboStartEventReceived(FGameplayEventData Payload);

	UFUNCTION()
	void OnContinueComboEndEventReceived(FGameplayEventData Payload);

	UFUNCTION()
	void OnContinueComboInputEventReceived(FGameplayEventData Payload);

	virtual void OnHitScanStartEventReceived(FGameplayEventData Payload) override;
	
};
