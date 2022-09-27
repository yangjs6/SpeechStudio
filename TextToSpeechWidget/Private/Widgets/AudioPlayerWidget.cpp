#include "Widgets/AudioPlayerWidget.h"

#include "MP3ImporterLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Misc/MessageDialog.h"

void UAudioPlayerWidget::NativeConstruct()
{
	LoadButton->OnClicked.AddDynamic(this, &UAudioPlayerWidget::OnLoad);
	PlayButton->OnClicked.AddDynamic(this, &UAudioPlayerWidget::OnPlay);
	PauseButton->OnClicked.AddDynamic(this,&UAudioPlayerWidget::OnPause);
	StopButton->OnClicked.AddDynamic(this,&UAudioPlayerWidget::OnStop);
	TimeSlider->OnValueChanged.AddDynamic(this,&UAudioPlayerWidget::OnTimeChanged);
	
	RefeshWidget();
	Super::NativeConstruct();
}

void UAudioPlayerWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (!AudioComponent)
	{
		return;
	}

	if (AudioComponent->GetPlayState() == EAudioComponentPlayState::Playing)
	{
		float PlaybackTime = GetPlaybackTime();
		SetPlaybackTime(PlaybackTime + InDeltaTime);
	}	
}

void UAudioPlayerWidget::OnLoad()
{
	FString Mp3File = FileTextBox->GetText().ToString();
	FString ErrorMessage;
	USoundWave* Sound = UMP3ImporterLibrary::CreateMp3Sound(Mp3File, ErrorMessage);
	if (Sound)
	{
		UpdateSound(Sound);
	}
}

bool UAudioPlayerWidget::LoadMp3Sound(const FString& InMp3File, FFeedbackContext* FeedbackContext)
{
	FString ErrorMessage;
	USoundWave* Sound = UMP3ImporterLibrary::CreateMp3Sound(InMp3File, ErrorMessage);
	if (!Sound)
	{
		FeedbackContext->Serialize(*ErrorMessage, ELogVerbosity::Error, FName("UAudioPlayerWidget::LoadMp3Sound"));
		return false;
	}
	FileTextBox->SetText(FText::FromString(InMp3File));
	UpdateSound(Sound);
	return true;
}

void UAudioPlayerWidget::UpdateSound(USoundBase* InSound)
{
	if (AudioComponent)
	{
		if (AudioComponent->Sound == InSound)
		{
			return;
		}
		AudioComponent->Stop();
		AudioComponent = nullptr;
	}	

	if (InSound)
	{
		AudioComponent = UGameplayStatics::CreateSound2D(this, InSound, 1, 1, 0, nullptr, false , false);
		AudioComponent->OnAudioPlayStateChanged.AddDynamic(this, &UAudioPlayerWidget::OnPlayStateChaged);
	}
	
	RefeshWidget();
}

void UAudioPlayerWidget::SetPlaybackTime(float InTime) const
{
	InTime = FMath::Clamp(InTime, TimeSlider->MinValue, TimeSlider->MaxValue);
	TimeSlider->SetValue(InTime);
	PlaybackTimeText->SetText(FText::FromString(FString::Printf(TEXT("%.1f"),TimeSlider->GetValue())));
}

float UAudioPlayerWidget::GetPlaybackTime() const
{
	return TimeSlider->GetValue();
}

void UAudioPlayerWidget::SetDuration(float InTime) const
{
	InTime = FMath::Max(InTime, 0.0f);
	TimeSlider->SetMinValue(0.0f);
	TimeSlider->SetMaxValue(InTime);
	DurationText->SetText(FText::FromString(FString::Printf(TEXT("%.1f"),TimeSlider->MaxValue)));
}

float UAudioPlayerWidget::GetDuration() const
{
	return TimeSlider->MaxValue;
}

void UAudioPlayerWidget::RefeshWidget()
{
	float Duration = 0;
	if (AudioComponent)
	{
		Duration = AudioComponent->Sound->Duration;
		PlayButton->SetIsEnabled(true);
		PauseButton->SetIsEnabled(false);
		StopButton->SetIsEnabled(false);
	}else
	{
		PlayButton->SetIsEnabled(false);
		PauseButton->SetIsEnabled(false);
		StopButton->SetIsEnabled(false);
	}

	SetPlaybackTime(0);
	SetDuration(Duration);
}

void UAudioPlayerWidget::OnPlayStateChaged(EAudioComponentPlayState PlayState)
{
	switch (PlayState)
	{
	case EAudioComponentPlayState::Playing:
		PlayButton->SetIsEnabled(false);
		PauseButton->SetIsEnabled(true);
		StopButton->SetIsEnabled(true);
		break;
	case EAudioComponentPlayState::Paused:
		PlayButton->SetIsEnabled(true);
		PauseButton->SetIsEnabled(false);
		StopButton->SetIsEnabled(true);
		break;
	case EAudioComponentPlayState::Stopped:
		PlayButton->SetIsEnabled(true);
		PauseButton->SetIsEnabled(false);
		StopButton->SetIsEnabled(false);
		break;
	default:
		break;
	}
}

void UAudioPlayerWidget::OnPlay()
{
	if (AudioComponent)
	{
		float PlaybackTime = GetPlaybackTime();
		AudioComponent->Play(PlaybackTime);	
		AudioComponent->SetPaused(false);	
	}
}

void UAudioPlayerWidget::OnPause()
{
	if (AudioComponent)
	{
		AudioComponent->SetPaused(true);		
	}
}

void UAudioPlayerWidget::OnStop()
{
	if (AudioComponent)
	{
		AudioComponent->Stop();		
	}
	SetPlaybackTime(0);
}

void UAudioPlayerWidget::OnTimeChanged(float InValue)
{
	if (AudioComponent)
	{
		AudioComponent->Play(InValue);		
	}
	SetPlaybackTime(InValue);
}

