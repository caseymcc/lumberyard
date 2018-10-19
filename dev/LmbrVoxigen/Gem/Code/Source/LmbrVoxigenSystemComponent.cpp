
#include "LmbrVoxigen_precompiled.h"

#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>

#include "LmbrVoxigenSystemComponent.h"

namespace LmbrVoxigen
{
    void LmbrVoxigenSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (AZ::SerializeContext* serialize = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serialize->Class<LmbrVoxigenSystemComponent, AZ::Component>()
                ->Version(0)
                ;

            if (AZ::EditContext* ec = serialize->GetEditContext())
            {
                ec->Class<LmbrVoxigenSystemComponent>("LmbrVoxigen", "[Description of functionality provided by this System Component]")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                        ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC("System"))
                        ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ;
            }
        }
    }

    void LmbrVoxigenSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC("LmbrVoxigenService"));
    }

    void LmbrVoxigenSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC("LmbrVoxigenService"));
    }

    void LmbrVoxigenSystemComponent::GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        (void)required;
    }

    void LmbrVoxigenSystemComponent::GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
        (void)dependent;
    }

    void LmbrVoxigenSystemComponent::Init()
    {
    }

    void LmbrVoxigenSystemComponent::Activate()
    {
        LmbrVoxigenRequestBus::Handler::BusConnect();
    }

    void LmbrVoxigenSystemComponent::Deactivate()
    {
        LmbrVoxigenRequestBus::Handler::BusDisconnect();
    }
}
