using System;
using System.IO;
using UnrealBuildTool;

public class ThugzBCfor53 : ModuleRules
{
    public ThugzBCfor53(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
            new string[] {
                Path.Combine(ModuleDirectory, "../ThirdParty/libsodium/include")
            }
        );

        PrivateIncludePaths.AddRange(
            new string[] {
                // Ajoutez ici d'autres chemins d'inclusion privés requis
            }
        );

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core", "InputCore", "Json", "JsonUtilities", "HTTP", "Engine", "CoreUObject", "RenderCore", "RHI", "ApplicationCore"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject", "Engine", "Slate", "SlateCore"
            }
        );

        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
                // Ajoutez ici les modules que votre module charge dynamiquement
            }
        );

        string ThirdPartyPath = Path.Combine(ModuleDirectory, "../ThirdParty/libsodium");

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "include"));
            PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "Win64", "libsodium.lib"));

            // Copier la DLL dans les répertoires de sortie appropriés
            string PluginBinariesDir = Path.Combine(ModuleDirectory, "../../Binaries/Win64");
            string ProjectBinariesDir = Path.Combine(Target.ProjectFile.Directory.ToString(), "Binaries/Win64");
            string DllPath = Path.Combine(ThirdPartyPath, "lib", "Win64", "libsodium.dll");

            // Copier dans le répertoire de sortie du plugin
            CopyFileToOutput(DllPath, PluginBinariesDir);

            // Copier dans le répertoire de sortie du projet
            CopyFileToOutput(DllPath, ProjectBinariesDir);

            // Ajouter la DLL aux dépendances runtime
            RuntimeDependencies.Add(Path.Combine(PluginBinariesDir, "libsodium.dll"));
            RuntimeDependencies.Add(Path.Combine(ProjectBinariesDir, "libsodium.dll"));
        }
        else if (Target.Platform == UnrealTargetPlatform.Android)
        {
            string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
            AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(PluginPath, "ThugzBCfor53_APL.xml"));

            string LibPath = Path.Combine(ThirdPartyPath, "lib", "Android");

            // Ajoutez les bibliothèques appropriées pour Android
            PublicAdditionalLibraries.Add(Path.Combine(LibPath, "armeabi-v7a", "libsodium.so"));
            PublicAdditionalLibraries.Add(Path.Combine(LibPath, "arm64-v8a", "libsodium.so"));
            PublicAdditionalLibraries.Add(Path.Combine(LibPath, "x86", "libsodium.so"));
            PublicAdditionalLibraries.Add(Path.Combine(LibPath, "x86_64", "libsodium.so"));

            PrivateIncludePaths.Add(Path.Combine(ThirdPartyPath, "include"));
        }

        // Add Libsodium for iOS
        if (Target.Platform == UnrealTargetPlatform.IOS)
        {
            string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
            AdditionalPropertiesForReceipt.Add("IOSPlugin", Path.Combine(PluginPath, "ThugzBCfor53_IOS.xml"));

            string LibPath = Path.Combine(ThirdPartyPath, "lib", "IOS");

            // Ajouter la bibliothèque statique pour iOS
            PublicAdditionalLibraries.Add(Path.Combine(LibPath, "libsodium-iOS.a"));

            PrivateIncludePaths.Add(Path.Combine(ThirdPartyPath, "include"));
        }

    }

    private void CopyFileToOutput(string SourceFilePath, string OutputDir)
    {
        if (!Directory.Exists(OutputDir))
        {
            Directory.CreateDirectory(OutputDir);
        }

        string FileName = Path.GetFileName(SourceFilePath);
        string DestPath = Path.Combine(OutputDir, FileName);

        try
        {
            if (!File.Exists(DestPath))
            {
                File.Copy(SourceFilePath, DestPath, true);
                System.Console.WriteLine($"Copied {SourceFilePath} to {DestPath}");
            }
            else
            {
                System.Console.WriteLine($"File {DestPath} already exists. Skipping copy.");
            }
        }
        catch (UnauthorizedAccessException e)
        {
            System.Console.WriteLine($"Access to the path {DestPath} is denied: {e.Message}");
        }
        catch (IOException e)
        {
            System.Console.WriteLine($"Failed to copy {SourceFilePath} to {DestPath}: {e.Message}");
        }
    }

}
