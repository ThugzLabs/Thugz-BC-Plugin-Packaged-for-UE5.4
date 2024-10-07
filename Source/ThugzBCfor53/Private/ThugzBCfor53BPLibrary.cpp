// Created in 2024. Copyright Thugz Labs SAS, all rights reserved.

#include "ThugzBCfor53BPLibrary.h"
#include "Engine/Texture2D.h"
#include "Engine/Texture.h"
#include "ThugzBCfor53.h"




FString UThugzBCBPLibrary::LastJsonResponse = FString("");
FString UThugzBCBPLibrary::LastTokenBalance = FString("");


/////////////////////////////////////////////////////////////HELLOMOON//////////////////////////////////////////////////////////////////////////////

// This request use the HelloMoon API to retrieve the NFTs owned by the Account. Barear is the hellomoon barear / Cette requête utilise l'API de HelloMoon pour récupérer les NFT appartenant au compte. Barear est le barear de hellomoon
void UThugzBCBPLibrary::MakeHelloMoonAPIRequest(const FString& Account, const FString& Barear)
{
    FString HelloMoonURL = TEXT("https://rest-api.hellomoon.io/v0/nft/mints-by-owner"); // It is the mint-by-owner method from the HelloMoon API

    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();

    HttpRequest->SetVerb(TEXT("POST"));
    HttpRequest->SetURL(HelloMoonURL);
    HttpRequest->SetHeader(TEXT("Accept"), TEXT("application/json"));
    HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    HttpRequest->SetHeader(TEXT("Authorization"), FString::Printf(TEXT("Bearer %s"), *Barear));

    FString JsonPayload = FString::Printf(TEXT("{\"ownerAccount\":\"%s\"}"), *Account);
    HttpRequest->SetContentAsString(JsonPayload);

    HttpRequest->OnProcessRequestComplete().BindStatic(&UThugzBCBPLibrary::HandleHelloMoonAPIResponse);

    HttpRequest->ProcessRequest();
}

//Requete pour récupérer l'URI de hellomoon / This request retrieve the URI from the NFT metadata
void UThugzBCBPLibrary::MakeURIRequest(const FString& URL)
{
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
    Request->OnProcessRequestComplete().BindStatic(&UThugzBCBPLibrary::HandleHelloMoonAPIResponse);
    Request->SetURL(URL);
    Request->SetVerb("GET");
    Request->SetHeader("Content-Type", "application/json");
    Request->ProcessRequest();
}
//This request retrieve the URL of the NFT image / Cette requête permet de récupérer l'URL de l'image NFT
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
//Récuperation de l'image et creation de la texture depuis l'URL obtenu par ParseImageURL / Retrieve the image and create the texture from the URL obtained by ParseImageURL
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

//Parsing en structure UE du JSON de HelloMoon API / Parsing en structure UE du JSON de HelloMoon API
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


//récupération de la Balance Solana avec Hellomoon / recovery of the Solana Scale with Hellomoon

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

//Requête récuperant la repons eJSON de getbalance d'HelloMoon pour la mettre dans un double en sortie / Request to retrieve HelloMoon's getbalance eJSON response and put it in an output double
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
                                                // EVM BLOCKCHAIN//
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
// SOLANA BLOCKCHAIN//
// Récupération du JSON de la balance Solana avec MORALIS /  Retrieving JSON from Solana scales with MORALIS
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

//Requête récuperant la reponse JSON de la requête balance solana MORALIS pour la mettre dans un double en sortie / Query that retrieves the JSON response from the MORALIS  Solana balance query and puts it in a duplicate output file
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

