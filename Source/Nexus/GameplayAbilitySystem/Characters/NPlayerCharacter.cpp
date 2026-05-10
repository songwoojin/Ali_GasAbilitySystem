// Fill out your copyright notice in the Description page of Project Settings.


#include "NPlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Abilities/Tasks/AbilityTask_WaitAttributeChange.h"
#include "Nexus/GameplayAbilitySystem/AttributeSets/NBasicAttributeSets.h"
#include "Nexus/GameplayAbilitySystem/NGameplayTagContainer.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Nexus/Component/NWeaponsManagerComponent.h"
#include "Nexus/GameplayAbilitySystem/NAbilitySystemComponent.h"

ANPlayerCharacter::ANPlayerCharacter()
	:DashAction(nullptr)
{
	WeaponsManager=CreateDefaultSubobject<UNWeaponsManagerComponent>("WeaponsManager");
}

void ANPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	ASC->GetGameplayAttributeValueChangeDelegate(UNBasicAttributeSets::GetStaminaAttribute()).AddUObject(
		this, &ThisClass::HandleStaminaChanged);
}

void ANPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EIC->BindAction(DashAction, ETriggerEvent::Started, this, &ANPlayerCharacter::Input_Dash);
		
		EIC->BindAction(EquipStaffAction, ETriggerEvent::Started, this, &ANPlayerCharacter::Input_EquipStaff);
		//EIC->BindAction(EquipStaffAction, ETriggerEvent::Completed, this, &ANPlayerCharacter::Input_UnequipStaff);
		EIC->BindAction(EquipAxeAction, ETriggerEvent::Started, this, &ANPlayerCharacter::Input_EquipAxe);
	}
}

void ANPlayerCharacter::Input_Dash()
{
	// if (ASC)
	// {
	// 	ASC->TryActivateAbilityByClass(DashAbility);
	// }

	FHitResult Hit;
	Hit.Location = GetLastMovementInputVector().GetSafeNormal();
	
	FGameplayEventData Payload;
	Payload.EventTag = TAG_Event_ActivateAbility_Dash;
	Payload.TargetData.Add(new FGameplayAbilityTargetData_SingleTargetHit(Hit));
	
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		this,
		Payload.EventTag,
		Payload
	);
}

void ANPlayerCharacter::Input_EquipStaff()
{
	FGameplayEventData Payload;

	Payload.EventTag = TAG_Event_ActivateAbility_EquipWeapon;
	Payload.Instigator = this;

	Payload.TargetTags.AddTag(TAG_Weapon_Ranged_Staff);

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		this,
		Payload.EventTag,
		Payload
	);
}

void ANPlayerCharacter::Input_EquipAxe()
{
	FGameplayEventData Payload;

	Payload.EventTag = TAG_Event_ActivateAbility_EquipWeapon;
	Payload.Instigator = this;

	Payload.TargetTags.AddTag(TAG_Weapon_Melee_Axe);

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		this,
		Payload.EventTag,
		Payload
	);
}

// void ANPlayerCharacter::Input_UnequipStaff()
// {
// 	WeaponsManager->UnEquipWeapon();
// }

void ANPlayerCharacter::HandleStaminaChanged(const FOnAttributeChangeData& Data)
{
	float NewValue= Data.NewValue;
	float OldValue= Data.OldValue;
	
	if (NewValue<OldValue)
	{
		if (RegenStaminaGE)
		{
			ASC->RemoveActiveEffectsWithGrantedTags(FGameplayTagContainer(TAG_Status_Stamina_Regen));
			
			//ASC->ApplyGameplayEffectToSelf(RegenStaminaGE->GetDefaultObject<UGameplayEffect>(), 1.0f, ASC->MakeEffectContext());
			ASC->BP_ApplyGameplayEffectToSelf(RegenStaminaGE, 1.0f, ASC->MakeEffectContext());
		}
	}
	else
	{
		//float MaxStamina=ASC->GetNumericAttribute(UNBasicAttributeSets::GetMaxStaminaAttribute());
		float MaxStamina = BasicAttributeSets->MaxStamina.GetCurrentValue();
		if (NewValue>=MaxStamina)
		{
			//ASC->RemoveActiveGameplayEffect(ActiveSpecHandle);
			ASC->RemoveActiveEffectsWithGrantedTags(FGameplayTagContainer(TAG_Status_Stamina_Regen));
		}
	}
}
