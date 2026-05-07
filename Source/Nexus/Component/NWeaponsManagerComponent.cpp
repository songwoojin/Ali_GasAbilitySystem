// Fill out your copyright notice in the Description page of Project Settings.


#include "Nexus/Component/NWeaponsManagerComponent.h"
#include "Nexus/GameplayAbilitySystem/Weapon/NWeapon_Base.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UNWeaponsManagerComponent::UNWeaponsManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UNWeaponsManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	OwningCharacter= Cast<ACharacter>(GetOwner());
}

void UNWeaponsManagerComponent::EquipWeapon(const TSubclassOf<ANWeapon_Base>& EquippedWeaponClass)
{
	if (!IsValid(OwningCharacter))	return;

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
		OwningCharacter->GetMesh()->SetAnimInstanceClass(EquippedWeapon->GetWeaponConfig().AnimClass);
		OwningCharacter->GetCharacterMovement()->MaxWalkSpeed=EquippedWeapon->GetWeaponConfig().MovementProperties.MaxWalkSpeed;
		OwningCharacter->GetCharacterMovement()->bOrientRotationToMovement=EquippedWeapon->GetWeaponConfig().MovementProperties.bOrientRotationToMovement;
		OwningCharacter->GetCharacterMovement()->bUseControllerDesiredRotation=EquippedWeapon->GetWeaponConfig().MovementProperties.bUseControllerDesiredRotation;
	}
}

void UNWeaponsManagerComponent::UnEquipWeapon()
{
	if (!IsValid(EquippedWeapon))	return;

	EquippedWeapon->Destroy();
	EquippedWeapon=nullptr;
	OwningCharacter->GetMesh()->SetAnimInstanceClass(UnarmedWeaponConfig.AnimClass);
	OwningCharacter->GetCharacterMovement()->MaxWalkSpeed=UnarmedWeaponConfig.MovementProperties.MaxWalkSpeed;
	OwningCharacter->GetCharacterMovement()->bOrientRotationToMovement=UnarmedWeaponConfig.MovementProperties.bOrientRotationToMovement;
	OwningCharacter->GetCharacterMovement()->bUseControllerDesiredRotation=UnarmedWeaponConfig.MovementProperties.bUseControllerDesiredRotation;
}

void UNWeaponsManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

