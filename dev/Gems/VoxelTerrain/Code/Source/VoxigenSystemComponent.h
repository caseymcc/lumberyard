#pragma once

#include <AzCore/Component/Component.h>

#include <Voxigen/VoxigenBus.h>

namespace Voxigen
{
    class VoxigenSystemComponent
        : public AZ::Component
        , protected VoxigenRequestBus::Handler
    {
    public:
        AZ_COMPONENT(VoxigenSystemComponent, "{18238363-D29E-4DFA-8FA7-E5163F0C84DA}");

        static void Reflect(AZ::ReflectContext* context);

        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

    protected:
        ////////////////////////////////////////////////////////////////////////
        // VoxigenRequestBus interface implementation

        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        // AZ::Component interface implementation
        void Init() override;
        void Activate() override;
        void Deactivate() override;
        ////////////////////////////////////////////////////////////////////////
    };
}
