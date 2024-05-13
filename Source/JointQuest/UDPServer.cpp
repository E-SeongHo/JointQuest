// Fill out your copyright notice in the Description page of Project Settings.


#include "UDPServer.h"

// Sets default values
AUDPServer::AUDPServer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AUDPServer::EndReceiver()
{
	delete UDPRecv;
	UDPRecv = nullptr;

	Super::EndReceiver();
}

bool AUDPServer::StartReceiver(
	const FString& SocketName,
	const FIPv4Address& Address,
	const int32 Port
) {
	FIPv4Endpoint Endpoint(Address, Port);

	int32 BufferSize = 2 * 1024 * 1024;

	Socket = FUdpSocketBuilder(*SocketName).AsNonBlocking().AsReusable().BoundToEndpoint(Endpoint).WithReceiveBufferSize(BufferSize);

	return true;
}


void AUDPServer::Listen()
{
	if (!Socket) {
		return;
	}
	TSharedRef<FInternetAddr> targetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	uint32 Size;
	while (Socket->HasPendingData(Size)) {
		uint8* Recv = new uint8[Size];
		int32 BytesRead = 0;

		ReceivedData.SetNumUninitialized(FMath::Min(Size, 65507u));
		Socket->RecvFrom(ReceivedData.GetData(), ReceivedData.Num(), BytesRead, *targetAddr);

		char Data[1024];
		memcpy(Data, ReceivedData.GetData(), BytesRead);
		Data[BytesRead] = 0;

		FString res = UTF8_TO_TCHAR(Data);

		BPEvent_DataReceived(res);
	}
}

