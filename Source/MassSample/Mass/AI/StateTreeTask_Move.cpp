// Fill out your copyright notice in the Description page of Project Settings.


#include "StateTreeTask_Move.h"

#include "MassMovementFragments.h"
#include "MassStateTreeExecutionContext.h"
#include "StateTreeLinker.h"

bool FStateTreeTask_Move::Link(FStateTreeLinker& Linker)
{
    Linker.LinkExternalData(Location);
    Linker.LinkExternalData(VelocityHandle);
    Linker.LinkExternalData(MassSignalSubsystemHandle);
    return true;
}

EStateTreeRunStatus FStateTreeTask_Move::EnterState(FStateTreeExecutionContext& Context,
    const FStateTreeTransitionResult& Transition) const
{
    GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Yellow, "Start Move");

    const FMassStateTreeExecutionContext& MassContext = static_cast<FMassStateTreeExecutionContext&>(Context);
    FMassVelocityFragment& MassVelocityFragment = MassContext.GetExternalData(VelocityHandle);
    MassVelocityFragment.Value = { 100, 0, 0 };

    UMassSignalSubsystem& MassSignalSubsystem = MassContext.GetExternalData(MassSignalSubsystemHandle);
    MassSignalSubsystem.DelaySignalEntity(UE::Mass::Signals::StandTaskFinished, MassContext.GetEntity(), 2);

    return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FStateTreeTask_Move::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
    return EStateTreeRunStatus::Succeeded;
}