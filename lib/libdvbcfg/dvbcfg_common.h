/**
 * dvbcfg common definitions.
 *
 * Copyright (c) 2005 by Andrew de Quincey <adq_dvb@lidskialf.net>
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */

#ifndef DVBCFG_COMMON_H
#define DVBCFG_COMMON_H 1

#include <stdint.h>

/**
 * Possible polarization values.
 */
#define DVBCFG_POLARIZATION_H 0
#define DVBCFG_POLARIZATION_V 1
#define DVBCFG_POLARIZATION_L 2
#define DVBCFG_POLARIZATION_R 3


/**
 * Possible types of source_id.
 */
enum dvbcfg_sourcetype {
	DVBCFG_SOURCETYPE_DVBS = 'S',
	DVBCFG_SOURCETYPE_DVBC = 'C',
	DVBCFG_SOURCETYPE_DVBT = 'T',
	DVBCFG_SOURCETYPE_ATSC = 'A',
};


/**
 * A <source_id> defines a unique standardised ID for all DVB networks. It is divided into
 * components as follows:
 *
 * <source_type>-<source_network>-<source_region>-<source_locale>
 *
 * <source_type> is a single character giving the type of DVB source:
 *   DVBS: "S"
 *   DVBT: "T"
 *   DVBC: "C"
 *   ATSC: "A"
 *
 * For DVBS, <source_network> is a unique identifier for a satellite cluster. It is defined to
 * be "S"<longitude><"E"|"W"> - i.e. the orbital position of the cluster. <source_region> and
 * <source_node> have no meaning for a DVBS source, and are omitted from the string representation,
 * and will be set to NULL in the below structure.
 *
 * All other DVB types have a complication. Unlike DVBS, these consist of multiple <source_locale>s
 * (e.g. DVBT transmitters) spaced over a geographical <source_region>. Finally, all the
 * <source_region>s together consitute a <source_network> (typically a country).
 *
 * Between each <source_locale>, the same services/multiplexes are available, but can be on
 * different frequencies.
 *
 * Between each <source_region>, the exact service lineup varies, providing regional programming.
 *
 * <source_network> is the name of the DVB network. Currently we are simply using the country
 * code for this (e.g "Tuk"). However if necessary, this can easily be extended to allow multiple
 * networks, for example "Tuknetwork1", "Tuknetwork2". Note that <source_network> may not
 * contain '-' or whitespace characters.
 *
 * <source_region> is the name of the broadcast region the source is a member of. For example,
 * in Scotland, the "borders" region has slightly programmes to the "grampian" region.
 *
 * Finally, <source_locale> desribes the physical location of where the source may be received. For
 * example, in the UK, the <source_locale> for a DVBT source is the name of the DVBT transmitter
 * (e.g. BlackHill).
 *
 * Note that <source_network>, <source_region>, and <source_locale> may not contain '-', ':', or
 * whitespace characters.
 */
struct dvbcfg_source_id {
        char source_type;
        char *source_network;
        char *source_region;
        char *source_locale;
};


/**
 * A Unique Multiplex ID (UMID) uniquely identifies a multiplex within a source - it is not necessarily globally unique.
 *
 * The externalised string version of this value is as follows:
 * <original_network_id>:<transport_stream_id>:<multiplex_differentiator>
 *
 * <original_network_id> Is the ID of the original broadcaster of the multiplex.
 * <transport_stream_id> Is the ID of the multiplex as allocated by the broadcaster.
 * <multiplex_differentiator> Inevitably there are clashes with the above values. This final value permits these
 * clashes to be resolved. The exact definition depends on the DVB type:
 *   DVBS: ((frequency / (symbolrate/1000)) << 2) | polarisation
 *   DVBT: (frequency / bandwidth_in_Hz)
 *   DVBC: (frequency / symbolrate)
 *   ATSC: (frequency / 6000000)
 *
 *   Note: if there are no clashes, the <multiplex_differentiator> should be set to 0.
 */
struct dvbcfg_umid
{
        uint32_t original_network_id;
        uint32_t transport_stream_id;
        uint32_t multiplex_differentiator;
};

/**
 * A Global Multiplex ID (GMID) uniquely identifies a multiplex across the global space of all multiplexes
 * in all DVB transmission types. It is basically just the concatenation of the source_id and the UMID. The externalised
 * string version is as follows:
 *
 * <source_id>:<original_network_id>:<transport_stream_id>:<multiplex_differentiator>
 */
struct dvbcfg_gmid
{
        struct dvbcfg_source_id source_id;
        struct dvbcfg_umid umid;
};

/**
 * A Unique Service ID (USID) uniquely identifies a service within its multiplex.
 *
 * The externalised string version of this value is as follows:
 * <program_number>:<service_differentiator>
 *
 * <program_number> The program number of the service as allocated by the broadcaster (this is also known as the service_id).
 * <service_differentiator> In case there are clashes with the above, this final value permits these clashes to be resolved.
 *   The value will be the index of the service in the PAT table transmitted in its multiplex.
 *   Note: this should be set to 0 there are no clashes.
 */
struct dvbcfg_usid
{
        uint32_t program_number;
        uint32_t service_differentiator;
};

