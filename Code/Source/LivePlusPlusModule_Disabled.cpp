#include <AzCore/Memory/SystemAllocator.h>
#include <AzCore/Module/Module.h>

namespace LivePlusPlus
{
    class LivePlusPlusModule
        : public AZ::Module
    {
    public:
        AZ_RTTI(LivePlusPlusModule, "{7630DDAC-5417-4DC6-9734-F8EE92C4555F}", AZ::Module);
        AZ_CLASS_ALLOCATOR(LivePlusPlusModule, AZ::SystemAllocator, 0);

        LivePlusPlusModule()
        {
        }
    };
}

// DO NOT MODIFY THIS LINE UNLESS YOU RENAME THE GEM
// The first parameter should be GemName_GemIdLower
// The second should be the fully qualified name of the class above
AZ_DECLARE_MODULE_CLASS(Gem_LivePlusPlus, LivePlusPlus::LivePlusPlusModule)
