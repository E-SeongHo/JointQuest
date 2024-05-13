// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Networking.h"
#include "Sockets.h"
#include "CoreMinimal.h"
#include "Server.h"
#include "TCPServer.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (Abstract, IsBlueprintBase = "true"))
class JOINTQUEST_API ATCPServer : public AServer
{
	GENERATED_BODY()

public:
	ATCPServer();

	void Listen() override;

	bool StartReceiver(
		const FString& SocketName,
		const FIPv4Address& Address,
		const int32 Port
	) override;

protected:
	FIPv4Endpoint Endpoint;

	virtual void Tick(float DeltaTime) override;

};
