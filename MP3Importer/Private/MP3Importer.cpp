// Distributed under the MIT License (MIT) (See accompanying file LICENSE
// or copy at http://opensource.org/licenses/MIT)

#include "MP3Importer.h"

#include "Interfaces/IPluginManager.h"
#include "HAL/PlatformProcess.h"
#include "mpg123.h"
#include "Misc/MessageDialog.h"

IMPLEMENT_MODULE(FMP3ImporterModule, MP3Importer)
DEFINE_LOG_CATEGORY(MP3ImporterLog);

int				(*mpg123_init)(void);
mpg123_handle *	(*mpg123_new)(const char* decoder, int *error);
size_t			(*mpg123_outblock)(mpg123_handle *mh);
int				(*mpg123_open_feed)(mpg123_handle *mh);
int				(*mpg123_feed)(mpg123_handle *mh, const unsigned char *in, size_t size);
int				(*mpg123_getformat)(mpg123_handle *mh, long *rate, int *channels, int *encoding);
int				(*mpg123_encsize)(int encoding);
int				(*mpg123_read)(mpg123_handle *mh, unsigned char *outmemory, size_t outmemsize, size_t *done);
void			(*mpg123_delete)(mpg123_handle *mh);
int				(*mpg123_close)(mpg123_handle *mh);
void			(*mpg123_exit)(void);

void FMP3ImporterModule::StartupModule()
{
	DLLHandle = nullptr;
	if (!LoadLibrary())
	{
		FMessageDialog::Open(EAppMsgType::Ok,
			FText::FromString(FString::Printf(TEXT("加载mp3模块失败，请确认%s是否存在"), DLL_NAME)));	
	}
}
 
void FMP3ImporterModule::ShutdownModule()
{
	UE_LOG(MP3ImporterLog, Display, TEXT("ShutdownModule"));

	if (DLLHandle != nullptr)
	{
		FPlatformProcess::FreeDllHandle(DLLHandle);
	}
}

bool FMP3ImporterModule::LoadLibrary()
{	
	// Get the base directory of this plugin
	const FString DllDir = FPaths::ProjectPluginsDir() + TEXT("/TextToSpeech/Source/MP3Importer/Lib/x64");
	const FString DllPath = FPaths::Combine(*DllDir,  DLL_NAME);
	if (!FPaths::FileExists(DllPath))
	{
		UE_LOG(MP3ImporterLog, Error, TEXT("Could not find %s in %s"), DLL_NAME, *DllPath);
	}
	FPlatformProcess::PushDllDirectory(*DllDir);
	DLLHandle = FPlatformProcess::GetDllHandle(DLL_NAME); // Retrieve the DLL.
	FPlatformProcess::PopDllDirectory(*DllDir);
	if (DLLHandle != nullptr)
	{
		mpg123_init =		(int(__cdecl *)(void))												FPlatformProcess::GetDllExport(DLLHandle, *FString("mpg123_init"));
		mpg123_new =		(mpg123_handle* (__cdecl *)(const char*, int *))					FPlatformProcess::GetDllExport(DLLHandle, *FString("mpg123_new"));
		mpg123_outblock =	(size_t(__cdecl *)(mpg123_handle*))									FPlatformProcess::GetDllExport(DLLHandle, *FString("mpg123_outblock"));
		mpg123_open_feed =	(int(__cdecl *)(mpg123_handle*))									FPlatformProcess::GetDllExport(DLLHandle, *FString("mpg123_open_feed"));
		mpg123_feed =		(int(__cdecl *)(mpg123_handle*, const unsigned char*, size_t))		FPlatformProcess::GetDllExport(DLLHandle, *FString("mpg123_feed"));
		mpg123_getformat =	(int(__cdecl *)(mpg123_handle*, long*, int*, int*))					FPlatformProcess::GetDllExport(DLLHandle, *FString("mpg123_getformat"));
		mpg123_encsize =	(int(__cdecl *)(int))												FPlatformProcess::GetDllExport(DLLHandle, *FString("mpg123_encsize"));
		mpg123_read =		(int(__cdecl *)(mpg123_handle*, unsigned char*, size_t, size_t*))	FPlatformProcess::GetDllExport(DLLHandle, *FString("mpg123_read"));
		mpg123_delete =		(void(__cdecl *)(mpg123_handle*))									FPlatformProcess::GetDllExport(DLLHandle, *FString("mpg123_delete"));
		mpg123_close =		(int(__cdecl *)(mpg123_handle*))									FPlatformProcess::GetDllExport(DLLHandle, *FString("mpg123_close"));
		mpg123_exit =		(void(__cdecl *)(void))												FPlatformProcess::GetDllExport(DLLHandle, *FString("mpg123_exit"));

		//TODO: check each for null?
		if (mpg123_init == nullptr ||
			mpg123_new == nullptr ||
			mpg123_outblock == nullptr ||
			mpg123_open_feed == nullptr ||
			mpg123_feed == nullptr ||
			mpg123_getformat == nullptr ||
			mpg123_encsize == nullptr ||
			mpg123_read == nullptr ||
			mpg123_delete == nullptr ||
			mpg123_close == nullptr ||
			mpg123_exit == nullptr )
		{
			UE_LOG(MP3ImporterLog, Error, TEXT("GetDllExport Error."));
			return false;
		}
	}
	else
	{
		UE_LOG(MP3ImporterLog, Error, TEXT("Could not GetDLLExports."));
		return false;
	}
	return true;
}

