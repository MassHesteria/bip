#pragma once

#include <vector>
#include "file.h"

class IpsPatch
{
   private:
      struct IpsHunk
      {
         int length = 0;
         int offset = 0;
         const char *payloadBuffer = NULL;
         int payloadIndex = 0;
         bool rle = false;

         auto Apply (File &Target) -> void;
      };

   private:
      vector<IpsHunk> hunks;
      File &patchFile;
      size_t maxHunkOffset;

   private:
      auto ReadHunk (int &Index) -> bool;

   public:
      IpsPatch (File &PatchFile);
      auto Apply (File &FileToPatch, File &TargetFile) -> bool;
      static auto IsIpsPatch (File &PatchFile) -> bool;
      auto VerifySource (File &FileToPatch, File &Origin) -> bool;
};
