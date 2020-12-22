#pragma once

#include <cstring>
#include <stdint.h>
#include <string>
using namespace std;

class File
{
   private:
      char *data;
      size_t size;

   public:
      File (void);
      ~File (void);
      auto Copy (const File &FileToCopy, const size_t MinSize) -> bool;
      auto Data (void) -> char *;
      auto EndsWith (string_view Footer) -> bool;
      static auto Exists (const string& fileName) -> bool;
      auto GetU8 (size_t Index) -> uint8_t;
      auto GetU32 (size_t Index) -> uint32_t;
      auto Length (void) -> size_t;
      auto Read (const string &fileName) -> bool;
      auto Resize (const size_t NewSize) -> void;
      auto StartsWith (string_view Header) -> bool;
      auto Write (const string& FileName) -> bool;
};
