// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DiggingSoilGameMode.generated.h"



UCLASS()
class JOINTQUEST_API ADiggingSoilGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	ADiggingSoilGameMode();

	virtual void BeginPlay() override;

	void ExecuteTrackingProcess();
	
	UFUNCTION(BlueprintCallable, Category = "GameMode")
	int32 GetScore() const;
};
