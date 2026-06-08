// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "NGameplayAbilty.generated.h"

/**
 * 
 */
UCLASS()
class NEXUS_API UNGameplayAbilty : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UNGameplayAbilty();
	
	UFUNCTION(BlueprintCallable,Category="Ability")
	bool GetShouldShowInAbilitiesBar() const {return bShouldShowInAbilitiesBar;};
	
protected:
	UPROPERTY(EditAnywhere,Category="Ability")
	bool bShouldShowInAbilitiesBar=false;
};
