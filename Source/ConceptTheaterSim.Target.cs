// Copyright Peter Crall 2025.

using UnrealBuildTool;
using System.Collections.Generic;

public class ConceptTheaterSimTarget : TargetRules
{
	public ConceptTheaterSimTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

		ExtraModuleNames.AddRange( new string[] { "ConceptTheaterSim" } );
	}
}
