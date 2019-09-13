
#include "StdAfx.h"
#include "voxelTerrain.h"

#include <voxigen/equiRectWorldGenerator.h>
#include <voxigen/chunkFunctions.h>

//typedef voxigen::GeneratorTemplate<voxigen::EquiRectWorldGenerator<VoxelWorld::ChunkType>> EquiRectWorldGenerator;

//force generator instantiation
template voxigen::GeneratorTemplate<voxigen::EquiRectWorldGenerator<VoxelWorld>>;

const char *VoxelTerrain::m_name="VoxelTerrain";

VoxelTerrain::VoxelTerrain(const STerrainInfo& TerrainInfo):
    m_renderCube(&m_world, &m_descriptors)
{
    setViewRadius(255.0f);

    m_whiteTextureId=GetRenderer()->EF_LoadTexture("EngineAssets/Textures/white.dds", FT_DONT_STREAM)->GetTextureID();

    {
        SInputShaderResources Res;

        Res.m_LMaterial.m_Opacity=1.0f;
//        m_material=MakeSystemMaterialFromShader("VoxelTerrain", &Res);
        m_material=Get3DEngine()->GetMaterialManager()->LoadMaterial("Materials/material_voxelterrain_default");
    }

    m_updateThreadRun=true;
    m_updateThread=std::thread(std::bind(&VoxelTerrain::updateThread, this));
}

VoxelTerrain::~VoxelTerrain()
{
    {
        std::unique_lock<std::mutex> lock(m_updateMutex);
        m_updateThreadRun=false;
    }

    m_updateEvent.notify_all();
    m_updateThread.join();
}

bool VoxelTerrain::SetCompiledData(byte* pData, int nDataSize, std::vector<IStatObj*>** ppStatObjTable, std::vector<_smart_ptr<IMaterial>>** ppMatTable, bool bHotUpdate, SHotUpdateInfo* pExportInfo)
{
//    STerrainChunkHeader* pTerrainChunkHeader=(STerrainChunkHeader*)pData;
//    STerrainInfo* pTerrainInfo=(STerrainInfo*)(pData+sizeof(STerrainChunkHeader));
//    SwapEndian(*pTerrainChunkHeader, eLittleEndian);
//
//    pData+=(sizeof(STerrainChunkHeader)+sizeof(STerrainInfo));
//    nDataSize-=(sizeof(STerrainChunkHeader)+sizeof(STerrainInfo));
//    return Load_T(pData, nDataSize, pTerrainChunkHeader, pTerrainInfo, ppStatObjTable, ppMatTable, bHotUpdate, pExportInfo);
    return true;
}

