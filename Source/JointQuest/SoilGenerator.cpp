
#include "SoilGenerator.h"
#include "MovableActor.h"

void USoilGenerator::Init()
{
	Super::Init();

	UE_LOG(LogTemp, Display, TEXT("Soil Generator Init"));
	// hack : for test
	LeftGenPos = {-353.000000,109.000000};
	RightGenPos = {-353.000000,-2273.000000};
	BackGenPos = {-782.000000,-972.000000};
	
	GenerateSoils();
}

void USoilGenerator::GenerateSoils()
{
	AMovableActor* TopSoil = *LeftSoilsQueue.Peek();
	float CurrentHeight = TopSoil == nullptr ? 0.0f : TopSoil->GetActorLocation().Z;

	FVector LeftSideSpawnLocation(LeftGenPos.X, LeftGenPos.Y, 0.0f);
	FVector RightSideSpawnLocation(RightGenPos.X, RightGenPos.Y, 0.0f);
	FVector BackSideSpawnLocation(BackGenPos.X, BackGenPos.Y, 0.0f);
	
	for(uint32 i = 0; i < GenAmount; i++)
	{
		float SpawnHeight = CurrentHeight - BlockHeight * i;
		LeftSideSpawnLocation.Z = SpawnHeight;
		RightSideSpawnLocation.Z = SpawnHeight;
		BackSideSpawnLocation.Z = SpawnHeight;

		AActor* LeftGen = GetWorld()->SpawnActor<AMovableActor>(LeftBlockTemplate->Get(),
			LeftSideSpawnLocation,
			FRotator::ZeroRotator);

		AActor* RightGen = GetWorld()->SpawnActor<AMovableActor>(RightBlockTemplate->Get(),
			RightSideSpawnLocation,
			FRotator::ZeroRotator);

		AActor* BackGen = GetWorld()->SpawnActor<AMovableActor>(BackBlockTemplate->Get(),
			BackSideSpawnLocation,
			FRotator::ZeroRotator);

		LeftSoilsQueue.Enqueue(Cast<AMovableActor>(LeftGen));
		RightSoilsQueue.Enqueue(Cast<AMovableActor>(RightGen));
		BackSoilsQueue.Enqueue(Cast<AMovableActor>(BackGen));

		GeneratedBlocks++;
	}
}
