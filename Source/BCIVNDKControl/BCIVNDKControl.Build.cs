using UnrealBuildTool;

public class BCIVNDKControl : ModuleRules
{
    public BCIVNDKControl(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "BCICore",
                "Core",
                "LevelSequence",
                "Json",
                "JsonUtilities",
                "MovieScene",
                "MovieSceneTracks",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore"
            }
        );
    }
}