// Fill out your copyright notice in the Description page of Project Settings.


#include "BFL/BFL_Plugin_APIInformation.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#include "HttpModule.h"
#include "Wappi/WappiApi.h"
#include "Interfaces/IHttpResponse.h"
#include "Windows/WindowsPlatformMisc.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "Misc/FileHelper.h"
#include "Misc/Base64.h"
#include "Interfaces/IHttpRequest.h"

void UBFL_Plugin_APIInformation::SendWhatsAppMessage(const FString SetMessage, const FString SetNumber, bool UseAsync)
{


	if (!CheckPhoneToValid(SetNumber)) { return; }

	FHttpModule* Http = &FHttpModule::Get();
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();

	UseAsync == true ?
		Request->SetURL(TEXT("https://wappi.pro/api/async/message/send?profile_id=56afd854-76ae")) :
		Request->SetURL(TEXT("https://wappi.pro/api/sync/message/send?profile_id=56afd854-76ae"));

	Request->SetVerb("POST");
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Authorization"), TEXT("a40e852c145d73a7c30f821a2d824247713bbea6"));


	//FString Message = TEXT("Privet Konstantin pishu iz coda? testiruiu? vdrug nelsa otpravit ome kto ne v kontaktah =) !");
	//FString Number = TEXT("79616348787");

	FString JsonData = FString::Printf(TEXT("{ \"body\": \"%s\" , \"recipient\": \"%s\" }"), *SetMessage, *SetNumber);


	Request->SetContentAsString(JsonData);
	Request->ProcessRequest();


}

void UBFL_Plugin_APIInformation::SendFilePDF(const FString SetMessage, const FString SetNumber, const FString inFilePath)
{

	if (!CheckPhoneToValid(SetNumber)) { return; }

	FString Base64String;
	if (ConvertToBase64(inFilePath, Base64String)) { return; }


	FHttpModule* Http = &FHttpModule::Get();
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();

	Request->SetURL(TEXT("https://wappi.pro/api/sync/message/send?profile_id=56afd854-76ae"));

	Request->SetVerb("POST");
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Authorization"), TEXT("a40e852c145d73a7c30f821a2d824247713bbea6"));

	FString JsonData = FString::Printf(TEXT("{  \"recipient\": \"%s\",\"caption\": \"%s\",\"b64_file\": \"%s\" }"),
		*SetNumber, *SetMessage,*Base64String);

	Request->SetContentAsString(JsonData);
	Request->ProcessRequest();

	
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
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load file: %s"), *inFilePath);
		return false;
	}
}
