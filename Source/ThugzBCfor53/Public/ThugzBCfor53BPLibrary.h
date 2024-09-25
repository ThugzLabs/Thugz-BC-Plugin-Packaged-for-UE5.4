// Created in 2024. Copyright Thugz Labs SAS, all rights reserved.

#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Serialization/JsonSerializer.h"
#include "Dom/JsonObject.h"
#include "JsonObjectConverter.h"
#include "Engine/Texture2D.h"
#include "Engine/Texture.h"
#include "RenderUtils.h"
#include "TextureResource.h"
#include "RenderResource.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Misc/Paths.h"
#include "../../ThirdParty/libsodium/include/sodium.h"
#include "ThugzBCfor53BPLibrary.generated.h"

class FJsonObject;

// Définition des structures UE qui acceuilleront les réponses JSON

//////////////////STRUCTURE POUR SOLANA HELLOMOON/////////////////////////////////

USTRUCT(BlueprintType)
struct FCreator
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thugz Labs JSON")
    FString Address;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thugz Labs JSON")
    bool Verified;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thugz Labs JSON")
    int32 Share;
};

USTRUCT(BlueprintType)
struct FMetadataJson
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thugz Labs JSON")
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thugz Labs JSON")
    FString Symbol;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thugz Labs JSON")
    FString Uri;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thugz Labs JSON")
    int32 SellerFeeBasisPoints;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thugz Labs JSON")
    TArray<FCreator> Creators;
};

USTRUCT(BlueprintType)
struct FNFTData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thugz Labs JSON")
    FString NftMint;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thugz Labs JSON")
    FString OwnerAccount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thugz Labs JSON")
    FString MetadataAddress;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thugz Labs JSON")
    FMetadataJson MetadataJson;

};

USTRUCT(BlueprintType)
struct FRootJson
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thugz Labs JSON")
    TArray<FNFTData> Data;
};
//////////////////END STRUCTURE POUR SOLANA HELLOMOON/////////////////////////////////



//////////////////STRUCTURE POUR EVM MORALIS/////////////////////////////////

USTRUCT(BlueprintType)
struct FEVMFNFTMetadata
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thugz Labs JSON")
    FString Image;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thugz Labs JSON")
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thugz Labs JSON")
    FString description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thugz Labs JSON")
    FString external_url;

};

USTRUCT(BlueprintType)
struct FEVMFNFTData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thugz Labs JSON")
    FString Amount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thugz Labs JSON")
    FString TokenId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thugz Labs JSON")
    FString TokenAddress;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thugz Labs JSON")
    FString ContractType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thugz Labs JSON")
    FString OwnerOf;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thugz Labs JSON")
    FString last_metadata_sync;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thugz Labs JSON")
    FString last_token_uri_sync;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thugz Labs JSON")
    FString block_number;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thugz Labs JSON")
    FString name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thugz Labs JSON")
    FString symbol;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thugz Labs JSON")
    FString token_hash;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thugz Labs JSON")
    FString token_uri;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thugz Labs JSON")
    FString minter_address;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thugz Labs JSON")
    FString verified_collection;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thugz Labs JSON")
    FEVMFNFTMetadata Metadata;
};

USTRUCT(BlueprintType)
struct FEVMFNFTResponse
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thugz Labs JSON")
    FString Status;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thugz Labs JSON")
    int32 Page;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thugz Labs JSON")
    int32 PageSize;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thugz Labs JSON")
    TArray<FEVMFNFTData> Result;

};
//////////////////END STRUCTURE POUR EVM MORALIS/////////////////////////////////

//////////////////STRUCTURE POUR SOLANA MORALIS/////////////////////////////////
USTRUCT(BlueprintType)
struct FThugzNFTData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thugz Labs JSON")
    FString AssociatedTokenAddress;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thugz Labs JSON")
    FString Mint;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thugz Labs JSON")
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thugz Labs JSON")
    FString Symbol;
};

USTRUCT(BlueprintType)
struct FSolMoralisMetaplex
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thugz Labs JSON")
    FString MetadataUri;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thugz Labs JSON")
    FString UpdateAuthority;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thugz Labs JSON")
    int32 SellerFeeBasisPoints;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thugz Labs JSON")
    int32 PrimarySaleHappened;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thugz Labs JSON")
    bool IsMutable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thugz Labs JSON")
    bool MasterEdition;
};

USTRUCT(BlueprintType)
struct FSolMoralisNFTMetadata
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thugz Labs JSON")
    FString Mint;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thugz Labs JSON")
    FString Standard;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thugz Labs JSON")
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thugz Labs JSON")
    FString Symbol;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thugz Labs JSON")
    FSolMoralisMetaplex Metaplex;
};

//////////////////END STRUCTURE POUR SOLANA MORALIS/////////////////////////////////