bool VoxelTerrain::GetCompiledData(byte* pData, int nDataSize, std::vector<IStatObj*>** ppStatObjTable, std::vector<_smart_ptr<IMaterial>>** ppMatTable, std::vector<struct IStatInstGroup*>** ppStatInstGroupTable, EEndian eEndian, SHotUpdateInfo* pExportInfo)
{
# if !ENGINE_ENABLE_COMPILATION
    CryFatalError("serialization code removed, please enable ENGINE_ENABLE_COMPILATION in Cry3DEngine/StdAfx.h");
    return false;
# else
    bool bHMap(!pExportInfo||pExportInfo->nHeigtmap);
    bool bObjs(!pExportInfo||pExportInfo->nObjTypeMask);

    // write header
    STerrainChunkHeader* pTerrainChunkHeader=(STerrainChunkHeader*)pData;
    pTerrainChunkHeader->nVersion=TERRAIN_CHUNK_VERSION;
    pTerrainChunkHeader->nDummy=0;

    pTerrainChunkHeader->nFlags=(eEndian==eBigEndian)?SERIALIZATION_FLAG_BIG_ENDIAN:0;
    pTerrainChunkHeader->nFlags|=SERIALIZATION_FLAG_SECTOR_PALETTES;

    pTerrainChunkHeader->nFlags2=(Get3DEngine()->m_bAreaActivationInUse?TCH_FLAG2_AREA_ACTIVATION_IN_USE:0);
    pTerrainChunkHeader->nChunkSize=nDataSize;

    STerrainInfo *pTerrainInfo=(STerrainInfo*)(pData+sizeof(STerrainChunkHeader));

    pTerrainInfo->type=m_terrainTypeId;
    pTerrainInfo->nTerrainSizeX_InUnits=m_descriptors.m_size.x;
    pTerrainInfo->nTerrainSizeY_InUnits=m_descriptors.m_size.y;
    pTerrainInfo->nTerrainSizeZ_InUnits=m_descriptors.m_size.z;
    pTerrainInfo->nUnitSize_InMeters=1;
    pTerrainInfo->nSectorSize_InMeters=m_descriptors.m_regionSize.x;
    pTerrainInfo->nSectorSizeY_InMeters=m_descriptors.m_regionSize.y;
    pTerrainInfo->nSectorSizeZ_InMeters=m_descriptors.m_regionSize.z;
    pTerrainInfo->nSectorsTableSize_InSectors=0;
    pTerrainInfo->fHeightmapZRatio=1.0f;
    pTerrainInfo->fOceanWaterLevel=0.0f;// m_descriptors.m_seaLevel;

    SwapEndian(*pTerrainChunkHeader, eEndian);
    SwapEndian(*pTerrainInfo, eEndian);

    UPDATE_PTR_AND_SIZE(pData, nDataSize, sizeof(STerrainChunkHeader)+sizeof(STerrainInfo));

    return (nDataSize==0);
# endif
}

int VoxelTerrain::GetCompiledDataSize(SHotUpdateInfo* pExportInfo)
{
    return sizeof(STerrainChunkHeader)+sizeof(STerrainInfo);
}

void VoxelTerrain::GetStatObjAndMatTables(DynArray<IStatObj*>* pStatObjTable, DynArray<_smart_ptr<IMaterial>>* pMatTable, DynArray<IStatInstGroup*>* pStatInstGroupTable, uint32 nObjTypeMask)
{}

void VoxelTerrain::SetTerrainElevation(int left, int bottom, int areaSize, const float* heightmap, int weightmapSize, const SurfaceWeight* surfaceWeightSet)
{}

void VoxelTerrain::SetOceanWaterLevel(float fOceanWaterLevel)
{}

void VoxelTerrain::ChangeOceanMaterial(_smart_ptr<IMaterial> pMat)
{}

IRenderNode *VoxelTerrain::AddVegetationInstance(int nStaticGroupID, const Vec3 &vPos, const float fScale, uint8 ucBright, uint8 angle, uint8 angleX, uint8 angleY)
{
    return nullptr;
}

bool VoxelTerrain::HasRootNode()
{
    return false;
}

AABB VoxelTerrain::GetRootBBoxVirtual()
{
    return AABB();
}

//CTerrainNode *VoxelTerrain::GetRootNode()
//{
//    return nullptr;
//}

//CTerrainNode *VoxelTerrain::GetLeafNodeAt(Meter x, Meter y)
//{
//    return nullptr;
//}


int VoxelTerrain::GetTerrainSize() const
{
    return 0;
}

int VoxelTerrain::GetSectorSize() const
{
    return 0;
}

Vec3i VoxelTerrain::GetSectorSizeVector() const
{
    const VoxelWorld::DescriptorType  &descriptors=m_world.getDescriptors();
    glm::ivec3 regionSize=descriptors.getRegionCellSize();

    return Vec3i(regionSize.x, regionSize.y, regionSize.z);
}

int VoxelTerrain::GetHeightMapUnitSize() const
{
    return 1;
}

int VoxelTerrain::GetTerrainTextureNodeSizeMeters()
{
    return 0;
}

void VoxelTerrain::GetResourceMemoryUsage(ICrySizer* pSizer, const AABB &crstAABB)
{
}

//const int VoxelTerrain::GetUnitToSectorBitShift()
//{
//    return 0;
//}
//
//const int VoxelTerrain::GetMeterToUnitBitShift const
//{
//    return 0;
//}

