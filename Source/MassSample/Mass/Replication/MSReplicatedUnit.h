#pragma once

#include "MassReplicationTypes.h"
#include "MassClientBubbleHandler.h"
#include "MassReplicationTransformHandlers.h"
#include "MSReplicatedUnit.generated.h"

/** The data that is replicated specific to each Crowd agent */
USTRUCT()
struct MASSSAMPLE_API FMSReplicatedUnitAgent : public FReplicatedAgentBase
{
    GENERATED_BODY()

    const FReplicatedAgentPositionYawData& GetReplicatedPositionYawData() const { return PositionYaw; }
    FReplicatedAgentPositionYawData& GetReplicatedPositionYawDataMutable() { return PositionYaw; }

private:
    UPROPERTY()
    FReplicatedAgentPositionYawData PositionYaw; // replicated data
};

/** Fast array item for efficient agent replication. Remember to make this dirty if any FReplicatedCrowdAgent member variables are modified */
USTRUCT()
struct MASSSAMPLE_API FMSUnitFastArrayItem : public FMassFastArrayItemBase
{
    GENERATED_BODY()

    FMSUnitFastArrayItem() = default;
    FMSUnitFastArrayItem(const FMSReplicatedUnitAgent& InAgent, const FMassReplicatedAgentHandle InHandle)
        : FMassFastArrayItemBase(InHandle)
        , Agent(InAgent)
    {}

    /** This typedef is required to be provided in FMassFastArrayItemBase derived classes (with the associated FReplicatedAgentBase derived class) */
    using FReplicatedAgentType = FMSReplicatedUnitAgent;

    UPROPERTY()
    FMSReplicatedUnitAgent Agent;
};