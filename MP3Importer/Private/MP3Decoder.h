// Distributed under the MIT License (MIT) (See accompanying file LICENSE
// or copy at http://opensource.org/licenses/MIT)

#pragma once

#include "CoreMinimal.h"
#include "mpg123.h"

//in byte
#define WAV_HEADER_SIZE 44
#define BITS_PER_SAMPLE 16

class UMP3Decoder
{
public:
	~UMP3Decoder();
	void Init(const TArray<uint8>& InBuffer);
	bool Decode(TArray<uint8> &OutBuffer);

	uint32 SizeInBytes = 0;
	uint32 Samplerate = 0;
	uint32 Channels = 0;
	uint32 BitsPerSample = 0;
private:
	static void WriteWaveHeader(uint8* header, uint32 dataLength, uint32 sampleRate, uint32 channels);
	mpg123_handle* Handle = nullptr;
	int ErrorHandle = 0;	
	size_t BlockBufferSize = 0;
};