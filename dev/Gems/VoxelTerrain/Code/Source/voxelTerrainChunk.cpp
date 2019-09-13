//#include "StdAfx.h"

#include "voxelTerrainChunk.h"

#include "voxelTerrain.h"
#include "voxelTerrainRegion.h"
#include "AlignedVerticesBuffer.h"

#include "voxigen/cubicMeshBuilder.h"


VoxelTerrainChunk::VoxelTerrainChunk():
m_parent(nullptr),
m_dataLoaded(false),
m_meshReady(false)
{

}

VoxelTerrainChunk::~VoxelTerrainChunk()
{

}

void VoxelTerrainChunk::SetChunk(VoxelChunkHandle chunkHandle)
{
    m_inUse=0;
    m_meshReady=false;
    m_dataLoaded=false;
    m_chunkHandle=chunkHandle;

    const glm::ivec3 &offset=chunkHandle->regionOffset();
    glm::ivec3 &size=chunkHandle->size();
    
    Vec3 min=Vec3((float)offset.x, (float)offset.y, (float)offset.z);
    Vec3 max=Vec3((float)offset.x+size.x, (float)offset.y+size.y, (float)offset.z+size.z);

    m_box=AABB(min, max);
}

void VoxelTerrainChunk::Render(const SRendParams &rendParams, const SRenderingPassInfo &passInfo)
{
    RenderMesh(rendParams, passInfo);
}

void VoxelTerrainChunk::drawText(Vec3 pos, const char* format, ...)
{
    SDrawTextInfo textInfo;

    textInfo.color[0]=1.0f;
    textInfo.color[1]=0.0f;
    textInfo.color[2]=0.0f;
    textInfo.color[3]=1.0f;

    textInfo.xscale=12.0f;
    textInfo.yscale=12.0f;

    //textInfo.flags=eDrawText_UseTransform|eDrawText_FixedSize;

    va_list args;
    va_start(args, format);
    GetRenderer()->GetIRenderAuxGeom()->RenderText(pos, textInfo, format, args);
    va_end(args);
}

void VoxelTerrainChunk::RenderMesh(const SRendParams &rendParams, const SRenderingPassInfo &passInfo)
{
//    SetupTexturing(passInfo);

    if(!passInfo.RenderTerrainDetailMaterial()||passInfo.IsShadowPass())
        return;

    const CCamera &camera=passInfo.GetCamera();
    Vec3 offset=Vec3((m_parent->Index()-camera.GetSector()).CompMul(camera.GetSectorSize()));

    m_currentBox=m_box;
    m_currentBox.Move(offset);
    //are we in the camera's view?
    if(!passInfo.GetCamera().IsAABBVisible_F(m_currentBox))
        return;
    
//    if(!m_dataLoaded)
//    {
//        //load data
//        return;
//    }
    const glm::ivec3 &chunkIndex=m_chunkHandle->regionOffset();
    
    drawText(m_currentBox.min, "(%d,%d,%d) %d", chunkIndex.x, chunkIndex.y, chunkIndex.z, m_chunkHandle->empty()?1:0);
    if(!m_meshReady)
    {
        GetRenderer()->GetIRenderAuxGeom()->DrawAABB(m_currentBox, false, ColorB(255, 0, 0, 255), eBBD_Faceted);
        //build mesh
        return;
    }

    GetRenderer()->GetIRenderAuxGeom()->DrawAABB(m_currentBox, false, ColorB(255, 0, 0, 128), eBBD_Faceted);
    

    CRenderObject *renderObject=GetIdentityCRenderObject(passInfo.ThreadID());

    if(!renderObject)
        return;

    VoxelChunk *chunk=m_chunkHandle->chunk();
//    Vec3 vOrigin(m_nOriginX, m_nOriginY, 0);
//    glm::ivec3 index=chunk->getIndex();
//    Vec3 vOrigin((float)index.x, (float)index.y, (float)index.z);

    renderObject->m_pRenderNode=0;
//    renderObject->m_fDistance=m_DistanceToCamera[passInfo.GetRecursiveLevel()];
//    renderObject->m_II.m_Matrix.SetIdentity();
//    renderObject->m_II.m_Matrix.SetTranslation(vOrigin);
    renderObject->m_II.m_Matrix=m_transform;
    renderObject->m_II.m_Matrix.SetTranslation(offset);

    VoxelTerrain *terrain=m_parent->GetTerrain();

    int whiteTextureId=terrain->GetWhiteTextureId();
    //testing set to white
    m_renderMesh->SetCustomTexID(whiteTextureId);

    _smart_ptr<IMaterial> material=terrain->GetMaterial();

//    m_renderMesh->AddRenderElements(material, renderObject, passInfo, EFSLIST_GENERAL, 1);
    m_renderMesh->Render(rendParams, renderObject, material, passInfo);

//    if(material && material->GetShaderItem().m_pShader)
//    {
//        bool isTerrainType=(material->GetShaderItem().m_pShader->GetShaderType()==eST_Terrain);
//
//        if(isTerrainType)
//            mesh->AddRenderElements(material, detailObject, passInfo, EFSLIST_TERRAINLAYER, 1);
//        else
//            gEnv->pLog->LogError("Terrain Layer - Incorrect Terrain Layer shader type - [%d] found in frame [%d]",
//                material->GetShaderItem().m_pShader->GetShaderType(), gEnv->pRenderer->GetFrameID());
//    }
//    else
//        gEnv->pLog->LogError("Terrain Layer - Unassigned material or shader in frame [%d]",
//            gEnv->pRenderer->GetFrameID());

}

