
#include "DiggingSoilGameMode.h"

ADiggingSoilGameMode::ADiggingSoilGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/MiniGame_KneeRehab/BluePrints/BP_MinerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ADiggingSoilGameMode::AwardPoints(int32 Points)
{
	CurrentScore += Points;
}

int32 ADiggingSoilGameMode::GetScore(int32 Score) const
{
	return CurrentScore;
}
