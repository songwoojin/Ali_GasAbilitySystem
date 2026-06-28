// Fill out your copyright notice in the Description page of Project Settings.


#include "NDashAbility.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionConstantForce.h"
#include "GameFramework/RootMotionSource.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../NGameplayTagContainer.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UNDashAbility::UNDashAbility()
	:Strength(2000.0f)
	,Duration(0.3f)
{
	//Dash는 움직임 자체인데 MovementComponent는 알아서 리플리케이션이 되기 때문
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateNo;

	//하나의 인스턴스만 생성
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UNDashAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	CommitAbilityCost(Handle, ActorInfo, ActivationInfo);

	/*
	if (!TriggerEventData)
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetDataHandle is invalid"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}
	
	if (!TriggerEventData->TargetData.IsValid(0))
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetDataHandle is invalid"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}
	
	const FHitResult* Hit = TriggerEventData->TargetData.Get(0)->GetHitResult();
	if (!Hit)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}
	
	FVector HitLocation = Hit->Location;
	if (HitLocation==FVector::ZeroVector)
	{
		HitLocation+=GetAvatarActorFromActorInfo()->GetActorForwardVector();
	}
	*/
	
	//UE_LOG(LogTemp, Warning, TEXT("Active Dash Ability"));
	UAbilityTask_ApplyRootMotionConstantForce* Task =
		UAbilityTask_ApplyRootMotionConstantForce::ApplyRootMotionConstantForce(
			this,
			NAME_None,
			GetDashDirection(),
			Strength,     // Strength
			Duration,       // Duration
			false,      // bIsAdditive
			nullptr,
			ERootMotionFinishVelocityMode::ClampVelocity
			,FVector(0.0f,0.0f,0.0f)
			,GetMaxSpeed()
			,false
		);

	if (Task)
	{
		Task->OnFinish.AddDynamic(this, &UNDashAbility::OnDashFinished);
		//태스크 실행
		Task->ReadyForActivation();
	}

	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	if (ASC)
	{
		//UE_LOG(LogTemp, Warning, TEXT("ASC Owner: %s"), *GetNameSafe(ASC->GetOwnerActor()));
		//UE_LOG(LogTemp, Warning, TEXT("ASC Avatar: %s"), *GetNameSafe(ASC->GetAvatarActor()));
		
		//ASC->ExecuteGameplayCue(TAG_GameplayCue_Dash_Active);
		ASC->AddGameplayCue(TAG_GameplayCue_Dash_Active);
		//ASC->AddGameplayCue(FGameplayTag::RequestGameplayTag(TAG_GameplayCue_Dash_Active));
		//UE_LOG(LogTemp, Warning, TEXT("Executed Gameplay Cue"));
	}
}

void UNDashAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (ASC)
	{
		ASC->RemoveGameplayCue(TAG_GameplayCue_Dash_Active);
	}

	CommitAbilityCooldown(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, nullptr);
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

// FVector UNDashAbility::GetDashDirection()
// {
// 	APawn* Pawn=Cast<APawn>(GetAvatarActorFromActorInfo());
// 	if (!Pawn)
// 	{
// 		return GetAvatarActorFromActorInfo()->GetActorForwardVector();
// 	}
// 	
// 	if (Pawn->GetLastMovementInputVector().IsZero())
// 	{
// 		return GetAvatarActorFromActorInfo()->GetActorForwardVector();
// 	}
// 	else
// 	{
// 		return Pawn->GetLastMovementInputVector().GetSafeNormal();
// 	}
// }

float UNDashAbility::GetMaxSpeed()
{
	APawn* Pawn=Cast<APawn>(GetAvatarActorFromActorInfo());
	if (Pawn)
	{
		return Pawn->GetMovementComponent()->GetMaxSpeed();
	}

	return 500.0f;
}

void UNDashAbility::OnDashFinished()
{
	//CommitAbilityCooldown(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, nullptr);
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

FVector UNDashAbility::GetDashDirection()
{
	FVector TargetLocation=GetAvatarActorFromActorInfo()->GetActorForwardVector();
	
	ACharacter* NCharacter = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	if (!NCharacter)
	{
		return TargetLocation;
	}

	TargetLocation = NCharacter->GetCharacterMovement()->GetCurrentAcceleration();
	if (TargetLocation==FVector::ZeroVector)
	{
		return GetAvatarActorFromActorInfo()->GetActorForwardVector();
	}

	return TargetLocation;
}

bool UNDashAbility::CommitAbilityCost(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                      FGameplayTagContainer* OptionalRelevantTags)
{
	return Super::CommitAbilityCost(Handle, ActorInfo, ActivationInfo, OptionalRelevantTags);
}

bool UNDashAbility::CommitAbilityCooldown(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const bool ForceCooldown, FGameplayTagContainer* OptionalRelevantTags)
{
	return Super::CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, ForceCooldown, OptionalRelevantTags);
}
