
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/EditContextConstants.inl>

#include "ExampleProjectSystemComponent.h"

namespace ExampleProject
{
    void ExampleProjectSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (AZ::SerializeContext* serialize = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serialize->Class<ExampleProjectSystemComponent, AZ::Component>()
                ->Version(0)
                ;

            if (AZ::EditContext* ec = serialize->GetEditContext())
            {
                ec->Class<ExampleProjectSystemComponent>("ExampleProject", "[Description of functionality provided by this System Component]")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                        ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC("System"))
                        ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ;
            }
        }
    }

    void ExampleProjectSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC("ExampleProjectService"));
    }

    void ExampleProjectSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC("ExampleProjectService"));
    }

    void ExampleProjectSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
    }

    void ExampleProjectSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
    }

    ExampleProjectSystemComponent::ExampleProjectSystemComponent()
    {
        if (ExampleProjectInterface::Get() == nullptr)
        {
            ExampleProjectInterface::Register(this);
        }
    }

    ExampleProjectSystemComponent::~ExampleProjectSystemComponent()
    {
        if (ExampleProjectInterface::Get() == this)
        {
            ExampleProjectInterface::Unregister(this);
        }
    }

    void ExampleProjectSystemComponent::Init()
    {
    }

    void ExampleProjectSystemComponent::Activate()
    {
        ExampleProjectRequestBus::Handler::BusConnect();
    }

    void ExampleProjectSystemComponent::Deactivate()
    {
        ExampleProjectRequestBus::Handler::BusDisconnect();
    }
}
