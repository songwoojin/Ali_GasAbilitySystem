// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "GC_Dash.generated.h"

/**
 * 
 */

class UNiagaraSystem;

UCLASS()
class NEXUS_API UGC_Dash : public UGameplayCueNotify_Static
{
	GENERATED_BODY()

public:
	UGC_Dash();
protected:
	UPROPERTY()
	ACharacter* Character;

	UPROPERTY(EditAnywhere,Category="Particles")
	UNiagaraSystem* DashStartNiagara;

	UPROPERTY(EditAnywhere,Category="Particles")
	UNiagaraSystem* DashEndNiagara;

	UPROPERTY(EditAnywhere,Category="Sounds")
	USoundBase* TeleportStartSound;

	UPROPERTY(EditAnywhere,Category="Sounds")
	USoundBase* TeleportEndSound;
protected:
	//virtual void HandleGameplayCue()
	virtual void HandleGameplayCue(AActor* MyTarget, EGameplayCueEvent::Type EventType, const FGameplayCueParameters& Parameters) override;
	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;
	virtual bool OnActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;
	virtual bool OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;
};
