// Fill out your copyright notice in the Description page of Project Settings.


#include "Nexus/Widget/NEnemyAvatar.h"

#include "NEnemyArmorBar.h"
#include "NEnemyHPBar.h"

void UNEnemyAvatar::SetOwningActor(AActor* NewOwningActor)
{
	HPBar->SetOwningActor(NewOwningActor);
	ArmorBar->SetOwningActor(NewOwningActor);
}

void UNEnemyAvatar::NativeConstruct()
{
	Super::NativeConstruct();
}