/**
 * A Global Service ID (GSID) uniquely identifies a service across the global space of all multiplexes
 * in all DVB transmission types. It is basically just the concatenation of the UMID and the USID. The externalised
 * string version is as follows:
 *
 * <source_id>:<original_network_id>:<transport_stream_id>:<multiplex_differentiator>:<program_number>:<service_differentiator>
 *
 * DVBT, DVBC, ATSC give more complications as usual. If the service represented by a GSID is receiveable across the
 * entire <source_network>, <source_region> and <source_locale> should be omitted/NULL.
 *
 * If the service is only receivable in a particular <source_region>, only <source_locale> should be omitted/NULL.
 */
struct dvbcfg_gsid
{
        struct dvbcfg_gmid gmid;
        struct dvbcfg_usid usid;
};

/**
 * Convert a source_id structure into a string.
 *
 * @param umid The source_id structure.
 * @return Pointer to a malloc()ed buffer containing the string, or NULL on failure.
 */
extern char* dvbcfg_source_id_to_string(struct dvbcfg_source_id* source_id);

/**
 * Parse a string into a source_id. (Note: the strings will be malloc()ed)
 *
 * @param string The string to parse.
 * @param umid Pointer to a source_id structure to fill out.
 *
 * @return 0 on success, nonzero on failure.
 */
extern int dvbcfg_source_id_from_string(char* string, struct dvbcfg_source_id* source_id);

/**
 * Check if two source_ids are equal.
 *
 * @param source_id1 First source_id.
 * @param source_id2 Second source_id.
 * @param fuzzy If 0, the two sourceids must match exactly. If 1, NULL is permitted as a wildcard in either sourceid.
 * @return 1 if they match, 0 if not.
 */
extern int dvbcfg_source_id_equal(struct dvbcfg_source_id* source_id1, struct dvbcfg_source_id* source_id2, int fuzzy);

/**
 * Free() the components of a source_id structure. The structure itself will not be free()d -
 * just the char* pointers. It will also be zeroed.
 *
 * @param source_id The source_id to free.
 */
extern void dvbcfg_source_id_free(struct dvbcfg_source_id* source_id);

/**
 * Convert a UMID structure into a string.
 *
 * @param umid The UMID structure.
 * @return Pointer to a malloc()ed buffer containing the string, or NULL on failure.
 */
extern char* dvbcfg_umid_to_string(struct dvbcfg_umid* umid);

/**
 * Parse a string into a UMID.
 *
 * @param string The string to parse.
 * @param umid Pointer to a UMID structure to fill out.
 *
 * @return 0 on success, nonzero on failure.
 */
extern int dvbcfg_umid_from_string(char* string, struct dvbcfg_umid* umid);

/**
 * Are two UMIDs equal?
 *
 * @param umid1 First UMID.
 * @param umid2 Second UMID.
 * @return 1 if they are, 0 if not.
 */
extern int dvbcfg_umid_equal(struct dvbcfg_umid* umid1, struct dvbcfg_umid* umid2);

/**
 * Convert a GMID structure into a string.
 *
 * @param umid The GMID structure.
 * @return Pointer to a malloc()ed buffer containing the string, or NULL on failure.
 */
extern char* dvbcfg_gmid_to_string(struct dvbcfg_gmid* gmid);

/**
 * Parse a string into a GMID.
 *
 * @param string The string to parse.
 * @param umid Pointer to a GMID structure to fill out.
 *
 * @return 0 on success, nonzero on failure.
 */
extern int dvbcfg_gmid_from_string(char* string, struct dvbcfg_gmid* gmid);

/**
 * Are two GMIDs equal?
 *
 * @param gmid1 First GMID.
 * @param gmid2 Second GMID.
 * @return 1 if they are, 0 if not.
 */
extern int dvbcfg_gmid_equal(struct dvbcfg_gmid* gmid1, struct dvbcfg_gmid* gmid2);

/**
 * Convert a USID structure into a string.
 *
 * @param usid The USID structure.
 * @return Pointer to a malloc()ed buffer containing the string, or NULL on failure.
 */
extern char* dvbcfg_usid_to_string(struct dvbcfg_usid* usid);

/**
 * Parse a string into a USID.
 *
 * @param string The string to parse.
 * @param umid Pointer to a USID structure to fill out.
 *
 * @return 0 on success, nonzero on failure.
 */
extern int dvbcfg_usid_from_string(char* string, struct dvbcfg_usid* usid);

/**
 * Are two USIDs equal?
 *
 * @param usid1 First USID.
 * @param usid2 Second USID.
 * @return 1 if they are, 0 if not.
 */
extern int dvbcfg_usid_equal(struct dvbcfg_usid* usid1, struct dvbcfg_usid* usid2);

/**
 * Convert a GSID structure into a string.
 *
 * @param umid The GSID structure.
 * @return Pointer to a malloc()ed buffer containing the string, or NULL on failure.
 */
extern char* dvbcfg_gsid_to_string(struct dvbcfg_gsid* gsid);

/**
 * Parse a string into a GSID.
 *
 * @param string The string to parse.
 * @param umid Pointer to a GSID structure to fill out.
 *
 * @return 0 on success, nonzero on failure.
 */
extern int dvbcfg_gsid_from_string(char* string, struct dvbcfg_gsid* gsid);

/**
 * Are two GSIDs equal?
 *
 * @param usid1 First GSID.
 * @param usid2 Second GSID.
 * @return 1 if they are, 0 if not.
 */
extern int dvbcfg_gsid_equal(struct dvbcfg_gsid* gsid1, struct dvbcfg_gsid* gsid2);

#endif
