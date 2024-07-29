// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AC_APIInformation.h"

#include "HttpModule.h"

UAC_APIInformation::UAC_APIInformation()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UAC_APIInformation::StartTimerForCheckPhoto()
{
	Timer = 0;
	GetWorld()->GetTimerManager().SetTimer(Timer_ScreenshotCheckTimer, this, &UAC_APIInformation::CheckIsReadyScreenshot, 0.5f, true, 2);

}

void UAC_APIInformation::CheckIsReadyScreenshot()
{
	if (FPaths::FileExists(FilePath))
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "CheckForScreenshot - Finded");
		GetWorld()->GetTimerManager().ClearTimer(Timer_ScreenshotCheckTimer);

		FTimerHandle TimerHandle;
		FTimerDelegate Delegate_DeleteFile;
		Delegate_DeleteFile.BindLambda([&]()
			{
				SendingPDF_File();
			});
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, Delegate_DeleteFile, 2.f, false, 2);

	}
	else
	{
		if (Timer >= 10)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "CheckForScreenshot - No finded");
			GetWorld()->GetTimerManager().ClearTimer(Timer_ScreenshotCheckTimer);
		}
		else
		{
			++Timer;
		}
	}

}



void UAC_APIInformation::SendMessage(const ETypeMessenger setChoiceTypeMessenger, bool isAsyncLogic,
	const FString SetMessage, const FString SetNumber)
{
	if (ChoiceTypeMessenger == ETypeMessenger::EM_NONE) { return; }
	if (!CheckPhoneToValid(SetNumber)) { return; }

	FString l_URL;
	URLSettings(ChoiceTypeMessenger, EA_Message, isAsyncLogic, l_URL);

	FString JsonData = FString::Printf(TEXT("{ \"body\": \"%s\" , \"recipient\": \"%s\" }"), *SetMessage, *SetNumber);

	SetConfigRequest(JsonData, l_URL);
}


void UAC_APIInformation::SendFilePDF(const ETypeMessenger setChoiceTypeMessenger,
	bool isAsyncLogic, const FString SetMessage, const FString SetNumber, FString inFilePath)
{
	if (setChoiceTypeMessenger == ETypeMessenger::EM_NONE) { return; }
	if (!CheckPhoneToValid(SetNumber)) { return; }

	ChoiceTypeMessenger = setChoiceTypeMessenger;
	AsyncLogic = isAsyncLogic;
	Number = SetNumber;
	Message = SetMessage;
	FilePath = inFilePath;

	if (FilePath.IsEmpty())
	{
		FilePath = Screenshot(); // We can change this to other logic;
		StartTimerForCheckPhoto();
	}
	else
	{
		SendingPDF_File();
	}


}
void UAC_APIInformation::SendingPDF_File()
{

	if (FPaths::FileExists(FilePath))
	{
		FString l_URL;
		URLSettings(ChoiceTypeMessenger, EA_Image, AsyncLogic, l_URL);

		FString l_Base64String;
		if (!ConvertToBase64(FilePath, l_Base64String)) { return; }

		//Data with information to WappiApi
		FString JsonData = FString::Printf(
			TEXT("{  \"recipient\": \"%s\",	\"caption\": \"%s\",	\"b64_file\": \"%s\" }"),
			*Number, *Message, *l_Base64String);


		SetConfigRequest(JsonData, l_URL);
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "SendingPDF_File - SendPhoto");



		//Auto delete only screenshot !
		if (FilePath.Contains("screenshot.png"))
		{
			//Delete
			FTimerHandle TimerHandle;
			FTimerDelegate Delegate_DeleteFile;
			Delegate_DeleteFile.BindLambda([&]()
				{
					if (FPaths::FileExists(FilePath))
					{
						// Удалите файл
						IFileManager::Get().Delete(*FilePath);
						GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "SendingPDF_File - DeletePhoto");
					}
				});

			GetWorld()->GetTimerManager().SetTimer(TimerHandle, Delegate_DeleteFile, 5.f, false, 3);
		}
	}
}

bool UAC_APIInformation::CheckPhoneToValid(const FString SetNumber)
{
	FString l_SetNumer = SetNumber;
	l_SetNumer = l_SetNumer.Replace(TEXT(" "), TEXT(""));

	if (l_SetNumer.StartsWith(TEXT("+7")) && l_SetNumer.Len() == 12) { return true; }
	if (l_SetNumer.StartsWith(TEXT("8")) && l_SetNumer.Len() == 11) { return true; }

	GEngine->AddOnScreenDebugMessage(5, 3, FColor::Red, FString("NoValidNumber").Append(l_SetNumer));

	return false;


}

bool UAC_APIInformation::ConvertToBase64(const FString& inFilePath, FString& Base64String)
{
	TArray<uint8> FileData;
	if (FFileHelper::LoadFileToArray(FileData, *inFilePath))
	{
		Base64String = FBase64::Encode(FileData);

		if (Base64String.IsEmpty())
		{
			UE_LOG(LogTemp, Warning, TEXT("Empty FBase64::Encode: Base64String"));
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load file: %s"), *inFilePath);
		return false;
	}
}

FString UAC_APIInformation::Screenshot()
{
	TArray<FString> selectedFiles;
	//GScreenshotResolutionX = 400;
	//GScreenshotResolutionY = 400;

	FString fileName = FPaths::Combine(*FPaths::ProjectSavedDir(), *FString("Screenshots\\screenshot.png"));

	FScreenshotRequest::RequestScreenshot(fileName, false, false);

	return	fileName;

}


void UAC_APIInformation::URLSettings(const ETypeMessenger inChoiceTypeMessenger, const ETypeAction inTypeAction, bool inAsyncTypeURL,
	FString& URL)
{

	FString l_TypeMessengerURL;
	FString l_ID_TypeMessengerURL;

	//Set need config for url 
	switch (inChoiceTypeMessenger)
	{
	case ETypeMessenger::EM_Whatsapp:
		l_TypeMessengerURL = TEXT("api/");
		l_ID_TypeMessengerURL = FString(TOSTRING(ID_PROFILE_WAPPI_WHATSAPP));

		break;
	case ETypeMessenger::EM_Telegram:
		l_TypeMessengerURL = TEXT("tapi/");
		l_ID_TypeMessengerURL = FString(TOSTRING(ID_PROFILE_WAPPI_TELEGRAM));

		break;
	}

	FString l_AsyncTypeURL = inAsyncTypeURL ? "async/" : "sync/";


	FString lTypeAction;
	switch (inTypeAction)
	{
	case ETypeAction::EA_Image:
		lTypeAction = ("img/");
		break;
	}

	URL = FString("https://wappi.pro/")
		.Append(l_TypeMessengerURL)
		.Append(l_AsyncTypeURL)
		.Append("message/")
		.Append(lTypeAction)
		.Append("send?profile_id=")
		.Append(l_ID_TypeMessengerURL);
}

void UAC_APIInformation::SetConfigRequest(FString inJsonData, FString inURL)
{

	FHttpModule* Http = &FHttpModule::Get();
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();

	Request->SetURL(inURL);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Authorization"), TOSTRING(TOKEN_API_WAPPI));

	Request->SetContentAsString(inJsonData);
	Request->ProcessRequest();

}
