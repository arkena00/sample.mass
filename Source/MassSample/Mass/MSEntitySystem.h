#pragma once
#include "MassSpawnerSubsystem.h"
#include "MassSpawnLocationProcessor.h"
#include "MSEntitySystem.generated.h"

class UMassEntityConfigAsset;
UCLASS()
class UMSEntitySystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "MassSample")
    void Spawn(UMassEntityConfigAsset* EntityConfig, const FTransform& Transform, int Count = 1);

protected:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
    virtual void PostInitialize() override;

private:
    TSharedPtr<FMassEntityManager> EntityManager;
};