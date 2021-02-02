// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ModulePractice : ModuleRules
{
	public ModulePractice(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"HeadMountedDisplay",
			"AnimGraphRuntime",
			"AnimGraph",
			"UnrealEd",
			"AnimationCore"
		});
	}
}
