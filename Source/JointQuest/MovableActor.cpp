
#include "MovableActor.h"
#include "Math/UnrealMathUtility.h"

AMovableActor::AMovableActor()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshCube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MovableCube"));
	MeshCube->SetupAttachment(RootComponent);
}

void AMovableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMovableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bIsMoving)
	{
		ElapsedTime += DeltaTime;
		if(ElapsedTime >= MoveDuration)
		{
			SetActorLocation(TargetLocation);
			bIsMoving = false;
		}
		else
		{
			FVector InterpedLocation =
				FMath::Lerp(StartLocation, TargetLocation, ElapsedTime / MoveDuration);
			SetActorLocation(InterpedLocation);
		}
	}
}

void AMovableActor::LiftUp(float MoveAmount, float MoveSpeed)
{
	FVector current = GetActorLocation();
	FVector goal = current + FVector(0.0f, 0.0f, MoveAmount);

	StartLocation = current;
	TargetLocation = goal;
	Speed = MoveSpeed;
	ElapsedTime = 0.0f;
	bIsMoving = true;
}

