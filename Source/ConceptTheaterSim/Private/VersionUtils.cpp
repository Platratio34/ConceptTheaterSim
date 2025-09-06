// Copyright Peter Crall 2025


#include "VersionUtils.h"

FString UVersionUtils::GetProjectVersion() {
    FString projectVersion;
    GConfig->GetString(
        TEXT("/Script/EngineSettings.GeneralProjectSettings"),
        TEXT("ProjectVersion"),
        projectVersion,
        GGameIni);
    return projectVersion;
}