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
#include <fml32.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <lwpr.h>
#include <arpa/inet.h>
#include "TLSLibtux.h"
#include "FML32API.h"


extern "C" int* LIBTUX_EXPORT _Fget_Ferror_addr32(void)
{
	return TLSLibtux::GetInstance()->GetFerrno();
}

extern "C" char* LIBTUX_EXPORT Fstrerror32(int err)
{
	switch(err)
	{
	case FALIGNERR:
		return "fielded buffer not aligned";
	case FNOTFLD:
		return "buffer not fielded";
	case FNOSPACE:
		return "no space in fielded buffer";
	case FNOTPRES:
		return "field not present";
	case FBADFLD:
		return "unknown field number or type";
	case FTYPERR:
		return "illegal field type";
	case FEUNIX:
		return "unix system call error";
	case FBADNAME:
		return "unknown field name";
	case FMALLOC:
		return "malloc failed";
	case FSYNTAX:
		return "bad syntax in boolean expression";
	case FFTOPEN:
		return "cannot find or open field table";
	case FFTSYNTAX:
		return "syntax error in field table";
	case FEINVAL:
		return "invalid argument to function";
	case FBADTBL:
		return "destructive concurrent access to field table";
	case FBADVIEW:
		return "cannot find or get view";
	case FVFSYNTAX:
		return "bad viewfile";
	case FVFOPEN:
		return "cannot find or open viewfile";
	case FBADACM:
		return "ACM contains negative value";
	case FNOCNAME:
		return "cname not found";
	case FEBADOP:
		return "operation invalid for field type";
	default:
		return NULL;
	}

	return NULL;
}

extern "C" long LIBTUX_EXPORT Fneeded32(FLDOCC32 F, FLDLEN32 V)
{
	if(F < 0)
	{
		Ferror32 = FEINVAL;
		return -1;
	}

	return FML32_HEADER_SIZE + sizeof(LWPR::UINT32) * 2 * F + V;
}

