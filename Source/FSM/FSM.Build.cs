// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FSM : ModuleRules
{
	public FSM(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        bEnforceIWYU = true;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine" });
    }
}
