// Fill out your copyright notice in the Description page of Project Settings.


#include "NCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Nexus/GameplayAbilitySystem/AttributeSets/NBasicAttributeSets.h"
#include "Nexus/GameplayAbilitySystem/NGameplayTagContainer.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Nexus/GameplayAbilitySystem/NAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Nexus/GameplayAbilitySystem/Abilities/NGameplayAbilty.h"

// Sets default values
ANCharacterBase::ANCharacterBase()
	:ASC(nullptr)
	,ASCReplicationMode(EGameplayEffectReplicationMode::Mixed)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ASC=CreateDefaultSubobject<UNAbilitySystemComponent>("ASC");
	ASC->SetIsReplicated(true);
	ASC->SetReplicationMode(ASCReplicationMode);

	bUseControllerRotationPitch=false;
	bUseControllerRotationYaw=false;
	bUseControllerRotationRoll=false;

	GetCharacterMovement()->bOrientRotationToMovement=true;
	GetCharacterMovement()->RotationRate=FRotator(0.0f,500.0f,0.0f);

	GetCharacterMovement()->JumpZVelocity=500.0f;
	GetCharacterMovement()->AirControl=0.35f;
	GetCharacterMovement()->MaxWalkSpeed=500.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed=20.0f;
	GetCharacterMovement()->BrakingDecelerationWalking=2000.0f;
	GetCharacterMovement()->BrakingDecelerationFalling=1500.0f;

	BasicAttributeSets=CreateDefaultSubobject<UNBasicAttributeSets>("BasicAttributeSets");
	
}

// Called when the game starts or when spawned
void ANCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (ASC)
	{
		ASC->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag("State.Dead")).
		AddUObject(this,&ANCharacterBase::OnDeadTagChanged);
	}
}

void ANCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (ASC)
	{
		ASC->InitAbilityActorInfo(this,this);
		GrantAbilities(StartingAbilities);
	}
}

TArray<FGameplayAbilitySpecHandle> ANCharacterBase::GrantAbilities(
	TArray<TSubclassOf<UGameplayAbility>> AbilitiesToGrant)
{
	if (!ASC || !HasAuthority())
	{
		return TArray<FGameplayAbilitySpecHandle>();
	}

	TArray<FGameplayAbilitySpecHandle> AbilitiesHandles;

	for (TSubclassOf<UGameplayAbility> AbilityClass : AbilitiesToGrant)
	{
		int32 InputID=-1;
		if (const UNGameplayAbilty* NexusAbilityCDO = GetDefault<UNGameplayAbilty>(AbilityClass))
		{
			InputID=static_cast<int32>(NexusAbilityCDO->GetAbilityInputID());
		}
		
		FGameplayAbilitySpecHandle SpecHandle = ASC->GiveAbility(FGameplayAbilitySpec(AbilityClass,1,InputID,this));
		AbilitiesHandles.Add(SpecHandle);
	}

	SendAbilitiesChangedEvent();

	return AbilitiesHandles;
}

void ANCharacterBase::RemoveAbilities(TArray<FGameplayAbilitySpecHandle> AbilityHandlesToRemove)
{
	if (!ASC || !HasAuthority())
	{
		return;
	}

	for (FGameplayAbilitySpecHandle AbilityHandle : AbilityHandlesToRemove)
	{
		ASC->ClearAbility(AbilityHandle);
	}

	SendAbilitiesChangedEvent();
}

void ANCharacterBase::SendAbilitiesChangedEvent()
{
	FGameplayEventData EventData;
	EventData.EventTag = TAG_Event_Abilities_Changed;
	EventData.Instigator = this;
	EventData.Target = this;
	
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
	this,
	EventData.EventTag,
	EventData
	);
}

void ANCharacterBase::ServerSendGameplayEventToSelf_Implementation(const FGameplayEventData& Payload)
{
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		this,
		Payload.EventTag,
		Payload
	);
}

void ANCharacterBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (ASC)
	{
		ASC->InitAbilityActorInfo(this,this);
	}
}

void ANCharacterBase::OnDeadTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	UE_LOG(LogTemp, Warning, TEXT("Dead Tag Changed: %d"), NewCount);
	
	if (NewCount>0)
	{
		HandleDeath();
	}
}

void ANCharacterBase::HandleDeath()
{
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->DisableMovement();

	FVector Impulse = GetActorForwardVector()* -10000;
	Impulse.Z=15000;
	GetMesh()->AddImpulseAtLocation(Impulse,GetActorLocation());
}

// Called every frame
void ANCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UAbilitySystemComponent* ANCharacterBase::GetAbilitySystemComponent() const
{
	return ASC;
}

