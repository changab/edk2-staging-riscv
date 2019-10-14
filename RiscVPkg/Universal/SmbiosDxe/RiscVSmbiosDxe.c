/** @file
  RISC-V generic SMBIOS DXE driver to build up SMBIOS type 4, type 7 and type 44 records.

  Copyright (c) 2019, Hewlett Packard Enterprise Development LP. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "RiscVSmbiosDxe.h"

#define RISCV_SMBIOS_DEBUG_INFO 1

STATIC EFI_SMBIOS_PROTOCOL   *mSmbios;

/**
  This function builds SMBIOS type 7 record according to
  the given  RISC_V_PROCESSOR_TYPE7_HOB_DATA.

  @param Type4HobData       Pointer to RISC_V_PROCESSOR_TYPE4_HOB_DATA
  @param Type7DataHob       Pointer to RISC_V_PROCESSOR_TYPE7_HOB_DATA
  @param SmbiosHandle       Pointer to SMBIOS_HANDLE

  @retval EFI_STATUS

**/
STATIC
EFI_STATUS
BuildSmbiosType7 (
 IN RISC_V_PROCESSOR_TYPE4_HOB_DATA *Type4HobData,
 IN RISC_V_PROCESSOR_TYPE7_HOB_DATA *Type7DataHob,
 OUT SMBIOS_HANDLE *SmbiosHandle
)
{
  EFI_STATUS Status;
  SMBIOS_HANDLE Handle;

  if (!CompareGuid (&Type4HobData->PrcessorGuid, &Type7DataHob->PrcessorGuid) ||
    Type4HobData->ProcessorUid != Type7DataHob->ProcessorUid) {
    return EFI_INVALID_PARAMETER;
  }
  Handle = SMBIOS_HANDLE_PI_RESERVED;
  Type7DataHob->SmbiosType7Cache.Hdr.Type = SMBIOS_TYPE_CACHE_INFORMATION;
  Type7DataHob->SmbiosType7Cache.Hdr.Length = sizeof(SMBIOS_TABLE_TYPE7);
  Type7DataHob->SmbiosType7Cache.Hdr.Handle = 0;
  Type7DataHob->EndingZero = 0;
  Status = mSmbios->Add (mSmbios, NULL, &Handle, &Type7DataHob->SmbiosType7Cache.Hdr);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Fail to add SMBIOS Type 7\n", __FUNCTION__));
    return Status;
  }
  DEBUG ((DEBUG_INFO, "SMBIOS Type 7 was added. SMBIOS Handle: 0x%x\n", Handle));
#if RISCV_SMBIOS_DEBUG_INFO
  DEBUG ((DEBUG_INFO, "     Cache belone to processor GUID: %g\n", &Type7DataHob->PrcessorGuid));
  DEBUG ((DEBUG_INFO, "     Cache belone processor  UID: %d\n", Type7DataHob->ProcessorUid));
  DEBUG ((DEBUG_INFO, "     ==============================\n"));
  DEBUG ((DEBUG_INFO, "     Socket Designation: %d\n", Type7DataHob->SmbiosType7Cache.SocketDesignation));
  DEBUG ((DEBUG_INFO, "     Cache Configuration: 0x%x\n", Type7DataHob->SmbiosType7Cache.CacheConfiguration));
  DEBUG ((DEBUG_INFO, "     Maximum Cache Size: 0x%x\n", Type7DataHob->SmbiosType7Cache.MaximumCacheSize));
  DEBUG ((DEBUG_INFO, "     Installed Size: 0x%x\n", Type7DataHob->SmbiosType7Cache.InstalledSize));
  DEBUG ((DEBUG_INFO, "     Supported SRAM Type: 0x%x\n", Type7DataHob->SmbiosType7Cache.SupportedSRAMType));
  DEBUG ((DEBUG_INFO, "     Current SRAMT ype: 0x%x\n", Type7DataHob->SmbiosType7Cache.CurrentSRAMType));
  DEBUG ((DEBUG_INFO, "     Cache Speed: 0x%x\n", Type7DataHob->SmbiosType7Cache.CacheSpeed));
  DEBUG ((DEBUG_INFO, "     Error Correction Type: 0x%x\n", Type7DataHob->SmbiosType7Cache.ErrorCorrectionType));
  DEBUG ((DEBUG_INFO, "     System Cache Type: 0x%x\n", Type7DataHob->SmbiosType7Cache.SystemCacheType));
  DEBUG ((DEBUG_INFO, "     Associativity: 0x%x\n", Type7DataHob->SmbiosType7Cache.Associativity));
