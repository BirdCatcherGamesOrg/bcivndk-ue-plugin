using UnrealBuildTool;

public class BCIVNDKSequencer : ModuleRules
{
    public BCIVNDKSequencer(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "MovieScene",
                "MovieSceneTracks",
                "Sequencer",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "BCICore",
                "BCIVNDKMovieScene",
                "CoreUObject",
                "Engine",
                "LevelSequence",
                "Slate",
                "SlateCore", 
                "UnrealEd",
            }
        );
    }
}