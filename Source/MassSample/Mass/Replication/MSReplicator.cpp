#include "MSReplicator.h"

#include "MassExecutionContext.h"
#include "MSReplicatedUnit.h"
#include "MassReplicationTransformHandlers.h"
#include "MSBubbleInfo.h"

//----------------------------------------------------------------------//
//  UMSReplicator
//----------------------------------------------------------------------//
void UMSReplicator::AddRequirements(FMassEntityQuery& EntityQuery)
{
    FMassReplicationProcessorPositionYawHandler::AddRequirements(EntityQuery);
}

void UMSReplicator::ProcessClientReplication(FMassExecutionContext& Context, FMassReplicationContext& ReplicationContext)
{
#if UE_REPLICATION_COMPILE_SERVER_CODE

    FMassReplicationProcessorPositionYawHandler PositionYawHandler;
    FMassReplicationSharedFragment* RepSharedFrag = nullptr;

    auto CacheViewsCallback = [&RepSharedFrag, &PositionYawHandler](FMassExecutionContext& Context)
    {
        PositionYawHandler.CacheFragmentViews(Context);
        RepSharedFrag = &Context.GetMutableSharedFragment<FMassReplicationSharedFragment>();
        check(RepSharedFrag);
    };

    auto AddEntityCallback = [&RepSharedFrag, &PositionYawHandler](FMassExecutionContext& Context, const int32 EntityIdx, FMSReplicatedUnitAgent& InReplicatedAgent, const FMassClientHandle ClientHandle) -> FMassReplicatedAgentHandle
    {
        AMSUnitClientBubbleInfo& UnitBubbleInfo = RepSharedFrag->GetTypedClientBubbleInfoChecked<AMSUnitClientBubbleInfo>(ClientHandle);

        PositionYawHandler.AddEntity(EntityIdx, InReplicatedAgent.GetReplicatedPositionYawDataMutable());

        return UnitBubbleInfo.GetUnitSerializer().Bubble.AddAgent(Context.GetEntity(EntityIdx), InReplicatedAgent);
    };

    auto ModifyEntityCallback = [&RepSharedFrag, &PositionYawHandler](FMassExecutionContext& Context, const int32 EntityIdx, const EMassLOD::Type LOD, const double Time, const FMassReplicatedAgentHandle Handle, const FMassClientHandle ClientHandle)
    {
        AMSUnitClientBubbleInfo& UnitBubbleInfo = RepSharedFrag->GetTypedClientBubbleInfoChecked<AMSUnitClientBubbleInfo>(ClientHandle);

        auto& Bubble = UnitBubbleInfo.GetUnitSerializer().Bubble;

        PositionYawHandler.ModifyEntity<FMSUnitFastArrayItem>(Handle, EntityIdx, Bubble.GetTransformHandlerMutable());
    };

    auto RemoveEntityCallback = [&RepSharedFrag](FMassExecutionContext& Context, const FMassReplicatedAgentHandle Handle, const FMassClientHandle ClientHandle)
    {
        AMSUnitClientBubbleInfo& UnitBubbleInfo = RepSharedFrag->GetTypedClientBubbleInfoChecked<AMSUnitClientBubbleInfo>(ClientHandle);

        UnitBubbleInfo.GetUnitSerializer().Bubble.RemoveAgentChecked(Handle);
    };

    CalculateClientReplication<FMSUnitFastArrayItem>(Context, ReplicationContext, CacheViewsCallback, AddEntityCallback, ModifyEntityCallback, RemoveEntityCallback);
#endif // UE_REPLICATION_COMPILE_SERVER_CODE
}