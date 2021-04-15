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

#include "app_uart.h"
#include "app_sd.h"

extern struct k_msgq main_msg_q;
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
	if (res)
	{
		printk("Error opening dir %s [%d]\n", path, res);
		return res;
	}

	printk("\nListing dir %s ...\n", path);
	for (;;)
	{
		/* Verify fs_readdir() */
		res = fs_readdir(&dirp, &entry);

		/* entry.name[0] == 0 means end-of-dir */
		if (res || entry.name[0] == 0)
		{
			break;
		}

		if (entry.type == FS_DIR_ENTRY_DIR)
		{
			printk("[DIR ] %s\n", entry.name);
		}
		else
		{
			printk("[FILE] %s (size = %zu)\n",
				   entry.name, entry.size);
		}
	}

	/* Verify fs_closedir() */
	fs_closedir(&dirp);

	return res;
}

static int look_for_file(const char *path, const char *filename)
{
	int res;
	struct fs_dir_t dirp;
	static struct fs_dirent entry;

	bool mission_params = false;

	res = fs_opendir(&dirp, path);
	if (res)
	{
		printk("Error opening dir %s [%d]\n", path, res);
		return res;
	}

	while (1)
	{
		/* Verify fs_readdir() */
		res = fs_readdir(&dirp, &entry);

		/* entry.name[0] == 0 means end-of-dir */
		if (res || entry.name[0] == 0)
		{
			break;
		}

		if (entry.type == FS_DIR_ENTRY_FILE)
		{
			printk("%s\n", entry.name);

			if (strcmp(entry.name, filename) == 0)
			{
				mission_params = true;

				k_msgq_put(&main_msg_q, &mission_params, K_NO_WAIT);

				return 0;
			}
		}
	}

	printk("File not found\n");
	k_msgq_put(&main_msg_q, &mission_params, K_NO_WAIT);

	return 0;
}

static int send_all_file_info(const char *path)
{
	int res;
	struct fs_dir_t dirp;
	static struct fs_dirent entry;

	uint8_t file_data[128];
	uint8_t temp_str[16];

	res = fs_opendir(&dirp, path);
	if (res)
	{
		printk("Error opening dir %s [%d]\n", path, res);
		return res;
	}

	while (1)
	{
		/* Verify fs_readdir() */
		res = fs_readdir(&dirp, &entry);

		/* entry.name[0] == 0 means end-of-dir */
		if (res || entry.name[0] == 0)
		{
			break;
		}

		if (entry.type == FS_DIR_ENTRY_FILE)
		{

			memset(file_data, 0, sizeof(file_data));
			strcat(file_data, entry.name);
			snprintf(temp_str, sizeof(temp_str), ":%u", entry.size);
			strcat(file_data, temp_str);

			// printk("\n%s", file_data);
			k_sleep(K_MSEC(10));

			uart_send(UART_2, file_data, sizeof(file_data));
			uart_send(UART_2, ";", sizeof(";"));
		}
	}

	return 0;
}

// Create absolute path for filename
static void create_file_path(char *file_path, char *filename)
{

	// printk("\nCreating file path");

	// empty file_path
	strcpy(file_path, "");

	strcat(file_path, disk_mount_pt);
	strcat(file_path, "/");
	strcat(file_path, filename);
}

// reads entire file
static int read_file(char *file_path, char *data, int size)
{

	// For catching return values from fs_functions
	int ret = 1;

	struct fs_file_t file;
	ret = fs_open(&file, file_path, FS_O_READ);

	// Read characters until end of file

	if (ret == 0)
	{
		uint8_t buffer[32] = "";
		while (1)
		{
			ret = fs_read(&file, &buffer, 31);
			if (ret == 0)
				break;

			//printk("%s", buffer);
			//strcat(buffer, ";");
			uart_send(UART_2, buffer, sizeof(buffer));
			uart_send(UART_2, ";", sizeof(";"));

			memset(buffer, 0, sizeof(buffer));
			k_sleep(K_MSEC(20));
		}
		k_sleep(K_MSEC(10));
		uart_send(UART_2, "EOF", sizeof("EOF"));
		uart_send(UART_2, ";", sizeof(";"));

		printk("\n\nFinished reading file");

		fs_close(&file);
	}
	else
	{
		printk("Error in reading file\n");
	}

	return 0;
}

