
#include <AzCore/Memory/SystemAllocator.h>
#include <AzCore/Module/Module.h>

#include <VoxigenSystemComponent.h>

namespace Voxigen
{
    class VoxigenModule
        : public AZ::Module
    {
    public:
        AZ_RTTI(VoxigenModule, "{87B1CFC4-B64F-4023-9A9C-65EF5FF4E5A0}", AZ::Module);
        AZ_CLASS_ALLOCATOR(VoxigenModule, AZ::SystemAllocator, 0);

        VoxigenModule()
            : AZ::Module()
        {
            // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
            m_descriptors.insert(m_descriptors.end(), {
                VoxigenSystemComponent::CreateDescriptor(),
            });
        }

        /**
         * Add required SystemComponents to the SystemEntity.
         */
        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList{
                azrtti_typeid<VoxigenSystemComponent>(),
            };
        }
    };
}

// DO NOT MODIFY THIS LINE UNLESS YOU RENAME THE GEM
// The first parameter should be GemName_GemIdLower
// The second should be the fully qualified name of the class above
AZ_DECLARE_MODULE_CLASS(Voxigen_03aa2988df744d3c96d6e78663f70f83, Voxigen::VoxigenModule)
