// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "NAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class NEXUS_API UNAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

protected:
	TArray<FGameplayAbilitySpec> LastActivatableAbilities;
	
public:
	UNAbilitySystemComponent();

protected:
	virtual void OnRep_ActivateAbilities() override;
};
