/*
 * Copyright (c) 2019 Tavish Naruka <tavishnaruka@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/* Sample which uses the filesystem API and SDHC driver */

#include <zephyr.h>
#include <device.h>
#include <disk/disk_access.h>
#include <logging/log.h>
#include <fs/fs.h>
#include <ff.h>
#include <string.h>
#include <dk_buttons_and_leds.h>




LOG_MODULE_REGISTER(main);

static int lsdir(const char *path);

static FATFS fat_fs;
/* mounting info */
static struct fs_mount_t mp = {
	.type = FS_FATFS,
	.fs_data = &fat_fs,
};

/*
*  Note the fatfs library is able to mount only strings inside _VOLUME_STRS
*  in ffconf.h
*/
static const char *disk_mount_pt = "/SD:";

static int lsdir(const char *path)
{
	int res;
	struct fs_dir_t dirp;
	static struct fs_dirent entry;

	/* Verify fs_opendir() */
	res = fs_opendir(&dirp, path);
	if (res) {
		printk("Error opening dir %s [%d]\n", path, res);
		return res;
	}

	printk("\nListing dir %s ...\n", path);
	for (;;) {
		/* Verify fs_readdir() */
		res = fs_readdir(&dirp, &entry);

		/* entry.name[0] == 0 means end-of-dir */
		if (res || entry.name[0] == 0) {
			break;
		}

		if (entry.type == FS_DIR_ENTRY_DIR) {
			printk("[DIR ] %s\n", entry.name);
		} else {
			printk("[FILE] %s (size = %zu)\n",
				entry.name, entry.size);
		}
	}

	/* Verify fs_closedir() */
	fs_closedir(&dirp);

	return res;
}

static void create_file_path(char *file_path, char *filename) {
	// Create absolute path for filename
	strcat(file_path, disk_mount_pt);
	strcat(file_path, "/");
	strcat(file_path, filename);
}


static int read_file(char *file_path, char *data, int size) {

	// For catching return values from fs_functions
	int ret = 1;

	// Open file for reading, if file doesnt exist, create one.
	struct fs_file_t file;
	fs_open(&file, file_path, FS_O_READ);

	// Read characters until end of file
	uint8_t buffer[8] = "";
	while (1) {
		ret = fs_read(&file, &buffer, 1);
		if (ret == 0) break;
		strcat(data, buffer);
	} 
	fs_close(&file);


	return 0;
}


static int write_file(char *file_path, char *data, int size) {
	struct fs_file_t file;
	fs_open(&file, file_path, (FS_O_WRITE | FS_O_APPEND));
	fs_write(&file, "\n", strlen("\n"));
	fs_write(&file, data, size);
	fs_close(&file);

	return 0;
}

static int mountSD() {
	
	static const char *disk_pdrv = "SD";
	uint64_t memory_size_mb;
	uint32_t block_count;
	uint32_t block_size;

	do {
		if (disk_access_init(disk_pdrv) != 0) {
		LOG_ERR("Storage init ERROR!");
		break;
		}

		if (disk_access_ioctl(disk_pdrv,
				DISK_IOCTL_GET_SECTOR_COUNT, &block_count)) {
			LOG_ERR("Unable to get sector count");
			break;
		}
		LOG_INF("Block count %u", block_count);

		if (disk_access_ioctl(disk_pdrv,
				DISK_IOCTL_GET_SECTOR_SIZE, &block_size)) {
			LOG_ERR("Unable to get sector size");
			break;
		}
		printk("Sector size %u\n", block_size);

		memory_size_mb = (uint64_t)block_count * block_size;
		printk("Memory Size(MB) %u\n", (uint32_t)(memory_size_mb >> 20));
		

		mp.mnt_point = disk_mount_pt;

		int res = fs_mount(&mp);

		if (res == FR_OK) {
			printk("Disk mounted.\n");
			lsdir(disk_mount_pt);
		} else {
			printk("Error mounting disk.\n");
			return res;
		}
	} while (0);

	return 0;
}


void main(void)
{
	mountSD();
	char dataBuffer[1024] = "";
	char filepath[32] = "";
	create_file_path(filepath, "test.txt");
	read_file(filepath, dataBuffer, sizeof(dataBuffer));
	create_file_path(filepath, "test2.txt");
	write_file(filepath, dataBuffer, strlen(dataBuffer));

	printk("%s", dataBuffer);




}

