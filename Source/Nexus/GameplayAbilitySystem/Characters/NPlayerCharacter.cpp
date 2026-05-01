// Fill out your copyright notice in the Description page of Project Settings.


#include "NPlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Abilities/Tasks/AbilityTask_WaitAttributeChange.h"
#include "Nexus/GameplayAbilitySystem/AttributeSets/NBasicAttributeSets.h"
#include "Nexus/GameplayAbilitySystem/NGameplayTagContainer.h"

ANPlayerCharacter::ANPlayerCharacter()
	:DashAction(nullptr)
{
}

void ANPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	GiveAbility();

	ASC->GetGameplayAttributeValueChangeDelegate(UNBasicAttributeSets::GetStaminaAttribute()).AddUObject(
		this, &ThisClass::HandleStaminaChanged);
}

void ANPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EIC->BindAction(DashAction, ETriggerEvent::Started, this, &ANPlayerCharacter::Input_Dash);
	}
}

void ANPlayerCharacter::GiveAbility()
{
	if (HasAuthority() && DashAbility)
	{
		ASC->GiveAbility(FGameplayAbilitySpec(DashAbility,1,0));
	}
}

void ANPlayerCharacter::Input_Dash()
{
	if (ASC)
	{
		ASC->TryActivateAbilityByClass(DashAbility);
	}
}

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
