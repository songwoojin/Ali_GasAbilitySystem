// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NCharacterBase.h"
#include "NPlayerCharacter.generated.h"

class UGameplayAbility;
class UInputAction;

/**
 * 
 */
UCLASS()
class NEXUS_API ANPlayerCharacter : public ANCharacterBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere,Category="Input")
	UInputAction* DashAction;
	
	UPROPERTY(EditAnywhere,Category="Ability")
	TSubclassOf<UGameplayAbility> DashAbility;

public:
	ANPlayerCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	void GiveAbility();
	void Input_Dash();
	
	
};
