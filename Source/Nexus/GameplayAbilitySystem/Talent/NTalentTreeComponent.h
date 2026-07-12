// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayEffectTypes.h"
#include "GameplayAbilitySpec.h"
#include "NTalentTreeComponent.generated.h"

class UAbilitySystemComponent;
class ANCharacterBase;
class UPDA_Talent;
class UGameplayAbility;
class UGameplayEffect;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTalentsChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPointsChanged);

USTRUCT(BlueprintType)
struct FGrantedTalent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	UPDA_Talent* Talent=nullptr;

	UPROPERTY(EditAnywhere)
	int32 Level=0;

	bool operator==(const FGrantedTalent& Other) const
	{
		return Talent == Other.Talent && Level == Other.Level;
	}
};

USTRUCT(BlueprintType)
struct FTalentEffectHandles
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<FActiveGameplayEffectHandle> GameplayEffectHandles;
};

USTRUCT(BlueprintType)
struct FTalentAbilityHandles
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NEXUS_API UNTalentTreeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FOnTalentsChanged OnTalentsChanged;

	UPROPERTY(BlueprintAssignable)
	FOnTalentsChanged OnPointsChanged;

public:	
	UNTalentTreeComponent();
	
	UFUNCTION(Server,Reliable,BlueprintCallable)
	void Server_SpendPointOnTalent(UPDA_Talent* Talent);
	
	void FindGrantedTalent(UPDA_Talent* Talent,FGrantedTalent*& OutGrantedTalent,bool& Success);

	UFUNCTION(BlueprintCallable)
	int32 GetAvailablePoints(){return PointsAvailable;}

	bool CanSpendPointsOnTalent(UPDA_Talent* Talent);
protected:
	UPROPERTY(EditAnywhere,ReplicatedUsing=OnRep_GrantedTalents)
	TArray<FGrantedTalent> GrantedTalents;

	UPROPERTY()
	ANCharacterBase* OwnerCharacter=nullptr;

	UPROPERTY()
	UAbilitySystemComponent* OwnerASC=nullptr;

	UPROPERTY()
	TMap<UPDA_Talent*, FTalentEffectHandles> EffectiveHandlesAppliedByTalent;

	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> AppliedEffectHandles;

	UPROPERTY()
	TMap<UPDA_Talent*, FTalentAbilityHandles> AbilitySpecsGrantedByTalent;

	UPROPERTY(EditAnywhere,ReplicatedUsing=OnRep_AvailablePointsChanged)
	int32 PointsAvailable = 10;

	UPROPERTY(EditAnywhere)
	TArray<FGrantedTalent> DefaultTalents;
	
protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	bool GrantTalent(UPDA_Talent* Talent,int32 StartingLevel,bool SkipPointsCheck);
	
	UFUNCTION()
	void OnRep_GrantedTalents();
	
	UFUNCTION()
	void OnRep_AvailablePointsChanged();

	void GiveAbilitiesToOwner(UPDA_Talent* Talent,TArray<TSubclassOf<UGameplayAbility>> Abilities,int32 Level);
	void ApplyGameplayEffectsToOwner(UPDA_Talent* Talent,TArray<TSubclassOf<UGameplayEffect>> GameplayEffect,int32 Level);
	bool LevelUpGrantedTalent(FGrantedTalent* GrantedTalent);
	void IncrementGrantedTalentLevel(FGrantedTalent* GrantedTalent);
	void SetLevelOfAbilitiesGrantedByTalent(UPDA_Talent* Talent);
	void SetLevelOfEffectsGrantedByTalent(UPDA_Talent* Talent);
	void DeductTalentPoints();
	bool CanGiveTalent(UPDA_Talent* Talent,bool SkipPointsCheck);
	bool CanLevelUpTalent(FGrantedTalent* GrantedTalent);
	
};
