// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AbilitySystemComponent.h"
#include "NEnemyHPBar.generated.h"

/**
 * 
 */

struct  FOnAttributeChangeData;
class UProgressBar;
UCLASS()
class NEXUS_API UNEnemyHPBar : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	void SetOwningActor(AActor* NewOwningActor);
protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HPProgressBar;

	AActor* OwningActor;
	
	float Health;
	float MaxHealth;

protected:
	void OnHealthChanged(const FOnAttributeChangeData& Data);
	void OnMaxHealthChanged(const FOnAttributeChangeData& Data);
	void SetHealthPercent();
};
