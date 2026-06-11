// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor_GroundTrace.h"
#include "NGA_TargetActorGroundTrace_Decal.generated.h"

/**
 * 
 */
UCLASS()
class NEXUS_API ANGA_TargetActorGroundTrace_Decal : public AGameplayAbilityTargetActor_GroundTrace
{
	GENERATED_BODY()

public:
	ANGA_TargetActorGroundTrace_Decal();

	void SetDecalSize(float InDecalSize){DecalSize=InDecalSize;}
	
protected:
	UPROPERTY(EditAnywhere)
	UMaterialInterface* DecalMaterial;

	UPROPERTY(EditAnywhere)
	float DecalSize;
	
protected:
	virtual void BeginPlay() override;
	virtual FHitResult PerformTrace(AActor* InSourceActor) override;
};