#endif

  *SmbiosHandle = Handle;
  return EFI_SUCCESS;
}

/**
  This function builds SMBIOS type 4 record according to
  the given  RISC_V_PROCESSOR_TYPE4_HOB_DATA.

  @param Type4HobData       Pointer to RISC_V_PROCESSOR_TYPE4_HOB_DATA
  @param SmbiosHandle       Pointer to SMBIOS_HANDLE

  @retval EFI_STATUS

**/
STATIC
EFI_STATUS
BuildSmbiosType4 (
  IN RISC_V_PROCESSOR_TYPE4_HOB_DATA *Type4HobData,
  OUT SMBIOS_HANDLE *SmbiosHandle
  )
{
  EFI_HOB_GUID_TYPE *GuidHob;
  RISC_V_PROCESSOR_TYPE7_HOB_DATA *Type7HobData;
  SMBIOS_HANDLE Cache;
  SMBIOS_HANDLE Processor;
  EFI_STATUS Status;

  DEBUG ((DEBUG_INFO, "Building Type 4.\n"));
  DEBUG ((DEBUG_INFO, "    Processor GUID: %g\n", &Type4HobData->PrcessorGuid));
  DEBUG ((DEBUG_INFO, "    Processor UUID: %d\n", Type4HobData->ProcessorUid));

  Type4HobData->SmbiosType4Processor.L1CacheHandle = RISC_V_CACHE_INFO_NOT_PROVIDED;
  Type4HobData->SmbiosType4Processor.L2CacheHandle = RISC_V_CACHE_INFO_NOT_PROVIDED;
  Type4HobData->SmbiosType4Processor.L3CacheHandle = RISC_V_CACHE_INFO_NOT_PROVIDED;
  GuidHob = (EFI_HOB_GUID_TYPE *)GetFirstGuidHob ((EFI_GUID *)PcdGetPtr(PcdProcessorSmbiosType7GuidHobGuid));
  if (GuidHob == NULL) {
    DEBUG ((DEBUG_ERROR, "No RISC-V SMBIOS Type7 data HOB found.\n"));
    return EFI_NOT_FOUND;
  }
  //
  // Go through each RISC_V_PROCESSOR_TYPE4_HOB_DATA for multiple processors.
  //
  do {
    Type7HobData = (RISC_V_PROCESSOR_TYPE7_HOB_DATA *)GET_GUID_HOB_DATA (GuidHob);
    Status = BuildSmbiosType7 (Type4HobData, Type7HobData, &Cache);
    if (EFI_ERROR (Status)) {
      return Status;
    }
    if ((Type7HobData->SmbiosType7Cache.SystemCacheType & RISC_V_CACHE_CONFIGURATION_CACHE_LEVEL_MASK) ==
        RISC_V_CACHE_CONFIGURATION_CACHE_LEVEL_1) {
      Type4HobData->SmbiosType4Processor.L1CacheHandle = Cache;
    } else if ((Type7HobData->SmbiosType7Cache.SystemCacheType & RISC_V_CACHE_CONFIGURATION_CACHE_LEVEL_MASK) ==
        RISC_V_CACHE_CONFIGURATION_CACHE_LEVEL_2) {
      Type4HobData->SmbiosType4Processor.L2CacheHandle = Cache;
    } else if ((Type7HobData->SmbiosType7Cache.SystemCacheType & RISC_V_CACHE_CONFIGURATION_CACHE_LEVEL_MASK) ==
        RISC_V_CACHE_CONFIGURATION_CACHE_LEVEL_3) {
      Type4HobData->SmbiosType4Processor.L3CacheHandle = Cache;
    } else {
      DEBUG ((DEBUG_ERROR, "Improper cache level of SMBIOS handle %d\n", Cache));
    }
    GuidHob = GetNextGuidHob((EFI_GUID *)PcdGetPtr(PcdProcessorSmbiosType7GuidHobGuid), GET_NEXT_HOB(GuidHob));
  } while (GuidHob != NULL);

  //
  // Build SMBIOS Type 4 record
  //
  Processor = SMBIOS_HANDLE_PI_RESERVED;
  Type4HobData->SmbiosType4Processor.Hdr.Type = SMBIOS_TYPE_PROCESSOR_INFORMATION;
  Type4HobData->SmbiosType4Processor.Hdr.Length = sizeof(SMBIOS_TABLE_TYPE4);
  Type4HobData->SmbiosType4Processor.Hdr.Handle = 0;
  Type4HobData->EndingZero = 0;
  Status = mSmbios->Add (mSmbios, NULL, &Processor, &Type4HobData->SmbiosType4Processor.Hdr);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "Fail to add SMBIOS Type 4\n"));
    return Status;
  }
  DEBUG ((DEBUG_INFO, "SMBIOS Type 4 was added. SMBIOS Handle: 0x%x\n", Processor));
