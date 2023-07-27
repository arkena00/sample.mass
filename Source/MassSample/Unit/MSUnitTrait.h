#pragma once
#include "MassActorSubsystem.h"
#include "MassEntityTemplateRegistry.h"
#include "MassEntityTraitBase.h"
#include "MassLODFragments.h"
#include "MassMovementFragments.h"
#include "MSHealthFragment.h"
#include "Example/MassSimpleMovementTrait.h"
#include "MSUnitTrait.generated.h"

UCLASS()
class UMSUnitTrait : public UMassEntityTraitBase
{
    GENERATED_BODY()

protected:

    virtual void BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const override
    {
        BuildContext.AddFragment<FTransformFragment>();
        BuildContext.AddFragment<FMSHealthFragment>();

        // Fragments required by MassVisualizationTrait
        BuildContext.AddFragment<FMassActorFragment>();
        BuildContext.AddFragment<FMassViewerInfoFragment>();

        // Movement
        auto& Velocity = BuildContext.AddFragment_GetRef<FMassVelocityFragment>();
        Velocity.Value.X = 100;
        Velocity.Value.Y = 100;
        BuildContext.AddTag<FMassSimpleMovementTag>();
    }
};