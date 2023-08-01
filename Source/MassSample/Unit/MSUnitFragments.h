#pragma once

#include "CoreMinimal.h"
#include "MassEntityTypes.h"
#include "MSUnitFragments.generated.h"

USTRUCT()
struct MASSSAMPLE_API FMSHealthFragment : public FMassFragment
{
    GENERATED_BODY()

    float Health = 0;
};

USTRUCT()
struct MASSSAMPLE_API FMSTargetFragment : public FMassFragment
{
    GENERATED_BODY()

    FVector TargetLocation;
};