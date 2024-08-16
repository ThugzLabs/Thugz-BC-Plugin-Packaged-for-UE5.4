// Created in 2024. Copyright Thugz Labs SAS, all rights reserved.

#include "ThugzBCfor53BPLibrary.h"
#include "Engine/Texture2D.h"
#include "Engine/Texture.h"
#include "ThugzBCfor53.h"




FString UThugzBCBPLibrary::LastJsonResponse = FString("");
FString UThugzBCBPLibrary::LastTokenBalance = FString("");


/////////////////////////////////////////////////////////////HELLOMOON//////////////////////////////////////////////////////////////////////////////

// requête pour HelloMoon API
void UThugzBCBPLibrary::MakeHelloMoonAPIRequest(const FString& Account, const FString& Barear)
{
    FString HelloMoonURL = TEXT("https://rest-api.hellomoon.io/v0/nft/mints-by-owner"); // Remplacez par l'URL de votre endpoint HelloMoon

    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();

    HttpRequest->SetVerb(TEXT("POST"));
    HttpRequest->SetURL(HelloMoonURL);
    HttpRequest->SetHeader(TEXT("Accept"), TEXT("application/json"));
    HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    HttpRequest->SetHeader(TEXT("Authorization"), FString::Printf(TEXT("Bearer %s"), *Barear));
    //HttpRequest->SetHeader(TEXT("Authorization"), TEXT("Bearer 84fc727a-66fd-4f4c-a723-17adb50ce7ca"));

    FString JsonPayload = FString::Printf(TEXT("{\"ownerAccount\":\"%s\"}"), *Account);
    HttpRequest->SetContentAsString(JsonPayload);

    HttpRequest->OnProcessRequestComplete().BindStatic(&UThugzBCBPLibrary::HandleHelloMoonAPIResponse);

    HttpRequest->ProcessRequest();
}

//Requete pour récupérer l'URI de hellomoon
void UThugzBCBPLibrary::MakeURIRequest(const FString& URL)
{
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
    Request->OnProcessRequestComplete().BindStatic(&UThugzBCBPLibrary::HandleHelloMoonAPIResponse);
    Request->SetURL(URL);
    Request->SetVerb("GET");
    Request->SetHeader("Content-Type", "application/json");
    Request->ProcessRequest();
}
bool UThugzBCBPLibrary::ParseImageURL(const FString& JsonString, FString& OutImageURL)
{
    LastJsonResponse = JsonString; // Store the raw JSON response

    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

    if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
    {
        if (JsonObject->HasField("image"))
        {
            OutImageURL = JsonObject->GetStringField("image");
            return true;
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("JSON does not contain 'image' field"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON"));
    }

    return false;
}
//Récuperation de l'image et creation de la texture depuis l'URL obtenu par ParseImageURL
void UThugzBCBPLibrary::DownloadImageAndCreateTexture(const FString& URL, UTexture2D*& OutTexture)
{
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
    Request->OnProcessRequestComplete().BindStatic(&UThugzBCBPLibrary::OnImageDownloaded, &OutTexture);
    Request->SetURL(URL);
    Request->SetVerb("GET");
    Request->SetHeader("Content-Type", "application/json");
    Request->ProcessRequest();
}

void UThugzBCBPLibrary::OnImageDownloaded(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful, UTexture2D** OutTexture)
{
    if (bWasSuccessful && Response.IsValid())
    {
        const TArray<uint8>& ImageData = Response->GetContent();
        *OutTexture = CreateTextureFromImageData(ImageData);
        if (*OutTexture)
        {
            UE_LOG(LogTemp, Log, TEXT("Image downloaded and texture created successfully."));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to create texture from downloaded image."));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to download image."));
    }
}

UTexture2D* UThugzBCBPLibrary::CreateTextureFromImageData(const TArray<uint8>& ImageData)
{
    IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
    EImageFormat ImageFormat = ImageWrapperModule.DetectImageFormat(ImageData.GetData(), ImageData.Num());

    if (ImageFormat == EImageFormat::Invalid)
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid image format."));
        return nullptr;
    }

    TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(ImageFormat);

    if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(ImageData.GetData(), ImageData.Num()))
    {
        TArray<uint8> UncompressedBGRA;
        if (ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, UncompressedBGRA))
        {
            UTexture2D* Texture = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_B8G8R8A8);

            if (!Texture)
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to create transient texture."));
                return nullptr;
            }

            void* TextureData = Texture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
            FMemory::Memcpy(TextureData, UncompressedBGRA.GetData(), UncompressedBGRA.Num());
            Texture->GetPlatformData()->Mips[0].BulkData.Unlock();

            Texture->UpdateResource();
            return Texture;
        }
    }

    UE_LOG(LogTemp, Error, TEXT("Failed to uncompress image."));
    return nullptr;
}


