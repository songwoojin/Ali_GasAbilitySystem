// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NMovementProperties.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FNMovementProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxWalkSpeed =500.0f ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bOrientRotationToMovement = true ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseControllerDesiredRotation = false;
};