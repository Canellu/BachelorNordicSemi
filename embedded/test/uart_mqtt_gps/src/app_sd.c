/*
 * Copyright (c) 2019 Tavish Naruka <tavishnaruka@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/* Sample which uses the filesystem API and SDHC driver */

#include <zephyr.h>
#include <device.h>
#include <disk/disk_access.h>
#include <fs/fs.h>
#include <ff.h>
#include <string.h>
#include <stdio.h>

#include "app_sd.h"

extern struct k_msgq sd_msg_q;

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
    strcpy(file_path, "");

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

static int read_JSON(char *file_path, char *data, int size, uint32_t *cursor) {

	// For catching return values from fs_functions
	int ret = 1;

	// Open file for reading, if file doesnt exist, create one.
	struct fs_file_t file;
	fs_open(&file, file_path, FS_O_READ);

	// TODO: seek end to read cursor value

	fs_seek(&file, *cursor, FS_SEEK_SET);

	// Read characters until end of file
	uint8_t buffer[8] = "";

	while (1) {
		ret = fs_read(&file, &buffer, 1);
		if (ret == 0)
		{
			break;
		}
		else if (strcmp(buffer, "{") == 0)
		{
			strcpy(data, "");
			strcat(data, buffer);
		}
		else if (strcmp(buffer, "}") == 0)
		{
			strcat(data, buffer);
			*cursor = fs_tell(&file);
			printk("\n%s %d", data, *cursor);
			return 0;
		}
		else
		{
			strcat(data, buffer);
		}
	}

	fs_close(&file);


	return 0;
}


static int write_file(char *file_path, char *data, int size) {
	struct fs_file_t file;
	fs_open(&file, file_path, (FS_O_WRITE | FS_O_APPEND | FS_O_CREATE));
	fs_write(&file, data, size);
	fs_write(&file, "\n", strlen("\n"));
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
		printk("Storage init ERROR!");
		break;
		}

		if (disk_access_ioctl(disk_pdrv,
				DISK_IOCTL_GET_SECTOR_COUNT, &block_count)) {
			printk("Unable to get sector count");
			break;
		}
		printk("Block count %u", block_count);

		if (disk_access_ioctl(disk_pdrv,
				DISK_IOCTL_GET_SECTOR_SIZE, &block_size)) {
			printk("Unable to get sector size");
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


void app_sd(void)
{
    oasys_data_t sd_msg;
    uint16_t	current_year = -1;
	uint8_t		current_month = -1;
	uint8_t		current_day = -1;

    uint8_t current_filename[16] = "";
	char file_path[32] = "";

	mountSD();


    while(1)
    {
        // get msg from main
		k_msgq_get(&sd_msg_q, &sd_msg, K_FOREVER);

        // test date vs currently saved
        if(current_year != sd_msg.year || current_month != sd_msg.month || current_day != sd_msg.day)
        {
            current_year = sd_msg.year;
            current_month = sd_msg.month;
            current_day = sd_msg.day;

            // create filename format: [yyyy-mm-dd]
            uint8_t temp_str[16];

            strcpy(current_filename, "");
 
            snprintf(temp_str, sizeof(temp_str), "%02u", sd_msg.year);
            strcat(current_filename, temp_str);
            snprintf(temp_str, sizeof(temp_str), "%02u", sd_msg.month);
            strcat(current_filename, temp_str);
            snprintf(temp_str, sizeof(temp_str), "%02u.txt", sd_msg.day);
            strcat(current_filename, temp_str);

			printk("\nCreating file path with name: %s", current_filename);

            // create file path
            create_file_path(file_path, current_filename);
        }

		printk("\nCreating file path");

        // write
        write_file(file_path, sd_msg.json_string, strlen(sd_msg.json_string));

		uint8_t json_text[128] = "";
		uint32_t cursor = 0;

        read_JSON(file_path, json_text, sizeof(json_text), &cursor);
        read_JSON(file_path, json_text, sizeof(json_text), &cursor);
    }
}