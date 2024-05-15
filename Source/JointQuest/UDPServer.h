#pragma once

#include "Networking.h"
#include "Sockets.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Server.h"
#include "UDPServer.generated.h"


UCLASS(ClassGroup = (Custom), meta = (Abstract, IsBlueprintBase = "true"))
class JOINTQUEST_API AUDPServer : public AServer
{
	GENERATED_BODY()
	
public:	

	// Sets default values for this actor's properties
	AUDPServer();

	void Listen() override;

	virtual bool StartReceiver(
		const FString& SocketName,
		const FString& Address,
		const int32 Port
	) override;

	virtual void EndReceiver() override;

protected:
	FUdpSocketReceiver* UDPRecv = nullptr;

	TArray<uint8> ReceivedData;
};
