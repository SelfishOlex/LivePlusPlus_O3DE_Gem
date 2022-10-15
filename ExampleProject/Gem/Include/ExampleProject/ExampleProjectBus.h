
#pragma once

#include <AzCore/EBus/EBus.h>
#include <AzCore/Interface/Interface.h>

namespace ExampleProject
{
    class ExampleProjectRequests
    {
    public:
        AZ_RTTI(ExampleProjectRequests, "{826B0F08-8209-4466-B1E0-D78B3D5BE1DC}");
        virtual ~ExampleProjectRequests() = default;
        // Put your public methods here
    };

    class ExampleProjectBusTraits
        : public AZ::EBusTraits
    {
    public:
        //////////////////////////////////////////////////////////////////////////
        // EBusTraits overrides
        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static constexpr AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;
        //////////////////////////////////////////////////////////////////////////
    };

    using ExampleProjectRequestBus = AZ::EBus<ExampleProjectRequests, ExampleProjectBusTraits>;
    using ExampleProjectInterface = AZ::Interface<ExampleProjectRequests>;

} // namespace ExampleProject
