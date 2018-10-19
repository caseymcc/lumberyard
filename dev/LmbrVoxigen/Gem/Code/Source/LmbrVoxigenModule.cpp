
#include "LmbrVoxigen_precompiled.h"
#include <platform_impl.h>

#include <AzCore/Memory/SystemAllocator.h>

#include "LmbrVoxigenSystemComponent.h"

#include <IGem.h>

namespace LmbrVoxigen
{
    class LmbrVoxigenModule
        : public CryHooksModule
    {
    public:
        AZ_RTTI(LmbrVoxigenModule, "{998C25CC-57A0-4340-BB81-3ED0C360C079}", CryHooksModule);
        AZ_CLASS_ALLOCATOR(LmbrVoxigenModule, AZ::SystemAllocator, 0);

        LmbrVoxigenModule()
            : CryHooksModule()
        {
            // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
            m_descriptors.insert(m_descriptors.end(), {
                LmbrVoxigenSystemComponent::CreateDescriptor(),
            });
        }

        /**
         * Add required SystemComponents to the SystemEntity.
         */
        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList{
                azrtti_typeid<LmbrVoxigenSystemComponent>(),
            };
        }
    };
}

// DO NOT MODIFY THIS LINE UNLESS YOU RENAME THE GEM
// The first parameter should be GemName_GemIdLower
// The second should be the fully qualified name of the class above
AZ_DECLARE_MODULE_CLASS(LmbrVoxigen_cea3f91411c7412abaa426d9735e1965, LmbrVoxigen::LmbrVoxigenModule)
