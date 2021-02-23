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


	// Read and write file

	struct fs_file_t test_file;
	int ret;

	const char *file_name = "/SD:/test1.txt";

	fs_open(&test_file, file_name, (FS_O_RDWR | FS_O_CREATE));
	printk("\nfile location: %d\n\n", fs_tell(&test_file));

	fs_seek(&test_file, 0, FS_SEEK_END);
	ret = fs_write(&test_file, "\nhello", sizeof("\nhello"));
	printk("\nwrite val: %d\n", ret);
	printk("\nfile location: %d\n\n", fs_tell(&test_file));

	fs_seek(&test_file, 0, FS_SEEK_CUR);
	printk("\nfile location: %d\n\n", fs_tell(&test_file));
	printk("\n\nDone with writing\n\n\n");

	fs_close(&test_file);


	fs_open(&test_file, file_name, FS_O_RDWR);
	printk("\nOpen file location: %d\n\n", fs_tell(&test_file));

	uint8_t line[128] = "";
	uint8_t bokstav[2] = "";


	do {
		ret = fs_read(&test_file, &bokstav, 1);
		printk("%s", bokstav);

	} while (ret > 0);
	// printk("\nline read: %s, ret value: %d\n", line, ret);

	// printk("\nfile location after 1 read: %d\n\n", fs_tell(&test_file));

	// ret = fs_read(&test_file, &line, 1);
	// printk("\nline read: %s, ret value: %d\n", line, ret);

	// // fs_read(&test_file, &line, sizeof(line));
	// // printk("\nline read: %s\n", line);

	// printk("\nfile location after 2 reads: %d\n\n", fs_tell(&test_file));

	fs_close(&test_file);

	


	return res;
}


void main(void)
{
	/* raw disk i/o */
	do {
		static const char *disk_pdrv = "SD";
		uint64_t memory_size_mb;
		uint32_t block_count;
		uint32_t block_size;

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
	} while (0);

	mp.mnt_point = disk_mount_pt;

	int res = fs_mount(&mp);

	if (res == FR_OK) {
		printk("Disk mounted.\n");
		lsdir(disk_mount_pt);
	} else {
		printk("Error mounting disk.\n");
	}

	while (1) {
		k_sleep(K_MSEC(1000));
	}
}