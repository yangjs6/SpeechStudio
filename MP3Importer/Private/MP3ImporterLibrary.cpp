// Distributed under the MIT License (MIT) (See accompanying file LICENSE
// or copy at http://opensource.org/licenses/MIT)

#include "MP3ImporterLibrary.h"
#include "MP3Decoder.h"
#include "Misc/FileHelper.h"
#include "Sound/SoundWave.h"

USoundWave* UMP3ImporterLibrary::CreateMp3Sound(const FString& Filename, FString& ErrorMessage)
{
	TArray<uint8> Buffer;
	FFileHelper FileHelper;
	if (!FileHelper.LoadFileToArray(Buffer, *Filename))
	{
		ErrorMessage = FString::Printf(TEXT("文件无法加载 %s."), *Filename);
		return nullptr;
	}
	
	if (mpg123_init == nullptr)
	{
		ErrorMessage = FString::Printf(TEXT("缺少 mpg123 库文件 %s "), DLL_NAME);
		return nullptr;
	}

	// Read the mp3 header and make sure we have valid data
	UMP3Decoder Decoder;
	Decoder.Init(Buffer);

	if (Decoder.BitsPerSample != 16)
	{
		ErrorMessage = FString::Printf(TEXT("只支持16位数据 (%s)."), *Filename);
		return nullptr;
	}

	if (Decoder.Channels != 1 && Decoder.Channels != 2)
	{
		ErrorMessage = FString::Printf(TEXT("只支持单声道或双声道 (%s)."), *Filename);
		return nullptr;
	}

	TArray<uint8> RawWavBuffer;
	RawWavBuffer.Reserve(Buffer.Num() * 16);
	Decoder.Decode(RawWavBuffer);
		
	USoundWave* Sound = NewObject<USoundWave>();

	
	// Calculate duration.
	Sound->Duration = (float)Decoder.SizeInBytes / Decoder.Samplerate / Decoder.Channels / (BITS_PER_SAMPLE / 8);
	Sound->SetSampleRate(Decoder.Samplerate);
	Sound->NumChannels = Decoder.Channels;
	Sound->RawPCMDataSize = Decoder.SizeInBytes;

	// Sound->RawData.Lock(LOCK_READ_WRITE);
	// void* LockedData = Sound->RawData.Realloc(RawWavBuffer.Num() * RawWavBuffer.GetTypeSize());
	// FMemory::Memcpy(LockedData, RawWavBuffer.GetData(), RawWavBuffer.Num() * RawWavBuffer.GetTypeSize());
	// Sound->RawData.Unlock();

	// RawPCMData also needs to fill when play audio in runtime game
	Sound->RawPCMData = (uint8*)FMemory::Malloc(Sound->RawPCMDataSize);
	FMemory::Memmove(Sound->RawPCMData, RawWavBuffer.GetData(), RawWavBuffer.Num());

	return Sound;
}