extern "C" int LIBTUX_EXPORT Fadd32(FBFR32 *fbfr, FLDID32 fieldid, char *value, FLDLEN32 len)
{
	if(NULL == fbfr
	   || NULL == value)
	{
		// "invalid argument to function"
		// One of the arguments to the function invoked was invalid. (For example, specifying a NULL value parameter to Fadd().)
		Ferror32 = FEINVAL;
		return -1;
	}

	LWPR::UINT32 magic_code = 0;
	LWPR::UINT32 body_size = 0;
	LWPR::UINT32 body_offset = 0;

	FMLPickHeader(fbfr, magic_code, body_size, body_offset);

	LWPR::UINT32 old_body_offset = body_offset;

	if(FML32_HEADER_MAGICODE != magic_code)
	{
		// "buffer not fielded"
		// The buffer is not a fielded buffer or has not been initialized by Finit().
		Ferror32 = FNOTFLD;
		return -1;
	}

	switch(Fldtype32(fieldid))
	{
	case FLD_SHORT:
		{
			LWPR::UINT16 x = (LWPR::UINT16)(*(short *)value);
			body_offset += sizeof(FLDID32) + sizeof(LWPR::UINT16);

			if(body_offset > body_size)
			{
				goto ERROR_FNOSPACE;
			}

			FMLWriteUINT16((char *)fbfr + FML32_HEADER_SIZE + old_body_offset, fieldid, x);
		}
		break;
	case FLD_LONG:
		{
			LWPR::UINT64 x = (LWPR::UINT64)(*(long *)value);
			body_offset += sizeof(FLDID32) + sizeof(LWPR::UINT64);

			if(body_offset > body_size)
			{
				goto ERROR_FNOSPACE;
			}

			FMLWriteUINT64((char *)fbfr + FML32_HEADER_SIZE + old_body_offset, fieldid, x);
		}
		break;
	case FLD_CHAR:
		{
			LWPR::UINT8 x = (LWPR::UINT8)(*(char *)value);
			body_offset += sizeof(FLDID32) + sizeof(LWPR::UINT8);

			if(body_offset > body_size)
			{
				goto ERROR_FNOSPACE;
			}

			FMLWriteUINT8((char *)fbfr + FML32_HEADER_SIZE + old_body_offset, fieldid, x);
		}
		break;
	case FLD_FLOAT:
		{
			LWPR::FP32 x = (LWPR::FP32)(*(float *)value);
			body_offset += sizeof(FLDID32) + sizeof(LWPR::FP32);

			if(body_offset > body_size)
			{
				goto ERROR_FNOSPACE;
			}

			FMLWriteFP32((char *)fbfr + FML32_HEADER_SIZE + old_body_offset, fieldid, x);
		}
		break;
	case FLD_DOUBLE:
		{
			LWPR::FP64 x = (LWPR::FP64)(*(double *)value);
			body_offset += sizeof(FLDID32) + sizeof(LWPR::FP64);

			if(body_offset > body_size)
			{
				goto ERROR_FNOSPACE;
			}

			FMLWriteFP64((char *)fbfr + FML32_HEADER_SIZE + old_body_offset, fieldid, x);
		}
		break;
	case FLD_STRING:
		{
			LWPR::UINT32 x = strlen(value);
			body_offset += sizeof(FLDID32) + sizeof(LWPR::UINT32) + x;

			if(body_offset > body_size)
			{
				goto ERROR_FNOSPACE;
			}

			FMLWriteUINT32((char *)fbfr + FML32_HEADER_SIZE + old_body_offset, fieldid, x);
			memcpy((char *)fbfr + FML32_HEADER_SIZE + old_body_offset + sizeof(FLDID32) + sizeof(LWPR::UINT32), value, x);
		}
		break;
	case FLD_CARRAY:
		{
			body_offset += sizeof(FLDID32) + sizeof(LWPR::UINT32) + len;

			if(body_offset > body_size)
			{
				goto ERROR_FNOSPACE;
			}

			FMLWriteUINT32((char *)fbfr + FML32_HEADER_SIZE + old_body_offset, fieldid, len);
			memcpy((char *)fbfr + FML32_HEADER_SIZE + old_body_offset + sizeof(FLDID32) + sizeof(LWPR::UINT32), value, len);
		}
		break;
	default:
		// "unknown field number or type"
		// A field identifier is specified which is not valid.
		Ferror32 = FBADFLD;
		return -1;
	}

	// 更新偏移量
	memcpy((char *)fbfr + sizeof(LWPR::UINT32) * 2, &body_offset, sizeof(LWPR::UINT32));
	return 0;

ERROR_FNOSPACE:
	// "no space in fielded buffer"
	// A field value is to be added or changed in a field buffer, but there is not enough space remaining in the buffer.
	Ferror32 = FNOSPACE;
	return -1;
}

/**
 * 1、oc == -1，相当于Fadd32(fbfr, fieldid, value, len)
 * 2、If the specified field occurrence is found, then the field value is modified to the value specified.
 * 3、If a field occurrence is specified that does not exist, then NULL values are added for the missing occurrences
 * 4、value == null，delete
 */
