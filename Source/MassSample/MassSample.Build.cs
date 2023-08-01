// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class MassSample : ModuleRules
{
    public MassSample(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
    
        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core", "CoreUObject", "Engine", "InputCore", "NetCore"
            , "MassEntity", "MassCommon", "MassMovement", "MassSpawner", "MassRepresentation", "MassLOD", "MassReplication", "MassSignals", "MassActors"
            , "AIModule", "MassAIBehavior"
            , "StateTreeModule"
            , "StructUtils"
        });

    PrivateDependencyModuleNames.AddRange(new string[] {  });
    }
}