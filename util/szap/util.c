/*
 * util functions for various ?zap implementations
 *
 * Copyright (C) 2001 Johannes Stezenbach (js@convergence.de)
 * for convergence integrated media
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <linux/dvb/frontend.h>
#include <linux/dvb/dmx.h>


int set_pesfilter(int dmxfd, int pid, int pes_type, int dvr)
{
	struct dmx_pes_filter_params pesfilter;

	/* ignore this pid to allow radio services */
	if (pid < 0 || pid >= 0x1fff || (pid == 0 && pes_type != DMX_PES_OTHER))
		return 0;

	if (dvr) {
		int buffersize = 64 * 1024;
		if (ioctl(dmxfd, DMX_SET_BUFFER_SIZE, buffersize) == -1)
			perror("DMX_SET_BUFFER_SIZE failed");
	}

	pesfilter.pid = pid;
	pesfilter.input = DMX_IN_FRONTEND;
	pesfilter.output = dvr ? DMX_OUT_TS_TAP : DMX_OUT_DECODER;
	pesfilter.pes_type = pes_type;
	pesfilter.flags = DMX_IMMEDIATE_START;

	if (ioctl(dmxfd, DMX_SET_PES_FILTER, &pesfilter) == -1) {
		fprintf(stderr, "DMX_SET_PES_FILTER failed "
			"(PID = 0x%04x): %d %m\n", pid, errno);
		return -1;
	}
	return 0;
}


int get_pmt_pid(char *dmxdev, int sid)
{
	int patfd, count;
	int pmt_pid = 0;
	int patread = 0;
	int section_length;
	unsigned char buft[4096];
	unsigned char *buf = buft;
	struct dmx_sct_filter_params f;

	memset(&f, 0, sizeof(f));
	f.pid = 0;
	f.filter.filter[0] = 0x00;
	f.filter.mask[0] = 0xff;
	f.timeout = 0;
	f.flags = DMX_IMMEDIATE_START | DMX_CHECK_CRC;

	if ((patfd = open(dmxdev, O_RDWR)) < 0) {
		perror("openening pat demux failed");
		return -1;
	}

	if (ioctl(patfd, DMX_SET_FILTER, &f) == -1) {
		perror("ioctl DMX_SET_FILTER failed");
		close(patfd);
		return -1;
	}

	while (!patread) {
		if (((count = read(patfd, buf, sizeof(buft))) < 0) && errno == EOVERFLOW)
			count = read(patfd, buf, sizeof(buft));

		if (count < 0) {
			perror("read_sections: read error");
			close(patfd);
			return -1;
		}
		section_length = ((buf[1] & 0x0f) << 8) | buf[2];
		if (count != section_length + 3)
			continue;

		buf += 8;
		section_length -= 8;

		patread = 1; /* assumes one section contains the whole pat */
		while (section_length > 0) {
			int service_id = (buf[0] << 8) | buf[1];

			if (service_id == sid) {
				pmt_pid = ((buf[2] & 0x1f) << 8) | buf[3];
				section_length = 0;
			}
			buf += 4;
			section_length -= 4;
		}
	}
	close(patfd);
	return pmt_pid;
}
