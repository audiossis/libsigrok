#include <config.h>
#include <string.h>
#include <libusb.h>
#include <libsigrok/libsigrok.h>
#include "libsigrok-internal.h"
#include "protocol.h"

static const uint32_t scanopts[] = {
	SR_CONF_CONN,
};

static const uint32_t drvopts[] = {
	SR_CONF_OSCILLOSCOPE,
};

static const uint32_t devopts[] = {
	SR_CONF_CONN | SR_CONF_GET,
};

#define RIGOL_DS5K_VID 0x0400
#define RIGOL_DS5K_PID 0xc55d

static struct sr_dev_inst *probe_device(struct sr_usb_dev_inst *usb)
{
	struct sr_dev_inst *sdi;
	struct rigol_ds5k_state *state;
	struct sr_channel_group *cg;
	struct sr_channel *ch0, *ch1;

	sdi = g_malloc0(sizeof(struct sr_dev_inst));
	sdi->vendor = g_strdup("Rigol");
	sdi->model = g_strdup("DS5102MA");
	sdi->status = SR_ST_INACTIVE;
	sdi->inst_type = SR_INST_USB;
	sdi->conn = usb;
	sdi->driver = NULL;

	state = g_malloc0(sizeof(struct rigol_ds5k_state));
	state->usb = usb;
	sdi->priv = state;

	cg = sr_channel_group_new(sdi, "Channels", NULL);
	ch0 = sr_channel_new(sdi, 0, SR_CHANNEL_ANALOG, TRUE, "CH1");
	ch1 = sr_channel_new(sdi, 1, SR_CHANNEL_ANALOG, TRUE, "CH2");
	cg->channels = g_slist_append(cg->channels, ch0);
	cg->channels = g_slist_append(cg->channels, ch1);
	sdi->channels = g_slist_append(sdi->channels, ch0);
	sdi->channels = g_slist_append(sdi->channels, ch1);
	sdi->channel_groups = g_slist_append(sdi->channel_groups, cg);

	return sdi;
}

static GSList *scan(struct sr_dev_driver *di, GSList *options)
{
	struct drv_context *drvc;
	struct sr_dev_inst *sdi;
	struct sr_usb_dev_inst *usb;
	struct sr_config *src;
	struct libusb_device_descriptor des;
	libusb_device **devlist;
	GSList *devices = NULL, *l;
	const char *conn = NULL;
	int i, bus, addr;

	drvc = di->context;
	for (l = options; l; l = l->next) {
		src = l->data;
		if (src->key == SR_CONF_CONN) {
			conn = g_variant_get_string(src->data, NULL);
			break;
		}
	}

	if (libusb_get_device_list(drvc->sr_ctx->libusb_ctx, &devlist) < 0)
		return std_scan_complete(di, NULL);

	for (i = 0; devlist[i]; i++) {
		if (libusb_get_device_descriptor(devlist[i], &des) < 0)
			continue;
		if (des.idVendor != RIGOL_DS5K_VID ||
		    des.idProduct != RIGOL_DS5K_PID)
			continue;

		bus = libusb_get_bus_number(devlist[i]);
		addr = libusb_get_device_address(devlist[i]);
		usb = sr_usb_dev_inst_new(bus, addr, NULL);
		sdi = probe_device(usb);
		devices = g_slist_append(devices, sdi);
	}

	libusb_free_device_list(devlist, 1);

	return std_scan_complete(di, devices);
}

static int dev_open(struct sr_dev_inst *sdi)
{
	struct sr_usb_dev_inst *usb = sdi->conn;
	int r;

	if (sr_usb_open(((struct drv_context *)sdi->driver->context)->sr_ctx->libusb_ctx, usb) != SR_OK) {
		sr_err("Failed to open Rigol DS5000USB.");
		return SR_ERR;
	}

	r = libusb_claim_interface(usb->devhdl, 0);
	if (r < 0) {
		if (libusb_kernel_driver_active(usb->devhdl, 0)) {
			libusb_detach_kernel_driver(usb->devhdl, 0);
			r = libusb_claim_interface(usb->devhdl, 0);
		}
	}
	if (r < 0) {
		sr_err("Failed to claim interface: %s", libusb_error_name(r));
		sr_usb_close(usb);
		return SR_ERR;
	}
	sr_dbg("Rigol DS5000USB opened.");

	return SR_OK;
}

static int dev_close(struct sr_dev_inst *sdi)
{
	struct sr_usb_dev_inst *usb = sdi->conn;

	if (usb->devhdl) {
		libusb_release_interface(usb->devhdl, 0);
		sr_usb_close(usb);
	}
	return SR_OK;
}

static int config_get(uint32_t key, GVariant **data, const struct sr_dev_inst *sdi,
               const struct sr_channel_group *cg)
{
	(void)key; (void)data; (void)sdi; (void)cg;
	return SR_ERR_NA;
}

static int config_set(uint32_t key, GVariant *data, const struct sr_dev_inst *sdi,
               const struct sr_channel_group *cg)
{
	(void)key; (void)data; (void)sdi; (void)cg;
	return SR_ERR_NA;
}

static int config_list(uint32_t key, GVariant **data, const struct sr_dev_inst *sdi,
               const struct sr_channel_group *cg)
{
	if (!cg) {
		switch (key) {
		case SR_CONF_SCAN_OPTIONS:
		case SR_CONF_DEVICE_OPTIONS:
			return STD_CONFIG_LIST(key, data, sdi, cg, scanopts, drvopts, devopts);
		default:
			return SR_ERR_NA;
		}
	}
	return SR_ERR_NA;
}

static int dev_acquisition_start(const struct sr_dev_inst *sdi)
{
	(void)sdi;
	return SR_OK;
}

static int dev_acquisition_stop(struct sr_dev_inst *sdi)
{
	(void)sdi;
	return SR_OK;
}

SR_PRIV struct sr_dev_driver rigol_ds5k_driver_info = {
	.name = "rigol-ds5k",
	.longname = "Rigol DS5000",
	.api_version = 1,
	.init = std_init,
	.cleanup = std_cleanup,
	.scan = scan,
	.dev_list = std_dev_list,
	.dev_clear = std_dev_clear,
	.config_get = config_get,
	.config_set = config_set,
	.config_list = config_list,
	.dev_open = dev_open,
	.dev_close = dev_close,
	.dev_acquisition_start = dev_acquisition_start,
	.dev_acquisition_stop = dev_acquisition_stop,
	.context = NULL,
};
SR_REGISTER_DEV_DRIVER(rigol_ds5k_driver_info);
