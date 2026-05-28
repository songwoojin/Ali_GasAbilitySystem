// Fill out your copyright notice in the Description page of Project Settings.


#include "NWeapon_Base.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

ANWeapon_Base::ANWeapon_Base()
	:HitScanRadius(30.0f)
	,CurrentHitScanEffectSpecHandle(nullptr)
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(SceneRoot);

	WeaponMesh=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);
	WeaponMesh->SetCollisionProfileName(TEXT("NoCollision"));

	TraceStart=CreateDefaultSubobject<USceneComponent>(TEXT("TraceStart"));
	TraceStart->SetupAttachment(RootComponent);

	TraceEnd=CreateDefaultSubobject<USceneComponent>(TEXT("TraceEnd"));
	TraceEnd->SetupAttachment(RootComponent);
	
	bReplicates=true;

}

void ANWeapon_Base::BeginPlay()
{
	Super::BeginPlay();
	
}

void ANWeapon_Base::HitScan()
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(GetInstigator());

	TArray<FHitResult> HitResults;
	
	bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
		this,
		TraceStart->GetComponentLocation(),
		TraceEnd->GetComponentLocation(),
		HitScanRadius,
		ObjectTypes,
		false,
		IgnoreActors,
		EDrawDebugTrace::ForDuration,
		 //EDrawDebugTrace::None,
		HitResults,
		true
	);

	if (!bHit)	return;

	for (const FHitResult& HitResult : HitResults)
	{
		AActor* HitActor = HitResult.GetActor();
		if (!IsValid(HitActor))	continue;

		UAbilitySystemComponent* TargetASC= UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitActor);
		if (!TargetASC)	continue;

		if (HitActors.Contains(HitActor))	continue;

		HitActors.AddUnique(HitActor);

		//이미 만들어지고 설정까지 끝난 Spec 적용
		TargetASC->ApplyGameplayEffectSpecToSelf(*CurrentHitScanEffectSpecHandle.Data.Get());
		
		
		//GameplayEffect 클래스 바로 적용
		//TargetASC->ApplyGameplayEffectToSelf();
	}

}

void ANWeapon_Base::HitScanStart(FGameplayEffectSpecHandle HitScanEffectSpecHandle)
{
	GetWorld()->GetTimerManager().SetTimer(HitScanTimer,this,&ANWeapon_Base::HitScan,0.033333f,true);

	CurrentHitScanEffectSpecHandle=HitScanEffectSpecHandle;
	
	HitActors.Empty();
}

void ANWeapon_Base::HitScanEnd()
{
	GetWorld()->GetTimerManager().ClearTimer(HitScanTimer);
}

void ANWeapon_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

