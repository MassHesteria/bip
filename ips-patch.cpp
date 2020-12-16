#include "ips-patch.h"

IpsPatch::IpsPatch (File &PatchFile) :
   patchFile (PatchFile), maxHunkOffset (0)
{
   int PatchIndex = 5;
   while (ReadHunk (PatchIndex));
}

auto IpsPatch::Apply (File &FileToPatch, File &TargetFile) -> bool
{
   if (hunks.empty ())
      return false;

   TargetFile.Copy (FileToPatch, maxHunkOffset);
   auto TargetBuffer = TargetFile.Data ();

   for (auto& Hunk : hunks)
   {
      if (Hunk.rle)
         memset (&TargetBuffer[Hunk.offset], Hunk.payloadBuffer[Hunk.payloadIndex], Hunk.length);
      else
         memcpy (&TargetBuffer[Hunk.offset], &Hunk.payloadBuffer[Hunk.payloadIndex], Hunk.length);
   }

   return true;
}

auto IpsPatch::IsIpsPatch (File &PatchFile) -> bool
{
   auto PatchBuffer = PatchFile.Data ();
   auto PatchLength = PatchFile.Length ();

   // No patch buffer or not enough bytes?
   if (NULL == PatchBuffer || 8 > PatchLength)
      return false;

   // Does the patch data not start with PATCH?
   if (strncmp (PatchBuffer, "PATCH", 5))
      return false;

   // Does the patch data no end with EOF?
   if (strncmp (&PatchBuffer[PatchLength - 3], "EOF", 3))
      return false;

   return true;
}

auto IpsPatch::ReadHunk (int &Index) -> bool
{
   auto Buffer = patchFile.Data ();
   auto Length = patchFile.Length ();

   if (Index > Length - 8)
      return false;

   IpsHunk Hunk;
   Hunk.offset = (Buffer[Index] & 0xff) << 16 | (Buffer[Index + 1] & 0xff) << 8 |
                 (Buffer[Index + 2] & 0xff);
   Hunk.length = (Buffer[Index + 3] &0xff) << 8 | (Buffer[Index + 4] & 0xff);
   Hunk.payloadBuffer = Buffer;

   Index += 5;

   if (Hunk.length == 0)
   {
      if (Index > Length - 6)
         return false;

      Hunk.rle = true;
      Hunk.length = (Buffer[Index] & 0xff) << 8 | (Buffer[Index + 1] & 0xff);
      Hunk.payloadIndex = Index + 2;
      Index += 3;
   }
   else
   {
      Hunk.payloadIndex = Index;
      Index += Hunk.length;
   }

   if (Hunk.offset + Hunk.length > maxHunkOffset)
      maxHunkOffset = Hunk.offset + Hunk.length;

   hunks.push_back (Hunk);
   return true;
}

auto IpsPatch::VerifySource (File &FileToPatch, File &Origin) -> bool
{
   auto FileBuffer = FileToPatch.Data ();
   auto OriginBuffer = Origin.Data ();

   for (auto& Hunk : hunks)
   {
      if (memcmp (&FileBuffer[Hunk.offset], &OriginBuffer[Hunk.offset], Hunk.length))
         return false;
   }

   return true;
}
