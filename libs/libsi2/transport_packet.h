/*
 * section and descriptor parser
 *
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

#ifndef _SI_TRANSPORT_PACKET_H
#define _SI_TRANSPORT_PACKET_H 1

#include <stdint.h>
#include "descriptor.h"

#define TRANSPORT_PACKET_LENGTH 188
#define TRANSPORT_PACKET_SYNC 0x47


/**
 * Enumeration of adaptation field control values.
 */
enum transport_adaptation_field_control {
	transport_adaptation_field_control_reserved		= 0x00,
	transport_adaptation_field_control_payload_only		= 0x01,
	transport_adaptation_field_control_adaptation_only	= 0x02,
	transport_adaptation_field_control_adaptation_payload	= 0x03,
};

/**
 * Enumeration of scrambling control values.
 */
enum transport_scrambling_control {
	transport_scrambling_control_unscrambled   		= 0x00,
	transport_scrambling_control_user_1			= 0x01,
	transport_scrambling_control_user_2			= 0x02,
	transport_scrambling_control_user_3			= 0x03,
};

/**
 * Enumeration of adaptation flags.
 */
enum transport_adaptation_flags {
	transport_adaptation_flag_discontinuity			= 0x80,
	transport_adaptation_flag_random_access			= 0x40,
	transport_adaptation_flag_es_priority			= 0x20,
	transport_adaptation_flag_pcr				= 0x10,
	transport_adaptation_flag_opcr				= 0x08,
	transport_adaptation_flag_splicing_point		= 0x04,
	transport_adaptation_flag_private_data			= 0x02,
	transport_adaptation_flag_extension			= 0x01,
};

/**
 * Enumeration of adaptation extension flags.
 */
enum transport_adaptation_extension_flags {
	transport_adaptation_extension_flag_ltw			= 0x80,
	transport_adaptation_extension_flag_piecewise_rate	= 0x40,
	transport_adaptation_extension_flag_seamless_splice	= 0x20,
};

/**
 * Enumeration of flags controlling which values to extract using the
 * transport_packet_values_extract() function.
 */
enum transport_value {
	/* normal adaptation */
	transport_value_pcr					= 0x0001,
	transport_value_opcr					= 0x0002,
	transport_value_splice_countdown			= 0x0004,
	transport_value_private_data				= 0x0008,

	/* extension adaptation */
	transport_value_ltw					= 0x0100,
	transport_value_piecewise_rate				= 0x0101,
	transport_value_seamless_splice				= 0x0102,
};

/**
 * Structure describing a transport packet header.
 */
struct transport_packet {
	uint8_t sync_byte;
  EBIT3(uint8_t transport_error_indicator 	: 1; ,
	uint8_t payload_unit_start_indicator	: 1; ,
	uint8_t pid_hi				: 6; );
	uint8_t pid_lo;
  EBIT3(uint8_t transport_scrambling_control	: 2; ,
	uint8_t adaptation_field_control	: 2; ,
	uint8_t continuity_counter		: 4; );
	/* values */
} packed;

/**
 * Structure to extract values into using the transport_packet_values_extract()
 * function.
 */
struct transport_values {
	enum transport_adaptation_flags flags; /* always extracted */
	uint8_t *payload;     /* always extracted */
	uint16_t payload_length;    /* always extracted */

	uint64_t pcr;
	uint64_t opcr;
	uint8_t  splice_countdown;
	uint8_t private_data_length;
	uint8_t *private_data;
	uint16_t ltw_offset;
	uint32_t piecewise_rate;
	uint8_t splice_type;
	uint64_t dts_next_au;
};

/**
 * Extract the PID from a transport packet.
 *
 * @param pkt The packet.
 * @return The PID.
 */
static inline int transport_packet_pid(struct transport_packet *pkt)
{
	return (pkt->pid_hi << 8) | (pkt->pid_lo);
}

/**
 * Extract selected fields from a transport packet.
 *
 * @param pkt The packet.
 * @param out Destination structure for values.
 * @param extract Orred bitmask of enum transport_value - tells it what fields
 * to extract if they are available.
 * @return < 0 => error. Otherwise, an orred bitmask of enum transport_value
 * telling you what fields were successfully extracted.
 */
extern int transport_packet_values_extract(struct transport_packet *pkt,
					   struct transport_values *out,
					   enum transport_value extract);

#endif
