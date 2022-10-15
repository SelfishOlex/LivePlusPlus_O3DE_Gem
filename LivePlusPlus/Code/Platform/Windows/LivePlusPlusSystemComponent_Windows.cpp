/*
MIT License

Copyright (c) 2021 Oleksandr Lozitskiy

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */

#include <LivePlusPlusSystemComponent_Windows.h>

#include <AzCore/IO/FileIO.h>
#include <AzCore/IO/Path/Path_fwd.h>
#include <AzCore/Module/DynamicModuleHandle.h>
#include <AzCore/Module/ModuleManagerBus.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Settings/SettingsRegistry.h>
#include <AzCore/std/string/conversions.h>
#include <AzCore/Utils/Utils.h>

namespace LivePlusPlus
{
    void LivePlusPlusSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (const auto serialize = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serialize->Class<LivePlusPlusSystemComponent, AZ::Component>()
                ->Version(0);

            if (AZ::EditContext* ec = serialize->GetEditContext())
            {
                ec->Class<LivePlusPlusSystemComponent>("LivePlusPlus",
                    "[Provides integration with hot-reloading Live++ tool, see https://liveplusplus.tech/]")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("System"))
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ;
            }
        }
    }

    void LivePlusPlusSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("LivePlusPlusService"));
    }

    void LivePlusPlusSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC_CE("LivePlusPlusService"));
    }

    void LivePlusPlusSystemComponent::OnSystemTick()
    {
#if defined(ENABLE_LIVEPP)
        // listen to hot-reload and hot-restart requests
        if (m_lppAgent.WantsReload())
        {
            // client code can do whatever it wants here, e.g. synchronize across several threads, the network, etc.
            // ...
            m_lppAgent.CompileAndReloadChanges(lpp::LPP_RELOAD_BEHAVIOUR_WAIT_UNTIL_CHANGES_ARE_APPLIED);
        }

        if (m_lppAgent.WantsRestart())
        {
            // client code can do whatever it wants here, e.g. finish logging, abandon threads, etc.
            // ...
            m_lppAgent.Restart(lpp::LPP_RESTART_BEHAVIOUR_INSTANT_TERMINATION, 0u);
        }
#endif
    }

    void LivePlusPlusSystemComponent::Activate()
    {
#if defined(ENABLE_LIVEPP)
        CreateLivePlusPlusAgent();
        ReadOptionalGemFilter();
        EnableLivePlusPlusForProjectGems();
        
        AZ::SystemTickBus::Handler::BusConnect();
#endif
    }

    void LivePlusPlusSystemComponent::Deactivate()
    {
        AZ::SystemTickBus::Handler::BusDisconnect();
#if defined(ENABLE_LIVEPP)
        lpp::LppDestroySynchronizedAgent(&m_lppAgent);
#endif
    }

#if defined(ENABLE_LIVEPP)
    void LivePlusPlusSystemComponent::CreateLivePlusPlusAgent()
    {
        TCHAR buffer[AZ::IO::MaxPathLength];
        GetCurrentDirectory(AZ::IO::MaxPathLength, buffer);

        const AZ::IO::FixedMaxPathString projectPath = AZ::Utils::GetProjectPath();

        const char* path = projectPath.c_str();
        wchar_t widePath[AZ::IO::MaxPathLength] = { 0 };
        AZStd::to_wstring(widePath, strlen(path), path);

        AZ::IO::FileIOBase* fileIo = AZ::IO::FileIOBase::GetInstance();
        AZ_Assert(fileIo, "FileIO is required to configure Live++ agent. Check your system components order.");
        AZ::IO::FixedMaxPathString resolvedPath;
        wchar_t gemPath[AZ::IO::MaxPathLength] = { 0 };
        if (fileIo != nullptr)
        {
            if (fileIo->ResolvePath("@gemroot:LivePlusPlus@",
                resolvedPath.data(), resolvedPath.capacity() + 1))
            {
                AZStd::to_wstring(gemPath, strlen(resolvedPath.c_str()), resolvedPath.c_str());
            }
        }

        AZStd::wstring fullWidePath(gemPath);
        fullWidePath += L"/3rdParty/LivePP";

        // create a default agent, loading the Live++ agent from the given path, e.g. "ThirdParty/LivePP"
        m_lppAgent = lpp::LppCreateSynchronizedAgent(fullWidePath.c_str());
    }

    void LivePlusPlusSystemComponent::ReadOptionalGemFilter()
    {
        // Read optional filter for the gems that will have Live++ enabled
        if (const auto registry = AZ::SettingsRegistry::Get())
        {
            AZStd::string optionalFilter;
            /*
             * Here is an example of a registry file, you can place it at <your project path>/Registry/liveplusplus.setreg
             * {
             *     "LivePlusPlus":
             *     {
             *         "Settings":
             *         {
             *             "GemFilter": ".*profile.MyGemPrefix.*"
             *         }
             *     }
             * }
             *
             * This would match paths such as "<my path>/profile/MyGemPrefixOneOfMyGems.dll".
             */
            if (registry->Get(optionalFilter, "/LivePlusPlus/Settings/GemFilter"))
            {
                m_approvedGemsRegex = optionalFilter.c_str();
            }
        }
    }

    void LivePlusPlusSystemComponent::EnableLivePlusPlusForProjectGems()
    {
        auto callback = [this](const AZ::ModuleData& moduleData)
        {
            if (moduleData.GetDynamicModuleHandle())
            {
                const char* path = moduleData.GetDynamicModuleHandle()->GetFilename().c_str();

                if (std::regex_match(path, m_approvedGemsRegex))
                {
                    // enable Live++
                    m_lppAgent.EnableModuleANSI(path, lpp::LPP_MODULES_OPTION_NONE);
                }
            }

            return true;
        };

        AZ::ModuleManagerRequestBus::Broadcast(&AZ::ModuleManagerRequestBus::Events::EnumerateModules, callback);
    }
#endif
}
