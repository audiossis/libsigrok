/*
 * This file is part of the libsigrok project.
 *
 * Copyright (C) 2026 <user>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LIBSIGROK_HARDWARE_RIGOL_DS5K_PROTOCOL_H
#define LIBSIGROK_HARDWARE_RIGOL_DS5K_PROTOCOL_H

#include <stdint.h>
#include <glib.h>
#include <libsigrok/libsigrok.h>
#include "libsigrok-internal.h"

#define LOG_PREFIX "rigol-ds5k"

enum {
	DS5K_TRIGGER_SOURCE_CH1 = 0,
	DS5K_TRIGGER_SOURCE_CH2,
};

struct rigol_ds5k_profile {
	uint16_t vendor_id;
	uint16_t product_id;
	const char *vendor;
	const char *model;
};

struct rigol_ds5k_state {
	const struct rigol_ds5k_profile *profile;
	struct sr_context *ctx;
	struct sr_usb_dev_inst *usb;
};

SR_PRIV int rigol_ds5k_send_text(struct sr_usb_dev_inst *usb, const char *cmd);
SR_PRIV int rigol_ds5k_read_text(struct sr_usb_dev_inst *usb, char *buf, size_t buf_len);

#endif
