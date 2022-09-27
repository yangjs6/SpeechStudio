#include "TextToSpeechWidget.h"

#include "TextToSpeech.h"
#include "TextToSpeechSettings.h"
#include "Components/Button.h"
#include "Components/CircularThrobber.h"
#include "Misc/MessageDialog.h"
#include "Widgets/AudioPlayerWidget.h"
#include "Widgets/TextEditWidget.h"
#include "GenericPlatform/GenericPlatformApplicationMisc.h"

void UTextToSpeechWidget::NativeConstruct()
{
	TextToSpeechSettings = UTextToSpeechSettings::Get();
	TextToSpeechPtr = MakeShareable(new FTextToSpeech());
	
	LoginButton->OnClicked.AddDynamic(this, &UTextToSpeechWidget::OnLogin);
	SynthButton->OnClicked.AddDynamic(this, &UTextToSpeechWidget::OnSynth);
	
	FeedbackContext = FGenericPlatformApplicationMisc::GetFeedbackContext();
	
	RefeshWidget();
	
	Super::NativeConstruct();
}

void UTextToSpeechWidget::NativeDestruct()
{
	TextToSpeechSettings = nullptr;
	TextToSpeechPtr = nullptr;
	Super::NativeDestruct();
}

void UTextToSpeechWidget::OnLogin()
{
	if (TextToSpeechPtr->Login(FeedbackContext, TextToSpeechSettings))
	{
		RefeshWidget();
	}
}

// 将FTextToSpeech::ISynthCallback包装成FHttpUtils::IRequestPostStringCallback
class FThrobberCallback : public FTextToSpeech::ISynthCallback
{
public:
	FThrobberCallback(UProgressBar* InThrobber)
		:Throbber(InThrobber)
	{
		
	}
	virtual void OnBegin() override
	{
		if (Throbber)
		{
			Throbber->SetVisibility(ESlateVisibility::Visible);
		}
	}
	virtual void OnTick(double DeltaTime) override
	{

	}
	virtual void OnEnd() override
	{
		if (Throbber)
		{
			Throbber->SetVisibility(ESlateVisibility::Hidden);
		}
	}
private:
	UProgressBar* Throbber;
};

void UTextToSpeechWidget::OnSynth()
{
	if (!TextToSpeechPtr.IsValid() || !TextToSpeechPtr->IsLogin())
	{
		return;
	}
	
	const FString InText = TextEditWidget->GetTextString();
	const FString TempDir = FPaths::ProjectSavedDir();
	const FString TempPrefix = TEXT("TTS_");
	const FString TempExtension = TEXT(".mp3");
	const FString TempSpeechFile = FPaths::CreateTempFilename(*TempDir, *TempPrefix, *TempExtension);

	FThrobberCallback Callback(WattingThrobber);
	if (TextToSpeechPtr->Synth(InText, TempSpeechFile, FeedbackContext, &Callback, TextToSpeechSettings))
	{
		AudioPlayerWidget->LoadMp3Sound(TempSpeechFile, FeedbackContext);
	}
}

void UTextToSpeechWidget::RefeshWidget()
{
	WattingThrobber->SetVisibility(ESlateVisibility::Hidden);
	if (TextToSpeechPtr.IsValid() && TextToSpeechPtr->IsLogin())
	{
		LoginButton->SetIsEnabled(false);
		SynthButton->SetIsEnabled(true);
	}else
	{
		LoginButton->SetIsEnabled(true);
		SynthButton->SetIsEnabled(false);
	}
}
