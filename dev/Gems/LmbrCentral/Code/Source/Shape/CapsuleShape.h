/*
* All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
* its licensors.
*
* For complete copyright and license terms please see the LICENSE at the root of this
* distribution (the "License"). All use of this software is governed by the License,
* or, if provided, by the license below or the license accompanying this file. Do not
* remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
*/

#pragma once

#include <AzCore/Component/TransformBus.h>
#include <Cry_Math.h>
#include <LmbrCentral/Shape/ShapeComponentBus.h>
#include <LmbrCentral/Shape/CapsuleShapeComponentBus.h>

namespace AzFramework
{
    class EntityDebugDisplayRequests;
}

namespace LmbrCentral
{
    extern const AZ::u32 g_capsuleDebugShapeSides;
    extern const AZ::u32 g_capsuleDebugShapeCapSegments;

    class CapsuleShape
        : public ShapeComponentRequestsBus::Handler
        , public CapsuleShapeComponentRequestsBus::Handler
        , public AZ::TransformNotificationBus::Handler
    {
    public:
        AZ_CLASS_ALLOCATOR(CapsuleShape, AZ::SystemAllocator, 0)
        AZ_RTTI(CapsuleShape, "{5184EF7A-CD81-4A04-BBF3-86BDAB726E67}")

        static void Reflect(AZ::ReflectContext* context);

        void Activate(AZ::EntityId entityId);
        void Deactivate();
        void InvalidateCache(InvalidateShapeCacheReason reason);

        // ShapeComponentRequestsBus::Handler
        AZ::Crc32 GetShapeType() override { return AZ_CRC("Capsule", 0xc268a183); }
        AZ::Aabb GetEncompassingAabb() override;
        bool IsPointInside(const AZ::Vector3& point) override;
        float DistanceSquaredFromPoint(const AZ::Vector3& point) override;
        bool IntersectRay(const AZ::Vector3& src, const AZ::Vector3& dir, AZ::VectorFloat& distance) override;

        // CapsuleShapeComponentRequestsBus::Handler
        CapsuleShapeConfig GetCapsuleConfiguration() override { return m_capsuleShapeConfig; }
        void SetHeight(float height) override;
        void SetRadius(float radius) override;
        float GetHeight() override;
        float GetRadius() override;

        // AZ::TransformNotificationBus::Handler
        void OnTransformChanged(const AZ::Transform& local, const AZ::Transform& world) override;

        const CapsuleShapeConfig& GetCapsuleConfiguration() const { return m_capsuleShapeConfig; }
        void SetCapsuleConfiguration(const CapsuleShapeConfig& capsuleShapeConfig) { m_capsuleShapeConfig = capsuleShapeConfig; }
        const AZ::Transform& GetCurrentTransform() const { return m_currentTransform; }

    private:
        /**
         * Runtime data - cache potentially expensive operations.
         */
        class CapsuleIntersectionDataCache : public IntersectionTestDataCache<CapsuleShapeConfig>
        {
            void UpdateIntersectionParamsImpl(
                const AZ::Transform& currentTransform, const CapsuleShapeConfig& configuration) override;

            friend class CapsuleShape;

            AZ::Vector3 m_basePlaneCenterPoint; ///< The base end-point of the cylinder inside the capsule.
            AZ::Vector3 m_topPlaneCenterPoint; ///< The top end-point of the cylinder inside the capsule.
            AZ::Vector3 m_axisVector; ///< A unit vector along the axis of this capsule (including entity scale).
            float m_internalHeight = 0.0f; ///< Internal height of the cylinder (total height, minus end caps) (including entity scale).
            float m_radius = 0.0f; ///< Radius of the capsule (including entity scale).
            bool m_isSphere = false; ///< Indicates whether the cylinder is actually just a sphere.
        };

        CapsuleShapeConfig m_capsuleShapeConfig; ///< Underlying capsule configuration.
        CapsuleIntersectionDataCache m_intersectionDataCache; ///< Caches transient intersection data.
        AZ::Transform m_currentTransform; ///< Caches the current World transform.
        AZ::EntityId m_entityId; ///< Id of the entity the shape is attached to.
    };

    /**
     * Generates a Capsule mesh with filled surface and outlines.
     */
    void GenerateCapsuleMesh(
        const AZ::Transform& worldFromLocal, float radius, float height,
        AZ::u32 sides, AZ::u32 capSegments, AZStd::vector<Vec3>& vertexBufferOut,
        AZStd::vector<vtx_idx>& indexBufferOut, AZStd::vector<Vec3>& lineBufferOut);

} // namespace LmbrCentral