extern "C" int LIBTUX_EXPORT Fchg32(FBFR32 *fbfr, FLDID32 fieldid, FLDOCC32 oc, char *value, FLDLEN32 len)
{
	if(oc < 0)
	{
		return Fadd32(fbfr, fieldid, value, len);
	}

	if(NULL == fbfr)
	{
		// "invalid argument to function"
		// One of the arguments to the function invoked was invalid. (For example, specifying a NULL value parameter to Fadd().)
		Ferror32 = FEINVAL;
		return -1;
	}

	LWPR::UINT32 magic_code = 0;
	LWPR::UINT32 body_size = 0;
	LWPR::UINT32 body_offset = 0;

	FMLPickHeader(fbfr, magic_code, body_size, body_offset);

	if(FML32_HEADER_MAGICODE != magic_code)
	{
		// "buffer not fielded"
		// The buffer is not a fielded buffer or has not been initialized by Finit().
		Ferror32 = FNOTFLD;
		return -1;
	}

	// 遍历Buffer
	char* pBuffer = (char *)fbfr + FML32_HEADER_SIZE;
	FLDOCC32 times = 0;
	for(LWPR::UINT32 i = 0; i < body_offset;)
	{
		FLDID32 id = 0;
		memcpy(&id, pBuffer + i, sizeof(FLDID32));
		id = ntohl(id);

		if(fieldid != id
		   || times++ != oc)
		{
			FLDOCC32 offset = FMLFieldNextOffset(pBuffer + i);
			if(-1 == offset) return -1;
			i += offset;
		}
		else
		{
			switch(Fldtype32(id))
			{
			case FLD_SHORT:
				{
					// update
					if(value)
					{
						LWPR::UINT16 x = (LWPR::UINT16)(*(short *)value);

						FMLWriteUINT16(pBuffer + i, fieldid, x);
					}
					// delete
					else
					{
						LWPR::UINT32 size = body_offset - (i + sizeof(FLDID32) + sizeof(LWPR::UINT16));
						memmove(pBuffer + i, pBuffer + i + sizeof(FLDID32) + sizeof(LWPR::UINT16), size);
						body_offset -= size;
					}
				}
				goto OK_END;
			case FLD_LONG:
				{
					// update
					if(value)
					{
						LWPR::UINT64 x = (LWPR::UINT64)(*(long *)value);

						FMLWriteUINT64(pBuffer + i, fieldid, x);
					}
					// delete
					else
					{
						LWPR::UINT32 size = body_offset - (i + sizeof(FLDID32) + sizeof(LWPR::UINT64));
						memmove(pBuffer + i, pBuffer + i + sizeof(FLDID32) + sizeof(LWPR::UINT64), size);
						body_offset -= size;
					}
				}
				goto OK_END;
			case FLD_CHAR:
				{
					// update
					if(value)
					{
						LWPR::UINT8 x = (LWPR::UINT8)(*(char *)value);

						FMLWriteUINT8(pBuffer + i, fieldid, x);
					}
					// delete
					else
					{
						LWPR::UINT32 size = body_offset - (i + sizeof(FLDID32) + sizeof(LWPR::UINT8));
						memmove(pBuffer + i, pBuffer + i + sizeof(FLDID32) + sizeof(LWPR::UINT8), size);
						body_offset -= size;
					}
				}
				goto OK_END;
			case FLD_FLOAT:
				{
					// update
					if(value)
					{
						LWPR::FP32 x = (LWPR::FP32)(*(float *)value);

						FMLWriteFP32(pBuffer + i, fieldid, x);
					}
					// delete
					else
					{
						LWPR::UINT32 size = body_offset - (i + sizeof(FLDID32) + sizeof(LWPR::FP32));
						memmove(pBuffer + i, pBuffer + i + sizeof(FLDID32) + sizeof(LWPR::FP32), size);
						body_offset -= size;
					}
				}
				goto OK_END;
			case FLD_DOUBLE:
				{
					// update
					if(value)
					{
						LWPR::FP64 x = (LWPR::FP64)(*(double *)value);

						FMLWriteFP64(pBuffer + i, fieldid, x);
					}
					// delete
					else
					{
						LWPR::UINT32 size = body_offset - (i + sizeof(FLDID32) + sizeof(LWPR::FP64));
						memmove(pBuffer + i, pBuffer + i + sizeof(FLDID32) + sizeof(LWPR::FP64), size);
						body_offset -= size;
					}
				}
				goto OK_END;
			case FLD_STRING:
				{
					LWPR::UINT32 x = 0;
					memcpy(&x, pBuffer + i + sizeof(FLDID32), sizeof(LWPR::UINT32));
					FLDLEN32 lenstr = ntohl(x);
					LWPR::UINT32 section = sizeof(FLDID32) + sizeof(LWPR::UINT32) + lenstr;
					LWPR::UINT32 size = body_offset - (i + section);

					// update
					if(value)
					{
						LWPR::UINT32 y = strlen(value);
						y = y > len ? len : y;

						body_offset += y - lenstr;

						if((i + section + y - lenstr) > body_size)
						{
							goto ERROR_FNOSPACE;
						}

						memmove(pBuffer + i + sizeof(FLDID32) + sizeof(LWPR::UINT32) + y,
						        pBuffer + i + section, size);
					}
					// delete
					else
					{
						memmove(pBuffer + i, pBuffer + i + section, size);
						body_offset -= size;
					}
				}
				goto OK_END;
			case FLD_CARRAY:
				{
					LWPR::UINT32 x = 0;
					memcpy(&x, pBuffer + i + sizeof(FLDID32), sizeof(LWPR::UINT32));
					FLDLEN32 lenstr = ntohl(x);
					LWPR::UINT32 section = sizeof(FLDID32) + sizeof(LWPR::UINT32) + lenstr;
					LWPR::UINT32 size = body_offset - (i + section);

					// update
					if(value)
					{
						LWPR::UINT32 y = len;

						body_offset += y - lenstr;

						if((i + section + y - lenstr) > body_size)
						{
							goto ERROR_FNOSPACE;
						}

						memmove(pBuffer + i + sizeof(FLDID32) + sizeof(LWPR::UINT32) + y,
						        pBuffer + i + section, size);
					}
					// delete
					else
					{
						memmove(pBuffer + i, pBuffer + i + section, size);
						body_offset -= size;
					}
				}
				goto OK_END;
			default:
				// "unknown field number or type"
				// A field identifier is specified which is not valid.
				Ferror32 = FBADFLD;
				return -1;
			}
		}
	}

	// "field not present"
	// A field occurrence is requested but the specified field and/or occurrence was not found in the fielded buffer.
	Ferror32 = FNOTPRES;
	return -1;

ERROR_FNOSPACE:
	// "no space in fielded buffer"
	// A field value is to be added or changed in a field buffer, but there is not enough space remaining in the buffer.
	Ferror32 = FNOSPACE;
	return -1;
OK_END:
	// 更新偏移量
	memcpy((char *)fbfr + sizeof(LWPR::UINT32) * 2, &body_offset, sizeof(LWPR::UINT32));
	return 0;
}

