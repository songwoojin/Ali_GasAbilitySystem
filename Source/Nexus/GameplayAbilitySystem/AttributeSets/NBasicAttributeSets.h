// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "NBasicAttributeSets.generated.h"

/**
 * 
 */

UCLASS()
class NEXUS_API UNBasicAttributeSets : public UAttributeSet
{
	GENERATED_BODY()

public:
	UNBasicAttributeSets();
	
	//Health Attributes
	UPROPERTY(BlueprintReadOnly, Category="Attributes",ReplicatedUsing=OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS_BASIC(UNBasicAttributeSets, Health);

	UPROPERTY(BlueprintReadOnly, Category="Attributes",ReplicatedUsing=OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS_BASIC(UNBasicAttributeSets, MaxHealth);

	//Stamina Attributes
	UPROPERTY(BlueprintReadOnly, Category="Attributes",ReplicatedUsing=OnRep_Stamina)
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS_BASIC(UNBasicAttributeSets, Stamina);

	UPROPERTY(BlueprintReadOnly, Category="Attributes",ReplicatedUsing=OnRep_MaxStamina)
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS_BASIC(UNBasicAttributeSets, MaxStamina);

	//Damage
	UPROPERTY(BlueprintReadOnly, Category="Attributes")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS_BASIC(UNBasicAttributeSets, Damage);

	//Shield Attributes
	UPROPERTY(BlueprintReadOnly, Category="Attributes",ReplicatedUsing=OnRep_Shield)
	FGameplayAttributeData Shield;
	ATTRIBUTE_ACCESSORS_BASIC(UNBasicAttributeSets, Shield);

	UPROPERTY(BlueprintReadOnly, Category="Attributes",ReplicatedUsing=OnRep_MaxShield)
	FGameplayAttributeData MaxShield;
	ATTRIBUTE_ACCESSORS_BASIC(UNBasicAttributeSets, MaxShield);

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;	
	
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue) const;
	
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue) const;
	
	UFUNCTION()
	void OnRep_Stamina(const FGameplayAttributeData& OldValue) const;
	
	UFUNCTION()
	void OnRep_MaxStamina(const FGameplayAttributeData& OldValue) const;

	UFUNCTION()
	void OnRep_Shield(const FGameplayAttributeData& OldValue) const;
	
	UFUNCTION()
	void OnRep_MaxShield(const FGameplayAttributeData& OldValue) const;
};
