/* PipeWire
 * Copyright (C) 2016 Wim Taymans <wim.taymans@gmail.com>
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

#ifndef __PIPEWIRE_EXT_CLIENT_NODE_H__
#define __PIPEWIRE_EXT_CLIENT_NODE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <spa/defs.h>
#include <spa/props.h>
#include <spa/format.h>
#include <spa/param-alloc.h>
#include <spa/node.h>

#define PIPEWIRE_TYPE__ClientNode                            PIPEWIRE_TYPE_NODE_BASE "Client"
#define PIPEWIRE_TYPE_CLIENT_NODE_BASE                       PIPEWIRE_TYPE__ClientNode ":"

/** information about a buffer */
struct pw_client_node_buffer {
	uint32_t mem_id;		/**< the memory id for the metadata */
	uint32_t offset;		/**< offset in memory */
	uint32_t size;			/**< size in memory */
	struct spa_buffer *buffer;	/**< buffer describing metadata and buffer memory */
};

#define PW_VERSION_CLIENT_NODE		0

#define PW_CLIENT_NODE_METHOD_DONE           0
#define PW_CLIENT_NODE_METHOD_UPDATE         1
#define PW_CLIENT_NODE_METHOD_PORT_UPDATE    2
#define PW_CLIENT_NODE_METHOD_EVENT          3
#define PW_CLIENT_NODE_METHOD_DESTROY        4
#define PW_CLIENT_NODE_METHOD_NUM            5

/** \ref pw_client_node methods */
struct pw_client_node_methods {
	/** Complete an async operation */
	void (*done) (void *object, int seq, int res);

	/**
	 * Update the node ports and properties
	 *
	 * Update the maximum number of ports and the properties of the
	 * client node.
	 * \param change_mask bitfield with changed parameters
	 * \param max_input_ports new max input ports
	 * \param max_output_ports new max output ports
	 * \param props new properties
	 */
	void (*update) (void *object,
#define PW_CLIENT_NODE_UPDATE_MAX_INPUTS   (1 << 0)
#define PW_CLIENT_NODE_UPDATE_MAX_OUTPUTS  (1 << 1)
#define PW_CLIENT_NODE_UPDATE_PROPS        (1 << 2)
			uint32_t change_mask,
			uint32_t max_input_ports,
			uint32_t max_output_ports,
			const struct spa_props *props);

	/**
	 * Update a node port
	 *
	 * Update the information of one port of a node.
	 * \param direction the direction of the port
	 * \param port_id the port id to update
	 * \param change_mask a bitfield of changed items
	 * \param n_possible_formats number of possible formats
	 * \param possible_formats array of possible formats on the port
	 * \param format the current format on the port
	 * \param n_params number of port parameters
	 * \param params array of port parameters
	 * \param info port information
	 */
	void (*port_update) (void *object,
			     enum spa_direction direction,
			     uint32_t port_id,
#define PW_CLIENT_NODE_PORT_UPDATE_POSSIBLE_FORMATS  (1 << 0)
#define PW_CLIENT_NODE_PORT_UPDATE_FORMAT            (1 << 1)
#define PW_CLIENT_NODE_PORT_UPDATE_PARAMS            (1 << 2)
#define PW_CLIENT_NODE_PORT_UPDATE_INFO              (1 << 3)
			     uint32_t change_mask,
			     uint32_t n_possible_formats,
			     const struct spa_format **possible_formats,
			     const struct spa_format *format,
			     uint32_t n_params,
			     const struct spa_param **params,
			     const struct spa_port_info *info);
	/**
	 * Send an event to the node
	 * \param event the event to send
	 */
	void (*event) (void *object, struct spa_event *event);
	/**
	 * Destroy the client_node
	 */
	void (*destroy) (void *object);
};

#define pw_client_node_do_done(r,...)         ((struct pw_client_node_methods*)r->iface->methods)->done(r,__VA_ARGS__)
#define pw_client_node_do_update(r,...)       ((struct pw_client_node_methods*)r->iface->methods)->update(r,__VA_ARGS__)
#define pw_client_node_do_port_update(r,...)  ((struct pw_client_node_methods*)r->iface->methods)->port_update(r,__VA_ARGS__)
#define pw_client_node_do_event(r,...)        ((struct pw_client_node_methods*)r->iface->methods)->event(r,__VA_ARGS__)
#define pw_client_node_do_destroy(r)          ((struct pw_client_node_methods*)r->iface->methods)->destroy(r)

#define PW_CLIENT_NODE_EVENT_SET_PROPS       0
#define PW_CLIENT_NODE_EVENT_EVENT           1
#define PW_CLIENT_NODE_EVENT_ADD_PORT        2
#define PW_CLIENT_NODE_EVENT_REMOVE_PORT     3
#define PW_CLIENT_NODE_EVENT_SET_FORMAT      4
#define PW_CLIENT_NODE_EVENT_SET_PARAM       5
#define PW_CLIENT_NODE_EVENT_ADD_MEM         6
#define PW_CLIENT_NODE_EVENT_USE_BUFFERS     7
#define PW_CLIENT_NODE_EVENT_NODE_COMMAND    8
#define PW_CLIENT_NODE_EVENT_PORT_COMMAND    9
#define PW_CLIENT_NODE_EVENT_TRANSPORT       10
#define PW_CLIENT_NODE_EVENT_NUM             11