// Traitement de la requête pour les API NFT (HelloMoon ou Moralis)
void UThugzBCBPLibrary::HandleHelloMoonAPIResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    if (!bWasSuccessful)
    {
        // Gérer l'erreur de la requête ici
        LastJsonResponse = FString(""); // Réinitialisez la réponse JSON en cas d'erreur
        return;
    }

    if (Response.IsValid() && Response->GetResponseCode() == 200)
    {
        FString JsonResponse = Response->GetContentAsString();
        LastJsonResponse = JsonResponse; // Stockez la réponse JSON

        // Vous pouvez maintenant utiliser JsonResponse qui contient la réponse JSON

        // Par exemple, vous pouvez désérialiser la chaîne JSON en une structure Unreal Engine
    }
    else
    {
        // Gérer l'erreur de réponse HTTP ici
        LastJsonResponse = FString("ERREUR"); // Réinitialisez la réponse JSON en cas d'erreur
    }

}

//Parsing en structure UE du JSON de HelloMoon API
FRootJson UThugzBCBPLibrary::ConvertSOLJSONtoStruct(FString JsonString)
{

    FString YourJsonFString; // Votre JSON ici
    TSharedPtr<FJsonObject> JsonObject;
    FRootJson RootStruct;

    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
    if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
    {

        TArray<TSharedPtr<FJsonValue>> DataArray = JsonObject->GetArrayField("data");

        for (int32 i = 0; i < DataArray.Num(); i++)
        {
            TSharedPtr<FJsonObject> DataObject = DataArray[i]->AsObject();

            FNFTData NFTData;
            NFTData.NftMint = DataObject->GetStringField("nftMint");
            NFTData.OwnerAccount = DataObject->GetStringField("ownerAccount");
            NFTData.MetadataAddress = DataObject->GetStringField("metadataAddress");

            // Désérialiser MetadataJson
            TSharedPtr<FJsonObject> MetadataJsonObject = DataObject->GetObjectField("metadataJson");
            NFTData.MetadataJson.Name = MetadataJsonObject->GetStringField("name");
            NFTData.MetadataJson.Symbol = MetadataJsonObject->GetStringField("symbol");
            NFTData.MetadataJson.Uri = MetadataJsonObject->GetStringField("uri");
            NFTData.MetadataJson.SellerFeeBasisPoints = MetadataJsonObject->GetNumberField("sellerFeeBasisPoints");
            // Répéter pour d'autres champs
            // Désérialiser le tableau des créateurs
            TArray<TSharedPtr<FJsonValue>> CreatorsArray = MetadataJsonObject->GetArrayField("creators");
            for (int32 j = 0; j < CreatorsArray.Num(); j++)
            {
                TSharedPtr<FJsonObject> CreatorObject = CreatorsArray[j]->AsObject();

                FCreator Creator;
                Creator.Address = CreatorObject->GetStringField("address");
                Creator.Verified = CreatorObject->GetBoolField("verified");
                Creator.Share = CreatorObject->GetIntegerField("share");

                NFTData.MetadataJson.Creators.Add(Creator);
            }

            // Ajouter l'objet NFTData à la structure racine
            RootStruct.Data.Add(NFTData);
        }
    }
    return RootStruct;
}


//Balance Solana avec Hellomoon