//Requête récupérant les NFT possédés d'un Wallet SOLANA dans un JSON / Request to retrieve the NFTs owned by a SOLANA Wallet in a JSON
void UThugzBCBPLibrary::MakeMoralisAPIRequest(const FString& OwnerAccount, const FString& APIKey)
{
    FString MoralisURL = FString::Printf(TEXT("https://solana-gateway.moralis.io/account/mainnet/%s/nft"), *OwnerAccount);

    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();

    HttpRequest->SetVerb(TEXT("GET"));
    HttpRequest->SetURL(MoralisURL);
    HttpRequest->SetHeader(TEXT("Accept"), TEXT("application/json"));
    HttpRequest->SetHeader(TEXT("X-API-Key"), APIKey);

    HttpRequest->OnProcessRequestComplete().BindStatic(&UThugzBCBPLibrary::HandleHelloMoonAPIResponse);

    HttpRequest->ProcessRequest();
}
//Parsing de la réponse JSON de la requête MakeMoralisAPIRequest dans une strcture adaptée / Parsing the JSON response to the MakeMoralisAPIRequest request into an adapted structure
TArray<FThugzNFTData> UThugzBCBPLibrary::ParseNFTDataFromMoralisJSON(const FString& JsonString)
{
    TArray<FThugzNFTData> NFTDataArray;

    // Create a JSON Reader
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

    // Declare a JSON object to hold the parsed data
    TArray<TSharedPtr<FJsonValue>> ParsedJson;
    if (FJsonSerializer::Deserialize(Reader, ParsedJson))
    {
        for (auto& Item : ParsedJson)
        {
            TSharedPtr<FJsonObject> JsonObject = Item->AsObject();

            FThugzNFTData NFTData;
            NFTData.AssociatedTokenAddress = JsonObject->GetStringField(TEXT("associatedTokenAddress"));
            NFTData.Mint = JsonObject->GetStringField(TEXT("mint"));
            NFTData.Name = JsonObject->GetStringField(TEXT("name"));
            NFTData.Symbol = JsonObject->GetStringField(TEXT("symbol"));

            NFTDataArray.Add(NFTData);
        }
    }

    return NFTDataArray;
}
//Requête récuperant les Metaddata d'un NFT donné / Request to retrieve Metaddata for a given NFT
void UThugzBCBPLibrary::MakeMoralisNFTMetadataRequest(const FString& NFTMintAddress, const FString& APIKey)
{
    FString MoralisURL = FString::Printf(TEXT("https://solana-gateway.moralis.io/nft/mainnet/%s/metadata"), *NFTMintAddress);

    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();

    HttpRequest->SetVerb(TEXT("GET"));
    HttpRequest->SetURL(MoralisURL);
    HttpRequest->SetHeader(TEXT("Accept"), TEXT("application/json"));
    HttpRequest->SetHeader(TEXT("X-API-Key"), APIKey);

    // Bind the request to handle the response
    HttpRequest->OnProcessRequestComplete().BindStatic(&UThugzBCBPLibrary::HandleHelloMoonAPIResponse);

    // Send the request
    HttpRequest->ProcessRequest();
}
//Parsing de la réponse JSON de la requête MakeMoralisNFTMetadataRequest dans une strcture adaptée / Parsing the JSON response to the MakeMoralisNFTMetadataRequest into a suitable structure
FSolMoralisNFTMetadata UThugzBCBPLibrary::ParseNFTMetadataFromJSON(const FString& JsonString)
{
    FSolMoralisNFTMetadata NFTMetadata;

    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

    TSharedPtr<FJsonObject> JsonObject;
    if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
    {
        NFTMetadata.Mint = JsonObject->GetStringField(TEXT("mint"));
        NFTMetadata.Standard = JsonObject->GetStringField(TEXT("standard"));
        NFTMetadata.Name = JsonObject->GetStringField(TEXT("name"));
        NFTMetadata.Symbol = JsonObject->GetStringField(TEXT("symbol"));

        TSharedPtr<FJsonObject> MetaplexObject = JsonObject->GetObjectField(TEXT("metaplex"));
        NFTMetadata.Metaplex.MetadataUri = MetaplexObject->GetStringField(TEXT("metadataUri"));
        NFTMetadata.Metaplex.UpdateAuthority = MetaplexObject->GetStringField(TEXT("updateAuthority"));
        NFTMetadata.Metaplex.SellerFeeBasisPoints = MetaplexObject->GetIntegerField(TEXT("sellerFeeBasisPoints"));
        NFTMetadata.Metaplex.PrimarySaleHappened = MetaplexObject->GetIntegerField(TEXT("primarySaleHappened"));
        NFTMetadata.Metaplex.IsMutable = MetaplexObject->GetBoolField(TEXT("isMutable"));
        NFTMetadata.Metaplex.MasterEdition = MetaplexObject->GetBoolField(TEXT("masterEdition"));
    }

    return NFTMetadata;
}

////////////////////////////////////////////////////////////////TRANSVERSE/////////////////////////////////////////////////////////////////////////////////

//requête récupéant la réponse de n'importe quel API pour la mettre dans un FSTRING à parser / request to retrieve the response from any API and put it in a FSTRING to be parsed
FString UThugzBCBPLibrary::GetLastJsonResponse()
{
    return LastJsonResponse;
}

