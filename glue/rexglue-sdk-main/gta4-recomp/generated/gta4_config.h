#pragma once
#ifndef PPC_CONFIG_H_INCLUDED
#define PPC_CONFIG_H_INCLUDED


#define PPC_IMAGE_BASE 0x82000000ull
#define PPC_IMAGE_SIZE 0x1300000ull
#define PPC_CODE_BASE 0x82140000ull
#define PPC_CODE_SIZE 0x94635Cull

#define REXCRT_HEAP 1

#include <rex/ppc/image_info.h>
extern const rex::PPCImageInfo PPCImageConfig;

#endif
