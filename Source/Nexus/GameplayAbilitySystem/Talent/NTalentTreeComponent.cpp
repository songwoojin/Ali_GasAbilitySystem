// Fill out your copyright notice in the Description page of Project Settings.


#include "Nexus/GameplayAbilitySystem/Talent/NTalentTreeComponent.h"
#include "PDA_Talent.h"
#include "Abilities/GameplayAbility.h"
#include "Net/UnrealNetwork.h"
#include "Nexus/GameplayAbilitySystem/Characters/NCharacterBase.h"

UNTalentTreeComponent::UNTalentTreeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

void UNTalentTreeComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OwnerCharacter=Cast<ANCharacterBase>(GetOwner());
	OwnerASC=OwnerCharacter?OwnerCharacter->GetAbilitySystemComponent():nullptr;
	
}

void UNTalentTreeComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UNTalentTreeComponent, GrantedTalents);
	DOREPLIFETIME(UNTalentTreeComponent, PointsAvailable);
}

void UNTalentTreeComponent::OnRep_GrantedTalents()
{
	OnTalentsChanged.Broadcast();
}

bool UNTalentTreeComponent::GrantTalent(UPDA_Talent* Talent,int32 StartingLevel)
{
	if (!CanGiveTalent(Talent))
	{
		return false;
	}

	StartingLevel = FMath::Clamp(StartingLevel, 1, Talent->MaxLevel);
	
	GiveAbilitiesToOwner(Talent,Talent->AbilitiesToGrant,StartingLevel);
	ApplyGameplayEffectsToOwner(Talent,Talent->EffectsToApply,StartingLevel);
	GrantedTalents.Add(FGrantedTalent{Talent,StartingLevel});

	return true;
}

void UNTalentTreeComponent::GiveAbilitiesToOwner(UPDA_Talent* Talent,TArray<TSubclassOf<UGameplayAbility>> Abilities,int32 Level)
{
	TArray<FGameplayAbilitySpecHandle> GrantedAbilities = OwnerCharacter->GrantAbilities(Abilities,Level);

	AbilitySpecsGrantedByTalent.Add(Talent,FTalentAbilityHandles{GrantedAbilities});
}

void UNTalentTreeComponent::ApplyGameplayEffectsToOwner(UPDA_Talent* Talent,TArray<TSubclassOf<UGameplayEffect>> GameplayEffect,int32 Level)
{
	for (TSubclassOf<UGameplayEffect> EffectClass : GameplayEffect)
	{
		if (OwnerASC && EffectClass)
		{
			FGameplayEffectSpecHandle SpecHandle = OwnerASC->MakeOutgoingSpec(EffectClass, Level, OwnerASC->MakeEffectContext());
			if (SpecHandle.IsValid())
			{
				FActiveGameplayEffectHandle ActiveGameplayEffectHandle = OwnerASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
				AppliedEffectHandles.AddUnique(ActiveGameplayEffectHandle);
			}
		}
	}

	EffectiveHandlesAppliedByTalent.Add(Talent, FTalentEffectHandles{AppliedEffectHandles});
}

void UNTalentTreeComponent::FindGrantedTalent(UPDA_Talent* Talent,FGrantedTalent*& OutGrantedTalent,bool& Success)
{
	for (FGrantedTalent& GrantedTalent : GrantedTalents)
	{
		if (GrantedTalent.Talent == Talent)
		{
			OutGrantedTalent=&GrantedTalent;
			Success=true;
			return;
		}
	}

	Success=false;
}

bool UNTalentTreeComponent::CanSpendPointsOnTalent(UPDA_Talent* Talent)
{
	FGrantedTalent* GrantedTalent;
	bool Success=false;
	FindGrantedTalent(Talent,GrantedTalent,Success);

	if (Success)
	{
		return CanLevelUpTalent(GrantedTalent);
	}
	else
	{
		return CanGiveTalent(Talent);
	}
}

void UNTalentTreeComponent::DeductTalentPoints()
{
	PointsAvailable= PointsAvailable-1;
	OnRep_AvailablePointsChanged();
}

