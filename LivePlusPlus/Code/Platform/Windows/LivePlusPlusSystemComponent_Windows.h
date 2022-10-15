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

#pragma once

#include <regex>
#include <AzCore/PlatformIncl.h>
#include <AzCore/Component/Component.h>
#include <AzCore/Component/TickBus.h>

#if defined(ENABLE_LIVEPP)
#include "../../3rdParty/LivePP/API/LPP_API_x64_CPP.h"
#endif

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
        // SystemTickBus interface implementation
        void OnSystemTick() override;

        // AZ::Component interface implementation
        void Activate() override;
        void Deactivate() override;

#if defined(ENABLE_LIVEPP)
        std::regex m_approvedGemsRegex{ ".*" }; // a regex that accepts any gem path
        lpp::LppSynchronizedAgent m_lppAgent;

        void CreateLivePlusPlusAgent();
        void ReadOptionalGemFilter();
        void EnableLivePlusPlusForProjectGems();
#endif
    };
}
