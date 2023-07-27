#pragma once

#include "MassReplicationTransformHandlers.h"
#include "MassClientBubbleHandler.h"
#include "MassClientBubbleInfoBase.h"
#include "MassEntityView.h"
#include "MSReplicatedUnit.h"

#include "MSBubbleInfo.generated.h"

class MASSSAMPLE_API FMSUnitClientBubbleHandler : public TClientBubbleHandlerBase<FMSUnitFastArrayItem>
{
public:
    typedef TClientBubbleHandlerBase<FMSUnitFastArrayItem> Super;
    typedef TMassClientBubbleTransformHandler<FMSUnitFastArrayItem> FMassClientBubbleTransformHandler;

    FMSUnitClientBubbleHandler()
        : TransformHandler(*this)
    {}

#if UE_REPLICATION_COMPILE_SERVER_CODE
    const FMassClientBubbleTransformHandler& GetTransformHandler() const { return TransformHandler; }
    FMassClientBubbleTransformHandler& GetTransformHandlerMutable() { return TransformHandler; }
#endif // UE_REPLICATION_COMPILE_SERVER_CODE


protected:
#if UE_REPLICATION_COMPILE_CLIENT_CODE
    virtual void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize) override;
    virtual void PostReplicatedChange(const TArrayView<int32> ChAMSedIndices, int32 FinalSize) override;

    void PostReplicatedChangeEntity(const FMassEntityView& EntityView, const FMSReplicatedUnitAgent& Item) const;
#endif //UE_REPLICATION_COMPILE_CLIENT_CODE


    FMassClientBubbleTransformHandler TransformHandler;
};

/** Mass client bubble, there will be one of these per client and it will handle replicating the fast array of Agents between the server and clients */
USTRUCT()
struct MASSSAMPLE_API FMSUnitClientBubbleSerializer : public FMassClientBubbleSerializerBase
{
    GENERATED_BODY()

    FMSUnitClientBubbleSerializer()
    {
        Bubble.Initialize(Units, *this);
    };

    bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
    {
        return FFastArraySerializer::FastArrayDeltaSerialize<FMSUnitFastArrayItem, FMSUnitClientBubbleSerializer>(Units, DeltaParams, *this);
    }

public:
    FMSUnitClientBubbleHandler Bubble;

protected:
    /** Fast Array of Agents for efficient replication. Maintained as a freelist on the server, to keep index consistency as indexes are used as Handles into the Array
     *  Note array order is not guaranteed between server and client so handles will not be consistent between them, FMassNetworkID will be.
     */
    UPROPERTY(Transient)
    TArray<FMSUnitFastArrayItem> Units;
};

template<>
struct TStructOpsTypeTraits<FMSUnitClientBubbleSerializer> : public TStructOpsTypeTraitsBase2<FMSUnitClientBubbleSerializer>
{
    enum
    {
        WithNetDeltaSerializer = true,
        WithCopy = false,
    };
};

/**
 *  This class will allow us to replicate Mass data based on the fidelity required for each player controller. There is one AMassReplicationActor per PlayerController and
 *  which is also its owner.
 */
UCLASS()
class MASSSAMPLE_API AMSUnitClientBubbleInfo : public AMassClientBubbleInfoBase
{
    GENERATED_BODY()

public:
    AMSUnitClientBubbleInfo(const FObjectInitializer& ObjectInitializer);

    FMSUnitClientBubbleSerializer& GetUnitSerializer() { return UnitSerializer; }

protected:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
    UPROPERTY(Replicated, Transient)
    FMSUnitClientBubbleSerializer UnitSerializer;
};