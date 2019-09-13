#pragma once

#include <AzCore/Component/Component.h>

#include <Voxel/VoxelBus.h>

namespace Voxel
{
    class VoxelSystemComponent
        : public AZ::Component
        , protected VoxelRequestBus::Handler
    {
    public:
        AZ_COMPONENT(VoxelSystemComponent, "{F4BEA7DD-D1DA-4DE5-801F-53B6294C9EB6}");

        static void Reflect(AZ::ReflectContext* context);

        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

    protected:
        ////////////////////////////////////////////////////////////////////////
        // VoxelRequestBus interface implementation

        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        // AZ::Component interface implementation
        void Init() override;
        void Activate() override;
        void Deactivate() override;
        ////////////////////////////////////////////////////////////////////////
    };
}