void UThugzBCBPLibrary::HelloMoonRequestForTokenBalance(const FString& Param, const FString& ApiKey, FString& OutResponse)
{
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
    JsonObject->SetStringField(TEXT("jsonrpc"), TEXT("2.0"));
    JsonObject->SetNumberField(TEXT("id"), 1);
    JsonObject->SetStringField(TEXT("method"), TEXT("getBalance"));
    TArray<TSharedPtr<FJsonValue>> Params;
    Params.Add(MakeShareable(new FJsonValueString(Param))); // Use the parameter passed to the function
    JsonObject->SetArrayField(TEXT("params"), Params);

    FString RequestContent;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestContent);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

    FHttpModule* Http = &FHttpModule::Get();
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = Http->CreateRequest();
    HttpRequest->SetURL(FString::Printf(TEXT("https://rpc.hellomoon.io/%s"), *ApiKey)); // Use the API Key in the URL
    HttpRequest->SetVerb(TEXT("POST"));
    HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    HttpRequest->SetHeader(TEXT("Accept"), TEXT("application/json"));

    HttpRequest->SetContentAsString(RequestContent);

    // Bind a lambda to the completion delegate
    HttpRequest->OnProcessRequestComplete().BindLambda([&OutResponse](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
        {
            if (bWasSuccessful && Response.IsValid())
            {
                // Assign the response content to the output parameter
                OutResponse = Response->GetContentAsString();
                LastTokenBalance = OutResponse;
            }
            else
            {
                OutResponse = TEXT("Failed to get a response");
            }
        });

    HttpRequest->ProcessRequest();
}

//Requête récuperant la repons eJSON de getbalance d'HelloMoon pour la mettre dans un double en sortie
void UThugzBCBPLibrary::GetTokenBamanceFromJsonHelloMoon(const FString& JsonString, double& OutValue)
{
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

    if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
    {
        // Obtention de l'objet "result"
        const TSharedPtr<FJsonObject>* ResultObject;
        if (JsonObject->TryGetObjectField(TEXT("result"), ResultObject))
        {
            // Extraction et affectation de la valeur à OutValue
            OutValue = (*ResultObject)->GetNumberField(TEXT("value")) / 1000000000.0;
        }
    }
}



////////////////////////////////////////////////////////MORALIS//////////////////////////////////////////////////////////////////////////////////////
// 
// requête pour Moralis API
void UThugzBCBPLibrary::MoralisAPIRequest(const FString& AccountAddress, const FString& ApiKey, const FString& Blockchain) {

    FString Url = FString::Printf(TEXT("https://deep-index.moralis.io/api/v2/%s/nft?chain=%s&format=decimal"), *AccountAddress, *Blockchain);

    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
    Request->SetURL(Url);
    Request->SetVerb(TEXT("GET"));
    Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
    Request->SetHeader(TEXT("X-API-Key"), *ApiKey);
    Request->ProcessRequest();

    Request->OnProcessRequestComplete().BindStatic(&UThugzBCBPLibrary::HandleHelloMoonAPIResponse);

    Request->ProcessRequest();

}

