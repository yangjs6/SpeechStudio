// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TextToSpeech : ModuleRules
{
	public TextToSpeech(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
            {
                "Core",
                "CoreUObject",
				"Projects",
				// ... add other public dependencies that you statically link with here ...
				
				"DeveloperSettings",
                "Communication",
                "HTTP"
            }
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				// ... add private dependencies that you statically link with here ...	
				"Json",
                "JsonUtilities",
            }
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
