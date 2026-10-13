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

#include <config.h>
#include <string.h>
#include <libusb.h>
#include <libsigrok/libsigrok.h>
#include "libsigrok-internal.h"
#include "protocol.h"

SR_PRIV int rigol_ds5k_send_text(struct sr_usb_dev_inst *usb, const char *cmd)
{
    int ret;
    size_t len, i;
    unsigned char c;

    len = strlen(cmd);
    for (i = 0; i < len; i++) {
        c = (unsigned char)cmd[i];
        ret = libusb_control_transfer(usb->devhdl, 0x40, 0x01, c, 0, NULL, 0, 1000);
        if (ret < 0)
            return SR_ERR;
    }
    ret = libusb_control_transfer(usb->devhdl, 0x40, 0x01, '\r', 0, NULL, 0, 1000);
    if (ret < 0)
        return SR_ERR;
    return SR_OK;
}

SR_PRIV int rigol_ds5k_read_text(struct sr_usb_dev_inst *usb, char *buf, size_t buf_len)
{
    int ret;
    unsigned char len_byte;

    if (buf_len < 1)
        return SR_ERR_ARG;

    ret = libusb_control_transfer(usb->devhdl, 0xc0, 0x00, 0, 0, &len_byte, 1, 1000);
    if (ret < 0)
        return SR_ERR;
    if (len_byte == 0 || (size_t)len_byte >= buf_len)
        return SR_ERR;
    ret = libusb_control_transfer(usb->devhdl, 0xc0, 0x00, 1, 0, (unsigned char *)buf, len_byte, 2000);
    if (ret < 0)
        return SR_ERR;
    buf[len_byte] = '\0';
    if (len_byte > 0 && buf[len_byte - 1] == '\n')
        buf[len_byte - 1] = '\0';
    return SR_OK;
}