#if RISCV_SMBIOS_DEBUG_INFO
  DEBUG ((DEBUG_INFO, "     Socket StringID: %d\n", Type4HobData->SmbiosType4Processor.Socket));
  DEBUG ((DEBUG_INFO, "     Processor Type: 0x%x\n", Type4HobData->SmbiosType4Processor.ProcessorType));
  DEBUG ((DEBUG_INFO, "     Processor Family: 0x%x\n", Type4HobData->SmbiosType4Processor.ProcessorFamily));
  DEBUG ((DEBUG_INFO, "     Processor Manufacture StringID: %d\n", Type4HobData->SmbiosType4Processor.ProcessorManufacture));
  DEBUG ((DEBUG_INFO, "     Processor Id: 0x%x:0x%x\n", \
          Type4HobData->SmbiosType4Processor.ProcessorId.Signature, Type4HobData->SmbiosType4Processor.ProcessorId.FeatureFlags));
  DEBUG ((DEBUG_INFO, "     Processor Version StringID: %d\n", Type4HobData->SmbiosType4Processor.ProcessorVersion));
  DEBUG ((DEBUG_INFO, "     Voltage: 0x%x\n", Type4HobData->SmbiosType4Processor.Voltage));
  DEBUG ((DEBUG_INFO, "     External Clock: 0x%x\n", Type4HobData->SmbiosType4Processor.ExternalClock));
  DEBUG ((DEBUG_INFO, "     Max Speed: 0x%x\n", Type4HobData->SmbiosType4Processor.MaxSpeed));
  DEBUG ((DEBUG_INFO, "     Current Speed: 0x%x\n", Type4HobData->SmbiosType4Processor.CurrentSpeed));
  DEBUG ((DEBUG_INFO, "     Status: 0x%x\n", Type4HobData->SmbiosType4Processor.Status));
  DEBUG ((DEBUG_INFO, "     ProcessorUpgrade: 0x%x\n", Type4HobData->SmbiosType4Processor.ProcessorUpgrade));
  DEBUG ((DEBUG_INFO, "     L1 Cache Handle: 0x%x\n", Type4HobData->SmbiosType4Processor.L1CacheHandle));
  DEBUG ((DEBUG_INFO, "     L2 Cache Handle: 0x%x\n",Type4HobData->SmbiosType4Processor.L2CacheHandle));
  DEBUG ((DEBUG_INFO, "     L3 Cache Handle: 0x%x\n", Type4HobData->SmbiosType4Processor.L3CacheHandle));
  DEBUG ((DEBUG_INFO, "     Serial Number StringID: %d\n", Type4HobData->SmbiosType4Processor.SerialNumber));
  DEBUG ((DEBUG_INFO, "     Asset Tag StringID: %d\n", Type4HobData->SmbiosType4Processor.AssetTag));
  DEBUG ((DEBUG_INFO, "     Part Number StringID: %d\n", Type4HobData->SmbiosType4Processor.PartNumber));
  DEBUG ((DEBUG_INFO, "     Core Count: %d\n", Type4HobData->SmbiosType4Processor.CoreCount));
  DEBUG ((DEBUG_INFO, "     Enabled CoreCount: %d\n", Type4HobData->SmbiosType4Processor.EnabledCoreCount));
  DEBUG ((DEBUG_INFO, "     Thread Count: %d\n", Type4HobData->SmbiosType4Processor.ThreadCount));
  DEBUG ((DEBUG_INFO, "     Processor Characteristics: 0x%x\n", Type4HobData->SmbiosType4Processor.ProcessorCharacteristics));
  DEBUG ((DEBUG_INFO, "     Processor Family2: 0x%x\n", Type4HobData->SmbiosType4Processor.ProcessorFamily2));
  DEBUG ((DEBUG_INFO, "     Core Count 2: %d\n", Type4HobData->SmbiosType4Processor.CoreCount2));
  DEBUG ((DEBUG_INFO, "     Enabled CoreCount : %d\n", Type4HobData->SmbiosType4Processor.EnabledCoreCount2));
  DEBUG ((DEBUG_INFO, "     Thread Count 2: %d\n", Type4HobData->SmbiosType4Processor.ThreadCount2));
