// Fill out your copyright notice in the Description page of Project Settings.


#include "TCPServer.h"

ATCPServer::ATCPServer() {
	PrimaryActorTick.bCanEverTick = true;
}


bool ATCPServer::StartReceiver(
	const FString& SocketName,
	const FString& Address,
	const int32 Port
) {

	UE_LOG(LogTemp, Warning, TEXT("Init"));
	uint32 BufferSize = 16 * 1024 * 1024;
	int32 temp;

	FIPv4Address IPAddr;
	FIPv4Address::Parse(Address, IPAddr);
	Endpoint = FIPv4Endpoint(IPAddr, Port);

	Socket = FTcpSocketBuilder(SocketName).AsReusable().BoundToEndpoint(Endpoint).WithReceiveBufferSize(BufferSize);
	Socket->SetReceiveBufferSize(BufferSize, temp);
	Socket->SetSendBufferSize(BufferSize, temp);
	
	Socket->Listen(8);
	Active = true;

	OnListenBegin.Broadcast();

	ServerFuture = Async(EAsyncExecution::Thread, [&]() { // start a lambda function thread
		TArray<uint8> RecvBuffer;
		TArray<TSharedPtr<FTCPClient>> ClientsDisconnected;

		FDateTime LastPing = FDateTime::Now();

		while (Active) {
			bool pendingConnection;
			Socket->HasPendingConnection(pendingConnection);
			if (pendingConnection) {
				TSharedPtr<FInternetAddr> Addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
				FSocket* Client = Socket->Accept(*Addr, TEXT("tcp-client"));
				const FString AddrStr = Addr->ToString(true);

				TSharedPtr<FTCPClient> ClientItem = MakeShareable(new FTCPClient());
				ClientItem->Address = AddrStr;
				ClientItem->Socket = Client;

				Clients.Add(AddrStr, ClientItem);

				AsyncTask(ENamedThreads::GameThread, [&, AddrStr]() {
					OnClientConnected.Broadcast(AddrStr);
					});
			}
			for (auto ClientPair : Clients) {

				TSharedPtr<FTCPClient> Client = ClientPair.Value;


				ESocketConnectionState ConnectionState = ESocketConnectionState::SCS_NotConnected;
				if (Client->Socket != nullptr) {
					ConnectionState = Client->Socket->GetConnectionState();
				}
				if (ConnectionState != ESocketConnectionState::SCS_Connected) {
					ClientsDisconnected.Add(Client);
					continue;
				}

				UE_LOG(LogTemp, Warning, TEXT("Checking to see if data pending"));
				if (Client->Socket->HasPendingData(BufferSize)) {
					UE_LOG(LogTemp, Warning, TEXT("Data pending"));
					RecvBuffer.SetNumUninitialized(BufferSize);
					int32 read = 0;

					Client->Socket->Recv(RecvBuffer.GetData(), RecvBuffer.Num(), read);
					if (bReceiveDataOnGameThread) {
						TArray<uint8> RecvBufferGT;
						RecvBufferGT.Append(RecvBuffer);

						AsyncTask(ENamedThreads::GameThread, [&, RecvBufferGT]() {
							OnReceivedBytes.Broadcast(RecvBufferGT);
							});
					}
					else {
						OnReceivedBytes.Broadcast(RecvBuffer);
					}
				}
			}

			UE_LOG(LogTemp, Warning, TEXT("Checking disconnected clients"));
			if (ClientsDisconnected.Num() > 0) 
			{
				for (TSharedPtr<FTCPClient> ClientToRemove : ClientsDisconnected) {
					const FString Addr = ClientToRemove->Address;
					Clients.Remove(Addr);
					AsyncTask(ENamedThreads::GameThread, [this, Addr]() {
						OnClientDisconnected.Broadcast(Addr);
						});
				}
				ClientsDisconnected.Empty();
			}

			FPlatformProcess::Sleep(0.0001);
		}

		for (auto ClientPair : Clients) {
			ClientPair.Value->Socket->Close();
		}
		Clients.Empty();

		AsyncTask(ENamedThreads::GameThread, [&]() {
			Clients.Empty();
			OnListenEnd.Broadcast();
			});
		});

	return true;
}

void ATCPServer::Listen() {

}

void ATCPServer::Tick(float DeltaTime) {

}

void ATCPServer::EndReceiver() {
	if (Socket) {
		Active = false;
		ServerFuture.Get();

		Super::EndReceiver();

		OnListenEnd.Broadcast();
	}
}