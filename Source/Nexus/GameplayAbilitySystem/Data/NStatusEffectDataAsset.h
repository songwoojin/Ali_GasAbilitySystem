// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "GameplayEffect.h"
#include "NStatusEffectDataAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class NEXUS_API UNStatusEffectDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag DebuffTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 RequiredStackCount = 3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> StatusEffectToApplyClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float StatusDuration = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Damage = 5.f;

	
};
