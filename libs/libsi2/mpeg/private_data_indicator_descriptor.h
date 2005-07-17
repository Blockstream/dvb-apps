/*
 * section and descriptor parser
 *
 * Copyright (C) 2005 Kenneth Aafloy (kenneth@linuxtv.org)
 * Copyright (C) 2005 Andrew de Quincey (adq_dvb@lidskialf.net)
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

#ifndef _SI_MPEG_PRIVATE_DATA_INDICATOR_DESCRIPTOR
#define _SI_MPEG_PRIVATE_DATA_INDICATOR_DESCRIPTOR 1

#include <si/descriptor.h>
#include <si/common.h>

struct mpeg_private_data_indicator_descriptor {
	struct descriptor d;

	uint32_t private_data_indicator;
} packed;

static inline struct mpeg_private_data_indicator_descriptor*
	mpeg_private_data_indicator_descriptor_parse(struct descriptor* d)
{
	if (d->len != (sizeof(struct mpeg_private_data_indicator_descriptor) - 2))
		return NULL;

	bswap32((uint8_t*) d + 2);

	return (struct mpeg_private_data_indicator_descriptor*) d;
}

#endif