#endif

  *SmbiosHandle = Processor;
  return EFI_SUCCESS;
}

/**
  This function builds SMBIOS type 44 record according..

  @param Type4HobData      Pointer to RISC_V_PROCESSOR_TYPE4_HOB_DATA
  @param Type4Handle       SMBIOS handle of type 4

  @retval EFI_STATUS

**/
EFI_STATUS
BuildSmbiosType44 (
  IN RISC_V_PROCESSOR_TYPE4_HOB_DATA *Type4HobData,
  IN SMBIOS_HANDLE Type4Handle
  )
{
  EFI_HOB_GUID_TYPE *GuidHob;
  RISC_V_PROCESSOR_SPECIFIC_HOB_DATA *ProcessorSpecificData;
  SMBIOS_HANDLE RiscVType44;
  SMBIOS_TABLE_TYPE44 *Type44Ptr;
  EFI_STATUS Status;

  DEBUG ((DEBUG_INFO, "Building Type 44 for...\n"));
#if RISCV_SMBIOS_DEBUG_INFO
  DEBUG ((DEBUG_INFO, "     Processor GUID: %g\n", &Type4HobData->PrcessorGuid));
  DEBUG ((DEBUG_INFO, "     Processor UUID: %d\n", Type4HobData->ProcessorUid));
#endif

  GuidHob = (EFI_HOB_GUID_TYPE *)GetFirstGuidHob ((EFI_GUID *)PcdGetPtr(PcdProcessorSpecificDataGuidHobGuid));
  if (GuidHob == NULL) {
    DEBUG ((DEBUG_ERROR, "No RISC_V_PROCESSOR_SPECIFIC_HOB_DATA found.\n"));
    return EFI_NOT_FOUND;
  }
  //
  // Go through each RISC_V_PROCESSOR_SPECIFIC_HOB_DATA for multiple cores.
  //
  do {
    ProcessorSpecificData = (RISC_V_PROCESSOR_SPECIFIC_HOB_DATA *)GET_GUID_HOB_DATA (GuidHob);
    if (!CompareGuid (&ProcessorSpecificData->ParentPrcessorGuid, &Type4HobData->PrcessorGuid) ||
      ProcessorSpecificData->ParentProcessorUid != Type4HobData->ProcessorUid) {
      GuidHob = GetNextGuidHob((EFI_GUID *)PcdGetPtr(PcdProcessorSpecificDataGuidHobGuid), GET_NEXT_HOB(GuidHob));
      if (GuidHob == NULL) {
        break;
      }
      continue;
    }

#if RISCV_SMBIOS_DEBUG_INFO
    DEBUG ((DEBUG_INFO, "================================\n"));
    DEBUG ((DEBUG_INFO, "Core GUID: %g\n", &ProcessorSpecificData->CoreGuid));
#endif

    Type44Ptr = AllocateZeroPool(sizeof(SMBIOS_TABLE_TYPE44) + sizeof(SMBIOS_RISC_V_PROCESSOR_SPECIFIC_DATA) + 2); // Two ending zero.
    if (Type44Ptr == NULL) {
      return EFI_NOT_FOUND;
    }
    Type44Ptr->Hdr.Type = SMBIOS_TYPE_PROCESSOR_ADDITIONAL_INFORMATION;
    Type44Ptr->Hdr.Handle = 0;
    Type44Ptr->Hdr.Length = sizeof(SMBIOS_TABLE_TYPE44) + sizeof(SMBIOS_RISC_V_PROCESSOR_SPECIFIC_DATA);
    Type44Ptr->RefHandle = Type4Handle;
    Type44Ptr->ProcessorSpecificBlock.Length = sizeof(SMBIOS_RISC_V_PROCESSOR_SPECIFIC_DATA);
    Type44Ptr->ProcessorSpecificBlock.ProcessorArchType = Type4HobData->SmbiosType4Processor.ProcessorFamily2 -
                                                          ProcessorFamilyRiscvRV32 + \
                                                          ProcessorSpecificBlockArchTypeRiscVRV32;
    CopyMem ((VOID *)(Type44Ptr + 1), (VOID *)&ProcessorSpecificData->ProcessorSpecificData, sizeof (SMBIOS_RISC_V_PROCESSOR_SPECIFIC_DATA));

#if RISCV_SMBIOS_DEBUG_INFO
    DEBUG ((DEBUG_INFO, "Core type: %d\n", Type44Ptr->ProcessorSpecificBlock.ProcessorArchType));
    DEBUG ((DEBUG_INFO, "     HartId = 0x%x\n", ((SMBIOS_RISC_V_PROCESSOR_SPECIFIC_DATA *)(Type44Ptr + 1))->HartId.Value64_L));
    DEBUG ((DEBUG_INFO, "     Is Boot Hart? = 0x%x\n", ((SMBIOS_RISC_V_PROCESSOR_SPECIFIC_DATA *)(Type44Ptr + 1))->BootHartId));
    DEBUG ((DEBUG_INFO, "     PrivilegeModeSupported = 0x%x\n", ((SMBIOS_RISC_V_PROCESSOR_SPECIFIC_DATA *)(Type44Ptr + 1))->PrivilegeModeSupported));
    DEBUG ((DEBUG_INFO, "     MModeExcepDelegation = 0x%x\n", ((SMBIOS_RISC_V_PROCESSOR_SPECIFIC_DATA *)(Type44Ptr + 1))->MModeExcepDelegation.Value64_L));
    DEBUG ((DEBUG_INFO, "     MModeInterruptDelegation = 0x%x\n", ((SMBIOS_RISC_V_PROCESSOR_SPECIFIC_DATA *)(Type44Ptr + 1))->MModeInterruptDelegation.Value64_L));
    DEBUG ((DEBUG_INFO, "     HartXlen = 0x%x\n", ((SMBIOS_RISC_V_PROCESSOR_SPECIFIC_DATA *)(Type44Ptr + 1))->HartXlen));
    DEBUG ((DEBUG_INFO, "     MachineModeXlen = 0x%x\n", ((SMBIOS_RISC_V_PROCESSOR_SPECIFIC_DATA *)(Type44Ptr + 1))->MachineModeXlen));
    DEBUG ((DEBUG_INFO, "     SupervisorModeXlen = 0x%x\n", ((SMBIOS_RISC_V_PROCESSOR_SPECIFIC_DATA *)(Type44Ptr + 1))->SupervisorModeXlen));
    DEBUG ((DEBUG_INFO, "     UserModeXlen = 0x%x\n", ((SMBIOS_RISC_V_PROCESSOR_SPECIFIC_DATA *)(Type44Ptr + 1))->UserModeXlen));
    DEBUG ((DEBUG_INFO, "     InstSetSupported = 0x%x\n", ((SMBIOS_RISC_V_PROCESSOR_SPECIFIC_DATA *)(Type44Ptr + 1))->InstSetSupported));
    DEBUG ((DEBUG_INFO, "     MachineVendorId = 0x%x\n", ((SMBIOS_RISC_V_PROCESSOR_SPECIFIC_DATA *)(Type44Ptr + 1))->MachineVendorId.Value64_L));
    DEBUG ((DEBUG_INFO, "     MachineArchId = 0x%x\n", ((SMBIOS_RISC_V_PROCESSOR_SPECIFIC_DATA *)(Type44Ptr + 1))->MachineArchId.Value64_L));
    DEBUG ((DEBUG_INFO, "     MachineImplId = 0x%x\n", ((SMBIOS_RISC_V_PROCESSOR_SPECIFIC_DATA *)(Type44Ptr + 1))->MachineImplId.Value64_L));
#endif

    //
    // Add to SMBIOS table.
    //
    RiscVType44 = SMBIOS_HANDLE_PI_RESERVED;
    Status = mSmbios->Add (mSmbios, NULL, &RiscVType44, &Type44Ptr->Hdr);
    if (EFI_ERROR(Status)) {
      DEBUG ((DEBUG_ERROR, "Fail to add SMBIOS Type 44\n"));
      return Status;
    }
    DEBUG ((DEBUG_INFO, "SMBIOS Type 44 was added. SMBIOS Handle: 0x%x\n", RiscVType44));

    GuidHob = GetNextGuidHob((EFI_GUID *)PcdGetPtr(PcdProcessorSpecificDataGuidHobGuid), GET_NEXT_HOB(GuidHob));
  } while (GuidHob != NULL);
  return EFI_SUCCESS;
}

