// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Nexus/GameplayAbilitySystem/Abilities/NGameplayAbilty.h"
#include "NGA_AOEAttack.generated.h"

class AGameplayAbilityTargetActor_GroundTrace;
/**
 * 
 */

UCLASS()
class NEXUS_API UNGA_AOEAttack : public UNGameplayAbilty
{
	GENERATED_BODY()

public:
	UNGA_AOEAttack();

protected:
	UPROPERTY(EditAnywhere,Category="Ability")
	UAnimMontage* TargetingMontage;

	UPROPERTY(EditAnywhere,Category="Ability")
	UAnimMontage* ConfirmMontage;

	bool bIsWaitingTargetData;

	UPROPERTY(EditAnywhere,Category="Ability")
	float AttackMaxRange;

	UPROPERTY(EditAnywhere,Category="Ability")
	TSubclassOf<AGameplayAbilityTargetActor_GroundTrace> GroundTraceTargetActorClass;

	UPROPERTY()
	FVector ConfirmedAOELocation;

	UPROPERTY(EditAnywhere,Category="Ability")
	float AOERadius;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float DamageAmount;
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	void LoopTargetingAnimation();

	UFUNCTION()
	void OnMontageBlendOut();

	UFUNCTION()
	void OnMontageInterrupted();

	void WaitTargetData();

	UFUNCTION()
	void OnTargetDataReady(const FGameplayAbilityTargetDataHandle& Data);

	UFUNCTION()
	void OnTargetDataCancelled(const FGameplayAbilityTargetDataHandle& Data);

	void ConfirmStrike();

	UFUNCTION()
	void OnConfirmMontageCompleted();

	UFUNCTION()
	void OnEventReceived(FGameplayEventData Payload);

	void ApplyDamageToTarget(AActor* TargetActor);
};
