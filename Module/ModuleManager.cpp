/* Copyright 2020 - 2026, Hansson Software. All rights reserved. */

#include "ModuleManager.h"
#include <HAL/HAL.h>
#include <Application/Application.h>
#include <Special/EngineConstants.h>
#include <Special/LaunchMeteorSuite.h>

#ifdef MR_PLATFORM_WINDOWS
#include <HAL/WindowsPlatform.h>
#include <libloaderapi.h>
#endif // MR_PLATFORM_WINDOWS
#include <cwchar>

#pragma warning(disable : 6387)

LOG_ADDCATEGORY(ModuleManager);


ModuleManager& ModuleManager::Get()
{
    static ModuleManager instance;
    return instance;
}

void ModuleManager::InitializeModules()
{
}

void ModuleManager::ShutdownModules()
{
    for (auto& mdl : modules)
    {
        mdl.data->ShutdownModule();
    }
}

typedef IModule* (*fv)(MemoryHandler*);

bool ModuleManager::RegisterModule(const StringView& moduleName)
{
    if (IsModuleLoaded(moduleName))
        return true;

#ifdef MR_PLATFORM_WINDOWS
    wchar_t libraryName[512] = {};

    swprintf(libraryName, 511, L"%hs%ls.dll", moduleName.ptr, LIBRARY_NAME_CONFIG_SUFFIX_WIDE);
    //HAL::ConvertToWide(libraryName, moduleName.size, (char*)moduleName.ptr);


    HMODULE module = LoadLibraryW(libraryName);
    if (module != nullptr)
    {
        fv moduleInstantiation = (fv)GetProcAddress(module, "InitialiseModule");

        if (moduleInstantiation)
        {
            IModule* newModule = moduleInstantiation(GetMemoryManager());
            newModule->name = moduleName;
            newModule->interface = module;
            MR_LOG(LogModuleManager, Log, "New module loaded: %hs", moduleName.ptr);
            newModule->StartupModule();

            modules[(char*)moduleName.ptr] = newModule;

            return true;
        }
    }
    else
    {
        MR_LOG(LogModuleManager, Error, "Unable to load %hs module: %d", moduleName.ptr, GetLastError());
    }

#endif // MR_PLATFORM_WINDOWS


    return false;
}

bool ModuleManager::UnloadModule(const StringView& moduleName)
{
    for (auto& mdl : modules)
    {
        if (mdl.data->GetName().Chr() != (char*)moduleName.ptr)
           continue;

        mdl.data->ShutdownModule();
        return true;
    }

    return true;
}

bool ModuleManager::IsModuleLoaded(const StringView& moduleName)
{
    for (auto& mdl : modules)
    {
        if (mdl.data->GetName().Chr() == (char*)moduleName.ptr)
        {
            return mdl.data->moduleState == IModule::ModuleState::Enabled;
        }
    }

    return false;
}

void ModuleManager::UpdateModules(float dt)
{
    for (auto& module : modules)
    {
        if (module.data->GetModuleState() != IModule::ModuleState::Running)
            continue;

        module.data->Update();
    }
}
