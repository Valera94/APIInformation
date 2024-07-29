// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AC_APIInformation.generated.h"

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



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class APIINFORMATION_API UAC_APIInformation : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAC_APIInformation();


	UFUNCTION(BlueprintCallable, Category = "Plugin_APIInformation")
	void SendMessage(const ETypeMessenger setChoiceTypeMessenger, bool isAsyncLogic, const FString SetMessage, const FString SetNumber);

	UFUNCTION(BlueprintCallable, Category = "Plugin_APIInformation")
	void SendFilePDF(const ETypeMessenger setChoiceTypeMessenger, bool isAsyncLogic, const FString SetMessage, const FString SetNumber, FString inFilePath);

	UFUNCTION(BlueprintCallable, Category = "Plugin_APIInformation")
	bool CheckPhoneToValid(const FString SetNumber);


protected:

	//Timer for check ready photo
	UFUNCTION() void StartTimerForCheckPhoto();
	UFUNCTION() void CheckIsReadyScreenshot();
	UPROPERTY()FTimerHandle Timer_ScreenshotCheckTimer;

	UPROPERTY()int Timer;

	UPROPERTY()FString FilePath;
	UPROPERTY()TEnumAsByte<ETypeMessenger> ChoiceTypeMessenger;
	UPROPERTY()bool AsyncLogic;
	UPROPERTY()FString Number;
	UPROPERTY()FString Message;



	void SendingPDF_File();


	//Configurate
	ETypeAction TypeAction;

	//bool true - success, false - fail load file
	bool ConvertToBase64(const FString& inFilePath, FString& Base64String);
	FString Screenshot();
	void URLSettings(const ETypeMessenger inChoiceTypeMessenger, const ETypeAction inTypeAction, bool inAsyncTypeURL, FString& URL);
	void SetConfigRequest(FString inJsonData, FString inURL);

};
