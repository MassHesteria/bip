#pragma once

#include "file.h"

class BpsPatch
{
   private:
      File &patchFile;
      size_t patchOffset;

   private:
      auto Crc32 (const char *Data, const size_t Length) -> uint32_t;
      auto Decode (void) -> uint64_t;
      auto Read (void) -> uint8_t;

   public:
      BpsPatch (File &PatchFile);
      auto Apply (File &FileToPatch, File &TargetFile) -> bool;
      static auto IsBpsPatch (File &PatchFile) -> bool;
};