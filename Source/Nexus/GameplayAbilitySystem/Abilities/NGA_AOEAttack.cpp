// Fill out your copyright notice in the Description page of Project Settings.


#include "Nexus/GameplayAbilitySystem/Abilities/NGA_AOEAttack.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
#include "Abilities/GameplayAbilityTargetActor_GroundTrace.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Nexus/GameplayAbilitySystem/NGameplayTagContainer.h"
#include "Nexus/GameplayAbilitySystem/TargetActors/NGA_TargetActorGroundTrace_Decal.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"
#include "Nexus/Interface/NTargetingInterface.h"

UNGA_AOEAttack::UNGA_AOEAttack()
	:TargetingMontage(nullptr)
	,bIsWaitingTargetData(false)
	,AttackMaxRange(2000.0f)
	,AOERadius(200.0f)
	,DamageAmount(-50.0f)
{
}

void UNGA_AOEAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                     const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                     const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (HasPC())
	{
		bIsWaitingTargetData=true;
		LoopTargetingAnimation();
		WaitTargetData();
		WaitCancleInput();
	}
	else
	{
		INTargetingInterface* TargetingInterface = Cast<INTargetingInterface>(GetAvatarActorFromActorInfo());
		if (TargetingInterface)
		{
			AActor* TargetActor = TargetingInterface->GetAttackTarget();
			if (TargetActor)
			{
				ConfirmedAOELocation = GetTargetGoundLocation(TargetActor);
				ConfirmStrike();
			}
		}
	}
}

void UNGA_AOEAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	GetAbilitySystemComponentFromActorInfo()->RemoveGameplayCue(
		FGameplayTag::RequestGameplayTag(FName("GameplayCue.AOEIndicator")));

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UNGA_AOEAttack::LoopTargetingAnimation()
{
	if (!TargetingMontage)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}
	
	UAbilityTask_PlayMontageAndWait* MontageTask =
	   UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		   this,
		   NAME_None,
		   TargetingMontage);

	if (!MontageTask)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}
	
	MontageTask->OnBlendOut.AddDynamic(this,&UNGA_AOEAttack::OnMontageBlendOut);

	MontageTask->OnInterrupted.AddDynamic(
		this,
		&UNGA_AOEAttack::OnMontageInterrupted);
	
	MontageTask->ReadyForActivation();
}

void UNGA_AOEAttack::OnMontageBlendOut()
{
	if (bIsWaitingTargetData)
	{
		LoopTargetingAnimation();
	}
	
}

void UNGA_AOEAttack::OnMontageInterrupted()
{
	if (bIsWaitingTargetData)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

void UNGA_AOEAttack::WaitTargetData()
{
	UAbilityTask_WaitTargetData* WaitTargetDataTask  = UAbilityTask_WaitTargetData::WaitTargetData(
		this,
		NAME_None,
		EGameplayTargetingConfirmation::UserConfirmed,
		GroundTraceTargetActorClass
		);
	
	if (!WaitTargetDataTask)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	AGameplayAbilityTargetActor* SpawnedActor = nullptr;

	WaitTargetDataTask->BeginSpawningActor(
		this,
		GroundTraceTargetActorClass,
		SpawnedActor);

	ANGA_TargetActorGroundTrace_Decal* GroundTraceActor =
		Cast<ANGA_TargetActorGroundTrace_Decal>(SpawnedActor);

	if (GroundTraceActor)
	{
		GroundTraceActor->StartLocation.LocationType = EGameplayAbilityTargetingLocationType::ActorTransform;
		GroundTraceActor->StartLocation.SourceActor = GetAvatarActorFromActorInfo();
		GroundTraceActor->MaxRange =AttackMaxRange;
		GroundTraceActor->bTraceAffectsAimPitch = true;
		GroundTraceActor->bDebug = true;

		GroundTraceActor->SetDecalSize(AOERadius);
	}
	
	WaitTargetDataTask->ValidData.AddDynamic(
		this,
		&ThisClass::OnTargetDataReady);

	WaitTargetDataTask->Cancelled.AddDynamic(
		this,
		&ThisClass::OnTargetDataCancelled);

	WaitTargetDataTask->FinishSpawningActor(
		this,
		SpawnedActor);


	WaitTargetDataTask->ReadyForActivation();
}

void UNGA_AOEAttack::OnTargetDataReady(const FGameplayAbilityTargetDataHandle& Data)
{
	if (Data.Num() <= 0 || !Data.Get(0))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	const FHitResult* HitResult = Data.Get(0)->GetHitResult();

	if (!HitResult)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}
	bIsWaitingTargetData=false;

	ConfirmedAOELocation = HitResult->Location;

	ConfirmStrike();
}

