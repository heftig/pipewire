/* Pinos
 * Copyright (C) 2015 Wim Taymans <wim.taymans@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef __PINOS_BUFFER_H__
#define __PINOS_BUFFER_H__

#include <glib-object.h>
#include <gio/gio.h>

G_BEGIN_DECLS

typedef struct _PinosBuffer PinosBuffer;
typedef struct _PinosBufferInfo PinosBufferInfo;
typedef struct _PinosBufferIter PinosBufferIter;
typedef struct _PinosBufferBuilder PinosBufferBuilder;

#define PINOS_BUFFER_VERSION 0

typedef struct {
  guint32 flags;
  guint32 seq;
  gint64 pts;
  gint64 dts_offset;
} PinosBufferHeader;

struct _PinosBuffer {
  /*< private >*/
  gsize x[16];
};

void               pinos_buffer_init_data        (PinosBuffer       *buffer,
                                                  gpointer           data,
                                                  gsize              size,
                                                  GSocketControlMessage *message);

void               pinos_buffer_clear            (PinosBuffer       *buffer);

const PinosBufferHeader *
                   pinos_buffer_get_header       (PinosBuffer       *buffer,
                                                  guint32           *version);
int                pinos_buffer_get_fd           (PinosBuffer       *buffer,
                                                  gint               index,
                                                  GError           **error);

gpointer           pinos_buffer_steal            (PinosBuffer       *buffer,
                                                  gsize             *size,
                                                  GSocketControlMessage **message);


typedef enum {
  PINOS_PACKET_TYPE_INVALID           = 0,

  PINOS_PACKET_TYPE_FD_PAYLOAD        = 1,
  PINOS_PACKET_TYPE_FORMAT_CHANGE     = 2,
  PINOS_PACKET_TYPE_PROPERTY_CHANGE   = 3,
} PinosPacketType;


/* iterating packets */
struct _PinosBufferIter {
  /*< private >*/
  gsize x[16];
};

void               pinos_buffer_iter_init_full   (PinosBufferIter *iter,
                                                  PinosBuffer     *buffer,
                                                  guint32          version);
#define pinos_buffer_iter_init(i,b)   pinos_buffer_iter_init_full(i,b, PINOS_BUFFER_VERSION);

gboolean           pinos_buffer_iter_next        (PinosBufferIter *iter);

PinosPacketType    pinos_buffer_iter_get_type    (PinosBufferIter *iter);
gpointer           pinos_buffer_iter_get_data    (PinosBufferIter *iter, gsize *size);

/**
 * PinosBufferBuilder:
 */
struct _PinosBufferBuilder {
  /*< private >*/
  gsize x[16];
};

void               pinos_buffer_builder_init_full  (PinosBufferBuilder      *builder,
                                                    guint32                  version);
#define pinos_buffer_builder_init(b)   pinos_buffer_builder_init_full(b, PINOS_BUFFER_VERSION);

void               pinos_buffer_builder_clear      (PinosBufferBuilder *builder);
void               pinos_buffer_builder_end        (PinosBufferBuilder *builder,
                                                    PinosBuffer        *buffer);

void               pinos_buffer_builder_set_header (PinosBufferBuilder      *builder,
                                                    const PinosBufferHeader *header);
gint               pinos_buffer_builder_add_fd     (PinosBufferBuilder *builder,
                                                    int                 fd,
                                                    GError            **error);
/* fd-payload packets */
/**
 * PinosPacketFDPayload:
 * @id: the unique id of this payload
 * @fd_index: the index of the fd with the data
 * @offset: the offset of the data
 * @size: the size of the data
 *
 * A Packet that contains data in an fd at @fd_index at @offset and with
 * @size.
 */
typedef struct {
  guint32 id;
  gint32 fd_index;
  guint64 offset;
  guint64 size;
} PinosPacketFDPayload;

void               pinos_buffer_iter_parse_fd_payload   (PinosBufferIter      *iter,
                                                         PinosPacketFDPayload *payload);
gboolean           pinos_buffer_builder_add_fd_payload  (PinosBufferBuilder   *builder,
                                                         PinosPacketFDPayload *payload);

#endif /* __PINOS_BUFFER_H__ */
