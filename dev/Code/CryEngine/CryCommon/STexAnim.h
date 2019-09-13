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

#ifndef CRYINCLUDE_CRYCOMMON_STEXANIM_H
#define CRYINCLUDE_CRYCOMMON_STEXANIM_H
#pragma once

#include "ITexture.h"

// Animating Texture sequence definition
struct STexAnim
{
    int m_nRefCount;
    TArray<CTexture*> m_TexPics;
    int m_Rand;
    int m_NumAnimTexs;
    bool m_bLoop;
    float m_Time;

    int Size()
    {
        int nSize = sizeof(STexAnim);
        nSize += m_TexPics.GetMemoryUsage();
        return nSize;
    }
    void Release()
    {
        long refCnt = CryInterlockedDecrement(&m_nRefCount);
        if (refCnt > 0)
        {
            return;
        }
        delete this;
    }
    void AddRef()
    {
        CryInterlockedIncrement(&m_nRefCount);
    }

    STexAnim()
    {
        m_nRefCount = 1;
        m_Rand = 0;
        m_NumAnimTexs = 0;
        m_bLoop = false;
        m_Time = 0.0f;
    }

    ~STexAnim()
    {
        for (uint32 i = 0; i < m_TexPics.Num(); i++)
        {
            ITexture* pTex = (ITexture*) m_TexPics[i];
            SAFE_RELEASE(pTex);
        }
        m_TexPics.Free();
    }

    STexAnim& operator = (const STexAnim& sl)
    {
        // make sure not same object
        if (this == &sl)
        {
            return *this;
        }

        for (uint32 i = 0; i < m_TexPics.Num(); i++)
        {
            ITexture* pTex = (ITexture*)m_TexPics[i];
            SAFE_RELEASE(pTex);
        }
        m_TexPics.Free();

        for (uint32 i = 0; i < sl.m_TexPics.Num(); i++)
        {
            ITexture* pTex = (ITexture*)sl.m_TexPics[i];
            if (pTex)
            {
                pTex->AddRef();
            }

            m_TexPics.AddElem(sl.m_TexPics[i]);
        }

        m_Rand = sl.m_Rand;
        m_NumAnimTexs = sl.m_NumAnimTexs;
        m_bLoop = sl.m_bLoop;
        m_Time = sl.m_Time;

        return *this;
    }
};


#endif // CRYINCLUDE_CRYCOMMON_STEXANIM_H
