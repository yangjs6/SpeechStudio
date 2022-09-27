using UnrealBuildTool;
using System;
using System.IO;
using Microsoft.Win32;

public class Communication : ModuleRules
{

    public Communication(ReadOnlyTargetRules Target) : base(Target)
    {
        bEnableUndefinedIdentifierWarnings = false;
        PCHUsage = PCHUsageMode.NoSharedPCHs;
        PrivatePCHHeaderFile = "Public/Communication.h";
        bUseUnity = false;
        PublicIncludePaths.AddRange(
            new string[] {
                Path.Combine(ModuleDirectory, "Public")
            }
            );


        PrivateIncludePaths.AddRange(
            new string[] {
                "Communication/Private"
            }
            );


        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
				"Http",
                "Json",
                "JsonUtilities",
            }
            );


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Engine",
            }
            );


        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
            }
            );
    }
}
