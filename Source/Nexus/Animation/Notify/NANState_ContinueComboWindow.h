// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "NANState_ContinueComboWindow.generated.h"

/**
 * 
 */
UCLASS()
class NEXUS_API UNANState_ContinueComboWindow : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UNANState_ContinueComboWindow();

protected:
	virtual FString GetNotifyName_Implementation() const override;
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, const FAnimNotifyEventReference& EventReference) override;
};
