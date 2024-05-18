// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkHandler.h"

#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Interfaces/IPv4/IPv4Address.h"

UNetworkHandler::UNetworkHandler()
{
	ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);

	Socket = SocketSubsystem->CreateSocket(NAME_Stream, TEXT("Joint Tracking Data Socket"), false);
}

UNetworkHandler::~UNetworkHandler()
{
	if (Socket)
	{
		Socket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);
	}
}

bool UNetworkHandler::Connect(FString IP, int32 Port)
{
	FIPv4Address Addr;
	FIPv4Address::Parse(IP, Addr);

	TSharedRef<FInternetAddr> InternetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	InternetAddr->SetIp(Addr.Value);
	InternetAddr->SetPort(Port);

	return Socket->Connect(*InternetAddr);
}

void UNetworkHandler::ReceiveData()
{
	if (Socket && Socket->GetConnectionState() == SCS_Connected)
	{
		TArray<uint8> ReceivedData;
		uint32 Size;
		while (Socket->HasPendingData(Size))
		{
			ReceivedData.SetNumUninitialized(FMath::Min(Size, 65507u));

			int32 Read = 0;
			Socket->Recv(ReceivedData.GetData(), ReceivedData.Num(), Read);

			// process received data
			
			
		}
	}
}
