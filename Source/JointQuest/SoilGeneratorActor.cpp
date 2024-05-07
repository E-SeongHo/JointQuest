
#include "SoilGeneratorActor.h"
#include "MovableActor.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Engine/StaticMeshActor.h"


ASoilGeneratorActor::ASoilGeneratorActor()
{
	PrimaryActorTick.bCanEverTick = true;
	
}

void ASoilGeneratorActor::BeginPlay()
{	
	Super::BeginPlay();
	UE_LOG(LogTemp, Display, TEXT("Soil Generator Init"));

	const float HalfWidth = 1000.0f;
	const float HalfDepth = 250.0f;
	
	FVector CharacterLocation = UGameplayStatics::GetPlayerCharacter(this, 0)->GetActorLocation();
	
	AActor* Stage = GetWorld()->SpawnActor<AActor>(CharacterStage,
			CharacterLocation - FVector(0.0f, 0.0f, 2 * HalfWidth + 50.0f), 
			FRotator::ZeroRotator);

	RefLocation = Stage->GetActorLocation();
	
	// hack : for test
	LeftGenPos = {RefLocation.X, RefLocation.Y + HalfWidth + 100.0f};
	RightGenPos = {RefLocation.X ,RefLocation.Y - (HalfWidth + 100.0f)};
	BackGenPos = {RefLocation.X - HalfDepth * 2 - 100.0f,0.0f};
	
	/*
	LeftGenPos = {-95.000000,1155.000000};
	RightGenPos = {-95.000000,-1155.000000};
	BackGenPos = {-696.000000,0.000000};*/

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

	//float CurrentHeight = PeekElem ? PeekElem->GetActorLocation().Z : 3427.0f;
	float CurrentHeight = PeekElem ? PeekElem->GetActorLocation().Z : RefLocation.Z + 2100.0f;

	FVector LeftSideSpawnLocation(LeftGenPos.X, LeftGenPos.Y, 0.0f);
	FVector RightSideSpawnLocation(RightGenPos.X, RightGenPos.Y, 0.0f);
	FVector BackSideSpawnLocation(BackGenPos.X, BackGenPos.Y, 0.0f);

	UE_LOG(LogTemp, Display, TEXT("%s"), *LeftGenPos.ToString());
	UE_LOG(LogTemp, Display, TEXT("%s"), *LeftSideSpawnLocation.ToString());
	
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
		UE_LOG(LogTemp, Display, TEXT("%s"), *LeftGen->GetActorLocation().ToString());
		
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
