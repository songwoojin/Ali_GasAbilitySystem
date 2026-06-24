// Fill out your copyright notice in the Description page of Project Settings.


#include "Nexus/Widget/NEnemyArmorBar.h"
#include "Components/ProgressBar.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Nexus/GameplayAbilitySystem/AttributeSets/NBasicAttributeSets.h"

void UNEnemyArmorBar::NativeConstruct()
{
	Super::NativeConstruct();

	ArmorProgressBar->SetPercent(1.0f);
}

void UNEnemyArmorBar::SetOwningActor(AActor* NewOwningActor)
{
	OwningActor=NewOwningActor;

	UAbilitySystemComponent* ASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwningActor);
	if (ASC)
	{
		Shield = ASC->GetNumericAttribute(UNBasicAttributeSets::GetShieldAttribute());
		MaxShield=ASC->GetNumericAttribute(UNBasicAttributeSets::GetMaxShieldAttribute());
		
		ASC->GetGameplayAttributeValueChangeDelegate(UNBasicAttributeSets::GetShieldAttribute()
		).AddUObject(
			this,
			&UNEnemyArmorBar::OnShieldChanged
		);

		ASC->GetGameplayAttributeValueChangeDelegate(UNBasicAttributeSets::GetMaxShieldAttribute()
		).AddUObject(
			this,
			&UNEnemyArmorBar::OnMaxShieldChanged
		);
	}
}

void UNEnemyArmorBar::OnShieldChanged(const FOnAttributeChangeData& Data)
{
	Shield = Data.NewValue;
	SetShieldPercent();
}

void UNEnemyArmorBar::OnMaxShieldChanged(const FOnAttributeChangeData& Data)
{
	MaxShield = Data.NewValue;
	SetShieldPercent();
}

void UNEnemyArmorBar::SetShieldPercent()
{
	if (ArmorProgressBar)
	{
		ArmorProgressBar->SetPercent(Shield/MaxShield);
	}
}


