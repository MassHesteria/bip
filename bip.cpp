#include <cstdio>
#include <iostream>

#include "file.h"
#include "bps-patch.h"
#include "ips-patch.h"

/* Forward declarations. */

auto processCommandLine (int argc, char **argv) -> bool;

auto main (int argc, char **argv) -> int
{
   if (!processCommandLine (argc, argv))
      return 1;

   return 0;
}

auto print (string_view view) -> void
{
   cout << view << endl;
}

auto printError (string_view view) -> void
{
   print (view);
   exit (1);
}

auto printHelp () -> void
{
   cout << "Usage: bip [-n ORIGIN_FILE] PATCH BINARY_FILE [-o OUTPUT_FILE]" << endl;
   cout << "Apply PATCH to BINARY_FILE" << endl;
   cout << "Example: bip g4_skip.bps SuperMetroid.sfc -o g4_skip.sfc" << endl;
   exit (1);
}

auto printUsage () -> void
{
   cout << "Usage: bip [-n ORIGIN_FILE] PATCH BINARY_FILE [-o OUTPUT_FILE]" << endl;
   cout << "Try 'bip --help' for more information." << endl;
   exit (1);
}

auto printVersion () -> void
{
   cout << "bip v0.1" << endl;
   exit (1);
}

auto processCommandLine (int argc, char **argv) -> bool
{
   bool InvalidArguments = false;
   string BinaryFilePath, PatchFilePath;
   string OriginFilePath, OutputFilePath;

   for (int i = 1; i < argc; i++)
   {
      if (!strcmp (argv[i], "-n"))
         OriginFilePath = argv[++i];

      else if (!strcmp (argv[i], "-o"))
         OutputFilePath = argv[++i];
      
      else if (!strcmp (argv[i], "--help"))
         printHelp ();

      else if (!strcmp (argv[i], "--version"))
         printVersion ();

      else if (argv[i][0] == '-')
      {
         InvalidArguments = true;
         break;
      }

      else if (PatchFilePath.empty ())
         PatchFilePath = argv[i];
      
      else if (BinaryFilePath.empty ())
         BinaryFilePath = argv[i];

      else
      {
         InvalidArguments = true;
         break;
      }
   }

   if (PatchFilePath.empty () || BinaryFilePath.empty () || InvalidArguments)
      printUsage ();

   if (OutputFilePath.empty ())
      OutputFilePath = BinaryFilePath;

   File BinaryFile, PatchFile, OutputFile;

   if (!PatchFile.Read (PatchFilePath))
      printError ("Failed to read patch file.");

   if (!BinaryFile.Read (BinaryFilePath))
      printError ("Failed to read binary file.");

   if (IpsPatch::IsIpsPatch (PatchFile))
   {
      IpsPatch IPS (PatchFile);

      if (!OriginFilePath.empty ())
      {
         File OriginFile;
         if (!OriginFile.Read (OriginFilePath))
            printError ("Failed to read origin file.");

         if (!IPS.VerifySource (BinaryFile, OriginFile))
            printError ("Origin and Binary file mismatch for patch.");

         print ("Verified with Origin.");
      }

      if (!IPS.Apply (BinaryFile, OutputFile))
         printError ("IPS patch application failed!");

      if (!OutputFile.Write (OutputFilePath))
         printError ("Failed to write output file!");

      print ("Applied IPS patch!");
   }
   else if (BpsPatch::IsBpsPatch (PatchFile))
   {
      BpsPatch BPS (PatchFile);

      if (!BPS.Apply (BinaryFile, OutputFile))
         printError ("BPS patch application failed!");

      if (!OutputFile.Write (OutputFilePath))
         printError ("Failed to write output file!");

      print ("Applied BPS patch!");
   }
   else
      printError ("Please provide a valid IPS or BPS patch file.");

   return true;
}
