// Fill out your copyright notice in the Description page of Project Settings.


#include "Wappi/WappiApi.h"

#include "HttpModule.h"

AWappiApi::AWappiApi()
{
}
void AWappiApi::SendWhatsAppMessage()
{
	//FHttpModule& httpModule = FHttpModule::Get();
	//TSharedRef<IHttpRequest, ESPMode::ThreadSafe> pRequest = httpModule.CreateRequest();

	//var request = new RestRequest("https://wappi.pro" +
	//$"/api/sync/message/document/send?profile_id={_profileID}", Method.Post);
	//https://wappi.pro/api/sync/message/send?profile_id={{profile_id}}

	FHttpModule* Http = &FHttpModule::Get();
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->SetURL(TEXT("https://wappi.pro/api/sync/message/send?profile_id=56afd854-76ae"));
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Authorization"), TEXT("a40e852c145d73a7c30f821a2d824247713bbea6"));


	FString Message = TEXT("qwe !");
	FString Number = TEXT("+79616348787");
	GEngine->AddOnScreenDebugMessage(3,3, FColor::Red, FString::FromInt(Number.Len()));
	FString JsonData = FString::Printf(TEXT("{ \"body\": \"%s\" , \"recipient\": \"%s\" }"), *Message,*Number);

	
	Request->SetContentAsString(JsonData);
	Request->ProcessRequest();

}