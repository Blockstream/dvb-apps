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

#ifndef _SI_DVB_MULTILINGUAL_COMPONENT_DESCRIPTOR
#define _SI_DVB_MULTILINGUAL_COMPONENT_DESCRIPTOR 1

#include <si/descriptor.h>
#include <si/common.h>

struct dvb_multilingual_component_descriptor {
	struct descriptor d;

	uint8_t component_tag;
	/* struct dvb_multilingual_component components[] */
} packed;

struct dvb_multilingual_component {
	uint8_t iso_639_language_code[3];
	uint8_t text_description_length;
	/* uint8_t text_char[] */
} packed;

static inline struct dvb_multilingual_component_descriptor*
	dvb_multilingual_component_descriptor_parse(struct descriptor* d)
{
	uint8_t* buf = (uint8_t*) d + 2;
	int pos = sizeof(struct dvb_multilingual_component_descriptor) - 2;
	int len = d->len;

	if (pos > len)
		return NULL;

	while(pos < len) {
		struct dvb_multilingual_component *e =
			(struct dvb_multilingual_component*) (buf+pos);

		pos += sizeof(struct dvb_multilingual_component);

		if (pos > len)
			return NULL;

		pos += e->text_description_length;

		if (pos > len)
			return NULL;
	}

	return (struct dvb_multilingual_component_descriptor*) d;
}

#define dvb_multilingual_component_descriptor_components_for_each(d, pos) \
	for ((pos) = dvb_multilingual_component_descriptor_components_first(d); \
	     (pos); \
	     (pos) = dvb_multilingual_component_descriptor_components_next(d, pos))

static inline uint8_t *
	dvb_multilingual_component_text_char(struct dvb_multilingual_component *e)
{
	return (uint8_t *) e + sizeof(struct dvb_multilingual_component);
}










/******************************** PRIVATE CODE ********************************/
static inline struct dvb_multilingual_component*
	dvb_multilingual_component_descriptor_components_first(struct dvb_multilingual_component_descriptor *d)
{
	if (d->d.len == 1)
		return NULL;

	return (struct dvb_multilingual_component *)
		(uint8_t*) d + sizeof(struct dvb_multilingual_component_descriptor);
}

static inline struct dvb_multilingual_component*
	dvb_multilingual_component_descriptor_components_next(struct dvb_multilingual_component_descriptor *d,
							      struct dvb_multilingual_component *pos)
{
	uint8_t *end = (uint8_t*) d + 2 + d->d.len;
	uint8_t *next =	(uint8_t *) pos +
			sizeof(struct dvb_multilingual_component) +
			pos->text_description_length;

	if (next >= end)
		return NULL;

	return (struct dvb_multilingual_component *) next;
}

#endif
