// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Nexus/GameplayAbilitySystem/Weapon/Data/NWeaponData.h"
#include "NWeaponsManagerComponent.generated.h"


struct FGameplayAbilitySpecHandle;
class ANCharacterBase;
class ANWeapon_Base;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NEXUS_API UNWeaponsManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UNWeaponsManagerComponent();
	void EquipWeapon(const TSubclassOf<ANWeapon_Base>& EquippedWeaponClass);
	void UnEquipWeapon();
	ANWeapon_Base* GetEquippedWeapon() const {return EquippedWeapon;};
	
protected:
	UPROPERTY(ReplicatedUsing=OnRep_EquippedWeapon)
	TObjectPtr<ANWeapon_Base> EquippedWeapon;

	UPROPERTY()
	ANCharacterBase* OwningCharacter;

	UPROPERTY(EditAnywhere,Category="Weapon")
	FWeaponConfig UnarmedWeaponConfig;

	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitesGrantedByWeapon;
	
protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	void SetEquippedWeaponProperties();
	void SetUnarmedWeaponProperties();
	
	UFUNCTION()
	void OnRep_EquippedWeapon();

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