extern "C" int LIBTUX_EXPORT Fget32(FBFR32 *fbfr, FLDID32 fieldid, FLDOCC32 oc, char *loc, FLDLEN32 *maxlen)
{
	if(NULL == fbfr)
	{
		// "invalid argument to function"
		// One of the arguments to the function invoked was invalid. (For example, specifying a NULL value parameter to Fadd().)
		Ferror32 = FEINVAL;
		return -1;
	}

	LWPR::UINT32 magic_code = 0;
	LWPR::UINT32 body_size = 0;
	LWPR::UINT32 body_offset = 0;

	FMLPickHeader(fbfr, magic_code, body_size, body_offset);

	if(FML32_HEADER_MAGICODE != magic_code)
	{
		// "buffer not fielded"
		// The buffer is not a fielded buffer or has not been initialized by Finit().
		Ferror32 = FNOTFLD;
		return -1;
	}

	// 遍历Buffer
	char* pBuffer = (char *)fbfr + FML32_HEADER_SIZE;
	FLDOCC32 times = 0;
	for(LWPR::UINT32 i = 0; i < body_offset;)
	{
		FLDID32 id = 0;
		memcpy(&id, pBuffer + i, sizeof(FLDID32));
		id = ntohl(id);

		if(fieldid != id
		   || times++ != oc)
		{
			FLDOCC32 offset = FMLFieldNextOffset(pBuffer + i);
			if(-1 == offset) return -1;
			i += offset;
		}
		else
		{
			switch(Fldtype32(id))
			{
			case FLD_SHORT:
				{
					if(loc)
					{
						LWPR::UINT16 x = 0;
						memcpy(&x, pBuffer + i + sizeof(FLDID32), sizeof(LWPR::UINT16));
						short y = ntohs(x);
						memcpy(loc, &y, sizeof(short));
					}
				}
				return 0;
			case FLD_LONG:
				{
					if(loc)
					{
						LWPR::UINT64 x = 0;
						memcpy(&x, pBuffer + i + sizeof(FLDID32), sizeof(LWPR::UINT64));
						long y = LWPR::Utility::ntohl64(x);
						memcpy(loc, &y, sizeof(long));
					}
				}
				return 0;
			case FLD_CHAR:
				{
					if(loc)
					{
						LWPR::UINT8 y = *(LWPR::UINT8*)(pBuffer + i + sizeof(FLDID32));
						memcpy(loc, &y, sizeof(char));
					}
				}
				return 0;
			case FLD_FLOAT:
				{
					if(loc)
					{
						LWPR::FP32 y = 0.0;
						memcpy(&y, pBuffer + i + sizeof(FLDID32), sizeof(LWPR::FP32));
						memcpy(loc, &y, sizeof(float));
					}
				}
				return 0;
			case FLD_DOUBLE:
				{
					if(loc)
					{
						LWPR::FP64 y = 0;
						memcpy(&y, pBuffer + i + sizeof(FLDID32), sizeof(LWPR::FP64));
						memcpy(loc, &y, sizeof(double));
					}
				}
				return 0;
			case FLD_STRING:
				{
					if(loc)
					{
						LWPR::UINT32 x = 0;
						memcpy(&x, pBuffer + i + sizeof(FLDID32), sizeof(LWPR::UINT32));
						FLDLEN32 len = ntohl(x);
						len  = len < *maxlen ? len : *maxlen;
						*maxlen = len;
						memcpy(loc, pBuffer + i + sizeof(FLDID32) + sizeof(LWPR::UINT32), len);
						*(loc + len) = 0;
					}
				}
				return 0;
			case FLD_CARRAY:
				{
					if(loc)
					{
						LWPR::UINT32 x = 0;
						memcpy(&x, pBuffer + i + sizeof(FLDID32), sizeof(LWPR::UINT32));
						FLDLEN32 len = ntohl(x);
						len  = len < *maxlen ? len : *maxlen;
						*maxlen = len;
						memcpy(loc, pBuffer + i + sizeof(FLDID32) + sizeof(LWPR::UINT32), len);
					}
				}
				return 0;
			default:
				// "unknown field number or type"
				// A field identifier is specified which is not valid.
				Ferror32 = FBADFLD;
				return -1;
			}
		}
	}

	// "field not present"
	// A field occurrence is requested but the specified field and/or occurrence was not found in the fielded buffer.
	Ferror32 = FNOTPRES;
	return -1;
}
extern "C" FLDOCC32 LIBTUX_EXPORT Foccur32(FBFR32 *fbfr, FLDID32 fieldid)
{
	if(NULL == fbfr)
	{
		// "invalid argument to function"
		// One of the arguments to the function invoked was invalid. (For example, specifying a NULL value parameter to Fadd().)
		Ferror32 = FEINVAL;
		return -1;
	}

	LWPR::UINT32 magic_code = 0;
	LWPR::UINT32 body_size = 0;
	LWPR::UINT32 body_offset = 0;

	FMLPickHeader(fbfr, magic_code, body_size, body_offset);

	if(FML32_HEADER_MAGICODE != magic_code)
	{
		// "buffer not fielded"
		// The buffer is not a fielded buffer or has not been initialized by Finit().
		Ferror32 = FNOTFLD;
		return -1;
	}

	// 遍历Buffer
	char* pBuffer = (char *)fbfr + FML32_HEADER_SIZE;
	FLDOCC32 times = 0;
	for(LWPR::UINT32 i = 0; i < body_offset;)
	{
		FLDID32 id = 0;
		memcpy(&id, pBuffer + i, sizeof(FLDID32));
		id = ntohl(id);

		if(fieldid == id) times++;

		i += FMLFieldNextOffset(pBuffer);
	}

	return times;
}

