// Fill out your copyright notice in the Description page of Project Settings.


#include "NAbilitySystemComponent.h"
#include "Characters/NCharacterBase.h"

UNAbilitySystemComponent::UNAbilitySystemComponent()
{
}

void UNAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();

	ANCharacterBase* Character=Cast<ANCharacterBase>(GetOwner());
	if (!Character)	return;
	
	if (LastActivatableAbilities.Num() !=ActivatableAbilities.Items.Num())
	{
		Character->SendAbilitiesChangedEvent();
	}
	else
	{
		for (int32 i=0; i<ActivatableAbilities.Items.Num(); ++i)
		{
			if (LastActivatableAbilities[i].Ability !=ActivatableAbilities.Items[i].Ability ||
				LastActivatableAbilities[i].Level !=ActivatableAbilities.Items[i].Level ||
				LastActivatableAbilities[i].InputID !=ActivatableAbilities.Items[i].InputID)
			{
				Character->SendAbilitiesChangedEvent();
				break;
			}
		}
	}
	
	LastActivatableAbilities=ActivatableAbilities.Items;
}