float VoxelTerrain::GetBilinearZ(MeterF xWS, MeterF yWS) const
{
    return 0.0f;
}

float VoxelTerrain::GetZ(Meter x, Meter y) const
{
    return 0.0f;
}

bool VoxelTerrain::IsHole(Meter x, Meter y) const
{
    return false;
}

bool VoxelTerrain::Recompile_Modified_Incrementaly_RoadRenderNodes()
{
    return false;
}

void VoxelTerrain::RemoveAllStaticObjects()
{}

bool VoxelTerrain::RemoveObjectsInArea(Vec3 vExploPos, float fExploRadius)
{
    return false;
}

void VoxelTerrain::MarkAllSectorsAsUncompiled()
{}


void VoxelTerrain::CheckVis(const SRenderingPassInfo &passInfo)
{}

void VoxelTerrain::AddVisSector(CTerrainNode* newsec)
{

}

void VoxelTerrain::ClearVisSectors()
{}

void VoxelTerrain::UpdateNodesIncrementaly(const SRenderingPassInfo &passInfo)
{}

void VoxelTerrain::DrawVisibleSectors(const SRenderingPassInfo &passInfo)
{
    AZ_TRACE_METHOD();

    SRendParams renderParams;
    
    renderParams.AmbientColor=ColorF(1, 1, 1, 1);

//    std::vector<VoxelTerrainChunk *> completedChunks=m_updateDispatcher.SyncAllJobs(false, passInfo);
//
//    if(!completedChunks.empty())
//    {
//        for(size_t i=0; i<completedChunks.size(); ++i)
//        {
//            VoxelTerrainChunk *chunk=completedChunks[i];
//
//            chunk->DecrementInUse();
//            chunk->SetMeshReady(true);
//        }
//    }

    if(!passInfo.RenderTerrain()||!Get3DEngine()->m_bShowTerrainSurface)
        return;

//    for(RegionRendererMap::value_type pair:m_regionRenderers)
//    {
//        VoxelTerrainRegion &region=pair.second;
//
//        auto &chunkRendererMap=region.ChunkRenderers();
//
//        for(auto chunkIter=chunkRendererMap.begin(); chunkIter!=chunkRendererMap.end(); ++chunkIter)
//        {
//            VoxelTerrainChunk *chunk=chunkIter->second;
//
////            if(chunk->IsMeshReady())
//            {
//                renderParams.pMatrix=&chunk->GetTransform();
//
//                chunk->RenderMesh(renderParams, passInfo);
//            }
//        }
//    }
//    m_renderCube.draw(&m_program, m_offsetId);
}

void VoxelTerrain::UpdateSectorMeshes(const SRenderingPassInfo &passInfo)
{
    const CCamera &camera=passInfo.GetCamera();
    Vec3i sector=camera.GetSector();
    Vec3 position=camera.GetPosition();

    glm::ivec3 cameraRegion(sector.x, sector.y, sector.z);
    glm::ivec3 cameraChunk((int)position.x, (int)position.y, (int)position.z);

    {
        std::unique_lock<std::mutex> lock(m_updateMutex);

        m_cameraRegion=cameraRegion;
        m_cameraChunk=cameraChunk;
    }

    m_updateEvent.notify_all();
//
//    //if region has changed we need to update our chunk search rings
//    if(m_currentRegion!=cameraRegion || m_currentChunk!=cameraChunk || m_updateSearch)
//        updateChunkSearch(camera);
//
//    //check if any chunks are ready from read thread
    UpdateLoadedChunks(passInfo);

//    //check if any chunks are ready to be meshed
}

void VoxelTerrain::updateThread()
{
    glm::ivec3 cameraRegion;
    glm::ivec3 cameraChunk;

    while(m_updateThreadRun)
    {
        {
            std::unique_lock<std::mutex> lock(m_updateMutex);

            cameraRegion=m_cameraRegion;
            cameraChunk=m_cameraChunk;

            if(m_currentRegion==cameraRegion && m_currentChunk==cameraChunk)
            {
                m_updateEvent.wait(lock);
                continue;
            }
        }

        if(m_currentRegion!=cameraRegion||m_currentChunk!=cameraChunk||m_updateSearch)
            updateChunkSearch(cameraRegion, cameraChunk);
    }
}

