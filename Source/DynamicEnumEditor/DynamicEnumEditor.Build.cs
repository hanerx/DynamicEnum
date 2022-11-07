using UnrealBuildTool;

public class DynamicEnumEditor : ModuleRules
{
    public DynamicEnumEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core", "ToolWidgets", "BlueprintGraph", "DynamicEnum",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore", "DynamicEnum","ToolWidgets"
            }
        );
    }
}