extern "C" int LIBTUX_EXPORT Fprint32(FBFR32 *fbfr)
{
	return 0;
}

extern "C" int LIBTUX_EXPORT Fldtype32(FLDID32 fieldid)
{
	div_t t = div(fieldid, FML32_BASE_FACTOR);

	return t.quot;
}

extern "C" int LIBTUX_EXPORT Finit32(FBFR32 *fbfr, FLDLEN32 buflen)
{
	if(fbfr == NULL)
	{
		Ferror32 = FNOTFLD;
		return -1;
	}
	else if(buflen <= FML32_HEADER_SIZE)
	{
		Ferror32 = FNOSPACE;
		return -1;
	}

	LWPR::UINT32 magic_code = FML32_HEADER_MAGICODE;
	LWPR::UINT32 body_size = buflen - FML32_HEADER_SIZE;
	LWPR::UINT32 body_offset = 0;

	memcpy((char *)fbfr + sizeof(LWPR::UINT32) * 0, &magic_code, sizeof(LWPR::UINT32));
	memcpy((char *)fbfr + sizeof(LWPR::UINT32) * 1, &body_size, sizeof(LWPR::UINT32));
	memcpy((char *)fbfr + sizeof(LWPR::UINT32) * 2, &body_offset, sizeof(LWPR::UINT32));

	return 0;
}

