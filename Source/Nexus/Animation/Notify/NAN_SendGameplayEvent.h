// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
#include "NAN_SendGameplayEvent.generated.h"

/**
 * 
 */
UCLASS()
class NEXUS_API UNAN_SendGameplayEvent : public UAnimNotify
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere,Category ="EventTag")
	FGameplayTag EventTag;
	
protected:
	virtual void Notify(
		USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference
	) override;
	
};
