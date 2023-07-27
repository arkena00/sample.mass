#pragma once

#include "CoreMinimal.h"
#include "MassEntityConfigAsset.h"
#include "Engine/DataAsset.h"

#include "MSGameData.generated.h"

UCLASS(BlueprintType, Blueprintable)
class MASSSAMPLE_API UMSGameData : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "NG2")
    TObjectPtr<UMassEntityConfigAsset> UnitEntityConfig;
};