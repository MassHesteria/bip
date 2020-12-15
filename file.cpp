#include "file.h"

File::File (void) :
   size (0), data (NULL)
{
}

File::~File (void)
{
   if (data != NULL)
      delete[] data;
}

auto File::Copy (const File &FileToCopy, const size_t MinSize) -> bool
{
   Resize (max (FileToCopy.size, MinSize));
   memcpy (data, FileToCopy.data, FileToCopy.size);
   return true;
}

auto File::Data (void) -> char *
{
   return data;
}

auto File::Exists (const string& fileName) -> bool
{
   auto File = fopen (fileName.c_str (), "r");

   if (NULL == File)
      return false;

   fclose (File);
   return true;
}

auto File::GetU8 (size_t Index) -> uint8_t
{
   return data[Index];
}

auto File::GetU32 (size_t Index) -> uint32_t
{
   return *((uint32_t *) &data[Index]);
}

auto File::Length (void) -> size_t
{
   return size;
}

auto File::Read (const string& fileName) -> bool
{
   auto FilePtr = fopen (fileName.c_str (), "r");

   if (!FilePtr)
      return false;

   fseek (FilePtr, 0, SEEK_END);
   Resize (ftell (FilePtr));
   fseek (FilePtr, 0, SEEK_SET);

   fread (data, sizeof (char), size, FilePtr);
   fclose (FilePtr);

   return true;
}

auto File::Resize (const size_t NewSize) -> void
{
   if (NewSize == size)
      return;

   if (data != NULL)
      delete[] data;

   size = NewSize;
   data = new char[size];
}

auto File::Write (const string& FileName) -> bool
{
   FILE *FilePtr = fopen (FileName.c_str (), "w");

   if (!FilePtr)
      return false;

   fwrite (data, sizeof (char), size, FilePtr);
   fclose (FilePtr);

   return true;
}