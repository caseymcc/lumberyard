
#include <AzCore/Memory/SystemAllocator.h>
#include <AzCore/Module/Module.h>

#include <VoxelTerrainSystemComponent.h>

namespace VoxelTerrain
{
    class VoxelTerrainModule
        : public AZ::Module
    {
    public:
        AZ_RTTI(VoxelTerrainModule, "{42E31C31-7AE5-44D5-8832-4F17263CC6B1}", AZ::Module);
        AZ_CLASS_ALLOCATOR(VoxelTerrainModule, AZ::SystemAllocator, 0);

        VoxelTerrainModule()
            : AZ::Module()
        {
            // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
            m_descriptors.insert(m_descriptors.end(), {
                VoxelTerrainSystemComponent::CreateDescriptor(),
            });
        }

        /**
         * Add required SystemComponents to the SystemEntity.
         */
        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList{
                azrtti_typeid<VoxelTerrainSystemComponent>(),
            };
        }
    };
}

// DO NOT MODIFY THIS LINE UNLESS YOU RENAME THE GEM
// The first parameter should be GemName_GemIdLower
// The second should be the fully qualified name of the class above
AZ_DECLARE_MODULE_CLASS(VoxelTerrain_00310bf422bd4e0b8d298690636640b2, VoxelTerrain::VoxelTerrainModule)
