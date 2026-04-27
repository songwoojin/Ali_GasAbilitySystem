// Fill out your copyright notice in the Description page of Project Settings.


#include "NCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ANCharacterBase::ANCharacterBase()
	:ASC(nullptr)
	,ASCReplicationMode(EGameplayEffectReplicationMode::Mixed)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ASC=CreateDefaultSubobject<UAbilitySystemComponent>("ASC");
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
}

// Called when the game starts or when spawned
void ANCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ANCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (ASC)
	{
		ASC->InitAbilityActorInfo(this,this);
	}
}

void ANCharacterBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (ASC)
	{
		ASC->InitAbilityActorInfo(this,this);
	}
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

