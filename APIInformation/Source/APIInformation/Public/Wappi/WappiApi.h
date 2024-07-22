// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/Actor.h"
#include "WappiApi.generated.h"



UCLASS()
class APIINFORMATION_API AWappiApi : public AActor
{
	GENERATED_BODY()

public:
	AWappiApi();
private:
	UFUNCTION(BlueprintCallable)
	static void  SendWhatsAppMessage();
	//const FString _profileID = "56afd854-76ae";
	//const FString _instanceDocumentUrl = "https://wappi.pro/api/sync/message/document/send?profile_id=fdf30c5f-ea3f";
	//const FString _instanceImageUrl = "https://wappi.pro/api/sync/message/img/send?profile_id=";

};
