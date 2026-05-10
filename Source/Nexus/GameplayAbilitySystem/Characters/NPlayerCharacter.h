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

	UPROPERTY(EditAnywhere,Category="Effect")
	TSubclassOf<UGameplayEffect> RegenStaminaGE;

	//FGameplayAbilitySpecHandle DashAbilitySpecHandle;

	UPROPERTY(VisibleAnywhere)
	UNWeaponsManagerComponent* WeaponsManager;

	UPROPERTY(EditAnywhere,Category="Weapon")
	TSubclassOf<ANWeapon_Base> StaffWeaponClass;

	UPROPERTY(EditAnywhere,Category="Weapon")
	TSubclassOf<ANWeapon_Base> AxeWeaponClass;
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
	
	void HandleStaminaChanged(const FOnAttributeChangeData& Data);

	//UFUNCTION(BlueprintCallable)
	//FGameplayAbilitySpecHandle GetDashAbilitySpecHandle() const {return DashAbilitySpecHandle;};
	
};	
