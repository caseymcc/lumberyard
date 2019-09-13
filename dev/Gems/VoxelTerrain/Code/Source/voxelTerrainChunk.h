#ifndef _Cry3DEngine_VoxelTerrainChunk_h_
#define _Cry3DEngine_VoxelTerrainChunk_h_
#pragma once

#include "voxigenDefine.h"

#include <platform.h>
#include <ISystem.h>
#include <IRenderAuxGeom.h>
#include <I3DEngine.h>
#include <CrySizer.h>
#include <Cry3DEngineBase.h>

class VoxelTerrainRegion;
class VoxelTerrainRegion;

class VoxelTerrainChunk:
public Cry3DEngineBase,
public IShadowCaster
{
public:
    friend class VoxelTerrain;

    VoxelTerrainChunk();
    ~VoxelTerrainChunk();

    virtual void Render(const SRendParams &RendParams, const SRenderingPassInfo &passInfo);
    virtual const AABB GetBBoxVirtual();
    virtual void FillBBox(AABB& aabb);
    virtual struct ICharacterInstance* GetEntityCharacter(unsigned int nSlot, Matrix34A* pMatrix=NULL, bool bReturnOnlyVisible=false);
    virtual bool IsRenderNode();
    virtual EERType GetRenderNodeType();

    VoxelTerrain *GetTerrain();

    void SetParent(VoxelTerrainRegion *parent) { m_parent=parent; }

    void SetChunk(VoxelChunkHandle chunkHandle);

    void SetLoaded(bool loaded) { m_dataLoaded=loaded; }
    bool IsLoaded() { return m_dataLoaded; }

    void SetMeshReady(bool value) { m_meshReady=value; }
    bool IsMeshReady() { return m_meshReady; }

    void IncrementInUse() { m_inUse++; }
    void DecrementInUse() { m_inUse--; }
    bool IsInUse() { return (m_inUse>0); }


    voxigen::RegionHash GetRegionHash() { return m_chunkHandle->regionHash(); }
    voxigen::ChunkHash GetChunkHash() { return m_chunkHandle->hash(); }
    bool empty() { return m_chunkHandle->empty(); }

    void Invalidate();

    bool update(const SRenderingPassInfo &passInfo, IGeneralMemoryHeap *heap);
    bool buildMesh(const SRenderingPassInfo &passInfo, IGeneralMemoryHeap *heap);

    void RenderMesh(const SRendParams &RendParams, const SRenderingPassInfo &passInfo);

    Matrix34 &GetTransform() { return m_transform; }
    void SetTransform(const Matrix34 &transform) { m_transform=transform; }

private:
    void drawText(Vec3 pos, const char* format, ...);

    VoxelTerrainRegion *m_parent;
    
    VoxelChunkHandle m_chunkHandle;
    AABB m_box;
    AABB m_currentBox;
    Matrix34 m_transform;

    bool m_dataLoaded;
    int m_inUse;

    bool m_meshReady;
    _smart_ptr<IRenderMesh> m_renderMesh;
};

#endif //_Cry3DEngine_VoxelTerrainChunk_h_