/**
 ********************************************************************************
 * 内部方法，用来执行字节序等操作
 ********************************************************************************
 */
void FMLHeaderToLocal(FBFR32 *fbfr)
{
	assert(NULL != fbfr);

	LWPR::UINT32 magic_code = 0;
	LWPR::UINT32 body_size = 0;
	LWPR::UINT32 body_offset = 0;

	memcpy(&magic_code, (char *)fbfr + sizeof(LWPR::UINT32) * 0, sizeof(LWPR::UINT32));
	memcpy(&body_size, (char *)fbfr + sizeof(LWPR::UINT32) * 1, sizeof(LWPR::UINT32));
	memcpy(&body_offset, (char *)fbfr + sizeof(LWPR::UINT32) * 2, sizeof(LWPR::UINT32));

	magic_code = ntohl(magic_code);
	body_size = ntohl(body_size);
	body_offset = ntohl(body_offset);

	memcpy((char *)fbfr + sizeof(LWPR::UINT32) * 0, &magic_code, sizeof(LWPR::UINT32));
	memcpy((char *)fbfr + sizeof(LWPR::UINT32) * 1, &body_size, sizeof(LWPR::UINT32));
	memcpy((char *)fbfr + sizeof(LWPR::UINT32) * 2, &body_offset, sizeof(LWPR::UINT32));
}

void FMLHeaderToNet(FBFR32 *fbfr)
{
	assert(NULL != fbfr);

	LWPR::UINT32 magic_code = 0;
	LWPR::UINT32 body_size = 0;
	LWPR::UINT32 body_offset = 0;

	memcpy(&magic_code, (char *)fbfr + sizeof(LWPR::UINT32) * 0, sizeof(LWPR::UINT32));
	memcpy(&body_size, (char *)fbfr + sizeof(LWPR::UINT32) * 1, sizeof(LWPR::UINT32));
	memcpy(&body_offset, (char *)fbfr + sizeof(LWPR::UINT32) * 2, sizeof(LWPR::UINT32));

	magic_code = htonl(magic_code);
	body_size = htonl(body_size);
	body_offset = htonl(body_offset);

	body_size = body_offset;

	memcpy((char *)fbfr + sizeof(LWPR::UINT32) * 0, &magic_code, sizeof(LWPR::UINT32));
	memcpy((char *)fbfr + sizeof(LWPR::UINT32) * 1, &body_size, sizeof(LWPR::UINT32));
	memcpy((char *)fbfr + sizeof(LWPR::UINT32) * 2, &body_offset, sizeof(LWPR::UINT32));
}