FString UThugzBCBPLibrary::GetLastTokenJsonResponse()
{
    return LastTokenBalance;
}
// Traitement de la requête pour les API NFT (HelloMoon ou Moralis) / Request processing for NFT APIs (HelloMoon or Moralis)
void UThugzBCBPLibrary::HandleHelloMoonAPIResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    //LastJsonResponse = FString(""); //On commence par vider la dernière réponse eventuelle / Start by clearing the last possible answer
    if (!bWasSuccessful)
    {
        LastJsonResponse = FString(""); // Réinitialisez la réponse JSON en cas d'erreur / Reset the JSON response in the event of an error
        return;
    }

    if (Response.IsValid() && Response->GetResponseCode() == 200)
    {
        FString JsonResponse = Response->GetContentAsString();
        LastJsonResponse = JsonResponse; // Stockez la réponse JSON / Store the JSON response

    }
    else
    {
        // Gestion de l'erreur de réponse HTTP / HTTP response error handling
        LastJsonResponse = FString("ERREUR"); // Réinitialisez la réponse JSON en cas d'erreur / Reset the JSON response in the event of an error
    }

}

void UThugzBCBPLibrary::CopyToClipboard(const FString& TextToCopy)
{
    FPlatformApplicationMisc::ClipboardCopy(*TextToCopy);  // Copy the FString to the system clipboard
}


//////////////////////////////////CREATION WALLET SOLANA avec/with SODIUM////////////////////////////////////////////////////////////////////////////////////////////////
void UThugzBCBPLibrary::GenerateSolanaKeyPair(FString& OutPublicKey, FString& OutPrivateKey)
{
    if (sodium_init() < 0)
    {
        // L'initialisation a échoué / Initialisation failed
        UE_LOG(LogTemp, Error, TEXT("Libsodium initialization failed!"));
        return;
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Libsodium initialization succeeded."));
    }

    unsigned char publicKey[crypto_sign_PUBLICKEYBYTES];
    unsigned char privateKey[crypto_sign_SECRETKEYBYTES];

    // Générer une paire de clés/Generate a key pair
    crypto_sign_keypair(publicKey, privateKey);

    // Convertir les clés en chaînes hexadécimales/Convert the keys to hexadecimal strings
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
void UThugzBCBPLibrary::GetSolanaAddressFromPrivateKey(const FString& PrivateKey, FString& PublicKeyHex, FString& PublicKeyBase58)
{
    // Assurez-vous que libsodium est initialisé/Make sure that libsodium is initialized.
    static bool bSodiumInitialized = false;
    if (!bSodiumInitialized) {
        if (sodium_init() == -1) {
            UE_LOG(LogTemp, Error, TEXT("Failed to initialize libsodium"));
        }
        bSodiumInitialized = true;
    }

    // Convertir la clé privée de Base58 à TArray<uint8>/Convert private key from Base58 to TArray<uint8>
    TArray<uint8> PrivateKeyArray = DecodeBase58(PrivateKey);

    // Vérifier la longueur de la clé privée/Verify the length of the private key
    if (PrivateKeyArray.Num() != crypto_sign_SECRETKEYBYTES) {
        UE_LOG(LogTemp, Error, TEXT("Invalid private key length: %d"), PrivateKeyArray.Num());
    }

    // Générer la clé publique à partir de la clé privée/Generate the public key from the private key
    uint8 PublicKey[crypto_sign_PUBLICKEYBYTES];
    crypto_sign_ed25519_sk_to_pk(PublicKey, PrivateKeyArray.GetData());

    // Convertir la clé publique en chaîne hexadécimale pour vérification/Convert the public key to a hexadecimal string for verification
    PublicKeyHex = BytesToHex(PublicKey, crypto_sign_PUBLICKEYBYTES);

    // Convertir la clé publique en chaîne de caractères Base58 pour obtenir l'adresse Solana
    TArray<uint8> PublicKeyArray;
    PublicKeyArray.Append(PublicKey, crypto_sign_PUBLICKEYBYTES);
    PublicKeyBase58 = EncodeBase58(PublicKeyArray);

    UE_LOG(LogTemp, Log, TEXT("Public Key (Hex): %s"), *PublicKeyHex);
    UE_LOG(LogTemp, Log, TEXT("Public Key (Base58): %s"), *PublicKeyBase58);

    //return PublicKeyBase58;
}