// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PDA_Talent.generated.h"

/**
 * 
 */
class UGameplayAbility;
class UGameplayEffect;

UENUM(BlueprintType)
enum class ETalentType : uint8
{
	None UMETA(DisplayName="None"),
	Active UMETA(DisplayName="Active"),
	Passive UMETA(DisplayName="Passive"),
	Triggered UMETA(DisplayName="Triggered")
};

UCLASS()
class NEXUS_API UPDA_Talent : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="Properties")
	ETalentType Type=ETalentType::None;

	UPROPERTY(EditAnywhere, Category="UI")
	FName Name="";
	
	UPROPERTY(EditAnywhere, Category="UI")
	UTexture2D* Icon=nullptr;
	
	UPROPERTY(EditAnywhere, Category="UI")
	FText Description=FText::FromString("No Description");

	UPROPERTY(EditAnywhere, Category="Rules")
	int32 MaxLevel=0;

	UPROPERTY(EditAnywhere, Category="AbilitiesAndEffect")
	TArray<TSubclassOf<UGameplayAbility>> AbilitiesToGrant= TArray<TSubclassOf<UGameplayAbility>>();

	UPROPERTY(EditAnywhere, Category="AbilitiesAndEffect")
	TArray<TSubclassOf<UGameplayEffect>> EffectsToApply = TArray<TSubclassOf<UGameplayEffect>>();
};
