// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TCPServer.h"
#include "MainServer.generated.h"

/**
 * 
 */
UCLASS()
class JOINTQUEST_API AMainServer : public ATCPServer
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	FString BytestreamToString(TArray<uint8> bytes);
	
	/*
	UFUNCTION(BlueprintCallable)
	FJsonObject* StringToJson(FString str);
	*/
};
