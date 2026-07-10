// Fill out your copyright notice in the Description page of Project Settings.


#include "Nexus/Widget/Talent/NW_Talent.h"

#include "Components/Border.h"
#include "Components/Image.h"
#include "Nexus/GameplayAbilitySystem/Talent/PDA_Talent.h"
#include "Components/TextBlock.h"
#include "Nexus/GameplayAbilitySystem/Talent/NTalentTreeComponent.h"
#include "Components/Button.h"
#include "Components/Overlay.h"

void UNW_Talent::NativeConstruct()
{
	Super::NativeConstruct();

	TalentTreeComponent = GetOwningPlayerPawn()->FindComponentByClass<UNTalentTreeComponent>();

	if (Button)
	{
		Button->OnClicked.AddDynamic(this, &UNW_Talent::OnTalentButtonClicked);
	}

	SetTalentInfo();
	TalentTreeComponent->OnTalentsChanged.AddDynamic(this, &UNW_Talent::SetTalentInfo);
}

void UNW_Talent::OnTalentButtonClicked()
{
	if (!IsValid(TalentTreeComponent))	return;
	if (!IsValid(TalentDA)) return;
	
	TalentTreeComponent->Server_SpendPointOnTalent(TalentDA);
}

void UNW_Talent::SetTalentInfo()
{
	if (!TalentDA)
	{
		return;
	}
	
	TalentIcon->SetBrushFromTexture(TalentDA->Icon);
	TalentName->SetText(FText::FromName(TalentDA->Name));

	if (!TalentTreeComponent)
	{
		return;
	}
	
	FGrantedTalent* GrantedTalent=nullptr;
	bool Success=false;
	TalentTreeComponent->FindGrantedTalent(TalentDA,GrantedTalent,Success);

	if (!GrantedTalent)
	{
		return;
	}
	
	if (Success)
	{
		TalentIcon->SetColorAndOpacity(ActiveColor);
	}
	
	int32 CurrentLevel =  GrantedTalent->Level;
	int32 MaxLevel = GrantedTalent->Talent->MaxLevel;
	LevelText->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"),CurrentLevel,MaxLevel)));

	if (TalentTreeComponent->CanSpendPointsOnTalent(TalentDA))
	{
		Overlay->SetRenderOpacity(1.0f);
	}
	else
	{
		Overlay->SetRenderOpacity(0.2f);
	}
}
