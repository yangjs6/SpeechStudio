#include "Communication.h"

void FCommunicationModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FCommunicationModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
}

DEFINE_LOG_CATEGORY(Communication);

IMPLEMENT_MODULE(FCommunicationModule, Communication);