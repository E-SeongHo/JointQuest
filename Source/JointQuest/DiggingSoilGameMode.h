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

	void AwardPoints(int32 Points);
	int32 GetScore(int32 Score) const;
	
private:
	float CurrentScore = 0.0f;
};
