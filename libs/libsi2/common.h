/*
 * section and descriptor parser
 *
 * Copyright (C) 2005 Kenneth Aafloy (kenneth@linuxtv.org)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

#ifndef _SI_COMMON_H
#define _SI_COMMON_H 1

#include <stdint.h>
#include <byteswap.h>

static inline void bswap16(uint8_t * buf) {
	*((uint16_t*)buf) = bswap_16((*(uint16_t*)buf));
}

static inline void bswap32(uint8_t * buf) {
	*((uint32_t*)buf) = bswap_32((*(uint32_t*)buf));
}

static inline void bswap64(uint8_t * buf) {
	*((uint64_t*)buf) = bswap_64((*(uint64_t*)buf));
}

static inline void bswap24(uint8_t * buf) {
	uint8_t tmp0 = buf[0];

	buf[0] = buf[2];
	buf[2] = tmp0;
}

static inline void bswap40(uint8_t * buf) {
	uint8_t tmp0 = buf[0];
	uint8_t tmp1 = buf[1];

	buf[0] = buf[4];
	buf[1] = buf[3];
	buf[3] = tmp1;
	buf[4] = tmp0;
}

static inline void bswap48(uint8_t * buf) {
	uint8_t tmp0 = buf[0];
	uint8_t tmp1 = buf[1];
	uint8_t tmp2 = buf[2];

	buf[0] = buf[5];
	buf[1] = buf[4];
	buf[2] = buf[3];
	buf[3] = tmp2;
	buf[4] = tmp1;
	buf[5] = tmp0;
}

#endif
