# APIInformation

Доброго времени суток.

Данный Plugin используется в качестве связи WhatsApp и Telegram через стороннего поставщика [Wappi](https://wappi.pro/)

Для каждого заказчика(клиента) важно установить их токены и настройки, все это делается в __"UBFL_Plugin_APIInformation:UBlueprintFunctionLibrary"__


Что бы изменить Токен и уникальный идентификатор для работы в файле __UBFL_Plugin_APIInformation.h__ 
есть константные предопределенные константы __#define__
Поставьте нужные токены и ID для каждого из данных Messenger 
получить их можно в личном кабинете Wappi:

  >//You Need Set Define variable from Wappi.com 
>
  >#define TOKEN_API_WAPPI a40e852c145d73a7c30f821a2d824247713bbea6
>
  >#define ID_PROFILE_WAPPI_TELEGRAM 6d13282a-dc14
>
  >#define ID_PROFILE_WAPPI_WHATSAPP 56afd854-76ae

Если Wappi расширит мессенджеры вы можете расширить функционал, просто добавив новое значение в UENUM

>UENUM(BlueprintType)
>
>enum ETypeMessenger
>
>{
>
>	EM_NONE		UMETA(DisplayName = "NONE"),
>
>	EM_Whatsapp UMETA(DisplayName = "Whatsapp"),
>
>	EM_Telegram UMETA(DisplayName = "Telegram"),
>
>};

Для увеличения функционала и заготовления Pipeline конструктора для URL вы должны дополнить другой UENUM
и вследствии перейти в *"static void URLSettings()"*  для настройки.

>UENUM(BlueprintType)
>
>enum ETypeAction
>
>{
>
>	EA_NONE		UMETA(DisplayName = "NONE"),
>
>	EA_Message	UMETA(DisplayName = "Message"),
>
>	EA_Image	UMETA(DisplayName = "Image"),
>
>};

На текущий момент мы имеем функции для Blueprint
> __static void SendMessage()__ - Внутри тела проходит логика для реализации отправки сообщения.
>
> __static void SendFilePDF()__ - Внутри тела проходит логика для реализации отправки сообщения и картинки.
>
> __static bool CheckPhoneToValid()__ - Проверка на актуальность номера.


На текущий момент мы имеем функции для С++
>__static bool ConvertToBase64()__ - Простая конвертация.
>
>__static FString Screenshot()__ - Будет изменен, в данный момент реализован как заглушка.
>
>__static void URLSettings()__ - Все для настройки URL, если вы хотите видоизменить или починить URL вам нужно ковырять тут.
>
>__static void SetConfigRequest()__ - Заготовленный конфиг для отправки сообщения в Wappi.