void VoxelTerrain::CheckNodesGeomUnload(const SRenderingPassInfo &passInfo)
{}

bool VoxelTerrain::RenderArea(Vec3 vPos, float fRadius, _smart_ptr<IRenderMesh> &pRenderMesh,
    CRenderObject *pObj, _smart_ptr<IMaterial> pMaterial, const char *szComment, float *pCustomData,
    Plane *planes, const SRenderingPassInfo &passInfo)
{
    return false;
}

void VoxelTerrain::IntersectWithShadowFrustum(PodArray<CTerrainNode*>* plstResult, ShadowMapFrustum* pFrustum, const SRenderingPassInfo& passInfo)
{}

void VoxelTerrain::IntersectWithBox(const AABB& aabbBox, PodArray<CTerrainNode*>* plstResult)
{

}

CTerrainNode *VoxelTerrain::FindMinNodeContainingBox(const AABB &someBox)
{
    return nullptr;
}

bool VoxelTerrain::RayTrace(Vec3 const& vStart, Vec3 const& vEnd, SRayTrace* prt)
{
    return false;
}

bool VoxelTerrain::IsOceanVisible() const
{
    return false;
}

COcean *VoxelTerrain::GetOcean()
{
    return nullptr;
}

float VoxelTerrain::GetWaterLevel()
{
    return 0.0f;
}

float VoxelTerrain::GetDistanceToSectorWithWater() const
{
    return 0.0f;
}

int VoxelTerrain::UpdateOcean(const SRenderingPassInfo &passInfo)
{
    return 0;
}

int VoxelTerrain::RenderOcean(const SRenderingPassInfo &passInfo)
{
    return 0;
}

void VoxelTerrain::InitTerrainWater(_smart_ptr<IMaterial> pTerrainWaterShader)
{}


void VoxelTerrain::ClearTextureSets()
{}

bool VoxelTerrain::IsTextureStreamingInProgress() const
{
    return false;
}

bool VoxelTerrain::TryGetTextureStatistics(MacroTexture::TileStatistics &statistics) const
{
    return false;
}

//MacroTexture *VoxelTerrain::GetMacroTexture()
//{
//    return nullptr;
//}
//
//int VoxelTerrain::GetWhiteTexId() const
//{
//    return 0;
//}

void VoxelTerrain::CloseTerrainTextureFile()
{}

void VoxelTerrain::SetTerrainSectorTexture(int nTexSectorX, int nTexSectorY, unsigned int textureId, bool bMergeNotAllowed)
{}

Vec3 VoxelTerrain::GetTerrainSurfaceNormal(Vec3 vPos, float fRange)
{
    return Vec3();
}

bool VoxelTerrain::IsMeshQuadFlipped(const Meter x, const Meter y, const Meter nUnitSize) const
{
    return false;
}

void VoxelTerrain::GetTerrainAlignmentMatrix(const Vec3& vPos, const float amount, Matrix33& matrix33)
{

}

ITerrain::SurfaceWeight VoxelTerrain::GetSurfaceWeight(Meter x, Meter y) const
{
    return ITerrain::SurfaceWeight();
}

//CTerrainUpdateDispatcher *VoxelTerrain::GetTerrainUpdateDispatcher() const
//{
//    return nullptr;
//}

int VoxelTerrain::GetActiveProcObjNodesCount()
{
    return 0;
}

//void VoxelTerrain::ActivateNodeProcObj(CTerrainNode* pNode)
//{
//}

//void VoxelTerrain::SetNodePyramid(int treeLevel, int x, int y, CTerrainNode *node)
//{
//}

void VoxelTerrain::LoadSurfaceTypesFromXML(XmlNodeRef pDoc)
{}

void VoxelTerrain::UpdateSurfaceTypes()
{}

void VoxelTerrain::InitHeightfieldPhysics()
{}

