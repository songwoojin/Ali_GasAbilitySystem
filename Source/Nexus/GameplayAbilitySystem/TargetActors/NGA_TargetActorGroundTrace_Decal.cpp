// Fill out your copyright notice in the Description page of Project Settings.


#include "Nexus/GameplayAbilitySystem/TargetActors/NGA_TargetActorGroundTrace_Decal.h"

#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"

ANGA_TargetActorGroundTrace_Decal::ANGA_TargetActorGroundTrace_Decal()
	:DecalMaterial(nullptr)
	,DecalSize(512.0f)
{
}

void ANGA_TargetActorGroundTrace_Decal::BeginPlay()
{
	Super::BeginPlay();

	UDecalComponent* Decal = UGameplayStatics::SpawnDecalAttached(
	DecalMaterial,
	FVector(50.f,DecalSize,DecalSize),
	RootComponent
	);
	
	if (Decal)
	{
		Decal->SetRelativeRotation(
			FRotator(-90.f, 0.f, 0.f));
	}
}

FHitResult ANGA_TargetActorGroundTrace_Decal::PerformTrace(AActor* InSourceActor)
{
	return Super::PerformTrace(InSourceActor);
}
