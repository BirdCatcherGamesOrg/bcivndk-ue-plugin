// SPDX-License-Identifier: MPL-2.0

using UnrealBuildTool;

public class BCIVNDKAssets : ModuleRules
{
    public BCIVNDKAssets(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "DeveloperSettings",
                "LevelSequence",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "BCICore",
                "CoreUObject",
                "Engine",
                "Json",
                "JsonUtilities",
                "Slate",
                "SlateCore",
            }
        );
    }
}