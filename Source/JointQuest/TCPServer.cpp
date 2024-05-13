// Fill out your copyright notice in the Description page of Project Settings.


#include "TCPServer.h"

ATCPServer::ATCPServer() {
	PrimaryActorTick.bCanEverTick = true;
}


bool ATCPServer::StartReceiver(
	const FString& SocketName,
	const FIPv4Address& Address,
	const int32 Port
) {
	int32 BufferSize = 2 * 1024 * 1024;

	Endpoint = FIPv4Endpoint(Address, Port);

	Socket = FTcpSocketBuilder(SocketName).AsNonBlocking().AsReusable().WithReceiveBufferSize(BufferSize);
	ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
	TSharedRef<FInternetAddr> addr = SocketSubsystem->CreateInternetAddr();
	addr->SetIp(Endpoint.Address.Value);
	addr->SetPort(Endpoint.Port);
	Socket->Bind(*addr);
	Socket->Listen(8);

	return true;
}

void ATCPServer::Listen() {

}

void ATCPServer::Tick(float DeltaTime) {

}