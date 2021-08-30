#include "LivePlusPlusWrapper_Windows.h"

#if defined(ENABLE_LIVEPP)
#include "../../3rdParty/LivePP/API/LPP_API.h"
#endif

#include <AzCore/IO/Path/Path_fwd.h>
#include <AzCore/Module/Environment.h>
#include <AzCore/std/string/conversions.h>
#include <AzCore/Utils/Utils.h>

namespace LivePlusPlus
{
#if defined(_RELEASE) || !defined(ENABLE_LIVEPP)
    HMODULE Initialize()
    {
        // Noop
        return nullptr;
    }
#else
    HMODULE Initialize()
    {
        static AZ::EnvironmentVariable<HMODULE> livePPModule = AZ::Environment::FindVariable<HMODULE>("Live++ Module");
        if (!livePPModule)
        {
            TCHAR buffer[1024];
            GetCurrentDirectory(1024, buffer);

            const AZ::IO::FixedMaxPathString projectPath = AZ::Utils::GetProjectPath();

            const char* path = projectPath.c_str();
            wchar_t widePath[1024] = { 0 };
            AZStd::to_wstring(widePath, strlen(path), path);

            AZStd::wstring fullWidePath(widePath);
            fullWidePath += L"\\..\\Gems\\LivePlusPlus\\3rdParty\\LivePP";

            livePPModule = AZ::Environment::CreateVariable<HMODULE>("Live++ Module");
            // load the Live++ DLL, check for version mismatch, and register process group
            livePPModule.Set(lpp::lppLoadAndRegister(fullWidePath.c_str(), "O3DE"));
        }

        if (livePPModule)
        {
            // enable Live++
            lpp::lppEnableAllCallingModulesAsync(livePPModule.Get());
            // enable Live++'s exception handler/error recovery
            lpp::lppInstallExceptionHandler(livePPModule.Get());
        }

        return livePPModule.Get();
    }
#endif
}