// Définition des fonctions pour exposition au Blueprint / Defining functions for Blueprint exposure
UCLASS()
class UThugzBCBPLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Web3 Thugz Labs Plugin")
    static void MakeHelloMoonAPIRequest(const FString& Account, const FString& Barear);

    UFUNCTION(BlueprintCallable, Category = "Web3 Thugz Labs Plugin")
    static void MoralisAPIRequest(const FString& AccountAddress, const FString& ApiKey, const FString& Blockchain);

    UFUNCTION(BlueprintCallable, Category = "Web3 Thugz Labs Plugin")
    static FString GetLastJsonResponse();

    UFUNCTION(BlueprintCallable, Category = "Web3 Thugz Labs Plugin")
    static FRootJson ConvertSOLJSONtoStruct(FString JsonString);

    UFUNCTION(BlueprintCallable, Category = "Web3 Thugz Labs Plugin")
    static FEVMFNFTResponse ConvertEVMJSONtoStruct(FString JsonString);

    UFUNCTION(BlueprintCallable, Category = "Web3 Thugz Labs Plugin")
    static void HelloMoonRequestForTokenBalance(const FString& Param, const FString& ApiKey, FString& OutResponse);

    UFUNCTION(BlueprintCallable, Category = "Web3 Thugz Labs Plugin")
    static FString GetLastTokenJsonResponse();

    UFUNCTION(BlueprintCallable, Category = "Web3 Thugz Labs Plugin")
    static void GetTokenBamanceFromJsonHelloMoon(const FString& JsonString, double& OutValue);

    UFUNCTION(BlueprintCallable, Category = "Web3 Thugz Labs Plugin")
    static void MakeMoraliseRequestForSOLBalance(const FString& Pkey, const FString& ApiKey, FString& OutResponse);

    UFUNCTION(BlueprintCallable, Category = "Web3 Thugz Labs Plugin")
    static void GetTokenBamanceFromJsonMoralis(const FString& JsonString, double& OutSolanaValue);

    UFUNCTION(BlueprintCallable, Category = "Web3 Thugz Labs Plugin")
    static void MakeURIRequest(const FString& URL);

    UFUNCTION(BlueprintCallable, Category = "Web3 Thugz Labs Plugin")
    static bool ParseImageURL(const FString& JsonString, FString& OutImageURL);

    UFUNCTION(BlueprintCallable, Category = "Web3 Thugz Labs Plugin")
    static void DownloadImageAndCreateTexture(const FString& URL, UTexture2D*& OutTexture);

    // Ajout de la fonction pour générer une paire de clés Solana / Addition of the function for generating a pair of Solana keys
    UFUNCTION(BlueprintCallable, Category = "Web3 Thugz Labs Plugin")
    static void GenerateSolanaKeyPair(FString& OutPublicKey, FString& OutPrivateKey);

    //Code base 58 pour encodage et decodage 58 + Hexadecimal vers Bytes / Base 58 code for encoding and decoding 58 + Hexadecimal to Bytes
    UFUNCTION(BlueprintCallable, Category = "Web3 Thugz Labs Plugin")
    static FString EncodeBase58(const TArray<uint8>& Data);
    UFUNCTION(BlueprintCallable, Category = "Web3 Thugz Labs Plugin")
    static TArray<uint8> DecodeBase58(const FString& Base58String);
    UFUNCTION(BlueprintCallable, Category = "Web3 Thugz Labs Plugin")
    static TArray<uint8> HexToBytes(const FString& HexString);

    //Récuperation de l'adresse publique SOlana depuis la clé privée / Retrieving the SOlana public address from the private key
    UFUNCTION(BlueprintCallable, Category = "Web3 Thugz Labs Plugin")
    static void GetSolanaAddressFromPrivateKey(const FString& PrivateKey, FString& PublicKeyHex, FString& PublicKeyBase58);

    //Requête Moralis pour récupérer les NFTs d'un wallet / Moralis request to retrieve NFTs from a wallet
    UFUNCTION(BlueprintCallable, Category = "Web3 Thugz Labs Plugin")
    static void MakeMoralisAPIRequest(const FString& OwnerAccount, const FString& APIKey);
    //Parsing de la réponse de la requête Moralis / Parsing the response to the Moralis request
    UFUNCTION(BlueprintCallable, Category = "Web3 Thugz Labs Plugin")
    static TArray<FThugzNFTData> ParseNFTDataFromMoralisJSON(const FString& JsonString);
    //Requête pour récuperer les metadata d'un NFT par Moralis / Request to retrieve the metadata of an NFT by Moralis
    UFUNCTION(BlueprintCallable, Category = "Web3 Thugz Labs Plugin")
    static void MakeMoralisNFTMetadataRequest(const FString& NFTMintAddress, const FString& APIKey);
    //Requête pour parser la réponse de la fonction: MakeMoralisNFTMetadataRequest / Request to parse the function response: MakeMoralisNFTMetadataRequest
    UFUNCTION(BlueprintCallable, Category = "JSON Parsing")
    static FSolMoralisNFTMetadata ParseNFTMetadataFromJSON(const FString& JsonString);



private:
    static FString LastJsonResponse;

    static FString LastTokenBalance;

    static void HandleHelloMoonAPIResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

    static void OnImageDownloaded(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful, UTexture2D** OutTexture);
    static UTexture2D* CreateTextureFromImageData(const TArray<uint8>& ImageData);

    static FString BytesToHex(const unsigned char* Bytes, int32 Length);

};



