#pragma once

#include "CoreMinimal.h"
#include "MassEntityTypes.h"
#include "MSHealthFragment.generated.h"

USTRUCT()
struct MASSSAMPLE_API FMSHealthFragment : public FMassFragment
{
    GENERATED_BODY()

    float Health = 0;
};