// Fill out your copyright notice in the Description page of Project Settings.


#include "Nexus/GameplayAbilitySystem/Abilities/NGA_ShootProjectileBsse.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
#include "Abilities/GameplayAbilityTargetActor_SingleLineTrace.h"
#include "DrawDebugHelpers.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Nexus/Projectile/NProjectileBase.h"
#include "Kismet/GameplayStatics.h"
#include "Nexus/Component/NWeaponsManagerComponent.h"
#include "Nexus/GameplayAbilitySystem/NGameplayTagContainer.h"
#include "Nexus/GameplayAbilitySystem/Weapon/NWeapon_Base.h"

UNGA_ShootProjectileBsse::UNGA_ShootProjectileBsse()
	: ShootMontage(nullptr)
{
}

void UNGA_ShootProjectileBsse::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilityTask_PlayMontageAndWait* Task =
	   UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		   this,
		   NAME_None,
		   ShootMontage
	   );

	if (Task)
	{
		Task->OnCompleted.AddDynamic(
			this,
			&UNGA_ShootProjectileBsse::OnMontageCompleted);

		Task->OnInterrupted.AddDynamic(
			this,
			&UNGA_ShootProjectileBsse::OnMontageCompleted);

		Task->OnCancelled.AddDynamic(
			this,
			&UNGA_ShootProjectileBsse::OnMontageCompleted);

		Task->ReadyForActivation();
	}

	UAbilityTask_WaitGameplayEvent* WaitShootProjectileEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this,TAG_Event_ShootProjectile);
	if (WaitShootProjectileEventTask)
	{
		WaitShootProjectileEventTask->EventReceived.AddDynamic(this,&UNGA_ShootProjectileBsse::OnShootProjectileEventReceived);
		WaitShootProjectileEventTask->ReadyForActivation();
	}
}

void UNGA_ShootProjectileBsse::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UNGA_ShootProjectileBsse::OnTargetDataReady(const FGameplayAbilityTargetDataHandle& Data)
{
	if (!GetAvatarActorFromActorInfo()->HasAuthority())
	{
		return;
	}
	
	if (Data.Num() <= 0)
	{
		return;
	}

	const FHitResult* HitResult = Data.Get(0)->GetHitResult();

	if (!HitResult)
	{
		return;
	}

	const FVector TraceStart = HitResult->TraceStart;
	const FVector TraceEnd = HitResult->bBlockingHit
		? HitResult->ImpactPoint
		: HitResult->TraceEnd;

	DrawDebugLine(
		GetWorld(),
		TraceStart,
		TraceEnd,
		FColor::Red,
		false,
		3.0f,
		0,
		2.0f
	);

	DrawDebugSphere(
		GetWorld(),
		TraceEnd,
		20.0f,
		16,
		FColor::Green,
		false,
		3.0f
	);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetAvatarActorFromActorInfo();
	SpawnParams.Instigator = Cast<APawn>(GetAvatarActorFromActorInfo());
	SpawnParams.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector SpawnLocation =GetSpawnLocation();
	FRotator SpawnRotation = FRotator(0.0f,0.0f,0.0f);

	/*
	ANProjectileBase* Projectile =
		GetWorld()->SpawnActor<ANProjectileBase>(
			ProjectileClass,
			SpawnLocation,
			SpawnRotation,
			SpawnParams
		);
	*/

	FTransform SpawnTransform(SpawnRotation, SpawnLocation);
	ANProjectileBase* Projectile =
	GetWorld()->SpawnActorDeferred<ANProjectileBase>(
		ProjectileClass,
		SpawnTransform,
		SpawnParams.Owner,
		SpawnParams.Instigator,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);

	if (!Projectile)
	{
		return;
	}
	
	Projectile->SetTargetLocation(TraceEnd);
	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, GetAbilityLevel());
	if (DamageSpecHandle.IsValid())
	{
		DamageSpecHandle.Data->SetSetByCallerMagnitude(
			TAG_Data_Damage,
			DamageMagnitude
		);
	}
	Projectile->SetGameplayDamageEffectSpec(DamageSpecHandle);

	UGameplayStatics::FinishSpawningActor(
	Projectile,
	SpawnTransform
	);
}

