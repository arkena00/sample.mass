#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "MassSample/Data/MSGameData.h"
#include "MSAssetManager.generated.h"

UCLASS()
class MASSSAMPLE_API UMSAssetManager : public UAssetManager
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "MassSample")
    void Load(UMassEntityConfigAsset* EntityConfig);

    TObjectPtr<UMSGameData> GameData;

    static UMSAssetManager* Get();

    UFUNCTION(BlueprintPure, Category="MassSample", meta=(WorldContext="WorldContextObject"))
    static UMSAssetManager* GetMSAssetManager();

protected:
    virtual void PostInitialAssetScan() override;

    template<class DataAssetType>
    TObjectPtr<DataAssetType> LoadAssetSync(const FPrimaryAssetType& PrimaryAsset)
    {
        TSharedPtr<FStreamableHandle> DataHandle = LoadPrimaryAssetsWithType(PrimaryAsset);
        if (DataHandle.IsValid())
        {
            DataHandle->WaitUntilComplete(0.f, false);
            return GetPrimaryAssetObjectClass<DataAssetType>(DataHandle->GetLoadedAsset()->GetPrimaryAssetId()).GetDefaultObject();
        }
        return nullptr;
    }

private:
    inline static FPrimaryAssetType GameDataType{ "MSGameData" };

};