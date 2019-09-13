#pragma once

#include <AzCore/Component/Component.h>

#include <VoxelTerrain/VoxelTerrainBus.h>

namespace VoxelTerrain
{
    class VoxelTerrainSystemComponent
        : public AZ::Component
        , protected VoxelTerrainRequestBus::Handler
    {
    public:
        AZ_COMPONENT(VoxelTerrainSystemComponent, "{0538D17F-E271-4CCC-88B8-164AC6B08881}");

        static void Reflect(AZ::ReflectContext* context);

        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

    protected:
        ////////////////////////////////////////////////////////////////////////
        // VoxelTerrainRequestBus interface implementation

        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        // AZ::Component interface implementation
        void Init() override;
        void Activate() override;
        void Deactivate() override;
        ////////////////////////////////////////////////////////////////////////
    };
}
