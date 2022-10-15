
#pragma once

#include <AzCore/Component/Component.h>

#include <ExampleProject/ExampleProjectBus.h>

namespace ExampleProject
{
    class ExampleProjectSystemComponent
        : public AZ::Component
        , protected ExampleProjectRequestBus::Handler
    {
    public:
        AZ_COMPONENT(ExampleProjectSystemComponent, "{E371BD82-F1B7-4855-8347-5709588FDCF7}");

        static void Reflect(AZ::ReflectContext* context);

        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

        ExampleProjectSystemComponent();
        ~ExampleProjectSystemComponent();

    protected:
        ////////////////////////////////////////////////////////////////////////
        // ExampleProjectRequestBus interface implementation

        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        // AZ::Component interface implementation
        void Init() override;
        void Activate() override;
        void Deactivate() override;
        ////////////////////////////////////////////////////////////////////////
    };
}
