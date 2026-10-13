##
## This file is part of the libsigrok project.
##
## Copyright (C) 2026 <user>
##
## This program is free software: you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program.  If not, see <http://www.gnu.org/licenses/>.
##

if HAVE_HW_RIGOL_DS5K

# Local lib, this is NOT meant to be installed!
noinst_LTLIBRARIES += src/hardware/rigol-ds5k/libsigrok_hw_rigol_ds5k.la

src_hardware_rigol_ds5k_libsigrok_hw_rigol_ds5k_la_SOURCES = \
	src/hardware/rigol-ds5k/api.c \
	src/hardware/rigol-ds5k/protocol.c \
	src/hardware/rigol-ds5k/protocol.h

src_hardware_rigol_ds5k_libsigrok_hw_rigol_ds5k_la_CFLAGS = \
	-I$(top_srcdir)/include -I$(top_builddir)/include \
	-I$(top_srcdir)/src \
	$(LIBSIGROK_CFLAGS) \
	$(AM_CFLAGS)

src_hardware_rigol_ds5k_libsigrok_hw_rigol_ds5k_la_LDFLAGS = $(LIBSIGROK_LIBS)

endif
