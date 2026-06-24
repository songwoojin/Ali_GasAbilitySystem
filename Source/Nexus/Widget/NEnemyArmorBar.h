// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NEnemyArmorBar.generated.h"

/**
 * 
 */
struct  FOnAttributeChangeData;
class UProgressBar;

UCLASS()
class NEXUS_API UNEnemyArmorBar : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	void SetOwningActor(AActor* NewOwningActor);
protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ArmorProgressBar;

	AActor* OwningActor;
	
	float Shield;
	float MaxShield;

protected:
	void OnShieldChanged(const FOnAttributeChangeData& Data);
	void OnMaxShieldChanged(const FOnAttributeChangeData& Data);
	void SetShieldPercent();
};
