#include <LivePlusPlusSystemComponent_Windows.h>

#include <LivePlusPlusWrapper_Windows.h>
#include <AzCore/Module/DynamicModuleHandle.h>
#include <AzCore/Module/ModuleManagerBus.h>
#include <AzCore/Serialization/EditContext.h>

#if defined(ENABLE_LIVEPP)
#include "../../3rdParty/LivePP/API/LPP_API.h"
#endif

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
        if (m_livePlusPlus)
        {
            lpp::lppSyncPoint(m_livePlusPlus.Get());
        }
#endif
    }

    void LivePlusPlusSystemComponent::Activate()
    {
#if defined(ENABLE_LIVEPP)
        LivePlusPlus::Initialize();

        if (!m_livePlusPlus)
        {
            m_livePlusPlus = AZ::Environment::FindVariable<HMODULE>("Live++ Module");
        }

        auto callback = [this](const AZ::ModuleData& moduleData)
        {
            if (m_livePlusPlus)
            {
                if (moduleData.GetDynamicModuleHandle())
                {
                    const char* path = moduleData.GetDynamicModuleHandle()->GetFilename().c_str();

                    if (std::regex_match(path, m_approvedRegex))
                    {
                        wchar_t widePath[1024] = { 0 };
                        AZStd::to_wstring(widePath, strlen(path), path);

                        // enable Live++
                        lpp::lppEnableModuleAsync(m_livePlusPlus.Get(), widePath);
                    }
                }
            }

            return true;
        };

        // enable Live++'s exception handler/error recovery
        lpp::lppInstallExceptionHandler(m_livePlusPlus.Get());

        AZ::ModuleManagerRequestBus::Broadcast(&AZ::ModuleManagerRequestBus::Events::EnumerateModules, callback);

        AZ::SystemTickBus::Handler::BusConnect();
#endif
    }

    void LivePlusPlusSystemComponent::Deactivate()
    {
        AZ::SystemTickBus::Handler::BusDisconnect();

        if (m_livePlusPlus && m_livePlusPlus.Get() != nullptr)
        {
            ::FreeLibrary(m_livePlusPlus.Get());
            (*m_livePlusPlus) = nullptr;
        }
    }
}
