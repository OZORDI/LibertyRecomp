/* PS4 linker script supplement — EH frame symbols + SCE sections */
SECTIONS
{
    .eh_frame_hdr : {
        PROVIDE(__eh_frame_hdr_start = .);
        *(.eh_frame_hdr)
        PROVIDE(__eh_frame_hdr_end = .);
    }
    .eh_frame : {
        PROVIDE(__eh_frame_start = .);
        *(.eh_frame)
        PROVIDE(__eh_frame_end = .);
    }
}
INSERT AFTER .rodata;

/* Preserve PS4 SCE process/module param as a distinct section.
   create-fself looks up .data.sce_process_param by name. */
SECTIONS
{
    .data.sce_process_param : {
        *(.data.sce_process_param)
        *(SORT(.data.rel.ro._sceLibcParam))
        *(SORT(.data.rel.ro._sceKernelMemParam))
        *(SORT(.data.rel.ro._sceKernelFsParam))
        *(SORT(.data.rel.ro._sceLibcMallocReplace))
        *(SORT(.data.rel.ro._sceLibcNewReplace))
        *(SORT(.data.rel.ro._sceLibcMallocReplaceForTls))
    }
}
INSERT BEFORE .data;
