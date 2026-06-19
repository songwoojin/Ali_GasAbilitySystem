// Fill out your copyright notice in the Description page of Project Settings.


#include "Nexus/Component/NWeaponsManagerComponent.h"
#include "Nexus/GameplayAbilitySystem/Weapon/NWeapon_Base.h"
#include "Nexus/GameplayAbilitySystem/Characters/NCharacterBase.h"
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

	OwningCharacter= Cast<ANCharacterBase>(GetOwner());

	CreateStartingWeapons();
}

void UNWeaponsManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UNWeaponsManagerComponent, EquippedWeapon);
}

void UNWeaponsManagerComponent::SetEquippedWeaponProperties()
{
	if (!IsValid(EquippedWeapon))	return;

	FWeaponConfig Config = EquippedWeapon->GetWeaponConfig();

	UE_LOG(LogTemp, Warning,
		TEXT("Authority=%d / Weapon=%s / WeaponClass=%s / ConfigAnimClass=%s"),
		OwningCharacter->HasAuthority(),
		*GetNameSafe(EquippedWeapon),
		*GetNameSafe(EquippedWeapon->GetClass()),
		*GetNameSafe(Config.AnimClass)
	);

	USkeletalMeshComponent* Mesh = OwningCharacter->GetMesh();
	UE_LOG(LogTemp, Warning, TEXT("Mesh=%s / SkeletalMesh=%s"),
		*GetNameSafe(Mesh),
		*GetNameSafe(Mesh->GetSkeletalMeshAsset()));
	
	OwningCharacter->GetMesh()->SetAnimInstanceClass(Config.AnimClass);
	UE_LOG(LogTemp, Warning, TEXT("AnimClassProperty=%s / AnimInstance=%s"),*GetNameSafe(OwningCharacter->GetMesh()->GetAnimClass()),
	*GetNameSafe(OwningCharacter->GetMesh()->GetAnimInstance() ? OwningCharacter->GetMesh()->GetAnimInstance()->GetClass() : nullptr));
	
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

void UNWeaponsManagerComponent::CreateStartingWeapons()
{
	if (!OwningCharacter)	return;
	if (!OwningCharacter->HasAuthority())	return;

	for (const TSubclassOf<ANWeapon_Base>& WeaponClass : StartingWeaponClasses)
	{
		if (!WeaponClass) continue;

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = OwningCharacter;
		SpawnParams.Instigator = OwningCharacter;

		ANWeapon_Base* NewWeapon =
			GetWorld()->SpawnActor<ANWeapon_Base>(
				WeaponClass,
				FVector::ZeroVector,
				FRotator::ZeroRotator,
				SpawnParams
			);

		if (!NewWeapon) continue;

		StartingWeapons.Add(NewWeapon);

		const FWeaponConfig& Config = NewWeapon->GetWeaponConfig();

		//해당 무기에 맞는 소켓 설정하기
		NewWeapon->AttachToComponent(
			OwningCharacter->GetMesh(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			Config.StowedSocketName
		);
	}
}

void UNWeaponsManagerComponent::AttachWeapon(const TSubclassOf<ANWeapon_Base>& WeaponToAttach)
{
	if (!IsValid(OwningCharacter)) return;
	if (!OwningCharacter->HasAuthority()) return;
	if (!WeaponToAttach) return;
	
	for (ANWeapon_Base* StartingWeapon : StartingWeapons)
	{
		if (!IsValid(StartingWeapon)) continue;

		if (StartingWeapon->GetClass()==WeaponToAttach)
		{
			FName EquipedSocketName = StartingWeapon->GetWeaponConfig().EquippedSocketName;
			StartingWeapon->AttachToComponent(OwningCharacter->GetMesh(),
				FAttachmentTransformRules::SnapToTargetNotIncludingScale,EquipedSocketName);
			break;
		}
	}
}

void UNWeaponsManagerComponent::DetachWeapon()
{
	if (!IsValid(EquippedWeapon)) return;
	
	EquippedWeapon->AttachToComponent(OwningCharacter->GetMesh(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale, EquippedWeapon->GetWeaponConfig().StowedSocketName);
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
		
		UnEquipWeapon();
	}
	
	for (ANWeapon_Base* StartingWeapon : StartingWeapons)
	{
		if (!IsValid(StartingWeapon)) continue;

		if (StartingWeapon->GetClass()==EquippedWeaponClass)
		{
			EquippedWeapon=StartingWeapon;
			break;
		}
	}
	
	if (EquippedWeapon)
	{
		AbilitesGrantedByWeapon = OwningCharacter->GrantAbilities(EquippedWeapon->GetWeaponConfig().AbilitiesToGrant);
		AttachWeapon(EquippedWeaponClass);
	}
}

void UNWeaponsManagerComponent::UnEquipWeapon()
{
	if (!IsValid(EquippedWeapon))	return;

	DetachWeapon();
	
	EquippedWeapon=nullptr;
	OwningCharacter->RemoveAbilities(AbilitesGrantedByWeapon);
	AbilitesGrantedByWeapon.Empty();
}

void UNWeaponsManagerComponent::ApplyWeaponState_Implementation()
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





