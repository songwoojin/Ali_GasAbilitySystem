// Fill out your copyright notice in the Description page of Project Settings.


#include "Nexus/Component/NWeaponsManagerComponent.h"
#include "Nexus/GameplayAbilitySystem/Weapon/NWeapon_Base.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

UNWeaponsManagerComponent::UNWeaponsManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
}

void UNWeaponsManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	OwningCharacter= Cast<ACharacter>(GetOwner());
}

void UNWeaponsManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UNWeaponsManagerComponent, EquippedWeapon);
}

void UNWeaponsManagerComponent::SetEquippedWeaponProperties()
{
	if (!IsValid(EquippedWeapon))	return;
	
	OwningCharacter->GetMesh()->SetAnimInstanceClass(EquippedWeapon->GetWeaponConfig().AnimClass);
	OwningCharacter->GetCharacterMovement()->MaxWalkSpeed=EquippedWeapon->GetWeaponConfig().MovementProperties.MaxWalkSpeed;
	OwningCharacter->GetCharacterMovement()->bOrientRotationToMovement=EquippedWeapon->GetWeaponConfig().MovementProperties.bOrientRotationToMovement;
	OwningCharacter->GetCharacterMovement()->bUseControllerDesiredRotation=EquippedWeapon->GetWeaponConfig().MovementProperties.bUseControllerDesiredRotation;
}

void UNWeaponsManagerComponent::SetUnarmedWeaponProperties()
{
	OwningCharacter->GetMesh()->SetAnimInstanceClass(UnarmedWeaponConfig.AnimClass);
	OwningCharacter->GetCharacterMovement()->MaxWalkSpeed=UnarmedWeaponConfig.MovementProperties.MaxWalkSpeed;
	OwningCharacter->GetCharacterMovement()->bOrientRotationToMovement=UnarmedWeaponConfig.MovementProperties.bOrientRotationToMovement;
	OwningCharacter->GetCharacterMovement()->bUseControllerDesiredRotation=UnarmedWeaponConfig.MovementProperties.bUseControllerDesiredRotation;
}

void UNWeaponsManagerComponent::OnRep_EquippedWeapon()
{
	if (IsValid(EquippedWeapon))
	{
		SetEquippedWeaponProperties();
	}
	else
	{
		SetUnarmedWeaponProperties();
	}
}

void UNWeaponsManagerComponent::EquipWeapon(const TSubclassOf<ANWeapon_Base>& EquippedWeaponClass)
{
	if (!IsValid(OwningCharacter))	return;

	if (!OwningCharacter->HasAuthority())	return;

	if (IsValid(EquippedWeapon))
	{
		if (EquippedWeapon->GetClass()==EquippedWeaponClass)
		{
			UnEquipWeapon();
			return;
		}
		else
		{
			UnEquipWeapon();
		}
	}
	
	FActorSpawnParameters SpawnParams;

	EquippedWeapon = GetWorld()->SpawnActor<ANWeapon_Base>(
		EquippedWeaponClass,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		SpawnParams
	);

	if (EquippedWeapon)
	{
		FName SocketName = EquippedWeapon->GetWeaponConfig().EquippedSocketName;
		EquippedWeapon->AttachToComponent(OwningCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale,SocketName);
	}

	OnRep_EquippedWeapon();
}

void UNWeaponsManagerComponent::UnEquipWeapon()
{
	if (!IsValid(EquippedWeapon))	return;

	EquippedWeapon->Destroy();
	EquippedWeapon=nullptr;
	
	OnRep_EquippedWeapon();
}

void UNWeaponsManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

