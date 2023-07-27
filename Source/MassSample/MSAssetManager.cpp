#include "MSAssetManager.h"

#include "MassEntityConfigAsset.h"

void UMSAssetManager::Load(UMassEntityConfigAsset* EntityConfig)
{
    EntityConfig->GetOrCreateEntityTemplate(*GetWorld());
}

UMSAssetManager* UMSAssetManager::Get()
{
    return &static_cast<UMSAssetManager&>(UAssetManager::Get());
}

UMSAssetManager* UMSAssetManager::GetMSAssetManager()
{
    return Get();
}

void UMSAssetManager::PostInitialAssetScan()
{
    Super::PostInitialAssetScan();

    GameData = LoadAssetSync<UMSGameData>(GameDataType);
    ensure(GameData);
}