void UNGA_AOEAttack::OnTargetDataCancelled(const FGameplayAbilityTargetDataHandle& Data)
{

}

void UNGA_AOEAttack::ConfirmStrike()
{
	if (!CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
	
	if (!ConfirmMontage)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	FGameplayCueParameters CueParams;
	CueParams.RawMagnitude = AOERadius;
	CueParams.Location= ConfirmedAOELocation;
	GetAbilitySystemComponentFromActorInfo()->AddGameplayCue(
		FGameplayTag::RequestGameplayTag(FName("GameplayCue.AOEIndicator")),
		CueParams);
		
	UAbilityTask_PlayMontageAndWait* MontageTask =
	   UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		   this,
		   NAME_None,
		   ConfirmMontage);

	if (!MontageTask)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}
	
	MontageTask->OnCompleted.AddDynamic(this,&UNGA_AOEAttack::OnConfirmMontageCompleted);

	MontageTask->OnInterrupted.AddDynamic(
		this,
		&UNGA_AOEAttack::OnConfirmMontageCompleted);
	
	MontageTask->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		FGameplayTag(TAG_Event_Montage_Trigger)
	);

	WaitEventTask->EventReceived.AddDynamic(
		this,
		&UNGA_AOEAttack::OnEventReceived);
	
	WaitEventTask->ReadyForActivation();
}

void UNGA_AOEAttack::OnConfirmMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UNGA_AOEAttack::OnEventReceived(FGameplayEventData Payload)
{
	if (!HasAuthority(&CurrentActivationInfo))
	{
		return;
	}

	// GetAbilitySystemComponentFromActorInfo()->RemoveGameplayCue(
	// 	FGameplayTag::RequestGameplayTag(FName("GameplayCue.AOEIndicator")));
	
	const FVector Start = ConfirmedAOELocation;
	const FVector End   = ConfirmedAOELocation;	

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetAvatarActorFromActorInfo());

	TArray<FHitResult> HitResults;

	const bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
		GetWorld(),
		Start,
		End,
		AOERadius,
		ObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		HitResults,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		2.0f
	);

	if (!bHit)
	{
		return;
	}

	TSet<AActor*> HitActors;

	for (const FHitResult& Hit : HitResults)
	{
		AActor* HitActor = Hit.GetActor();

		if (!HitActor)
		{
			continue;
		}

		if (HitActors.Contains(HitActor))
		{
			continue;
		}

		HitActors.Add(HitActor);

		// 여기서 데미지 적용
		ApplyDamageToTarget(HitActor);
	}
}

void UNGA_AOEAttack::ApplyDamageToTarget(AActor* TargetActor)
{
	FGameplayEffectSpecHandle SpecHandle =
	MakeOutgoingGameplayEffectSpec(
		DamageEffectClass,
		1.f);

	SpecHandle.Data->SetSetByCallerMagnitude(
	TAG_Data_Damage,
	DamageAmount);

	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!TargetASC)
	{
		return;
	}
	
	TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

FVector UNGA_AOEAttack::GetTargetGoundLocation(AActor* AttackTarget)
{
	FHitResult HitResult;

	FVector Start = AttackTarget->GetActorLocation();
	FVector End = Start - FVector(0.0f, 0.0f, 9999.0f);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetAvatarActorFromActorInfo());

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Visibility,
		Params
	);
	

	if (bHit)
	{
		return HitResult.Location;
	}

	return Start;
}

void UNGA_AOEAttack::WaitCancleInput()
{
	UAbilityTask_WaitInputPress* WaitTask=UAbilityTask_WaitInputPress::WaitInputPress(this);
	if (WaitTask)
	{
		WaitTask->OnPress.AddDynamic(this,&UNGA_AOEAttack::OnInputPressed);
		WaitTask->ReadyForActivation();
	}
}

void UNGA_AOEAttack::OnInputPressed(float WaitedTime)
{
	CancelAbility(CurrentSpecHandle,CurrentActorInfo,CurrentActivationInfo,true);
}