void VoxelTerrain::ResetTerrainVertBuffers()
{}

bool VoxelTerrain::LoadHandle(AZ::IO::HandleType fileHandle, int dataSize, STerrainChunkHeader* chunkHeader, STerrainInfo* terrainInfo, std::vector<struct IStatObj*>** staticObjectTable, std::vector<_smart_ptr<IMaterial> >** materialTable)
{
    return Load(Get3DEngine()->GetLevelFilePath(""), terrainInfo);
}

bool VoxelTerrain::Load(const char *levelPath, STerrainInfo* terrainInfo)
{
    std::string terrainPath=std::string(levelPath)+"/terrain";
    
    ICryPak &cryPak=*gEnv->pCryPak;
    AZ::IO::FileIOBase *fileIO=AZ::IO::FileIOBase::GetDirectInstance(); 

    if(!fileIO->Exists(terrainPath.c_str()))
    {
        fileIO->CreatePath(terrainPath.c_str());

        glm::ivec3 size;

        size.x=terrainInfo->nTerrainSizeX_InUnits;
        size.y=terrainInfo->nTerrainSizeY_InUnits;
        size.z=terrainInfo->nTerrainSizeZ_InUnits;

        m_world.create(terrainPath, "test", size, "EquiRectWorldGenerator");
    }
    else
        m_world.load(terrainPath);

    m_descriptors=m_world.getDescriptors();
    return true;
}

void VoxelTerrain::Save(const char *path)
{
    m_world.saveTo(path);
}

void VoxelTerrain::setViewRadius(float radius)
{
    buildChunkSearchRings(radius);
    m_updateSearch=true; //force search update
}

void VoxelTerrain::buildChunkSearchRings(float radius)
{
    //this build a search area around center based on 3D rings of the player position
    //this are in relative cell positions from player
    m_viewRadius=radius;
    m_viewRadiusMax=radius*1.5f;
    size_t maxChunkRing=(size_t)std::ceil(radius/std::max(std::max(VoxelChunk::sizeX::value, VoxelChunk::sizeY::value), VoxelChunk::sizeZ::value));

//    m_chunkSearchRings.resize(maxChunkRing);
//    m_chunkQueryOrder.resize(maxChunkRing);

//    for(size_t i=0; i<maxChunkRing; ++i)
//        voxigen::ringCube<VoxelChunk>(m_chunkSearchRings[i], i);
}

struct ChunkQueryOffset
{
    ChunkQueryOffset(size_t queryRing, glm::vec3 &offset):queryRing(queryRing), offset(offset) {}

    size_t queryRing;
    glm::vec3 offset;
};

