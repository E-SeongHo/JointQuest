

#include "SoilGeneratorActor.h"
#include "MovableActor.h"

ASoilGeneratorActor::ASoilGeneratorActor()
{
	PrimaryActorTick.bCanEverTick = true;
	
}

void ASoilGeneratorActor::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Display, TEXT("Soil Generator Init"));
	// hack : for test
	LeftGenPos = {-353.000000,109.000000};
	RightGenPos = {-353.000000,-2273.000000};
	BackGenPos = {-782.000000,-972.000000};

	GenerateSoils();
}

void ASoilGeneratorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASoilGeneratorActor::GenerateSoils()
{
	AMovableActor** PeekPtr = LeftSoilsQueue.Peek();
	AMovableActor* PeekElem = PeekPtr ? *PeekPtr : nullptr;

	float CurrentHeight = PeekElem ? PeekElem->GetActorLocation().Z : 443.0f;

	FVector LeftSideSpawnLocation(LeftGenPos.X, LeftGenPos.Y, 0.0f);
	FVector RightSideSpawnLocation(RightGenPos.X, RightGenPos.Y, 0.0f);
	FVector BackSideSpawnLocation(BackGenPos.X, BackGenPos.Y, 0.0f);

	for(int32 i = 0; i < GenAmount; i++)
	{
		UE_LOG(LogTemp, Display, TEXT("Soil Generator Gens %d"), i);

		float SpawnHeight = CurrentHeight - (BlockHeight * i);
		LeftSideSpawnLocation.Z = SpawnHeight;
		RightSideSpawnLocation.Z = SpawnHeight;
		BackSideSpawnLocation.Z = SpawnHeight;

		AActor* LeftGen = GetWorld()->SpawnActor<AMovableActor>(LeftBlockTemplate,
			LeftSideSpawnLocation,
			FRotator::ZeroRotator);

		AActor* RightGen = GetWorld()->SpawnActor<AMovableActor>(RightBlockTemplate,
			RightSideSpawnLocation,
			FRotator::ZeroRotator);

		AActor* BackGen = GetWorld()->SpawnActor<AMovableActor>(BackBlockTemplate,
			BackSideSpawnLocation,
			FRotator::ZeroRotator);

		LeftSoilsQueue.Enqueue(Cast<AMovableActor>(LeftGen));
		RightSoilsQueue.Enqueue(Cast<AMovableActor>(RightGen));
		BackSoilsQueue.Enqueue(Cast<AMovableActor>(BackGen));

		GeneratedBlocks++;
	}
}
