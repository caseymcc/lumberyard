#include <StdAfx.h>
#include "EditorTerrainFactory.h"

struct EditorTerrainInfo
{
    EditorTerrainInfo(const char *name, EditorTerrainFactory::FactoryFunction func):name(name), func(func) {}

    std::string name;
    EditorTerrainFactory::FactoryFunction func;
};

struct EditorTerrainFactoryHidden
{
    std::vector<EditorTerrainInfo> terrains;
};
EditorTerrainFactoryHidden g_editorTerrainFactoryHidden;

size_t EditorTerrainFactory::getTerrainId(const char *name)
{
    for(size_t i=0; i<g_editorTerrainFactoryHidden.terrains.size(); ++i)
    {
        if(g_editorTerrainFactoryHidden.terrains[i].name==name)
            return i;
    }
    return std::numeric_limits<size_t>::max();
}

const char *EditorTerrainFactory::getTerrainName(size_t id)
{
    assert(id>=0&&id<g_editorTerrainFactoryHidden.terrains.size());

    return g_editorTerrainFactoryHidden.terrains[id].name.c_str();
}

size_t EditorTerrainFactory::terrainSize()
{
    return g_editorTerrainFactoryHidden.terrains.size();
}

IEditorTerrain *EditorTerrainFactory::create(size_t id)
{
    assert(id>=0&&id<g_editorTerrainFactoryHidden.terrains.size());

    return g_editorTerrainFactoryHidden.terrains[id].func();
}

size_t EditorTerrainFactory::registerTerrain(const char *name, FactoryFunction func)
{
    size_t id=g_editorTerrainFactoryHidden.terrains.size();

    g_editorTerrainFactoryHidden.terrains.emplace_back(name, func);
    return id;
}