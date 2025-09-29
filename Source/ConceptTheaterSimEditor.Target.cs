// Copyright Peter Crall 2025.

using UnrealBuildTool;
using System.Collections.Generic;

public class ConceptTheaterSimEditorTarget : TargetRules
{
	public ConceptTheaterSimEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

		ExtraModuleNames.AddRange(["ConceptTheaterSim"]);
	}
}
