/**
 * dvbcfg_source configuration file support.
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

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "dvbcfg_source.h"
#include "dvbcfg_util.h"


int dvbcfg_source_load(struct dvbcfg_source_backend *backend,
                       struct dvbcfg_source **sources)
{
        int status;
        while(!(status = backend->get_source(backend, sources)));

        if (status < 0)
                return status;

        return 0;
}

int dvbcfg_source_save(struct dvbcfg_source_backend *backend,
                       struct dvbcfg_source *sources)
{
        int status;

        while(sources) {
                if ((status = backend->put_source(backend, sources)) != 0)
                        return status;

                sources = sources->next;
        }

        return 0;
}

struct dvbcfg_source* dvbcfg_source_new(struct dvbcfg_source **sources,
                                        char* source_idstr, char* description)
{
        struct dvbcfg_source_id source_id;
        struct dvbcfg_source* source;

        if (dvbcfg_source_id_from_string(source_idstr, &source_id)) {
                return NULL;
        }

        source = dvbcfg_source_new2(sources, &source_id, description);

        dvbcfg_source_id_free(&source_id);
        return source;
}

struct dvbcfg_source* dvbcfg_source_new2(struct dvbcfg_source **sources,
                                         struct dvbcfg_source_id* source_id,
                                         char* description)
{
        struct dvbcfg_source* newsource;
        struct dvbcfg_source* cursource;

        /* create new structure */
        newsource = (struct dvbcfg_source*) malloc(sizeof(struct dvbcfg_source));
        if (newsource == NULL)
                return NULL;
        memset(newsource, 0, sizeof(struct dvbcfg_source));
        newsource->description = dvbcfg_strdupandtrim(description, -1);
        if (newsource->description == NULL) {
                free(newsource);
                return NULL;
        }

        /* parse the source_id */
        newsource->source_id.source_type = source_id->source_type;
        if (source_id->source_network)
                newsource->source_id.source_network =
                        dvbcfg_strdupandtrim(source_id->source_network, -1);
        if (source_id->source_region)
                newsource->source_id.source_region =
                        dvbcfg_strdupandtrim(source_id->source_region, -1);
        if (source_id->source_locale)
                newsource->source_id.source_locale =
                        dvbcfg_strdupandtrim(source_id->source_locale, -1);

        /* check */
        if (((newsource->source_id.source_network == NULL) !=
              (source_id->source_network == NULL)) ||
            ((newsource->source_id.source_region == NULL) !=
              (source_id->source_region == NULL)) ||
            ((newsource->source_id.source_locale == NULL) !=
              (source_id->source_locale == NULL))) {
                dvbcfg_source_id_free(&newsource->source_id);
                free(newsource->description);
                free(newsource);
                return NULL;
        }

        /* add it to the list */
        if (*sources == NULL)
                *sources = newsource;
        else {
                cursource = *sources;
                while(cursource->next)
                        cursource = cursource->next;
                cursource->next = newsource;
        }

        return newsource;
}

struct dvbcfg_source *dvbcfg_source_find(struct dvbcfg_source *sources,
                                         char source_type, char *source_network,
                                         char* source_region, char* source_locale)
{
        struct dvbcfg_source_id source_id;

        source_id.source_type = source_type;
        source_id.source_network = source_network;
        source_id.source_region = source_region;
        source_id.source_locale = source_locale;

        return dvbcfg_source_find2(sources, &source_id);
}

struct dvbcfg_source *dvbcfg_source_find2(struct dvbcfg_source *sources,
                                          struct dvbcfg_source_id* source_id)
{
        while (sources) {
                if (dvbcfg_source_id_equal(source_id, &sources->source_id, 1))
                        return sources;

                sources = sources->next;
        }

        return NULL;
}


void dvbcfg_source_free(struct dvbcfg_source **sources,
                        struct dvbcfg_source *tofree)
{
        struct dvbcfg_source *next;
        struct dvbcfg_source *cur;

        next = tofree->next;

        /* free internal structures */
        dvbcfg_source_id_free(&tofree->source_id);
        if (tofree->description)
                free(tofree->description);
        free(tofree);

        /* adjust pointers */
        if (*sources == tofree)
                *sources = next;
        else {
                cur = *sources;
                while((cur->next != tofree) && (cur->next))
                        cur = cur->next;
                if (cur->next == tofree)
                        cur->next = next;
        }
}

void dvbcfg_source_free_all(struct dvbcfg_source *sources)
{
        while (sources)
                dvbcfg_source_free(&sources, sources);
}
