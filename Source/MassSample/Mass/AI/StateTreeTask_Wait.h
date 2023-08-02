#pragma once

#include "CoreMinimal.h"
#include "MassMovementFragments.h"
#include "MassSignalSubsystem.h"
#include "MassStateTreeExecutionContext.h"
#include "MassStateTreeTypes.h"
#include "StateTreeExecutionContext.h"
#include "StateTreeLinker.h"
#include "StateTreeTypes.h"
#include "StateTreeTask_Wait.generated.h"

USTRUCT()
struct FStateTreeTask_WaitData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category = Parameter)
    float Duration;
};

USTRUCT()
struct MASSSAMPLE_API FStateTreeTask_Wait : public FMassStateTreeTaskBase
{
    GENERATED_BODY()

    using FInstanceDataType = FStateTreeTask_WaitData;

    virtual bool Link(FStateTreeLinker& Linker) override
    {
        Linker.LinkExternalData(MassSignalSubsystemHandle);
        Linker.LinkExternalData(VelocityHandle);

        return true;
    }

    virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override
    {
        auto& Data = Context.GetInstanceData(*this);

        const FMassStateTreeExecutionContext& MassContext = static_cast<FMassStateTreeExecutionContext&>(Context);
        FMassVelocityFragment& MassVelocityFragment = MassContext.GetExternalData(VelocityHandle);
        MassVelocityFragment.Value = FVector::Zero();

        UMassSignalSubsystem& MassSignalSubsystem = Context.GetExternalData(MassSignalSubsystemHandle);
        MassSignalSubsystem.DelaySignalEntity(UE::Mass::Signals::StandTaskFinished, MassContext.GetEntity(), Data.Duration);

        return EStateTreeRunStatus::Running;
    }

    virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override
    {
        return EStateTreeRunStatus::Succeeded;
    }

    virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override
    {
        //GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, "Exit Wait");
    }

    virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

    virtual void StateCompleted(FStateTreeExecutionContext& Context, const EStateTreeRunStatus CompletionStatus, const FStateTreeActiveStates& CompletedActiveStates) const override
    {
    }

    TStateTreeExternalDataHandle<UMassSignalSubsystem> MassSignalSubsystemHandle;
    TStateTreeExternalDataHandle<FMassVelocityFragment> VelocityHandle;

};