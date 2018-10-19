
#pragma once

#include <AzCore/Component/Component.h>

#include <LmbrVoxigen/LmbrVoxigenBus.h>

namespace LmbrVoxigen
{
    class LmbrVoxigenSystemComponent
        : public AZ::Component
        , protected LmbrVoxigenRequestBus::Handler
    {
    public:
        AZ_COMPONENT(LmbrVoxigenSystemComponent, "{4C6BBFE0-1A40-429A-973B-135FC774305F}");

        static void Reflect(AZ::ReflectContext* context);

        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

    protected:
        ////////////////////////////////////////////////////////////////////////
        // LmbrVoxigenRequestBus interface implementation

        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        // AZ::Component interface implementation
        void Init() override;
        void Activate() override;
        void Deactivate() override;
        ////////////////////////////////////////////////////////////////////////
    };
}
