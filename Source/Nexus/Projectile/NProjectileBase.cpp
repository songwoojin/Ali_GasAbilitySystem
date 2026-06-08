// Fill out your copyright notice in the Description page of Project Settings.


#include "Nexus/Projectile/NProjectileBase.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystemGlobals.h"
#include "Nexus/GameplayAbilitySystem/NGameplayTagContainer.h"

ANProjectileBase::ANProjectileBase()
	:Speed(2000.0f)
{
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision= CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereCollision->SetCollisionObjectType(ECC_WorldDynamic);
	SphereCollision->SetCollisionResponseToAllChannels(ECR_Overlap);
	SphereCollision->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Block);
	SphereCollision->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Block);
	RootComponent=SphereCollision;

	ProjectileMovement=CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bAutoActivate = false;
	ProjectileMovement->ProjectileGravityScale = 0.0f;

	bReplicates = true;
	SetReplicateMovement(true);
}

void ANProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	SphereCollision->OnComponentBeginOverlap.AddDynamic(this,&ANProjectileBase::OnSphereBeginOverlap);
	SphereCollision->OnComponentHit.AddDynamic(this,&ANProjectileBase::OnSphereHit);
	
	ProjectileMovement->Velocity=(TargetLocation-GetActorLocation()).GetSafeNormal()*Speed;
	ProjectileMovement->Activate();

	SpawnProjectileCue();
}

void ANProjectileBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ANProjectileBase, Speed);
	DOREPLIFETIME(ANProjectileBase, TargetLocation);
	
}

void ANProjectileBase::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetInstigator())
	{
		return;
	}

	IAbilitySystemInterface* OtherASI = Cast<IAbilitySystemInterface>(OtherActor);
	if (OtherASI)
	{
		UAbilitySystemComponent* OtherASC=OtherASI->GetAbilitySystemComponent();
		if (OtherASC)
		{
			if (HasAuthority())
			{
				OtherASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
			}
		}
	}

	ImpactProjectileCue();
	Destroy();
}

void ANProjectileBase::OnSphereHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ImpactProjectileCue();
	Destroy();
}

void ANProjectileBase::SpawnProjectileCue()
{
	AActor* OwnerActor = GetOwner();

	UAbilitySystemComponent* OwnerASC =UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwnerActor);

	if (OwnerASC)
	{
		FGameplayCueParameters Params;
		Params.Location = GetActorLocation();
		Params.Instigator = GetInstigator();
		Params.SourceObject = this;

		OwnerASC->ExecuteGameplayCue(GameplayCueSpawn, Params);
	}
}

void ANProjectileBase::ImpactProjectileCue()
{
	AActor* OwnerActor = GetOwner();

	UAbilitySystemComponent* OwnerASC =UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwnerActor);

	if (OwnerASC)
	{
		FGameplayCueParameters Params;
		Params.Location = GetActorLocation();
		Params.Instigator = GetInstigator();
		Params.SourceObject = this;

		OwnerASC->ExecuteGameplayCue(GameplayCueImpact, Params);
	}
}