//void VoxelTerrain::updateChunkSearch(const glm::ivec3 &playerRegionIndex, const glm::ivec3 &playerChunkIndex)
//{
//    m_updateSearch=false;
//
//    glm::ivec3 chunkOffset;
//    std::unordered_map<voxigen::Key::Type, ChunkQueryOffset> chunks;
//    glm::ivec3 index;
//    glm::ivec3 currentRegionIndex;
//
//    glm::ivec3 &regionCellSize=m_world.getDescriptors().m_regionCellSize;
//
//    Matrix34 transform=Matrix34::CreateIdentity();
//
//    //loop through all seach rings
//    for(size_t i=0; i<m_chunkSearchRings.size(); ++i)
//    {
//        std::vector<glm::ivec3> &chunkSearchRing=m_chunkSearchRings[i];
//
//        for(size_t j=0; j<chunkSearchRing.size(); ++j)
//        {
//            index=playerChunkIndex+chunkSearchRing[j];
//            currentRegionIndex=playerRegionIndex;
//
//            glm::vec3 regionOffset=m_world.getDescriptors().adjustRegion(currentRegionIndex, index);
//            voxigen::Key key=m_world.getHashes(currentRegionIndex, index);
//
//            chunks.insert(std::pair<voxigen::Key::Type, ChunkQueryOffset>(key.hash, ChunkQueryOffset(i, regionOffset)));
//        }
//    }
//
//    //update region renders and chunks
//    for(auto regionIter=m_regionRenderers.begin(); regionIter!=m_regionRenderers.end(); )
//    {
//        VoxelTerrainRegion &regionRenderer=regionIter->second;
//        Vec3i regionOffset=regionRenderer.Index()-sector;
//
//        regionRenderer.SetOffset(regionOffset.CompMul(Vec3i(regionCellSize.x, regionCellSize.y, regionCellSize.z)));
//
//        auto &chunkRendererMap=regionRenderer.ChunkRenderers();
//
//        for(auto chunkIter=chunkRendererMap.begin(); chunkIter!=chunkRendererMap.end(); )
//        {
//            auto *chunkRenderer=chunkIter->second;
//            voxigen::Key key(chunkRenderer->GetRegionHash(), chunkRenderer->GetChunkHash());
//
//            glm::ivec3 regionIndex=m_world.getDescriptors().regionIndex(key.regionHash);
//            glm::ivec3 chunkIndex=m_world.getDescriptors().chunkIndex(key.chunkHash);
//            float chunkDistance=m_world.getDescriptors().distance(playerRegionIndex, playerChunkIndex, regionIndex, chunkIndex);
//
//            //chunk outside of range so invalidate
//            if(chunkDistance > m_viewRadiusMax)
//            {
//                if(!chunkRenderer->IsInUse()) //need to keep if in use in another thread
//                {
//                    chunkRenderer->Invalidate();
//                    chunkIter=chunkRendererMap.erase(chunkIter);
//                    m_freeChunkRenderers.push_back(chunkRenderer);
//                    continue;
//                }
//            }
//            else
//            {
//                auto iter=chunks.find(key.hash);
//
//                if(iter!=chunks.end())
//                {
//                    chunkOffset=((playerRegionIndex-regionIndex)*m_world.getDescriptors().m_regionCellSize)+chunkIndex;
//
//                    transform.SetTranslation(Vec3((float)chunkOffset.x, (float)chunkOffset.y, (float)chunkOffset.z));
//                    chunkRenderer->SetTransform(transform);
//
//                    m_chunkQueryOrder[iter->second.queryRing].push_back(chunkRenderer);
//                    chunks.erase(iter);
//
//                    CryLog("Chunk (%x, %x) erased", chunkRenderer->GetRegionHash(), chunkRenderer->GetChunkHash());
//                }
//            }
//            ++chunkIter;
//        }
//
//        if(chunkRendererMap.empty())
//            regionIter=m_regionRenderers.erase(regionIter);
//        else
//            ++regionIter;
//    }
//
//    for(auto iter=chunks.begin(); iter!=chunks.end(); ++iter)
//    {
//        VoxelTerrainChunk *chunkRenderer=GetFreeChunkRenderer();
//
//        if(chunkRenderer==nullptr)
//            continue;
//
//        voxigen::Key key(iter->first);
//        VoxelRegionHandle regionHandle=m_world.getRegion(key.regionHash);
//        VoxelChunkHandle chunkHandle=m_world.getChunk(key.regionHash, key.chunkHash);
//
//        chunkRenderer->SetChunk(chunkHandle);
//        m_chunkQueryOrder[iter->second.queryRing].push_back(chunkRenderer);
//
//        auto regionIter=m_regionRenderers.find(key.regionHash);
//
//        if(regionIter==m_regionRenderers.end())
//        {
//            glm::ivec3 index=m_world.getRegionIndex(key.regionHash);
//
//            auto interResult=m_regionRenderers.insert(RegionRendererMap::value_type(key.regionHash, VoxelTerrainRegion(this, key.regionHash, Vec3i(index.x, index.y, index.z), Vec3(iter->second.offset.x, iter->second.offset.y, iter->second.offset.z))));
//
//            assert(interResult.second);
//            regionIter=interResult.first;
//        }
//
//        chunkRenderer->SetParent(&regionIter->second);
//        regionIter->second.ChunkRenderers().insert(VoxelTerrainRegion::ChunkRendererMap::value_type(key.chunkHash, chunkRenderer));
//
//        //request the chunk
//        glm::ivec3 &regionIndex=m_world.getDescriptors().regionIndex(key.regionHash);
//        glm::ivec3 &chunkIndex=chunkHandle->regionOffset();
//
//        chunkOffset=((regionIndex-playerRegionIndex)*m_world.getDescriptors().m_regionCellSize)+chunkIndex;
//
//        transform.SetTranslation(Vec3((float)chunkOffset.x, (float)chunkOffset.y, (float)chunkOffset.z));
//
//        chunkRenderer->SetTransform(transform);
//        chunkRenderer->IncrementInUse();//about to send this thing of to a thread for loading, so lets remember someone else is using
//
//        CryLog("Chunk (%x, %x) request load", chunkHandle->regionHash(), chunkHandle->hash());
//
//        m_world.loadChunk(chunkHandle, 0);
//    }
//
//    m_currentRegion=playerRegionIndex;
//    m_currentChunk=playerChunkIndex;
//}

