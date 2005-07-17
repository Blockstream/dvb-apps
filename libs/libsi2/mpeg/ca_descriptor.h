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

#ifndef _SI_MPEG_CA_DESCRIPTOR
#define _SI_MPEG_CA_DESCRIPTOR 1

#include <si/descriptor.h>
#include <si/common.h>

struct mpeg_ca_descriptor {
	struct descriptor d;

	uint16_t ca_system_id;
  EBIT2(uint16_t reserved	: 3;  ,
	uint16_t ca_pid		: 13; );
	/* uint8_t  data[] */
} packed;

static inline struct mpeg_ca_descriptor*
	mpeg_ca_descriptor_parse(struct descriptor* d)
{
	if (d->len < (sizeof(struct mpeg_ca_descriptor) - 2))
		return NULL;

	bswap16((uint8_t*) d + 2);
	bswap16((uint8_t*) d + 4);

	return (struct mpeg_ca_descriptor*) d;
}

static inline uint8_t *
	mpeg_ca_descriptor_data(struct mpeg_ca_descriptor *d)
{
	return (uint8_t *) d + sizeof(struct mpeg_ca_descriptor);
}

static inline int
	mpeg_ca_descriptor_data_length(struct mpeg_ca_descriptor *d)
{
	return d->d.len - 4;
}

#endif
