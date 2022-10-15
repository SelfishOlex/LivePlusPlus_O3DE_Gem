
#include <AzCore/Memory/SystemAllocator.h>
#include <AzCore/Module/Module.h>

#include "ExampleProjectSystemComponent.h"

namespace ExampleProject
{
    class ExampleProjectModule
        : public AZ::Module
    {
    public:
        AZ_RTTI(ExampleProjectModule, "{AE4B4766-FDE8-4B84-9B22-25980EA4B019}", AZ::Module);
        AZ_CLASS_ALLOCATOR(ExampleProjectModule, AZ::SystemAllocator, 0);

        ExampleProjectModule()
            : AZ::Module()
        {
            // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
            m_descriptors.insert(m_descriptors.end(), {
                ExampleProjectSystemComponent::CreateDescriptor(),
            });
        }

        /**
         * Add required SystemComponents to the SystemEntity.
         */
        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList{
                azrtti_typeid<ExampleProjectSystemComponent>(),
            };
        }
    };
}// namespace ExampleProject

AZ_DECLARE_MODULE_CLASS(Gem_ExampleProject, ExampleProject::ExampleProjectModule)
