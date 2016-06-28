/* Spa Volume plugin
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

#include <spa/plugin.h>
#include <spa/node.h>

SpaHandle * spa_audiotestsrc_new (void);

static SpaResult
audiotestsrc_instantiate (const SpaHandleFactory  *factory,
                          SpaHandle              **handle)
{
  if (factory == NULL || handle == NULL)
    return SPA_RESULT_INVALID_ARGUMENTS;

  *handle = spa_audiotestsrc_new ();

  return SPA_RESULT_OK;
}

static const SpaInterfaceInfo audiotestsrc_interfaces[] =
{
  { SPA_INTERFACE_ID_NODE,
    SPA_INTERFACE_ID_NODE_NAME,
    SPA_INTERFACE_ID_NODE_DESCRIPTION,
  },
};

static SpaResult
audiotestsrc_enum_interface_info (const SpaHandleFactory  *factory,
                                  unsigned int             index,
                                  const SpaInterfaceInfo **info)
{
  if (index >= 1)
    return SPA_RESULT_ENUM_END;

  *info = &audiotestsrc_interfaces[index];

  return SPA_RESULT_OK;
}

static const SpaHandleFactory factories[] =
{
  { "audiotestsrc",
    NULL,
    audiotestsrc_instantiate,
    audiotestsrc_enum_interface_info,
  },
};


SpaResult
spa_enum_handle_factory (unsigned int             index,
                         const SpaHandleFactory **factory)
{
  if (index >= 1)
    return SPA_RESULT_ENUM_END;

  *factory = &factories[index];

  return SPA_RESULT_OK;
}