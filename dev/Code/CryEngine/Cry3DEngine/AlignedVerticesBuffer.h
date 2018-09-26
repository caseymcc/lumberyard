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

#ifndef CRYINCLUDE_CRY3DENGINE_ALIGNEDVERTICESBUFFER_H
#define CRYINCLUDE_CRY3DENGINE_ALIGNEDVERTICESBUFFER_H
#pragma once

namespace cryengine
{

template<typename _IndexType, typename _VertexType, size_t _Alignment>
struct AlignedVerticesBuffer
{
    size_t indicesCount;
    _IndexType *indices _ALIGN(128);
    size_t verticesCount;
    _VertexType *vertices _ALIGN(128);
};

template<typename _IndexType, typename _VertexType, size_t _Alignment>
AlignedVerticesBuffer<_IndexType, _VertexType, _Alignment> *allocateAlignedVerticesBuffer(IGeneralMemoryHeap *heap, size_t indices, size_t vertices)
{
    AlignedVerticesBuffer<_IndexType, _VertexType, _Alignment> *buffer;
    const unsigned alignPad=TARGET_DEFAULT_ALIGN;

    size_t allocSize=sizeof(AlignedVerticesBuffer<_IndexType, _VertexType, _Alignment>);
    
    allocSize+=alignPad;//extra space for alignment
    allocSize+=sizeof(_IndexType)*indices;
    allocSize+=alignPad;//extra space for alignment
    allocSize+=sizeof(_VertexType)*vertices;

    uint8 *data=(uint8 *)heap->Memalign(__alignof(AlignedVerticesBuffer<_IndexType, _VertexType, _Alignment>), allocSize, "Chunk Mesh");

    if(data==nullptr)
        return nullptr;

    buffer=reinterpret_cast<AlignedVerticesBuffer<_IndexType, _VertexType, _Alignment> *>(data);
    buffer->indicesCount=indices;
    buffer->verticesCount=vertices;
    data+=sizeof(AlignedVerticesBuffer<_IndexType, _VertexType, _Alignment>);
    data=reinterpret_cast<uint8 *>((reinterpret_cast<uintptr_t>(data)+alignPad) & ~uintptr_t(alignPad));//align
    buffer->indices=reinterpret_cast<_IndexType *>(data);
    data+=sizeof(_IndexType)*indices;
    data=reinterpret_cast<uint8 *>((reinterpret_cast<uintptr_t>(data)+alignPad) & ~uintptr_t(alignPad));//align
    buffer->vertices=reinterpret_cast<_VertexType *>(data);
    
    return buffer;
}

}

#endif//CRYINCLUDE_CRY3DENGINE_ALIGNEDVERTICESBUFFER_H
