// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NCharacterBase.h"
#include "GameplayEffect.h"
#include "NPlayerCharacter.generated.h"

class ANWeapon_Base;
class UGameplayAbility;
class UGameplayEffect;
class UInputAction;
class UNWeaponsManagerComponent;

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

	UPROPERTY(EditAnywhere,Category="Input")
	UInputAction* EquipStaffAction;

	UPROPERTY(EditAnywhere,Category="Input")
	UInputAction* EquipAxeAction;

	UPROPERTY(EditAnywhere,Category="Input")
	UInputAction* MeleeAttackAxeSwingAction;

	UPROPERTY(EditAnywhere,Category="Input")
	UInputAction* MeleeAttackAxeComboAction;

	UPROPERTY(EditAnywhere,Category="Input")
	UInputAction* ShootProjectileAction;

	UPROPERTY(EditAnywhere,Category="Input")
	UInputAction* AOEAttackTargettingAction;

	UPROPERTY(EditAnywhere,Category="Input")
	UInputAction* AOEAttackTargetConfirmAction;

	UPROPERTY(EditAnywhere,Category="Effect")
	TSubclassOf<UGameplayEffect> RegenStaminaGE;

	//FGameplayAbilitySpecHandle DashAbilitySpecHandle;

	UPROPERTY(VisibleAnywhere)
	UNWeaponsManagerComponent* WeaponsManager;

public:
	ANPlayerCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	void Input_Dash();
	void Input_EquipStaff();
	//void Input_UnequipStaff();
	void Input_EquipAxe();
	//void Input_Dash();
	void Input_MeleeAttackAxeSwing();
	
	void HandleStaminaChanged(const FOnAttributeChangeData& Data);

	//UFUNCTION(BlueprintCallable)
	//FGameplayAbilitySpecHandle GetDashAbilitySpecHandle() const {return DashAbilitySpecHandle;};

	void Input_MeleeAttackAxeCombo();
	void Input_ShootProjectile();
	void Input_AOEAttackTargeting();
	void Input_AOEAttackTargetConfirm();
	
};	
