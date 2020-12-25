#include <cstdio>

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

auto print (const char *msg) -> void
{
   printf ("%s\n", msg);
}

auto printError (const char *msg) -> void
{
   print (msg);
   exit (1);
}

auto printHelp () -> void
{
   printf ("Usage: bip [-n ORIGIN_FILE] PATCH BINARY_FILE [-o OUTPUT_FILE]\n");
   printf ("Apply PATCH to BINARY_FILE\n");
   printf ("Example: bip g4_skip.bps SuperMetroid.sfc -o g4_skip.sfc\n\n");
   printf ("Usage: bip --crc32 BINARY_FILE\n");
   printf ("Compute CRC32 for BINARY_FILE\n");
   printf ("Example: bip --crc32 SuperMetroid.sfc\n");
   exit (1);
}

auto printUsage () -> void
{
   printf ("Usage: bip [-n ORIGIN_FILE] PATCH BINARY_FILE [-o OUTPUT_FILE]\n");
   printf ("Try 'bip --help' for more information.\n");
   exit (1);
}

auto printVersion () -> void
{
   printf ("bip v0.3\n");
   exit (1);
}

auto processCommandLine (int argc, char **argv) -> bool
{
   bool InvalidArguments = false;
   string BinaryFilePath, PatchFilePath;
   string OriginFilePath, OutputFilePath;

   auto i = 1;
   auto getNextArg = [&] () {
      if (++i > argc)
      {
         print ("Not enough arguments");
         printUsage ();
      }

      return argv[i];
   };

   for (; i < argc; i++)
   {
      if (!strcmp (argv[i], "-n"))
         OriginFilePath = getNextArg ();

      else if (!strcmp (argv[i], "-o"))
         OutputFilePath = getNextArg ();
      
      else if (!strcmp (argv[i], "--help"))
         printHelp ();

      else if (!strcmp (argv[i], "--version"))
         printVersion ();

      else if (!strcmp (argv[i], "--crc32"))
      {
         File InputFile;

         if (!InputFile.Read (getNextArg ()))
            printError ("Failed to read specified file.");

         printf ("%x\n", BpsPatch::Crc32 (InputFile.Data (), InputFile.Length ()));
         exit (0);
      }

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

   string Message;

   if (OutputFilePath.empty ())
   {
      OutputFilePath = BinaryFilePath;
      Message = BinaryFilePath + " += " + PatchFilePath;
   }
   else
      Message = OutputFilePath + " = " + BinaryFilePath + " + " + PatchFilePath;

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

         if (!IPS.VerifyOrigin (BinaryFile, OriginFile))
            printError ("Origin and Binary file mismatch for patch.");
      }

      if (!IPS.Apply (BinaryFile, OutputFile))
         printError ("IPS patch application failed!");
   }
   else if (BpsPatch::IsBpsPatch (PatchFile))
   {
      BpsPatch BPS (PatchFile);

      if (!BPS.Apply (BinaryFile, OutputFile))
         printError ("BPS patch application failed!");
   }
   else
      printError ("Please provide a valid IPS or BPS patch file.");

   if (!OutputFile.Write (OutputFilePath))
      printError ("Failed to write output file!");

   print (Message.c_str ());
   return true;
}
