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

#include "Editor/Terrain/IEditorTerrain.h"
#include "CryCommon/ITerrain.h"
#include "CryCommon/TerrainFactory.h"

class EditorVoxelTerrain:public IEditorTerrain
{
public:
    EditorVoxelTerrain();
    EditorVoxelTerrain(const EditorVoxelTerrain &);
    virtual ~EditorVoxelTerrain();

//IEditorTerrain
    virtual int GetType() { return TerrainFactory::getTerrainId("Voxel"); }
    virtual void GetSectorsInfo(SSectorInfo &si);

    virtual void Init();
    virtual void Update();

//    virtual void GetSectorsInfo(SSectorInfo &si);
    virtual uint64 GetWidth() const;
    virtual uint64 GetHeight() const;
    virtual uint64 GetDepth() const;

    virtual float GetMaxHeight() const;
    virtual void SetMaxHeight(float maxHeight, bool scale=false);

    virtual void SetWaterLevel(float waterLevel);
    virtual float GetWaterLevel() const;

    virtual int GetUnitSize() const;
    virtual void SetUnitSize(int unitSize);

    Vec3i GetSectorSizeVector() const;

    virtual QPoint FromWorld(const Vec3& wp) const;
    virtual Vec3 ToWorld(const QPoint& pos) const;

    virtual QRect WorldBoundsToRect(const AABB& worldBounds) const;

    virtual void SetSurfaceTextureSize(int width, int height);
    
    virtual bool IsAllocated();
    virtual void GetMemoryUsage(ICrySizer* pSizer);

    virtual void Resize(int width, int height, int unitSize, bool cleanOld=true, bool forceKeepVegetation=false);
    virtual void Resize(int width, int height, int depth, int unitSize, bool cleanOld=true, bool forceKeepVegetation=false);
    virtual void CleanUp();

    virtual void Serialize(CXmlArchive& xmlAr);
    virtual void SerializeTerrain(CXmlArchive& xmlAr);

//    virtual void ExportBlock(const QRect& rect, CXmlArchive& ar, bool bIsExportVegetation=true, std::set<int>* pLayerIds=0, std::set<int>* pSurfaceIds=0)=0;
//    virtual QPoint ImportBlock(CXmlArchive& ar, const QPoint& newPos, bool bUseNewPos=true, float heightOffset=0.0f, bool bOnlyVegetation=false, ImageRotationDegrees rotation=ImageRotationDegrees::Rotate0)=0;

//local
private:
    size_t m_dimX;
    size_t m_dimY;
    size_t m_dimZ;

    float m_maxHeight;
    int m_unitSize;
    float m_waterLevel;

};

#endif // CRYINCLUDE_EDITOR_TERRAIN_VOXELTERRAIN_H
