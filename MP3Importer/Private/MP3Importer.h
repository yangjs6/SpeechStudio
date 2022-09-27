// Distributed under the MIT License (MIT) (See accompanying file LICENSE
// or copy at http://opensource.org/licenses/MIT)

#pragma once
 
#include "Modules/ModuleManager.h"

#include "Logging/LogCategory.h"
#include "Logging/LogMacros.h"
#include "Logging/LogVerbosity.h"

DECLARE_LOG_CATEGORY_EXTERN(MP3ImporterLog, All, All)

class FMP3ImporterModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private: 
	bool LoadLibrary();
	void *DLLHandle = nullptr;
};