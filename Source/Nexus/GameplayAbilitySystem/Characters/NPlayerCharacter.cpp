// Fill out your copyright notice in the Description page of Project Settings.


#include "NPlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

ANPlayerCharacter::ANPlayerCharacter()
	:DashAction(nullptr)
{
}

void ANPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	GiveAbility();
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
