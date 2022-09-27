// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TextToSpeechWidget : ModuleRules
{
	public TextToSpeechWidget(ReadOnlyTargetRules Target) : base(Target)
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
                "Engine",
                "ApplicationCore",
				// ... add other public dependencies that you statically link with here ...
				
				"UMG",
				"SlateCore",
				"DeveloperSettings",
                "TextToSpeech",
                "MP3Importer"
            }
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				// ... add private dependencies that you statically link with here ...	
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
