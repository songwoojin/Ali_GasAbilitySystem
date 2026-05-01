// Fill out your copyright notice in the Description page of Project Settings.


#include "NBasicAttributeSets.h"

#include "Net/UnrealNetwork.h"

UNBasicAttributeSets::UNBasicAttributeSets()
	:Health(100.0f)
	,MaxHealth(100.0f)
	,Stamina(100.0f)
	,MaxStamina(100.0f)
{
}

void UNBasicAttributeSets::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UNBasicAttributeSets, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UNBasicAttributeSets, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UNBasicAttributeSets, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UNBasicAttributeSets, MaxStamina, COND_None, REPNOTIFY_Always);
}

void UNBasicAttributeSets::OnRep_Health(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UNBasicAttributeSets, Health, OldValue);
}

void UNBasicAttributeSets::OnRep_MaxHealth(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UNBasicAttributeSets, MaxHealth, OldValue);
}

void UNBasicAttributeSets::OnRep_Stamina(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UNBasicAttributeSets, Stamina, OldValue);
}

void UNBasicAttributeSets::OnRep_MaxStamina(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UNBasicAttributeSets, MaxStamina, OldValue);
}