FLDLEN32 FMLValidSize(FBFR32 *fbfr)
{
	assert(NULL != fbfr);

	LWPR::UINT32 body_offset = 0;

	memcpy(&body_offset, (char *)fbfr + sizeof(LWPR::UINT32) * 2, sizeof(LWPR::UINT32));

	return FML32_HEADER_SIZE + body_offset;
}


void FMLPickHeader(FBFR32 *fbfr, LWPR::UINT32& code, LWPR::UINT32& size, LWPR::UINT32& offset)
{
	assert(NULL != fbfr);

	memcpy(&code, (char *)fbfr + sizeof(LWPR::UINT32) * 0, sizeof(LWPR::UINT32));
	memcpy(&size, (char *)fbfr + sizeof(LWPR::UINT32) * 1, sizeof(LWPR::UINT32));
	memcpy(&offset, (char *)fbfr + sizeof(LWPR::UINT32) * 2, sizeof(LWPR::UINT32));
}

void FMLWriteUINT8(char* buf, FLDID32 id, LWPR::UINT8 value)
{
	assert(NULL != buf);

	id = htonl(id);

	memcpy(buf, &id, sizeof(FLDID32));
	memcpy(buf + sizeof(FLDID32), &value, sizeof(value));
}

void FMLWriteUINT16(char* buf, FLDID32 id, LWPR::UINT16 value)
{
	assert(NULL != buf);

	id = htonl(id);
	value = htons(value);

	memcpy(buf, &id, sizeof(FLDID32));
	memcpy(buf + sizeof(FLDID32), &value, sizeof(value));
}

void FMLWriteUINT32(char* buf, FLDID32 id, LWPR::UINT32 value)
{
	assert(NULL != buf);

	id = htonl(id);
	value = htonl(value);

	memcpy(buf, &id, sizeof(FLDID32));
	memcpy(buf + sizeof(FLDID32), &value, sizeof(value));
}

void FMLWriteUINT64(char* buf, FLDID32 id, LWPR::UINT64 value)
{
	assert(NULL != buf);

	id = htonl(id);
	value = LWPR::Utility::htonl64(value);

	memcpy(buf, &id, sizeof(FLDID32));
	memcpy(buf + sizeof(FLDID32), &value, sizeof(value));
}

void FMLWriteFP32(char* buf, FLDID32 id, LWPR::FP32 value)
{
	assert(NULL != buf);

	id = htonl(id);

	memcpy(buf, &id, sizeof(FLDID32));
	memcpy(buf + sizeof(FLDID32), &value, sizeof(value));
}

void FMLWriteFP64(char* buf, FLDID32 id, LWPR::FP64 value)
{
	assert(NULL != buf);

	id = htonl(id);

	memcpy(buf, &id, sizeof(FLDID32));
	memcpy(buf + sizeof(FLDID32), &value, sizeof(value));
}

FLDOCC32 FMLFieldNextOffset(char* buf)
{
	assert(NULL != buf);

	FLDID32 id = 0;
	memcpy(&id, buf, sizeof(FLDID32));
	id = ntohl(id);

	switch(Fldtype32(id))
	{
	case FLD_SHORT:
		return sizeof(FLDID32) + sizeof(LWPR::UINT16);
	case FLD_LONG:
		return sizeof(FLDID32) + sizeof(LWPR::UINT64);
	case FLD_CHAR:
		return sizeof(FLDID32) + sizeof(LWPR::UINT8);
	case FLD_FLOAT:
		return sizeof(FLDID32) + sizeof(LWPR::FP32);
	case FLD_DOUBLE:
		return sizeof(FLDID32) + sizeof(LWPR::FP64);
	case FLD_STRING:
	case FLD_CARRAY:
		{
			LWPR::UINT32 len = 0;
			memcpy(&len, buf + sizeof(FLDID32), sizeof(LWPR::UINT32));
			len = ntohl(len);
			return sizeof(FLDID32) + sizeof(LWPR::UINT32) + len;
		}
	default:
		// "unknown field number or type"
		// A field identifier is specified which is not valid.
		Ferror32 = FBADFLD;
		return -1;
	}
}
