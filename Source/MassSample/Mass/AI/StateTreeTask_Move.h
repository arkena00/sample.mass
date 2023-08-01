#pragma once

#include "CoreMinimal.h"
#include "MassSample/Unit/MSUnitFragments.h"
#include "StateTreeTypes.h"
#include "MassAIBehavior/Public/MassStateTreeTypes.h"
#include "MassMovementFragments.h"
#include "MassSignalSubsystem.h"
#include "StateTreeTask_Move.generated.h"

USTRUCT()
struct FStateTreeTask_MoveData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category = Parameter)
    float Velocity = 0.f;
};

USTRUCT()
struct MASSSAMPLE_API FStateTreeTask_Move : public FMassStateTreeTaskBase
{
    GENERATED_BODY()

    using FInstanceDataType = FStateTreeTask_MoveData;

    virtual bool Link(FStateTreeLinker& Linker) override;
    virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
    virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
    virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

    TStateTreeExternalDataHandle<FMSTargetFragment> Location;

    TStateTreeExternalDataHandle<FMassVelocityFragment> VelocityHandle;
    TStateTreeExternalDataHandle<UMassSignalSubsystem> MassSignalSubsystemHandle;
};