const AABB VoxelTerrainChunk::GetBBoxVirtual()
{
    return m_currentBox;
}

void VoxelTerrainChunk::FillBBox(AABB& aabb)
{

}

ICharacterInstance *VoxelTerrainChunk::GetEntityCharacter(unsigned int nSlot, Matrix34A *matrix, bool bReturnOnlyVisible) 
{
    return nullptr;
}

bool VoxelTerrainChunk::IsRenderNode() 
{
    return false;
}

EERType VoxelTerrainChunk::GetRenderNodeType()
{
    return eERType_NotRenderNode;
}

VoxelTerrain *VoxelTerrainChunk::GetTerrain() 
{
    return m_parent->GetTerrain();
}

void VoxelTerrainChunk::Invalidate()
{

}

bool VoxelTerrainChunk::update(const SRenderingPassInfo &passInfo, IGeneralMemoryHeap *heap)
{
    return buildMesh(passInfo, heap);
}

bool VoxelTerrainChunk::buildMesh(const SRenderingPassInfo &passInfo, IGeneralMemoryHeap *heap)
{
    CryLog("Chunk (%x, %x) building mesh", m_chunkHandle->regionHash(), m_chunkHandle->hash());

    voxigen::ChunkMesh mesh;
    voxigen::buildCubicMesh(mesh, m_chunkHandle->chunk());

    std::vector<voxigen::ChunkMeshVertex> &meshVerticies=mesh.getVerticies();
    std::vector<int> &meshIndices=mesh.getIndices();
    
    cryengine::AlignedVerticesBuffer<vtx_idx, SVF_P3F_C4B_T2F, TARGET_DEFAULT_ALIGN> *vertexBuffer;

    vertexBuffer=cryengine::allocateAlignedVerticesBuffer<vtx_idx, SVF_P3F_C4B_T2F, TARGET_DEFAULT_ALIGN>(heap, meshIndices.size(), meshVerticies.size());

    if(vertexBuffer==nullptr)
        return false;

    const glm::ivec3 &offset=m_chunkHandle->regionOffset();

    //HACK: need to update mesher to use existing buffers
    for(size_t i=0; i<meshIndices.size(); ++i)
        vertexBuffer->indices[i]=meshIndices[i];
    for(size_t i=0; i<meshVerticies.size(); ++i)
    {
        SVF_P3F_C4B_T2F &vertex=vertexBuffer->vertices[i];
        voxigen::ChunkMeshVertex &meshVertex=meshVerticies[i];

        vertex.xyz.x=(float)meshVertex.x+offset.x;
        vertex.xyz.y=(float)meshVertex.y+offset.y;
        vertex.xyz.z=(float)meshVertex.z+offset.z;

        vertex.color.r=255;
        vertex.color.g=255;
        vertex.color.b=255;
        vertex.color.a=255;

        vertex.st.x=0.0f;
        vertex.st.y=0.0f;
    }

    m_renderMesh=GetRenderer()->CreateRenderMeshInitialized(
        vertexBuffer->vertices, vertexBuffer->verticesCount, eVF_P3F_C4B_T2F,
        vertexBuffer->indices, vertexBuffer->indicesCount,
        prtTriangleList,
        "TerrainChunk", "TerrainChunk",
        eRMT_Dynamic, 1, 0, NULL, NULL, false, true, nullptr);

    VoxelTerrain *terrain=m_parent->GetTerrain();
    _smart_ptr<IMaterial> material=terrain->GetMaterial();

    m_renderMesh->SetChunk(material, 0, vertexBuffer->verticesCount, 0, vertexBuffer->indicesCount, 1.0f, eVF_P3F_C4B_T2F);

    heap->Free(vertexBuffer);

    CryLog("Chunk (%x, %x) mesh built", m_chunkHandle->regionHash(), m_chunkHandle->hash());

    return true;
}

