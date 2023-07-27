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


    for (int i = 0; i < Count; ++i)
    {
        TArray<FInstancedStruct> FragmentStructs;
        FTransformFragment TransformData;

        FVector RabbitLocation = Transform.GetLocation();
        RabbitLocation.X += i * 500;
        TransformData.SetTransform(FTransform(RabbitLocation));

        auto* ReplicationSubsystem = GetWorld()->GetSubsystem<UMassReplicationSubsystem>();
        // Need to manually set NetID because the UMassNetworkIDFragmentInitializer observer is not trigger by CreateEntity, only by BatchCreateEntities
        FMassNetworkIDFragment NetIdData;
        NetIdData.NetID = ReplicationSubsystem->GetNextAvailableMassNetID();
        FReplicationTemplateIDFragment TemplateIdData;
        TemplateIdData.ID = EntityTemplate.GetTemplateID();

        FragmentStructs.Emplace(FInstancedStruct::Make(TransformData));
        FragmentStructs.Emplace(FInstancedStruct::Make(NetIdData));
        FragmentStructs.Emplace(FInstancedStruct::Make(TemplateIdData));

        if (EntityTemplate.IsValid())
        {
            auto Entity = EntityManager->CreateEntity(EntityTemplate.GetArchetype(), EntityTemplate.GetSharedFragmentValues());
            EntityManager->SetEntityFragmentsValues(Entity, EntityTemplate.GetInitialFragmentValues());
            EntityManager->SetEntityFragmentsValues(Entity, FragmentStructs);
        }
    }
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