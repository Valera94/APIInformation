// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BFL_Plugin_APIInformation.generated.h"

UCLASS()
class APIINFORMATION_API UBFL_Plugin_APIInformation : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	//	Todo NeedSetThisVariableToOtherPlace mb ProjectSettings
	//	Todo Id and Token.
	//	У каждого заказчика будет свой токен и id поэтому, он должен устанавливаться в отдельном файле,
	//	где есть вся атрибутика определенного заказчика.

	UFUNCTION(BlueprintCallable, Category = "Plugin_APIInformation|WhatsApp")
	static void  SendWhatsAppMessage(const FString SetMessage, const FString SetNumber, bool UseAsync = false);

	UFUNCTION(BlueprintCallable, Category = "Plugin_APIInformation|WhatsApp")
	static void SendFilePDF(const FString SetMessage, const FString SetNumber, const FString inFilePath);


	UFUNCTION(BlueprintCallable, Category = "Plugin_APIInformation")
	static bool CheckPhoneToValid(const FString SetNumber);


	//bool true - success, false - fail load file
	static bool ConvertToBase64(const FString& inFilePath, FString& Base64String);


};
