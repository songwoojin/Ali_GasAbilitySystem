// Fill out your copyright notice in the Description page of Project Settings.


#include "Nexus/GameplayAbilitySystem/Characters/Enemy/NEnemy_Base.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Nexus/GameplayAbilitySystem/NGameplayTagContainer.h"
#include "Nexus/GameplayAbilitySystem/NAbilitySystemComponent.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "Nexus/Widget/NEnemyHPBar.h"

ANEnemy_Base::ANEnemy_Base()
	:AttackDuration(4.0f)
{
	HPWidgetComponent =CreateDefaultSubobject<UWidgetComponent>(TEXT("HPWidgetComponent"));
	HPWidgetComponent->SetupAttachment(GetMesh());
	HPWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	HPWidgetComponent->SetDrawAtDesiredSize(true);
}

void ANEnemy_Base::BeginPlay()
{
	Super::BeginPlay();

	if (HPWidgetClass)
	{
		HPWidgetComponent->SetWidgetClass(HPWidgetClass);
		HPWidgetComponent->InitWidget();
		UNEnemyHPBar* HPWidget = Cast<UNEnemyHPBar>(HPWidgetComponent->GetUserWidgetObject());
		if (HPWidget)
		{
			HPWidget->SetOwningActor(this);
		}
	}
}

void ANEnemy_Base::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	GetWorld()->GetTimerManager().SetTimerForNextTick(
			this,
			&ANEnemy_Base::SendWeaponEquipEvent
		);
}

AActor* ANEnemy_Base::GetAttackTarget()
{
	ACharacter* PlayerCharacter=UGameplayStatics::GetPlayerCharacter(GetWorld(),0);
	
	return PlayerCharacter;

}

void ANEnemy_Base::SendWeaponEquipEvent()
{
	FGameplayEventData Payload;

	Payload.EventTag = TAG_Event_ActivateAbility_EquipWeapon;
	Payload.Instigator = this;

	//Payload.TargetTags.AddTag(TAG_Weapon_Melee_Axe);
	Payload.TargetTags.AddTag(TAG_Weapon_Ranged_Staff);

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		this,
		Payload.EventTag,
		Payload
	);

	GetWorld()->GetTimerManager().SetTimer(
		AttackTimerHandle,
		this,
		&ANEnemy_Base::Attack,
		AttackDuration,
		true
	);
}

void ANEnemy_Base::Attack()
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		AActor* AttackTarget = GetAttackTarget();
		if (AttackTarget)
		{
			AIController->SetFocus(AttackTarget);
		}
		
	}
	
	{
		TArray<FGameplayTag> AttackTags =
		{
			FGameplayTag::RequestGameplayTag(TEXT("GameplayAbility.ShootProjectile")),
			FGameplayTag::RequestGameplayTag(TEXT("GameplayAbility.AOEAttack"))
		};

		const int32 Index = FMath::RandRange(0, AttackTags.Num()-1);

		FGameplayTagContainer AbilityTags;
		AbilityTags.AddTag(AttackTags[Index]);

		ASC->TryActivateAbilitiesByTag(AbilityTags);
	}

	/*
	{
	 	FGameplayTagContainer AbilityTags;
		AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("GameplayAbility.MeleeAttack")));
		TArray<FGameplayAbilitySpecHandle> OutAbilityHandles;
		ASC->FindAllAbilitiesWithTags(OutAbilityHandles, AbilityTags);
		if (OutAbilityHandles.Num() <= 0)
		{
			return;
		}

		const int32 RandomIndex = FMath::RandRange(0, OutAbilityHandles.Num() - 1);
		ASC->TryActivateAbility(OutAbilityHandles[RandomIndex], true);
	}
	*/
}

void ANEnemy_Base::HandleDeath()
{
	Super::HandleDeath();

	if (HPWidgetComponent)
	{
		HPWidgetComponent->DestroyComponent();
	}
}