bool UNTalentTreeComponent::CanGiveTalent(UPDA_Talent* Talent)
{
	FGrantedTalent* GrantedTalent;
	bool Success=false;
	FindGrantedTalent(Talent,GrantedTalent,Success);

	return !Success && PointsAvailable>0;
}

bool UNTalentTreeComponent::CanLevelUpTalent(FGrantedTalent* GrantedTalent)
{
	return (GrantedTalent->Talent->MaxLevel > GrantedTalent->Level) && PointsAvailable>0;
}

void UNTalentTreeComponent::OnRep_AvailablePointsChanged()
{
	OnPointsChanged.Broadcast();
}

bool UNTalentTreeComponent::LevelUpGrantedTalent(FGrantedTalent* GrantedTalent)
{
	if (!CanLevelUpTalent(GrantedTalent))
	{
		return false;
	}

	IncrementGrantedTalentLevel(GrantedTalent);
	SetLevelOfAbilitiesGrantedByTalent(GrantedTalent->Talent);
	SetLevelOfEffectsGrantedByTalent(GrantedTalent->Talent);
	
	return true;
}

void UNTalentTreeComponent::IncrementGrantedTalentLevel(FGrantedTalent* GrantedTalent)
{
	FGrantedTalent OldGrantedTalent = *GrantedTalent;
	
	FGrantedTalent NewGrantedTalent={
		GrantedTalent->Talent,
		GrantedTalent->Level+1
	};
	
	GrantedTalents.AddUnique(NewGrantedTalent);
	GrantedTalents.Remove(OldGrantedTalent);

	//Trigger
}

void UNTalentTreeComponent::SetLevelOfAbilitiesGrantedByTalent(UPDA_Talent* Talent)
{
	FGrantedTalent* GrantedTalent;
	bool Success=false;
	FindGrantedTalent(Talent,GrantedTalent,Success);

	if (FTalentAbilityHandles* TalentAbilityHandles = AbilitySpecsGrantedByTalent.Find(Talent))
	{
		for (FGameplayAbilitySpecHandle AbilitySpecHandle : TalentAbilityHandles->AbilitySpecHandles)
		{
			if (OwnerASC && AbilitySpecHandle.IsValid())
			{
				FGameplayAbilitySpec* Spec = OwnerASC->FindAbilitySpecFromHandle(AbilitySpecHandle);
				Spec->Level=GrantedTalent->Level;
				OwnerASC->MarkAbilitySpecDirty(*Spec);
				// UNGameplayAbilty* NexusAbility = Cast<UNGameplayAbilty>(Spec->Ability);
				// if (NexusAbility)
				// {
				// 	NexusAbility->SetAbilityLevel(GrantedTalent->Level);
				// }
			}
		}
	}
}

void UNTalentTreeComponent::SetLevelOfEffectsGrantedByTalent(UPDA_Talent* Talent)
{
	FGrantedTalent* GrantedTalent;
	bool Success=false;
	FindGrantedTalent(Talent,GrantedTalent,Success);
	
	if (FTalentEffectHandles* TalentEffectHandles =EffectiveHandlesAppliedByTalent.Find(Talent))
	{
		for (FActiveGameplayEffectHandle ActiveEffectHandle : TalentEffectHandles->GameplayEffectHandles)
		{
			if (OwnerASC && ActiveEffectHandle.IsValid())
			{
				OwnerASC->SetActiveGameplayEffectLevel(ActiveEffectHandle, GrantedTalent->Level);
			}
		}
	}
}

void UNTalentTreeComponent::Server_SpendPointOnTalent_Implementation(UPDA_Talent* Talent)
{
	FGrantedTalent* GrantedTalent;
	bool Success=false;
	FindGrantedTalent(Talent,GrantedTalent,Success);
	
	if (Success)
	{
		if (LevelUpGrantedTalent(GrantedTalent))
		{
			DeductTalentPoints();
		}
	}
	else
	{
		if (GrantTalent(Talent,1))
		{
			DeductTalentPoints();
		}
	}

	OnRep_GrantedTalents();
}