void UNGA_ShootProjectileBsse::OnTargetDataCancelled(const FGameplayAbilityTargetDataHandle& Data)
{
}

void UNGA_ShootProjectileBsse::OnShootProjectileEventReceived(FGameplayEventData Payload)
{
	// ① Task 생성 — 클래스만 넘기고 스폰은 아직 안 됨
	UAbilityTask_WaitTargetData* WaitTargetDataTask = UAbilityTask_WaitTargetData::WaitTargetData(
		this,
		NAME_None,
		EGameplayTargetingConfirmation::Instant,
		//EGameplayTargetingConfirmation::UserConfirmed,
		AGameplayAbilityTargetActor_SingleLineTrace::StaticClass()); // 기본 클래스 그대로

	UE_LOG(LogTemp, Warning, TEXT("WaitTargetData Created"));
		
	WaitTargetDataTask->ValidData.AddDynamic(this, &UNGA_ShootProjectileBsse::OnTargetDataReady);
	WaitTargetDataTask->Cancelled.AddDynamic(this, &UNGA_ShootProjectileBsse::OnTargetDataCancelled);

	AGameplayAbilityTargetActor* SpawnedActor = nullptr;

	if (WaitTargetDataTask->BeginSpawningActor(this, 
			AGameplayAbilityTargetActor_SingleLineTrace::StaticClass(), SpawnedActor))
	{
		// ③ 스폰된 직후 — FinishSpawning 전이므로 BeginPlay 전 상태
		//    여기서 원하는 값을 자유롭게 주입
		AGameplayAbilityTargetActor_SingleLineTrace* LineTraceActor = CastChecked<AGameplayAbilityTargetActor_SingleLineTrace>(SpawnedActor);

		// 트레이스 거리 설정
		LineTraceActor->MaxRange = 999999.0f;
		LineTraceActor->bTraceAffectsAimPitch = true;
		LineTraceActor->StartLocation.LocationType = EGameplayAbilityTargetingLocationType::ActorTransform;
		LineTraceActor->StartLocation.SourceActor = GetCurrentActorInfo()->AvatarActor.Get();

		// 채널, 필터 등 추가 설정
		LineTraceActor->TraceProfile = FCollisionProfileName(TEXT("NoCollision"));
		//LineTraceActor->TraceProfile = FCollisionProfileName(TEXT("BlockAll"));
		//LineTraceActor->CollisionParams.AddIgnoredActor(AvatarActor);


#if ENABLE_DRAW_DEBUG
		LineTraceActor->bDebug = false;
#endif

		// ④ 스폰 완료 — 이후 BeginPlay 호출됨
		WaitTargetDataTask->FinishSpawningActor(this, SpawnedActor);
	}

	// ⑤ Task 활성화 — 내부적으로 트레이스 실행
	WaitTargetDataTask->ReadyForActivation();
}

FVector UNGA_ShootProjectileBsse::GetSpawnLocation()
{
	UNWeaponsManagerComponent* WeaponsManagerComponent = Cast<UNWeaponsManagerComponent>(GetAvatarActorFromActorInfo()->GetComponentByClass(UNWeaponsManagerComponent::StaticClass()));
	if (IsValid(WeaponsManagerComponent))
	{
		if (WeaponsManagerComponent->GetEquippedWeapon())
		{
			FVector ProjectileSpawnLocation = WeaponsManagerComponent->GetEquippedWeapon()->GetSpawnPointLocation();
			if (ProjectileSpawnLocation != FVector::ZeroVector)
			{
				return ProjectileSpawnLocation;
			}
			else
			{
				return GetAvatarActorFromActorInfo()->GetActorLocation();
			}
		}
		else
		{
			return GetAvatarActorFromActorInfo()->GetActorLocation();
		}
	}
	else
	{
		return GetAvatarActorFromActorInfo()->GetActorLocation();
	}
}
