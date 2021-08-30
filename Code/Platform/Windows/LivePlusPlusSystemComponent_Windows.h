#pragma once

#include <AzCore/PlatformIncl.h>
#include <AzCore/Component/Component.h>
#include <AzCore/Component/TickBus.h>
#include <regex>

namespace LivePlusPlus
{
    class LivePlusPlusSystemComponent
        : public AZ::Component
        , public AZ::SystemTickBus::Handler
    {
    public:
        AZ_COMPONENT(LivePlusPlusSystemComponent, "{2C2CAF93-8D0F-4A6C-93B0-F52AE4CF5849}");

        static void Reflect(AZ::ReflectContext* context);

        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);

    protected:
        // SystemTickBus
        void OnSystemTick() override;

        // AZ::Component interface implementation
        void Activate() override;
        void Deactivate() override;

        AZ::EnvironmentVariable<HMODULE> m_livePlusPlus;

        std::regex m_approvedRegex{ ".*" }; // Matches any path
    };
}
