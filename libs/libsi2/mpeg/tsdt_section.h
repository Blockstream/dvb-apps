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

#ifndef _SI_MPEG_TSDT_SECTION_H
#define _SI_MPEG_TSDT_SECTION_H 1

#include <si/section.h>

struct mpeg_tsdt_section {
	struct section_ext head;

	/* struct descriptor descriptors[] */
} packed;

extern struct mpeg_tsdt_section *mpeg_tsdt_section_parse(struct section_ext *);

#define mpeg_tsdt_section_descriptors_for_each(tsdt, pos) \
	for ((pos) = mpeg_tsdt_section_descriptors_first(tsdt); \
	     (pos); \
	     (pos) = mpeg_tsdt_section_descriptors_next(tsdt, pos))










/******************************** PRIVATE CODE ********************************/
static inline struct descriptor *
	mpeg_tsdt_section_descriptors_first(struct mpeg_tsdt_section * tsdt)
{
	int pos = sizeof(struct mpeg_tsdt_section);

	if (pos >= section_ext_length(&tsdt->head))
		return NULL;

	return (struct descriptor*)((uint8_t *) tsdt + pos);
}

static inline struct descriptor *
	mpeg_tsdt_section_descriptors_next(struct mpeg_tsdt_section *tsdt,
					   struct descriptor* pos)
{
	return next_descriptor((uint8_t *) tsdt + sizeof(struct mpeg_tsdt_section),
			      section_ext_length(&tsdt->head) - sizeof(struct mpeg_tsdt_section),
			      pos);
}

#endif
