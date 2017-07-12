/* PipeWire
 * Copyright (C) 2016 Axis Communications <dev-gstreamer@axis.com>
 * @author Linus Svensson <linus.svensson@axis.com>
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

#ifndef __PIPEWIRE_MODULE_H__
#define __PIPEWIRE_MODULE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <pipewire/core.h>

#define PW_TYPE__Module           PW_TYPE_OBJECT_BASE "Module"
#define PW_TYPE_MODULE_BASE       PW_TYPE__Module ":"

#define PIPEWIRE_SYMBOL_MODULE_INIT "pipewire__module_init"

/** \class pw_module
 *
 * A dynamically loadable module
 */
struct pw_module {
	struct pw_core *core;		/**< the core object */
	struct spa_list link;		/**< link in the core module_list */
	struct pw_global *global;	/**< global object for this module */

	struct pw_module_info info;	/**< introspectable module info */

	void *user_data;		/**< module user_data */

	/** Emited when the module is destroyed */
	PW_SIGNAL(destroy_signal, (struct pw_listener *listener, struct pw_module *module));
};

/** Module init function signature
 *
 * \param module A \ref pw_module
 * \param args Arguments to the module
 * \return true on success, false otherwise
 *
 * A module should provide an init function with this signature. This function
 * will be called when a module is loaded.
 *
 * \memberof pw_module
 */
typedef bool (*pw_module_init_func_t) (struct pw_module *module, char *args);

struct pw_module *
pw_module_load(struct pw_core *core,
	       const char *name, const char *args, char **err);

void
pw_module_destroy(struct pw_module *module);

#ifdef __cplusplus
}
#endif

#endif /* __PIPEWIRE_MODULE_H__ */
