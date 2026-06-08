// Fill out your copyright notice in the Description page of Project Settings.


#include "Nexus/Animation/Notify/NAN_SendGameplayEvent.h"
#include "AbilitySystemBlueprintLibrary.h"

void UNAN_SendGameplayEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp)
	{
		return;
	}

	AActor* Owner = MeshComp->GetOwner();

	if (!Owner)
	{
		return;
	}

	FGameplayEventData Payload;
	Payload.Instigator = Owner;
	Payload.Target = Owner;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		Owner,
		EventTag,
		Payload
	);
}
