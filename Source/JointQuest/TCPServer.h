// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Networking.h"
#include "Sockets.h"
#include "CoreMinimal.h"
#include "Server.h"
#include "TCPServer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTCPEventSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTCPMessageSignature, const TArray<uint8>&, Bytes);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTCPClientSignature, const FString&, Client);

struct FTCPClient
{
	FSocket* Socket;
	FString Address;

	bool operator==(const FTCPClient& Other)
	{
		return Address == Other.Address;
	}
};

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (Abstract, IsBlueprintBase = "true"))
class JOINTQUEST_API ATCPServer : public AServer
{
	GENERATED_BODY()

public:
	//Async events

	/** On message received on the receiving socket. */
	UPROPERTY(BlueprintAssignable, Category = "TCP Events")
	FTCPMessageSignature OnReceivedBytes;

	/** Callback when we start listening on the TCP receive socket*/
	UPROPERTY(BlueprintAssignable, Category = "TCP Events")
	FTCPEventSignature OnListenBegin;

	/** Called after receiving socket has been closed. */
	UPROPERTY(BlueprintAssignable, Category = "TCP Events")
	FTCPEventSignature OnListenEnd;

	/** Callback when we start listening on the TCP receive socket*/
	UPROPERTY(BlueprintAssignable, Category = "TCP Events")
	FTCPClientSignature OnClientConnected;

	//This will only get called if an emit fails due to how FSocket works. Use custom disconnect logic if possible.
	UPROPERTY(BlueprintAssignable, Category = "TCP Events")
	FTCPClientSignature OnClientDisconnected;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TCP Connection Properties")
	int32 ListenPort;

	/** Whether we should process our data on the game thread or the TCP thread. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TCP Connection Properties")
	bool bReceiveDataOnGameThread;

	ATCPServer();

	void Listen() override;

	bool StartReceiver(
		const FString& SocketName,
		const FString& Address,
		const int32 Port
	) override;

protected:
	TMap<FString, TSharedPtr<FTCPClient>> Clients;

	FThreadSafeBool Active = false;

	FIPv4Endpoint Endpoint;

	virtual void Tick(float DeltaTime) override;

	TFuture<void> ServerFuture;

	virtual void EndReceiver() override;
};
