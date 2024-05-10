// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NetworkHandler.generated.h"

UCLASS()
class JOINTQUEST_API UNetworkHandler : public UObject
{
	GENERATED_BODY()

public:
	UNetworkHandler();
	~UNetworkHandler();

	bool Connect(FString IP, int32 Port);
	void ReceiveData();

private:
	FSocket* Socket;
};