// reads per JSON
static int read_JSON(char *file_path, char *data, int size, uint32_t *cursor)
{

	// For catching return values from fs_functions
	int ret = 1;

	// Open file for reading, if file doesnt exist, create one.
	struct fs_file_t file;
	ret = fs_open(&file, file_path, FS_O_READ);

	if (ret == 0)
	{
		// TODO: seek end to read cursor value

		fs_seek(&file, *cursor, FS_SEEK_SET);

		uint8_t buffer[8] = "";

		while (1)
		{
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
				// printk("\n%s %d", data, *cursor);
				return 0;
			}
			else
			{
				strcat(data, buffer);
			}
		}

		fs_close(&file);
	}
	else
	{
		printk("Error in reading file\n");
	}

	return 0;
}

// write into file, creates new file if it doesn't exist
static int write_file(char *file_path, char *data, int size)
{
	struct fs_file_t file;
	fs_open(&file, file_path, (FS_O_WRITE | FS_O_APPEND | FS_O_CREATE));
	fs_write(&file, data, size);
	fs_write(&file, "\n", strlen("\n"));
	fs_close(&file);

	return 0;
}

static int mountSD()
{

	static const char *disk_pdrv = "SD";
	uint64_t memory_size_mb;
	uint32_t block_count;
	uint32_t block_size;

	do
	{
		if (disk_access_init(disk_pdrv) != 0)
		{
			printk("Storage init ERROR!");
			break;
		}

		if (disk_access_ioctl(disk_pdrv,
							  DISK_IOCTL_GET_SECTOR_COUNT, &block_count))
		{
			printk("Unable to get sector count");
			break;
		}
		printk("Block count %u", block_count);

		if (disk_access_ioctl(disk_pdrv,
							  DISK_IOCTL_GET_SECTOR_SIZE, &block_size))
		{
			printk("Unable to get sector size");
			break;
		}
		printk("Sector size %u\n", block_size);

		memory_size_mb = (uint64_t)block_count * block_size;
		printk("Memory Size(MB) %u\n", (uint32_t)(memory_size_mb >> 20));

		mp.mnt_point = disk_mount_pt;

		int res = fs_mount(&mp);

		if (res == FR_OK)
		{
			printk("Disk mounted.\n");
			lsdir(disk_mount_pt);
		}
		else
		{
			printk("Error mounting disk.\n");
			return res;
		}
	} while (0);

	return 0;
}

void app_sd_thread(void *unused1, void *unused2, void *unused3)
{
	oasys_data_t sd_msg;

	char file_path[32] = "";

	uint8_t file_text[1024] = "";
	uint8_t json_text[128] = "";
	uint32_t cursor = 0;

	mountSD();

	while (1)
	{
		// get msg from main
		k_msgq_get(&sd_msg_q, &sd_msg, K_FOREVER);

		switch (sd_msg.event)
		{
		case WRITE_FILE:
			// create file path
			create_file_path(file_path, sd_msg.filename);
			write_file(file_path, sd_msg.json_string, strlen(sd_msg.json_string));

			break;
		case FIND_FILE:
			// find specified file
			look_for_file(disk_mount_pt, sd_msg.filename);

			break;
		case SEND_FILE_INFO:
			send_all_file_info(disk_mount_pt);

			break;
		case READ_JSON:
			// create file path
			create_file_path(file_path, sd_msg.filename);
			read_JSON(file_path, json_text, sizeof(json_text), &cursor);
			printk("\nJSON: %s, cursor: %d", json_text, cursor);

			break;
		case READ_FILE:
			printk("\nReading file: %s", sd_msg.filename);
			// create file path
			create_file_path(file_path, sd_msg.filename);
			read_file(file_path, file_text, sizeof(file_text));
			// printk("\nFile content:\n\n%s", file_text);

			break;
		default:
			printk("\nUnknown SD event type");

		} // switch case end
	}
}
