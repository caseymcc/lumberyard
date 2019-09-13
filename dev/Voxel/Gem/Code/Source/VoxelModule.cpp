
#include <AzCore/Memory/SystemAllocator.h>
#include <AzCore/Module/Module.h>

#include <VoxelSystemComponent.h>

namespace Voxel
{
    class VoxelModule
        : public AZ::Module
    {
    public:
        AZ_RTTI(VoxelModule, "{6E9B923D-7404-4425-82A8-5180796F7CAB}", AZ::Module);
        AZ_CLASS_ALLOCATOR(VoxelModule, AZ::SystemAllocator, 0);

        VoxelModule()
            : AZ::Module()
        {
            // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
            m_descriptors.insert(m_descriptors.end(), {
                VoxelSystemComponent::CreateDescriptor(),
            });
        }

        /**
         * Add required SystemComponents to the SystemEntity.
         */
        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList{
                azrtti_typeid<VoxelSystemComponent>(),
            };
        }
    };
}

// DO NOT MODIFY THIS LINE UNLESS YOU RENAME THE GEM
// The first parameter should be GemName_GemIdLower
// The second should be the fully qualified name of the class above
AZ_DECLARE_MODULE_CLASS(Voxel_f299b1a33b27448382c67b4c9a5afc2a, Voxel::VoxelModule)