/**
  Entry point of RISC-V SMBIOS builder.

  @param ImageHandle     Image handle this driver.
  @param SystemTable     Pointer to the System Table.

  @retval EFI_SUCCESS           Thread can be successfully created
  @retval EFI_OUT_OF_RESOURCES  Cannot allocate protocol data structure
  @retval EFI_DEVICE_ERROR      Cannot create the thread

**/
EFI_STATUS
EFIAPI
RiscVSmbiosBuilderEntry (
  IN EFI_HANDLE                            ImageHandle,
  IN EFI_SYSTEM_TABLE                      *SystemTable
  )
{
  EFI_STATUS Status;
  EFI_HOB_GUID_TYPE *GuidHob;
  RISC_V_PROCESSOR_TYPE4_HOB_DATA *Type4HobData;
  SMBIOS_HANDLE Processor;

  DEBUG ((DEBUG_INFO, "%a: entry\n", __FUNCTION__));

  Status = gBS->LocateProtocol (
                  &gEfiSmbiosProtocolGuid,
                  NULL,
                  (VOID **)&mSmbios
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Locate SMBIOS Protocol fail\n"));
    return Status;
  }
  GuidHob = (EFI_HOB_GUID_TYPE *)GetFirstGuidHob ((EFI_GUID *)PcdGetPtr(PcdProcessorSmbiosType4GuidHobGuid));
  if (GuidHob == NULL) {
    DEBUG ((DEBUG_ERROR, "No RISC-V SMBIOS information found.\n"));
    return EFI_NOT_FOUND;
  }
  Type4HobData = (RISC_V_PROCESSOR_TYPE4_HOB_DATA *)GET_GUID_HOB_DATA (GuidHob);
  Status = EFI_NOT_FOUND;
  //
  // Go through each RISC_V_PROCESSOR_TYPE4_HOB_DATA for multiple processors.
  //
  do {
    Status = BuildSmbiosType4 (Type4HobData, &Processor);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "No RISC-V SMBIOS type 4 created.\n"));
      ASSERT (FALSE);
    }
    Status = BuildSmbiosType44 (Type4HobData, Processor);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "No RISC-V SMBIOS type 44 found.\n"));
      ASSERT (FALSE);
    }

    GuidHob = GetNextGuidHob((EFI_GUID *)PcdGetPtr(PcdProcessorSmbiosType4GuidHobGuid), GET_NEXT_HOB(GuidHob));
  } while (GuidHob != NULL);
  DEBUG ((DEBUG_INFO, "%a: exit\n", __FUNCTION__));
  return Status;
}