/** \ref pw_client_node events */
struct pw_client_node_events {
	/**
	 * Notify of a property change
	 *
	 * When the server configures the properties on the node
	 * this event is sent
	 *
	 * \param seq a sequence number
	 * \param props the props to set
	 */
	void (*set_props) (void *object,
			   uint32_t seq,
			   const struct spa_props *props);
	/**
	 * Receive an event from the client node
	 * \param event the received event */
	void (*event) (void *object, const struct spa_event *event);
	/**
	 * A new port was added to the node
	 *
	 * The server can at any time add a port to the node when there
	 * are free ports available.
	 *
	 * \param seq a sequence number
	 * \param direction the direction of the port
	 * \param port_id the new port id
	 */
	void (*add_port) (void *object,
			  uint32_t seq,
			  enum spa_direction direction,
			  uint32_t port_id);
	/**
	 * A port was removed from the node
	 *
	 * \param seq a sequence number
	 * \param direction a port direction
	 * \param port_id the remove port id
	 */
	void (*remove_port) (void *object,
			     uint32_t seq,
			     enum spa_direction direction,
			     uint32_t port_id);
	/**
	 * A format was configured on the port
	 *
	 * \param seq a sequence number
	 * \param direction a port direction
	 * \param port_id the port id
	 * \param flags flags used when setting the format
	 * \param format the new format
	 */
	void (*set_format) (void *object,
			    uint32_t seq,
			    enum spa_direction direction,
			    uint32_t port_id,
			    uint32_t flags,
			    const struct spa_format *format);
	/**
	 * A parameter was configured on the port
	 *
	 * \param seq a sequence number
	 * \param direction a port direction
	 * \param port_id the port id
	 * \param param the new param
	 */
	void (*set_param) (void *object,
			   uint32_t seq,
			   enum spa_direction direction,
			   uint32_t port_id,
			   const struct spa_param *param);
	/**
	 * Memory was added for a port
	 *
	 * \param direction a port direction
	 * \param port_id the port id
	 * \param mem_id the id of the memory
	 * \param type the memory type
	 * \param memfd the fd of the memory
	 * \param flags flags for the \a memfd
	 * \param offset valid offset of mapped memory from \a memfd
	 * \param size valid size of mapped memory from \a memfd
	 */
	void (*add_mem) (void *object,
			 enum spa_direction direction,
			 uint32_t port_id,
			 uint32_t mem_id,
			 uint32_t type,
			 int memfd,
			 uint32_t flags,
			 uint32_t offset,
			 uint32_t size);
	/**
	 * Notify the port of buffers
	 *
	 * \param seq a sequence number
	 * \param direction a port direction
	 * \param port_id the port id
	 * \param n_buffer the number of buffers
	 * \param buffers and array of buffer descriptions
	 */
	void (*use_buffers) (void *object,
			     uint32_t seq,
			     enum spa_direction direction,
			     uint32_t port_id,
			     uint32_t n_buffers,
			     struct pw_client_node_buffer *buffers);
	/**
	 * Notify of a new node command
	 *
	 * \param seq a sequence number
	 * \param command the command
	 */
	void (*node_command) (void *object, uint32_t seq, const struct spa_command *command);
	/**
	 * Notify of a new port command
	 *
	 * \param direction a port direction
	 * \param port_id the port id
	 * \param command the command
	 */
	void (*port_command) (void *object,
			      enum spa_direction direction,
			      uint32_t port_id,
			      const struct spa_command *command);

	/**
	 * Notify of a new transport area
	 *
	 * The transport area is used to exchange real-time commands between
	 * the client and the server.
	 *
	 * \param readfd fd for signal data can be read
	 * \param writefd fd for signal data can be written
	 * \param memfd the memory fd of the area
	 * \param offset the offset to map
	 * \param size the size to map
	 */
	void (*transport) (void *object,
			   int readfd,
			   int writefd,
			   int memfd,
			   uint32_t offset,
			   uint32_t size);
};

#define pw_client_node_notify_set_props(r,...)    ((struct pw_client_node_events*)r->iface->events)->props(r,__VA_ARGS__)
#define pw_client_node_notify_event(r,...)        ((struct pw_client_node_events*)r->iface->events)->event(r,__VA_ARGS__)
#define pw_client_node_notify_add_port(r,...)     ((struct pw_client_node_events*)r->iface->events)->add_port(r,__VA_ARGS__)
#define pw_client_node_notify_remove_port(r,...)  ((struct pw_client_node_events*)r->iface->events)->remove_port(r,__VA_ARGS__)
#define pw_client_node_notify_set_format(r,...)   ((struct pw_client_node_events*)r->iface->events)->set_format(r,__VA_ARGS__)
#define pw_client_node_notify_set_param(r,...)    ((struct pw_client_node_events*)r->iface->events)->set_param(r,__VA_ARGS__)
#define pw_client_node_notify_add_mem(r,...)      ((struct pw_client_node_events*)r->iface->events)->add_mem(r,__VA_ARGS__)
#define pw_client_node_notify_use_buffers(r,...)  ((struct pw_client_node_events*)r->iface->events)->use_buffers(r,__VA_ARGS__)
#define pw_client_node_notify_node_command(r,...) ((struct pw_client_node_events*)r->iface->events)->node_command(r,__VA_ARGS__)
#define pw_client_node_notify_port_command(r,...) ((struct pw_client_node_events*)r->iface->events)->port_command(r,__VA_ARGS__)
#define pw_client_node_notify_transport(r,...)    ((struct pw_client_node_events*)r->iface->events)->transport(r,__VA_ARGS__)

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif /* __PIPEWIRE_EXT_CLIENT_NODE_H__ */