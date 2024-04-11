
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovableActor.generated.h"

UCLASS()
class JOINTQUEST_API AMovableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AMovableActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	void LiftUp(float MoveAmount, float MoveSpeed);
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MeshCube;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Move, meta = (AllowPrivateAccess = "true"))
	float Speed = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Move, meta = (AllowPrivateAccess = "true"))
	float MoveDuration = 1.5f;

	bool bIsMoving = false;
	float ElapsedTime = 0.0f;
	FVector StartLocation;
	FVector TargetLocation;
	
};
