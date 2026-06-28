// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "NGameplayAbilty.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EAbilityInputID : uint8
{
	None UMETA(DisplayName="None"),
	PrimaryAbility UMETA(DisplayName="Primary Ability"),
	SecondaryAbility UMETA(DisplayName="Secondary Ability"),
	DefensiveAbility UMETA(DisplayName="Defensive Ability"),
	MovementAbility UMETA(DisplayName="Movement Ability"),
};

UCLASS()
class NEXUS_API UNGameplayAbilty : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UNGameplayAbilty();
	
	UFUNCTION(BlueprintCallable,Category="Ability")
	bool GetShouldShowInAbilitiesBar() const {return bShouldShowInAbilitiesBar;};

	EAbilityInputID GetAbilityInputID() const {return AbilityInputID;};
	
protected:
	UPROPERTY(EditAnywhere,Category="Ability")
	bool bShouldShowInAbilitiesBar=false;

	UPROPERTY(EditAnywhere,Category="Input")
	EAbilityInputID AbilityInputID= EAbilityInputID::None;

protected:
	bool HasPC();
};
