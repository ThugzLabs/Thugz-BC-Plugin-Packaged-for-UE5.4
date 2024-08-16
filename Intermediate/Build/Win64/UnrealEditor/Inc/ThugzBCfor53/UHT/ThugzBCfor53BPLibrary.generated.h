// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "ThugzBCfor53BPLibrary.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class UTexture2D;
struct FEVMFNFTResponse;
struct FRootJson;
#ifdef THUGZBCFOR53_ThugzBCfor53BPLibrary_generated_h
#error "ThugzBCfor53BPLibrary.generated.h already included, missing '#pragma once' in ThugzBCfor53BPLibrary.h"
#endif
#define THUGZBCFOR53_ThugzBCfor53BPLibrary_generated_h

#define FID_Users_ronan_Documents_Unreal_Projects_test_Plugins_ThugzBCfor53_Source_ThugzBCfor53_Public_ThugzBCfor53BPLibrary_h_32_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FCreator_Statics; \
	THUGZBCFOR53_API static class UScriptStruct* StaticStruct();


template<> THUGZBCFOR53_API UScriptStruct* StaticStruct<struct FCreator>();

#define FID_Users_ronan_Documents_Unreal_Projects_test_Plugins_ThugzBCfor53_Source_ThugzBCfor53_Public_ThugzBCfor53BPLibrary_h_47_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FMetadataJson_Statics; \
	THUGZBCFOR53_API static class UScriptStruct* StaticStruct();


template<> THUGZBCFOR53_API UScriptStruct* StaticStruct<struct FMetadataJson>();

#define FID_Users_ronan_Documents_Unreal_Projects_test_Plugins_ThugzBCfor53_Source_ThugzBCfor53_Public_ThugzBCfor53BPLibrary_h_68_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FNFTData_Statics; \
	THUGZBCFOR53_API static class UScriptStruct* StaticStruct();


template<> THUGZBCFOR53_API UScriptStruct* StaticStruct<struct FNFTData>();

#define FID_Users_ronan_Documents_Unreal_Projects_test_Plugins_ThugzBCfor53_Source_ThugzBCfor53_Public_ThugzBCfor53BPLibrary_h_88_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FRootJson_Statics; \
	THUGZBCFOR53_API static class UScriptStruct* StaticStruct();


template<> THUGZBCFOR53_API UScriptStruct* StaticStruct<struct FRootJson>();

#define FID_Users_ronan_Documents_Unreal_Projects_test_Plugins_ThugzBCfor53_Source_ThugzBCfor53_Public_ThugzBCfor53BPLibrary_h_102_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FEVMFNFTMetadata_Statics; \
	THUGZBCFOR53_API static class UScriptStruct* StaticStruct();


template<> THUGZBCFOR53_API UScriptStruct* StaticStruct<struct FEVMFNFTMetadata>();

#define FID_Users_ronan_Documents_Unreal_Projects_test_Plugins_ThugzBCfor53_Source_ThugzBCfor53_Public_ThugzBCfor53BPLibrary_h_121_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FEVMFNFTData_Statics; \
	THUGZBCFOR53_API static class UScriptStruct* StaticStruct();


template<> THUGZBCFOR53_API UScriptStruct* StaticStruct<struct FEVMFNFTData>();

#define FID_Users_ronan_Documents_Unreal_Projects_test_Plugins_ThugzBCfor53_Source_ThugzBCfor53_Public_ThugzBCfor53BPLibrary_h_172_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FEVMFNFTResponse_Statics; \
	THUGZBCFOR53_API static class UScriptStruct* StaticStruct();


template<> THUGZBCFOR53_API UScriptStruct* StaticStruct<struct FEVMFNFTResponse>();

#define FID_Users_ronan_Documents_Unreal_Projects_test_Plugins_ThugzBCfor53_Source_ThugzBCfor53_Public_ThugzBCfor53BPLibrary_h_195_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execGetSolanaAddressFromPrivateKey); \
	DECLARE_FUNCTION(execHexToBytes); \
	DECLARE_FUNCTION(execDecodeBase58); \
	DECLARE_FUNCTION(execEncodeBase58); \
	DECLARE_FUNCTION(execGenerateSolanaKeyPair); \
	DECLARE_FUNCTION(execDownloadImageAndCreateTexture); \
	DECLARE_FUNCTION(execParseImageURL); \
	DECLARE_FUNCTION(execMakeURIRequest); \
	DECLARE_FUNCTION(execGetTokenBamanceFromJsonMoralis); \
	DECLARE_FUNCTION(execMakeMoraliseRequestForSOLBalance); \
	DECLARE_FUNCTION(execGetTokenBamanceFromJsonHelloMoon); \
	DECLARE_FUNCTION(execGetLastTokenJsonResponse); \
	DECLARE_FUNCTION(execHelloMoonRequestForTokenBalance); \
	DECLARE_FUNCTION(execConvertEVMJSONtoStruct); \
	DECLARE_FUNCTION(execConvertSOLJSONtoStruct); \
	DECLARE_FUNCTION(execGetLastJsonResponse); \
	DECLARE_FUNCTION(execMoralisAPIRequest); \
	DECLARE_FUNCTION(execMakeHelloMoonAPIRequest);


#define FID_Users_ronan_Documents_Unreal_Projects_test_Plugins_ThugzBCfor53_Source_ThugzBCfor53_Public_ThugzBCfor53BPLibrary_h_195_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUThugzBCBPLibrary(); \
	friend struct Z_Construct_UClass_UThugzBCBPLibrary_Statics; \
public: \
	DECLARE_CLASS(UThugzBCBPLibrary, UBlueprintFunctionLibrary, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/ThugzBCfor53"), NO_API) \
	DECLARE_SERIALIZER(UThugzBCBPLibrary)


#define FID_Users_ronan_Documents_Unreal_Projects_test_Plugins_ThugzBCfor53_Source_ThugzBCfor53_Public_ThugzBCfor53BPLibrary_h_195_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UThugzBCBPLibrary(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	UThugzBCBPLibrary(UThugzBCBPLibrary&&); \
	UThugzBCBPLibrary(const UThugzBCBPLibrary&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UThugzBCBPLibrary); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UThugzBCBPLibrary); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UThugzBCBPLibrary) \
	NO_API virtual ~UThugzBCBPLibrary();


#define FID_Users_ronan_Documents_Unreal_Projects_test_Plugins_ThugzBCfor53_Source_ThugzBCfor53_Public_ThugzBCfor53BPLibrary_h_192_PROLOG
#define FID_Users_ronan_Documents_Unreal_Projects_test_Plugins_ThugzBCfor53_Source_ThugzBCfor53_Public_ThugzBCfor53BPLibrary_h_195_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Users_ronan_Documents_Unreal_Projects_test_Plugins_ThugzBCfor53_Source_ThugzBCfor53_Public_ThugzBCfor53BPLibrary_h_195_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Users_ronan_Documents_Unreal_Projects_test_Plugins_ThugzBCfor53_Source_ThugzBCfor53_Public_ThugzBCfor53BPLibrary_h_195_INCLASS_NO_PURE_DECLS \
	FID_Users_ronan_Documents_Unreal_Projects_test_Plugins_ThugzBCfor53_Source_ThugzBCfor53_Public_ThugzBCfor53BPLibrary_h_195_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> THUGZBCFOR53_API UClass* StaticClass<class UThugzBCBPLibrary>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Users_ronan_Documents_Unreal_Projects_test_Plugins_ThugzBCfor53_Source_ThugzBCfor53_Public_ThugzBCfor53BPLibrary_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
