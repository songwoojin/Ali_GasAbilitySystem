// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Nexus/GameplayAbilitySystem/Abilities/NGA_Weapon.h"
#include "NGA_Shield.generated.h"

/**
 * 
 */
UCLASS()
class NEXUS_API UNGA_Shield : public UNGA_Weapon
{
	GENERATED_BODY()
public:
	UNGA_Shield();

protected:
	UPROPERTY(EditAnywhere)
	UAnimMontage* ShieldMontage;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> ShieldEffectClass;
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnShootProjectileEventReceived(FGameplayEventData Payload);
	
};
