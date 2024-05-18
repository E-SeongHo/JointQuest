// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class JointQuest : ModuleRules
{
	public JointQuest(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", 
			"HeadMountedDisplay", "EnhancedInput", "UMG",
			"Networking", "Sockets", "Slate", "SlateCore",
			"Niagara", "Json", "JsonUtilities"
		});
	}
}
