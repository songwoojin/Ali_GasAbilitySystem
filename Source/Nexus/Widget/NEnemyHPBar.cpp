// Fill out your copyright notice in the Description page of Project Settings.


#include "Nexus/Widget/NEnemyHPBar.h"
#include "Components/ProgressBar.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Nexus/GameplayAbilitySystem/AttributeSets/NBasicAttributeSets.h"

void UNEnemyHPBar::NativeConstruct()
{
	Super::NativeConstruct();

	
	HPProgressBar->SetPercent(1.0f);
	
}

void UNEnemyHPBar::SetOwningActor(AActor* NewOwningActor)
{
	OwningActor=NewOwningActor;

	UAbilitySystemComponent* ASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwningActor);
	if (ASC)
	{
		Health = ASC->GetNumericAttribute(UNBasicAttributeSets::GetHealthAttribute());
		MaxHealth=ASC->GetNumericAttribute(UNBasicAttributeSets::GetMaxHealthAttribute());
		
		ASC->GetGameplayAttributeValueChangeDelegate(UNBasicAttributeSets::GetHealthAttribute()
		).AddUObject(
			this,
			&UNEnemyHPBar::OnHealthChanged
		);

		ASC->GetGameplayAttributeValueChangeDelegate(UNBasicAttributeSets::GetMaxHealthAttribute()
		).AddUObject(
			this,
			&UNEnemyHPBar::OnMaxHealthChanged
		);
	}
}

void UNEnemyHPBar::SetHealthPercent()
{
	if (HPProgressBar)
	{
		HPProgressBar->SetPercent(Health/MaxHealth);
	}
}

void UNEnemyHPBar::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	Health = Data.NewValue;
	SetHealthPercent();
}

void UNEnemyHPBar::OnMaxHealthChanged(const FOnAttributeChangeData& Data)
{
	MaxHealth = Data.NewValue;
	SetHealthPercent();
}