void VoxelTerrain::updateChunkSearch(const glm::ivec3 &playerRegionIndex, const glm::ivec3 &playerChunkIndex)
{
    
}

void VoxelTerrain::UpdateLoadedChunks(const SRenderingPassInfo &passInfo)
{
    std::vector<voxigen::RegionHash> updatedRegions;
    std::vector<voxigen::Key> updatedChunks;

    m_world.getUpdated(updatedRegions, updatedChunks);

    if(!updatedChunks.empty())
    {
        for(size_t i=0; i<updatedChunks.size(); ++i)
        {
            voxigen::Key &hash=updatedChunks[i];

            CryLog("Chunk (%x, %x) loaded", hash.regionHash, hash.chunkHash);

            auto regionIter=m_regionRenderers.find(hash.regionHash);

            if(regionIter!=m_regionRenderers.end())
            {
                VoxelTerrainRegion::ChunkRendererMap &chunkMap=regionIter->second.ChunkRenderers();

                auto chunkIter=chunkMap.find(hash.chunkHash);

                if(chunkIter!=chunkMap.end())
                {
                    CryLog("Chunk (%x, %x) queue mesh build", hash.regionHash, hash.chunkHash);

                    VoxelTerrainChunk *terrainChunk=chunkIter->second;

                    if(!terrainChunk->empty())
                    {
                        chunkIter->second->IncrementInUse(); //make sure we hold onto it while it is in the prepThread

//                        m_updateDispatcher.QueueJob(chunkIter->second, passInfo);
                    }
                    chunkIter->second->DecrementInUse();//no longer in load thread
                }
            }
            else
            {
                int x=0;
            }
        }
    }
}


//VoxelTerrainChunk *VoxelTerrain::GetFreeChunkRenderer()
//{
//    if(m_freeChunkRenderers.empty())
//    {
//        size_t minRendererCount=0;
//
//        for(size_t i=0; i<m_chunkSearchRings.size(); ++i)
//            minRendererCount+=m_chunkSearchRings[i].size();
//        minRendererCount=(minRendererCount*3)/2;
//
//        if(minRendererCount<m_chunkRenderers.size())
//            minRendererCount=m_chunkRenderers.size()+1;
//
//        if(m_chunkRenderers.size()<minRendererCount)
//        {
//            size_t buildIndex=m_chunkRenderers.size();
//            m_chunkRenderers.resize(minRendererCount);
//
//            //need to setup buffers for new chunks
//            for(size_t i=buildIndex; i<m_chunkRenderers.size(); ++i)
//            {
//                VoxelTerrainChunk *chunkRenderer=new VoxelTerrainChunk();
//
//                m_chunkRenderers[i].reset(chunkRenderer);
//
////                chunkRenderer->SetParent(this);
//
//                m_freeChunkRenderers.push_back(chunkRenderer);
//            }
//        }
//    }
//
//    if(m_freeChunkRenderers.empty())
//        return nullptr;
//
//    VoxelTerrainChunk *renderer=m_freeChunkRenderers.back();
//
//    m_freeChunkRenderers.pop_back();
//    return renderer;
//}
