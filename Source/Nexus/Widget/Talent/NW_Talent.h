// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NW_Talent.generated.h"

/**
 * 
 */

class UNTalentTreeComponent;
class UTextBlock;
class UButton;
class UImage;
class UPDA_Talent;
class UBorder;
class UOverlay;

UCLASS()
class NEXUS_API UNW_Talent : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOverlay> Overlay;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* TalentName;

	UPROPERTY(meta=(BindWidget))
	UImage* TalentIcon;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* LevelText;

	UPROPERTY(meta=(BindWidget))
	UButton* Button;

	UPROPERTY(EditAnywhere)
	UPDA_Talent* TalentDA;

	UPROPERTY()
	UNTalentTreeComponent* TalentTreeComponent;

	UPROPERTY(EditAnywhere)
	FLinearColor ActiveColor;

public:
	UFUNCTION(BlueprintCallable)
	void SetTalentInfo();
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnTalentButtonClicked();
};
