﻿#pragma once

#include "MassReplicationProcessor.h"

#include "MSReplicator.generated.h"

/** Class that handles replication and only runs on the server. It queries Mass entity fragments and sets those values when appropriate using the MassClientBubbleHandler. */
UCLASS()
class MASSSAMPLE_API UMSReplicator : public UMassReplicatorBase
{
    GENERATED_BODY()

public:
    /**
     * Overridden to add specific entity query requirements for replication.
     * Usually we add replication processor handler requirements.
     */
    void AddRequirements(FMassEntityQuery& EntityQuery) override;

    /**
     * Overridden to process the client replication.
     * This methods should call CalculateClientReplication with the appropriate callback implementation.
     */
    void ProcessClientReplication(FMassExecutionContext& Context, FMassReplicationContext& ReplicationContext) override;
};