//Parsing du JSON de l'API MORALIS
FEVMFNFTResponse UThugzBCBPLibrary::ConvertEVMJSONtoStruct(FString JsonString)
{

    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
    FEVMFNFTResponse NFTResponse;

    if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
    {


        NFTResponse.Status = JsonObject->GetStringField("status");
        NFTResponse.Page = JsonObject->GetIntegerField("page");
        NFTResponse.PageSize = JsonObject->GetIntegerField("page_size");

        TArray<TSharedPtr<FJsonValue>> ResultsArray = JsonObject->GetArrayField("result");
        for (TSharedPtr<FJsonValue> Value : ResultsArray)
        {
            TSharedPtr<FJsonObject> ResultObject = Value->AsObject();
            FEVMFNFTData NFTData;

            NFTData.Amount = ResultObject->GetStringField("amount");
            NFTData.TokenId = ResultObject->GetStringField("token_id");
            NFTData.TokenAddress = ResultObject->GetStringField("token_address");
            NFTData.ContractType = ResultObject->GetStringField("contract_type");
            NFTData.OwnerOf = ResultObject->GetStringField("owner_of");
            NFTData.last_metadata_sync = ResultObject->GetStringField("last_metadata_sync");
            NFTData.last_token_uri_sync = ResultObject->GetStringField("last_token_uri_sync");
            NFTData.block_number = ResultObject->GetStringField("block_number");
            NFTData.name = ResultObject->GetStringField("name");
            NFTData.symbol = ResultObject->GetStringField("symbol");
            NFTData.token_hash = ResultObject->GetStringField("token_hash");
            NFTData.token_uri = ResultObject->GetStringField("token_uri");
            NFTData.minter_address = ResultObject->GetStringField("minter_address");
            NFTData.verified_collection = ResultObject->GetStringField("verified_collection");


            FString MetadataString = ResultObject->GetStringField("metadata");
            TSharedPtr<FJsonObject> MetadataObject;
            TSharedRef<TJsonReader<>> MetadataReader = TJsonReaderFactory<>::Create(MetadataString);
            if (FJsonSerializer::Deserialize(MetadataReader, MetadataObject) && MetadataObject.IsValid())
            {
                FEVMFNFTMetadata Metadata;
                Metadata.Image = MetadataObject->GetStringField("image");
                Metadata.Name = MetadataObject->GetStringField("name");
                Metadata.description = MetadataObject->GetStringField("description");
                Metadata.external_url = MetadataObject->GetStringField("external_url");

                NFTData.Metadata = Metadata;
            }

            NFTResponse.Result.Add(NFTData);
        }


    }
    return NFTResponse;
}
// Récupération du JSON de la balance Solana avec MORALIS
void UThugzBCBPLibrary::MakeMoraliseRequestForSOLBalance(const FString& Pkey, const FString& ApiKey, FString& OutResponse)
{
    FString Url = FString::Printf(TEXT("https://solana-gateway.moralis.io/account/mainnet/%s/balance"), *Pkey);
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
    Request->SetURL(Url);
    Request->SetVerb(TEXT("GET"));
    Request->SetHeader(TEXT("accept"), TEXT("application/json"));
    Request->SetHeader(TEXT("X-API-Key"), ApiKey);

    Request->OnProcessRequestComplete().BindLambda([&OutResponse](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
        {
            if (bWasSuccessful && Response.IsValid())
            {
                OutResponse = Response->GetContentAsString();
            }
            else
            {
                OutResponse = TEXT("Failed to fetch balance");
            }
        });

    Request->ProcessRequest();
}

//Requête récuperant la reponse JSON de la requête MORALIS pour la mettre dans un double en sortie
void UThugzBCBPLibrary::GetTokenBamanceFromJsonMoralis(const FString& JsonString, double& OutSolanaValue)
{
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

    if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
    {
        if (JsonObject->HasField(TEXT("solana")))
        {
            FString SolanaString = JsonObject->GetStringField(TEXT("solana"));
            OutSolanaValue = FCString::Atod(*SolanaString);
        }
    }
}

////////////////////////////////////////////////////////////////TRANSVERSE/////////////////////////////////////////////////////////////////////////////////

//requête récupéant la réponse de n'importe quel API pour la mettre dans un FSTRING à parser
FString UThugzBCBPLibrary::GetLastJsonResponse()
{
    return LastJsonResponse;
}

FString UThugzBCBPLibrary::GetLastTokenJsonResponse()
{
    return LastTokenBalance;
}


//////////////////////////////////CREATION WALLET SOLANA avec SODIUM////////////////////////////////////////////////////////////////////////////////////////////////
void UThugzBCBPLibrary::GenerateSolanaKeyPair(FString& OutPublicKey, FString& OutPrivateKey)
{
    if (sodium_init() < 0)
    {
        // L'initialisation a échoué
        UE_LOG(LogTemp, Error, TEXT("Libsodium initialization failed!"));
        return;
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Libsodium initialization succeeded."));
    }

    unsigned char publicKey[crypto_sign_PUBLICKEYBYTES];
    unsigned char privateKey[crypto_sign_SECRETKEYBYTES];

    // Générer une paire de clés
    crypto_sign_keypair(publicKey, privateKey);

    // Convertir les clés en chaînes hexadécimales
    OutPublicKey = BytesToHex(publicKey, crypto_sign_PUBLICKEYBYTES);
    OutPrivateKey = BytesToHex(privateKey, crypto_sign_SECRETKEYBYTES);
}

FString UThugzBCBPLibrary::BytesToHex(const unsigned char* Bytes, int32 Length)
{
    FString HexString;
    for (int32 i = 0; i < Length; i++)
    {
        HexString += FString::Printf(TEXT("%02x"), Bytes[i]);
    }
    return HexString;
}
const FString Base58Alphabet = TEXT("123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz");

FString UThugzBCBPLibrary::EncodeBase58(const TArray<uint8>& Data)
{
    int32 length = Data.Num();
    TArray<int32> digits;
    digits.Add(0);

    for (int32 i = 0; i < length; ++i)
    {
        int carry = Data[i];
        for (int32 j = 0; j < digits.Num(); ++j)
        {
            carry += digits[j] << 8;
            digits[j] = carry % 58;
            carry /= 58;
        }
        while (carry)
        {
            digits.Add(carry % 58);
            carry /= 58;
        }
    }

    FString encoded;
    for (int32 k = digits.Num() - 1; k >= 0; --k)
    {
        encoded += Base58Alphabet[digits[k]];
    }

    for (int32 i = 0; i < length && Data[i] == 0; ++i)
    {
        encoded = TEXT("1") + encoded;
    }

    return encoded;
}

TArray<uint8> UThugzBCBPLibrary::DecodeBase58(const FString& Base58String)
{
    int32 length = Base58String.Len();
    TArray<int32> digits;
    digits.Add(0);

    for (int32 i = 0; i < length; ++i)
    {
        TCHAR c = Base58String[i];
        int32 carry;
        if (!Base58Alphabet.FindChar(c, carry))
        {
            // Handle invalid character in Base58 string
            return TArray<uint8>();
        }

        for (int32 j = 0; j < digits.Num(); ++j)
        {
            carry += digits[j] * 58;
            digits[j] = carry & 0xFF;
            carry >>= 8;
        }
        while (carry)
        {
            digits.Add(carry & 0xFF);
            carry >>= 8;
        }
    }

    TArray<uint8> decoded;
    for (int32 k = digits.Num() - 1; k >= 0; --k)
    {
        decoded.Add(static_cast<uint8>(digits[k]));
    }

    for (int32 i = 0; i < length && Base58String[i] == TEXT('1'); ++i)
    {
        decoded.Insert(0, 0);
    }

    return decoded;
}
TArray<uint8> UThugzBCBPLibrary::HexToBytes(const FString& HexString)
{
    TArray<uint8> Bytes;
    int32 Length = HexString.Len();

    for (int32 i = 0; i < Length; i += 2)
    {
        FString ByteString = HexString.Mid(i, 2);
        uint8 Byte = FParse::HexDigit(ByteString[0]) * 16 + FParse::HexDigit(ByteString[1]);
        Bytes.Add(Byte);
    }

    return Bytes;
}


//////////////////////////////////IMPORT WALLET SOLANA avec SODIUM////////////////////////////////////////////////////////////////////////////////////////////////
FString UThugzBCBPLibrary::GetSolanaAddressFromPrivateKey(const FString& PrivateKey, FString& PublicKeyHex)
{
    // Assurez-vous que libsodium est initialisé
    static bool bSodiumInitialized = false;
    if (!bSodiumInitialized) {
        if (sodium_init() == -1) {
            UE_LOG(LogTemp, Error, TEXT("Failed to initialize libsodium"));
            return "";
        }
        bSodiumInitialized = true;
    }

    // Convertir la clé privée de Base58 à TArray<uint8>
    TArray<uint8> PrivateKeyArray = DecodeBase58(PrivateKey);

    // Vérifier la longueur de la clé privée
    if (PrivateKeyArray.Num() != crypto_sign_SECRETKEYBYTES) {
        UE_LOG(LogTemp, Error, TEXT("Invalid private key length: %d"), PrivateKeyArray.Num());
        return "";
    }

    // Générer la clé publique à partir de la clé privée
    uint8 PublicKey[crypto_sign_PUBLICKEYBYTES];
    crypto_sign_ed25519_sk_to_pk(PublicKey, PrivateKeyArray.GetData());

    // Convertir la clé publique en chaîne hexadécimale pour vérification
    PublicKeyHex = BytesToHex(PublicKey, crypto_sign_PUBLICKEYBYTES);

    // Convertir la clé publique en chaîne de caractères Base58 pour obtenir l'adresse Solana
    TArray<uint8> PublicKeyArray;
    PublicKeyArray.Append(PublicKey, crypto_sign_PUBLICKEYBYTES);
    FString PublicKeyBase58 = EncodeBase58(PublicKeyArray);

    UE_LOG(LogTemp, Log, TEXT("Public Key (Hex): %s"), *PublicKeyHex);
    UE_LOG(LogTemp, Log, TEXT("Public Key (Base58): %s"), *PublicKeyBase58);

    return PublicKeyBase58;
}