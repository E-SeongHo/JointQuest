// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Networking.h"
#include "Sockets.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Server.generated.h"


UCLASS(ClassGroup = (Custom), meta = (Abstract, IsBlueprintBase = "true"))
class JOINTQUEST_API AServer : public AActor
{
	GENERATED_BODY()
	
public:	

	// Sets default values for this actor's properties
	AServer();

	UFUNCTION(BlueprintImplementableEvent)
	void BPEvent_DataReceived(const FString& recv);

	FSocket* Socket;

	TArray<uint8> ReceivedData;

	virtual void Listen();

	virtual bool StartReceiver(
		const FString& SocketName,
		const FIPv4Address& Address,
		const int32 Port
	);

	virtual void EndReceiver();

protected:
	// Called when the game starts or when spawned
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
