using UnrealBuildTool;

public class BCIVNDKMovieScene : ModuleRules
{
    public BCIVNDKMovieScene(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            [
                "BCIVNDKControl",
                "Core",
                "Json",
                "JsonUtilities",
                "MovieScene",
                "MovieSceneTracks",
            ]
        );

        PrivateDependencyModuleNames.AddRange(
            [
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore"
            ]
        );
    }
}