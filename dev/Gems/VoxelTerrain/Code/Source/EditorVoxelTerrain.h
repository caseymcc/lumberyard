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
// Original file Copyright Crytek GMBH or its affiliates, used under license.

#ifndef CRYINCLUDE_EDITOR_TERRAIN_VOXELTERRAIN_H
#define CRYINCLUDE_EDITOR_TERRAIN_VOXELTERRAIN_H
#pragma once

#include "CryCommon/ITerrain.h"
#include "CryCommon/TerrainFactory.h"
#include "Editor/Terrain/IEditorTerrain.h"
#include "Editor/Terrain/EditorTerrainFactory.h"

class EditorVoxelTerrain:
    public RegisterEditorTerrain<EditorVoxelTerrain, IEditorTerrain>
{
public:
    EditorVoxelTerrain();
    EditorVoxelTerrain(const EditorVoxelTerrain &);
    virtual ~EditorVoxelTerrain();

    static const char *Name() { return m_name; }

//IEditorTerrain
    void Init() override;
    void Update() override;

    bool SupportEditing() override { return false; }
    bool SupportLayers() override { return false; }
    bool SupportSerialize() override { return false; }
    bool SupportSerializeTexture() override { return false; }
    bool SupportHeightMap() override { return false; }

    int GetType() override { return m_terrainTypeId; }
    void GetSectorsInfo(SSectorInfo &si) override;

    Vec3i GetSectorSizeVector() const override;
    void InitSectorGrid(int numSectors) override;
    int GetNumSectors() const override;
    Vec3 SectorToWorld(const QPoint& sector) const override;
    
//    virtual void GetSectorsInfo(SSectorInfo &si);
    uint64 GetWidth() const override;
    uint64 GetHeight() const override;
    uint64 GetDepth() const override;

    float GetMaxHeight() const override;
    void SetMaxHeight(float maxHeight, bool scale=false) override;

    float GetOceanLevel() const override;
    void SetOceanLevel(float waterLevel) override;

    int GetUnitSize() const override;
    void SetUnitSize(int unitSize) override;

    QPoint FromWorld(const Vec3& wp) const override;
    Vec3 ToWorld(const QPoint& pos) const override;

    QRect WorldBoundsToRect(const AABB& worldBounds) const override;

    void SetSurfaceTextureSize(int width, int height) override;
    void EraseLayerID(uint8 id, uint8 replacementId) override;

    bool IsAllocated() override;
    void GetMemoryUsage(ICrySizer* pSizer) override;

    void Resize(int width, int height, int unitSize, bool cleanOld=true, bool forceKeepVegetation=false) override;
    void Resize(int width, int height, int depth, int unitSize, bool cleanOld=true, bool forceKeepVegetation=false) override;
    void CleanUp() override;

    void Update(bool bOnlyElevation, bool boUpdateReloadSurfacertypes) override {}
    void UpdateSectors() override {}

    void Serialize(CXmlArchive& xmlAr) override;
    void SerializeTerrain(CXmlArchive& xmlAr) override;

    void ClearTerrain() override {}
//    virtual void ExportBlock(const QRect& rect, CXmlArchive& ar, bool bIsExportVegetation=true, std::set<int>* pLayerIds=0, std::set<int>* pSurfaceIds=0)=0;
//    virtual QPoint ImportBlock(CXmlArchive& ar, const QPoint& newPos, bool bUseNewPos=true, float heightOffset=0.0f, bool bOnlyVegetation=false, ImageRotationDegrees rotation=ImageRotationDegrees::Rotate0)=0;

//local
private:
    static const char *m_name;

    size_t m_dimX;
    size_t m_dimY;
    size_t m_dimZ;

    float m_maxHeight;
    int m_unitSize;
    float m_waterLevel;

};

#endif // CRYINCLUDE_EDITOR_TERRAIN_VOXELTERRAIN_H
