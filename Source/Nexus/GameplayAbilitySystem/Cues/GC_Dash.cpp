// Fill out your copyright notice in the Description page of Project Settings.


#include "GC_Dash.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Nexus/GameplayAbilitySystem/NGameplayTagContainer.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"

UGC_Dash::UGC_Dash()
	:Character(nullptr)
{
	
}

void UGC_Dash::HandleGameplayCue(AActor* MyTarget, EGameplayCueEvent::Type EventType,
	const FGameplayCueParameters& Parameters)
{
	Super::HandleGameplayCue(MyTarget, EventType, Parameters);

	UE_LOG(LogTemp, Log, TEXT("UGC_Dash::HandleGameplayCue"));

	Character= Cast<ACharacter>(MyTarget);

	if (EventType==EGameplayCueEvent::Executed)
	{
		UE_LOG(LogTemp, Warning, TEXT("Execute Dash Cue"));
	}
	else if (EventType==EGameplayCueEvent::OnActive)
	{
		UE_LOG(LogTemp, Warning, TEXT("Active Dash Cue"));
	}
	else if (EventType==EGameplayCueEvent::Removed)
	{
		
	}
}

bool UGC_Dash::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	UE_LOG(LogTemp, Log, TEXT("OnExecute_Implementation"));
	
	return Super::OnExecute_Implementation(MyTarget, Parameters);
}

bool UGC_Dash::OnActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	 // UGameplayStatics::SpawnEmitterAttached(
	 // DashStartParticle,
	 // MyTarget->GetRootComponent(),
	 // NAME_None,
	 // FVector::ZeroVector,
	 // FRotator::ZeroRotator,
	 // FVector(0.3f,0.3f,0.3f)
	 // );

	UNiagaraFunctionLibrary::SpawnSystemAttached(
	DashStartNiagara,
	MyTarget->GetRootComponent(),
	NAME_None,
	FVector::ZeroVector,
	FRotator::ZeroRotator,
	EAttachLocation::KeepRelativeOffset,
	true
);
	
	 UGameplayStatics::PlaySoundAtLocation(
	 GetWorld(),
	 TeleportStartSound,
	 MyTarget->GetActorLocation()
	 );
	
	 if (IsValid(Character))
	 {
	 	Character->GetMesh()->SetVisibility(false,true);	
	 }
	
	 UE_LOG(LogTemp, Warning, TEXT("Active Dash Cue"));
	
	return Super::OnActive_Implementation(MyTarget, Parameters);
}

bool UGC_Dash::OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	UE_LOG(LogTemp, Log, TEXT("OnRemove_Implementation"));
	
	// UGameplayStatics::SpawnEmitterAtLocation(
	// 	GetWorld(),
	// 	DashEndParticle,
	// 	MyTarget->GetActorLocation(),
	// 	MyTarget->GetActorRotation()
	// );

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
	GetWorld(),
	DashEndNiagara,
	MyTarget->GetActorLocation()
);

	UGameplayStatics::PlaySoundAtLocation(
	GetWorld(),
	TeleportEndSound,
	MyTarget->GetActorLocation()
	);

	if (IsValid(Character))
	{
		Character->GetMesh()->SetVisibility(true,true);	
	}
	
	return Super::OnRemove_Implementation(MyTarget, Parameters);
}
