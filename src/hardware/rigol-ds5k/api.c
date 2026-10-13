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

static GSList *scan(struct sr_dev_driver *di, GSList *options)
{
	(void)di; (void)options;
	return std_scan_complete(di, NULL);
}

static int dev_open(struct sr_dev_inst *sdi)
{
	(void)sdi;
	return SR_OK;
}

static int dev_close(struct sr_dev_inst *sdi)
{
	(void)sdi;
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
