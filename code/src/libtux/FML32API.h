/**
 * Copyright 2012 Wangxr, vintage.wang@gmail.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef TX_FML32_API_H__
#define TX_FML32_API_H__
#include <fml32.h>
#include <stdlib.h>
#include <lwpr.h>
#include "FML32API.h"

//
// |------------|------------|------------|-----------------------------------
// | MAGIC CODE | BODY SIZE  | BODY OFFSET|           BODY DATA
// |------------|------------|------------|-----------------------------------
//

#define FML32_HEADER_SIZE			(sizeof(LWPR::UINT32) * 3)
#define FML32_HEADER_MAGICODE		(10504816 ^ 10225799)

void FMLHeaderToLocal(FBFR32 *fbfr);
void FMLHeaderToNet(FBFR32 *fbfr);
FLDLEN32 FMLValidSize(FBFR32 *fbfr);
void FMLPickHeader(FBFR32 *fbfr, LWPR::UINT32& code, LWPR::UINT32& size, LWPR::UINT32& offset);

/**
 * 向FML32 内存缓冲区中写入数据，需要转化字节序
 */
void FMLWriteUINT8(char* buf, FLDID32 id, LWPR::UINT8 value);
void FMLWriteUINT16(char* buf, FLDID32 id, LWPR::UINT16 value);
void FMLWriteUINT32(char* buf, FLDID32 id, LWPR::UINT32 value);
void FMLWriteUINT64(char* buf, FLDID32 id, LWPR::UINT64 value);
void FMLWriteFP32(char* buf, FLDID32 id, LWPR::FP32 value);
void FMLWriteFP64(char* buf, FLDID32 id, LWPR::FP64 value);

/**
 * 从FML32 内存缓冲区读出数据，需要转化字节序
 */
FLDOCC32 FMLFieldNextOffset(char* buf);

#endif // end of TX_FML32_API_H__
