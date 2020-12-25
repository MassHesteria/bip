__bip__ is a command line utility for patching binary files. This application was developed while researching popular ROM patch formats and is not intended to be a replacement for any of the other excellent patching applications that are already available.


Features
--------
- Supports popular ROM patch formats (IPS & BPS)
- Origin verification for IPS patches
- Computes CRC32 checksums


Origin Verification
-------------------
*Incremental patching* refers to applying more than one patch to the same binary file. This technique is useful when multiple developers produce patches independently. There is inherent risk in *incremental patching* because typically each patch was based on a common __origin file__, and there is nothing to stop one patch from modifying the same bytes as another patch.

IPS patches lend themselves to *incremental patching* because of how they encode information and because they do not contain embedded source or target verification data. bip supports the ability to specify the __origin file__ while applying an IPS patch. When provided, bip verifies that the sections of the source file that will be modified by the patch match the same sections within the __origin file__ prior to patching. This does not guarantee compatibility between patches but can give increased confidence.
