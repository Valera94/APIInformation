// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BFL_Plugin_APIInformation.generated.h"

//You Need Set Define variable from Wappi.com  
#define TOKEN_API_WAPPI a40e852c145d73a7c30f821a2d824247713bbea6
#define ID_PROFILE_WAPPI_TELEGRAM 6d13282a-dc14
#define ID_PROFILE_WAPPI_WHATSAPP 56afd854-76ae

// Don't touch
#define STRINGIFY(s) #s
#define TOSTRING(s) STRINGIFY(s)


UENUM(BlueprintType)
enum ETypeMessenger
{
	EM_NONE		UMETA(DisplayName = "NONE"),
	EM_Whatsapp UMETA(DisplayName = "Whatsapp"),
	EM_Telegram UMETA(DisplayName = "Telegram"),
};

UENUM(BlueprintType)
enum ETypeAction
{
	EA_NONE		UMETA(DisplayName = "NONE"),
	EA_Message	UMETA(DisplayName = "Message"),
	EA_Image	UMETA(DisplayName = "Image"),
};

UCLASS()
class APIINFORMATION_API UBFL_Plugin_APIInformation : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "Plugin_APIInformation")
	static void SendMessage(const ETypeMessenger ChoiceTypeMessenger, bool isAsyncLogic, const FString SetMessage, const FString SetNumber);

	UFUNCTION(BlueprintCallable, Category = "Plugin_APIInformation")
	static void SendFilePDF(const ETypeMessenger ChoiceTypeMessenger, bool isAsyncLogic, const FString SetMessage, const FString SetNumber, FString inFilePath);

	UFUNCTION(BlueprintCallable, Category = "Plugin_APIInformation")
	static bool CheckPhoneToValid(const FString SetNumber);


	//Configurate

	//bool true - success, false - fail load file
	static bool ConvertToBase64(const FString& inFilePath, FString& Base64String);
	static FString Screenshot();
	static void URLSettings(const ETypeMessenger inChoiceTypeMessenger, const ETypeAction inTypeAction, bool inAsyncTypeURL, FString& URL);
	static void SetConfigRequest(FString inJsonData, FString inURL);

};


