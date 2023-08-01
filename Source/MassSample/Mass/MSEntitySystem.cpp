#include "MSEntitySystem.h"

#include "MassCommonFragments.h"
#include "MassEntityConfigAsset.h"
#include "MassEntitySubsystem.h"
#include "MassReplicationFragments.h"
#include "MassReplicationSubsystem.h"
#include "MassSpawnerTypes.h"
#include "MassSample/MSAssetManager.h"
#include "Replication/MSBubbleInfo.h"
#include "MassSample/Data/MSGameData.h"

void UMSEntitySystem::Spawn(UMassEntityConfigAsset* EntityConfig, const FTransform& Transform, int Count)
{
    const FMassEntityTemplate& EntityTemplate = EntityConfig->GetOrCreateEntityTemplate(*GetWorld());

    int SqrtCount = FMath::Sqrt((float)Count);

    int j = 0;
    for (int i = 0; i < Count; ++i)
    {
        TArray<FInstancedStruct> FragmentStructs;
        FTransformFragment TransformData;

        FVector RabbitLocation = Transform.GetLocation();
        RabbitLocation.X = (i % SqrtCount) * 100;
        if (i % SqrtCount == 0) ++j;
        RabbitLocation.Y = j * 100;
        TransformData.SetTransform(FTransform(RabbitLocation));

        auto* ReplicationSubsystem = GetWorld()->GetSubsystem<UMassReplicationSubsystem>();
        FragmentStructs.Emplace(FInstancedStruct::Make(TransformData));
    }

    TArray<FMassEntityHandle> Entities;
    auto CreationContext = EntityManager->BatchCreateEntities(EntityTemplate.GetArchetype(), EntityTemplate.GetSharedFragmentValues(), Count, Entities);
    TConstArrayView<FInstancedStruct> FragmentInstances = EntityTemplate.GetInitialFragmentValues();
    EntityManager->BatchSetEntityFragmentsValues(CreationContext->GetEntityCollection(), FragmentInstances);
}

void UMSEntitySystem::Initialize(FSubsystemCollectionBase& Collection)
{
    auto* MassSubsystem = Collection.InitializeDependency<UMassEntitySubsystem>();
    check(MassSubsystem);

    EntityManager = MassSubsystem->GetMutableEntityManager().AsShared();
}

void UMSEntitySystem::Deinitialize()
{
    EntityManager.Reset();
}

void UMSEntitySystem::PostInitialize()
{
    auto* ReplicationSubsystem = UWorld::GetSubsystem<UMassReplicationSubsystem>(GetWorld());
    check(ReplicationSubsystem);

    ReplicationSubsystem->RegisterBubbleInfoClass(AMSUnitClientBubbleInfo::StaticClass());

    auto* SpawnerSubsystem = UWorld::GetSubsystem<UMassSpawnerSubsystem>(GetWorld());
    check(SpawnerSubsystem);

    if (UMSAssetManager::Get()->GameData)
    {
        const auto& EntityTemplate = UMSAssetManager::Get()->GameData->UnitEntityConfig->GetOrCreateEntityTemplate(*GetWorld());
        ensure(EntityTemplate.IsValid());
    }
}