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

#ifndef _SI_MPEG_VIDEO_STREAM_DESCRIPTOR
#define _SI_MPEG_VIDEO_STREAM_DESCRIPTOR 1

#include <si/descriptor.h>
#include <si/common.h>

struct mpeg_video_stream_descriptor {
	struct descriptor d;

  EBIT5(uint8_t multiple_frame_rate_flag	: 1; ,
	uint8_t frame_rate_code			: 4; ,
	uint8_t mpeg_1_only_flag		: 1; ,
	uint8_t constrained_parameter_flag	: 1; ,
	uint8_t still_picture_flag		: 1; );
	/* if (mpeg_1_only_flag == 0) struct mpeg_video_stream_extra extra */
} packed;

struct mpeg_video_stream_extra {
	uint8_t profile_and_level_indication;
  EBIT3(uint8_t chroma_format			: 2; ,
	uint8_t frame_rate_extension		: 1; ,
	uint8_t reserved			: 5; );
} packed;

static inline struct mpeg_video_stream_descriptor*
	mpeg_video_stream_descriptor_parse(struct descriptor* d)
{
	struct mpeg_video_stream_descriptor* vsd =
			(struct mpeg_video_stream_descriptor*) d;

	if (d->len < (sizeof(struct mpeg_video_stream_descriptor) - 2))
		return NULL;

	if (!vsd->mpeg_1_only_flag) {
		if (d->len != (sizeof(struct mpeg_video_stream_descriptor) +
				  sizeof(struct mpeg_video_stream_extra) - 2))
			return NULL;
	}

	return (struct mpeg_video_stream_descriptor*) d;
}

static inline struct mpeg_video_stream_extra*
	mpeg_video_stream_descriptor_extra(struct mpeg_video_stream_descriptor* d)
{
	if (d->mpeg_1_only_flag != 0)
		return NULL;

	return (struct mpeg_video_stream_extra*)
		((uint8_t*) d + sizeof(struct mpeg_video_stream_descriptor));
}

#endif
