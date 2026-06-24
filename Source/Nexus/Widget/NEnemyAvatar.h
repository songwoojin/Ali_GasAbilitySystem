// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NEnemyAvatar.generated.h"

class UNEnemyArmorBar;
class UNEnemyHPBar;
/**
 * 
 */
UCLASS()
class NEXUS_API UNEnemyAvatar : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UNEnemyHPBar* HPBar;

	UPROPERTY(meta = (BindWidget))
	UNEnemyArmorBar* ArmorBar;

public:
	void SetOwningActor(AActor* NewOwningActor);
	
protected:
	virtual void NativeConstruct() override;
	
};
