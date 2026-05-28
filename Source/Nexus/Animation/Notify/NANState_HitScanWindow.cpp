// Fill out your copyright notice in the Description page of Project Settings.


#include "Nexus/Animation/Notify/NANState_HitScanWindow.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Nexus/GameplayAbilitySystem/NGameplayTagContainer.h"

UNANState_HitScanWindow::UNANState_HitScanWindow()
{
}

FString UNANState_HitScanWindow::GetNotifyName_Implementation() const
{
	return TEXT("Hit Scan Window");
}

void UNANState_HitScanWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	if (MeshComp)
	{
		AActor* Owner = MeshComp->GetOwner();
		if (Owner)
		{
			FGameplayEventData Payload;
			Payload.EventTag = TAG_Event_HitScan_Start;
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Owner,Payload.EventTag,Payload);
		}
	}
}

void UNANState_HitScanWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		AActor* Owner = MeshComp->GetOwner();
		if (Owner)
		{
			FGameplayEventData Payload;
			Payload.EventTag = TAG_Event_HitScan_End;
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Owner,Payload.EventTag,Payload);
		}
	}
}




