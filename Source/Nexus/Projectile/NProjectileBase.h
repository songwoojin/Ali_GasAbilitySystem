// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameFramework/Actor.h"
#include "NProjectileBase.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class NEXUS_API ANProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ANProjectileBase();
	void SetTargetLocation(const FVector& Location) {TargetLocation=Location;};
	void SetGameplayDamageEffectSpec(const FGameplayEffectSpecHandle& InSpecHandle) {DamageEffectSpecHandle = InSpecHandle;};
	
protected:
	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereCollision;

	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditDefaultsOnly,Replicated)
	float Speed;

	UPROPERTY(Replicated)
	FVector TargetLocation;

	UPROPERTY()
	FGameplayEffectSpecHandle  DamageEffectSpecHandle;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag GameplayCueSpawn;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag GameplayCueImpact;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> DebuffFireEffectClass;
	
protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnSphereBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OnSphereHit(
		UPrimitiveComponent* HitComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit
	);

	void SpawnProjectileCue();
	void ImpactProjectileCue();
};
