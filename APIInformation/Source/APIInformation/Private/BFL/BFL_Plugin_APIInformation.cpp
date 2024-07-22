// Fill out your copyright notice in the Description page of Project Settings.


#include "BFL/BFL_Plugin_APIInformation.h"

#include "DesktopPlatformModule.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Windows/WindowsPlatformMisc.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "Misc/FileHelper.h"
#include "Misc/Base64.h"
#include "Interfaces/IHttpRequest.h"



void UBFL_Plugin_APIInformation::SendMessage(const ETypeMessenger ChoiceTypeMessenger, bool isAsyncLogic,
	const FString SetMessage, const FString SetNumber)
{
	if (ChoiceTypeMessenger == ETypeMessenger::EM_NONE) { return; }
	if (!CheckPhoneToValid(SetNumber)) { return; }

	FString l_URL;
	URLSettings(ChoiceTypeMessenger,EA_Message, isAsyncLogic, l_URL);

	FString JsonData = FString::Printf(TEXT("{ \"body\": \"%s\" , \"recipient\": \"%s\" }"), *SetMessage, *SetNumber);

	SetConfigRequest(JsonData, l_URL);
}

void UBFL_Plugin_APIInformation::SendFilePDF(const ETypeMessenger ChoiceTypeMessenger, 
                                             bool isAsyncLogic, const FString SetMessage, const FString SetNumber, FString inFilePath)
{
	if (ChoiceTypeMessenger == ETypeMessenger::EM_NONE) { return; }
	if (!CheckPhoneToValid(SetNumber)) { return; }

	if(inFilePath.IsEmpty())
	{
		inFilePath = Screenshot(); // We can change this to other logic;
	}

	FString l_URL;
	URLSettings(ChoiceTypeMessenger, EA_Image, isAsyncLogic, l_URL);

	FString l_Base64String;
	if (!ConvertToBase64(inFilePath, l_Base64String)) { return; }

	//Data with information to WappiApi
	FString JsonData = FString::Printf(
		TEXT("{  \"recipient\": \"%s\",	\"caption\": \"%s\",	\"b64_file\": \"%s\" }"),
		*SetNumber, *SetMessage, *l_Base64String);

	SetConfigRequest(JsonData, l_URL);
}
 

bool UBFL_Plugin_APIInformation::CheckPhoneToValid(const FString SetNumber)
{
	FString l_SetNumer = SetNumber;
	l_SetNumer = l_SetNumer.Replace(TEXT(" "), TEXT(""));

	if (l_SetNumer.StartsWith(TEXT("+7")) && l_SetNumer.Len() == 12) { return true; }
	if (l_SetNumer.StartsWith(TEXT("8")) && l_SetNumer.Len() == 11) { return true; }

	GEngine->AddOnScreenDebugMessage(5, 3, FColor::Red, FString("NoValidNumber").Append(l_SetNumer));

	return false;


}

bool UBFL_Plugin_APIInformation::ConvertToBase64(const FString& inFilePath, FString& Base64String)
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

FString UBFL_Plugin_APIInformation::Screenshot()
{
	// Change to other settings

	TArray<FString> selectedFiles;
	//GScreenshotResolutionX = 400;
	//GScreenshotResolutionY = 400;

	FString fileName = FPaths::Combine(*FPaths::ProjectSavedDir(), *FString("Screenshots\\screenshot.png"));

	FScreenshotRequest::RequestScreenshot(fileName, false, false);

	 return	fileName;

}


void UBFL_Plugin_APIInformation::URLSettings(const ETypeMessenger inChoiceTypeMessenger,const ETypeAction inTypeAction, bool inAsyncTypeURL,
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

void UBFL_Plugin_APIInformation::SetConfigRequest(FString inJsonData, FString inURL)
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
  