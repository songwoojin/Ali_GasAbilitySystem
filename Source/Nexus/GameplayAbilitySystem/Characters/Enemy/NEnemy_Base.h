// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Nexus/GameplayAbilitySystem/Characters/NCharacterBase.h"
#include "NEnemy_Base.generated.h"

/**
 * 
 */

class UWidgetComponent;

UCLASS()
class NEXUS_API ANEnemy_Base : public ANCharacterBase,public INTargetingInterface
{
	GENERATED_BODY()

public:
	ANEnemy_Base();
	virtual void BeginPlay() override;

protected:
	FTimerHandle AttackTimerHandle;

	UPROPERTY(EditAnywhere)
	float AttackDuration;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UWidgetComponent* HPWidgetComponent;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> HPWidgetClass;
	
protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual AActor* GetAttackTarget() override;
	void SendWeaponEquipEvent();
	void Attack();
	virtual void HandleDeath() override;
	
};
