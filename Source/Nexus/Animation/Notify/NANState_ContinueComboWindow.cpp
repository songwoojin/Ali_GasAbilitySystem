// Fill out your copyright notice in the Description page of Project Settings.


#include "Nexus/Animation/Notify/NANState_ContinueComboWindow.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Nexus/GameplayAbilitySystem/NGameplayTagContainer.h"

UNANState_ContinueComboWindow::UNANState_ContinueComboWindow()
{
}

FString UNANState_ContinueComboWindow::GetNotifyName_Implementation() const
{
	return TEXT("Continue Combo Window");
}

void UNANState_ContinueComboWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	if (MeshComp)
	{
		AActor* Owner = MeshComp->GetOwner();
		if (Owner)
		{
			FGameplayEventData Payload;
			Payload.EventTag = TAG_Event_ContinueCombo_Start;
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Owner,Payload.EventTag,Payload);
		}
	}
}

void UNANState_ContinueComboWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		AActor* Owner = MeshComp->GetOwner();
		if (Owner)
		{
			FGameplayEventData Payload;
			Payload.EventTag = TAG_Event_ContinueCombo_End;
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Owner,Payload.EventTag,Payload);
		}
	}
}
