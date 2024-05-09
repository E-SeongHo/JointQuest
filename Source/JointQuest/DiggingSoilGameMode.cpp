
#include "DiggingSoilGameMode.h"

ADiggingSoilGameMode::ADiggingSoilGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/MiniGame_KneeRehab/BluePrints/BP_MinerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	
}
