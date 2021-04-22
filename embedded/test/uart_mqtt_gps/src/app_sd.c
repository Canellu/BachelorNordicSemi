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
#include <stdlib.h>
#include <cJSON.h>

#include "app_uart.h"
#include "app_sd.h"
// #include "cJSON.h"

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
static const int json_size = 50; // rough length (bytes) of each json object

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
			// printk("%s\n", entry.name);

			if (strcmp(entry.name, filename) == 0)
			{
				res = entry.size;
				break;
			}
		}
	}

	/* Verify fs_closedir() */
	fs_closedir(&dirp);

	return res;
}

static int send_all_file_info(const char *path)
{
	int res;
	struct fs_dir_t dirp;
	static struct fs_dirent entry;

	uint8_t file_data[128] = "";
	uint8_t temp_str[16] = "";

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
			uart_send(UART_2, "\r", sizeof("\r"));
		}
	}

	return 0;
}

// Create absolute path for filename
static void create_file_path(char *file_path, char *filename)
{

	// empty file_path
	strcpy(file_path, "");

	strcat(file_path, disk_mount_pt);
	strcat(file_path, "/");
	strcat(file_path, filename);
}

// reads entire file
static int read_file(char *file_path)
{

	// For catching return values from fs_functions
	int ret = 1;

	struct fs_file_t file;
	ret = fs_open(&file, file_path, FS_O_READ);

	// Read characters until end of file

	if (ret == 0)
	{
		uint8_t line[512] = "";
		while (1)
		{
			uint8_t buf[2] = "";
			ret = fs_read(&file, &buf, 1);

			// test for EOF
			if (ret == 0)
				break;
			// handling overflow
			else if (strlen(line) >= sizeof(line) - 1)
			{
				printk("overflow, deleting str\n");
				memset(line, 0, sizeof(line));
			}
			// delimiter
			else if (buf[0] == '\r')
			{
				printk("line: %s %d\n", line, strlen(line));
				memset(line, 0, sizeof(line));
			}
			// filter for unwanted characters
			else if (buf[0] == '\n')
			{
			}
			// add to string
			else
			{
				strcat(line, buf);
			}

			// //printk("%s", buffer);
			// //strcat(buffer, ";");
			// uart_send(UART_2, buffer, sizeof(buffer));
			// uart_send(UART_2, "\r", sizeof("\r"));

			// memset(buffer, 0, sizeof(buffer));
			// k_sleep(K_MSEC(20));
		}
		k_sleep(K_MSEC(10));
		// uart_send(UART_2, "EOF", sizeof("EOF"));
		// uart_send(UART_2, "\r", sizeof("\r"));

		printk("\nFinished reading file");

		fs_close(&file);
	}
	else
	{
		printk("Error in reading file\n");
	}

	return 0;
}

// reads per JSON
static int read_JSON(char *file_path, uint32_t *cursor, int interval,
					 int *current_msg_total, int msg_total)
{
	int counter = 0;

	// interval = 30; // json_total_first / msg_max;

	printk("\nCursor: %d\n", *cursor);

	// For catching return values from fs_functions
	int ret = 1;

	// Open file for reading, if file doesnt exist, create one.
	struct fs_file_t file;
	ret = fs_open(&file, file_path, FS_O_READ);
	fs_seek(&file, *cursor, FS_SEEK_SET);

	if (ret == 0)
	{
		uint8_t line[512] = "";
		while (1)
		{
			uint8_t buf[2] = "";
			ret = fs_read(&file, &buf, 1);

			// test for EOF
			if (ret == 0)
			{
				(*cursor) = 0;
				break;
			}
			// handling overflow
			else if (strlen(line) >= sizeof(line) - 1)
			{
				printk("overflow, deleting str\n");
				memset(line, 0, sizeof(line));
			}
			// delimiter
			else if (buf[0] == '\r')
			{
				if ((counter % interval) == 0)
				{
					cJSON *line_JSON = cJSON_Parse(line);
					if (cJSON_IsObject(line_JSON))
					{
						// printk("\n%s", line);
						k_msgq_put(&main_msg_q, &line, K_NO_WAIT);
						(*current_msg_total)++;
					}
					memset(line, 0, sizeof(line));
					cJSON_Delete(line_JSON);

					if ((*current_msg_total) == msg_total)
					{
						(*current_msg_total) = 0;
						(*cursor) += 0;
						break;
					}
				}
				counter++;
			}
			// filter for unwanted characters
			else if (buf[0] == '\n')
			{
			}
			// add to string
			else
			{
				if ((counter % interval) == 0)
				{
					strcat(line, buf);
				}
			}
		}

		uint8_t sd_msg_response[] = "OK";
		k_msgq_put(&main_msg_q, &sd_msg_response, K_NO_WAIT);

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
	fs_write(&file, "\r\n", strlen("\r\n"));
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

static int copy_sd_msg(sd_msg_t *sd_msg_dest, sd_msg_t *sd_msg_source)
{
	sd_msg_dest->event = sd_msg_source->event;
	strcpy(sd_msg_dest->filename, sd_msg_source->filename);
	strcpy(sd_msg_dest->string, sd_msg_source->string);

	return 0;
}

void app_sd_thread(void *unused1, void *unused2, void *unused3)
{
	int ret = 0;

	sd_msg_t sd_msg_prev;
	sd_msg_t sd_msg;

	char file_path[32] = "";

	// helper variables for event read_JSON
	size_t size_total_prev = 0;
	size_t size_total = 0;
	uint32_t cursor = 0;
	int msg_total = 0;
	int current_msg_total = 0;
	int interval = 1;

	uint8_t filenames[8][16] = {""};
	uint8_t tmp_str[16];

	// counters
	int sd_msg_arr = 0;
	int tmp_str_arr = 0;
	int filenames_arr = 0;
	int i = 0;

	mountSD();

	while (1)
	{
		// get msg from main
		k_msgq_get(&sd_msg_q, &sd_msg, K_FOREVER);

		// printk("\n%s %s\n", sd_msg.filename, sd_msg.string);

		switch (sd_msg.event)
		{
		case WRITE_FILE:
			// create file path
			create_file_path(file_path, sd_msg.filename);
			write_file(file_path, sd_msg.string, strlen(sd_msg.string));

			break;
		case FIND_FILE:

			// find specified file
			ret = look_for_file(disk_mount_pt, sd_msg.filename);
			if (ret >= 0)
			{
				k_msgq_put(&main_msg_q, &ret, K_NO_WAIT);
			}
			else
			{
				printk("File not found\n");
			}

			break;
		case SEND_FILE_INFO:
			send_all_file_info(disk_mount_pt);

			break;
		case READ_JSON:
			// reset params
			size_total = 0;
			sd_msg_arr = 0;
			tmp_str_arr = 0;
			filenames_arr = 0;
			i = 0;

			// parse string into parameters and filenames
			for (sd_msg_arr = 0; sd_msg_arr < strlen(sd_msg.string); sd_msg_arr++)
			{
				// msg total on delim
				if (sd_msg.string[sd_msg_arr] == '\r' && i == 0)
				{
					tmp_str[tmp_str_arr] = 0;
					// printk("\n%s", tmp_str);
					msg_total = strtol(tmp_str, NULL, 10);

					memset(tmp_str, 0, sizeof(tmp_str));
					tmp_str_arr = 0;
					i++;
				}
				// filenames on delim
				else if (sd_msg.string[sd_msg_arr] == '\r' && i > 0)
				{
					tmp_str[tmp_str_arr] = 0;
					// printk("\n%s", tmp_str);
					strcpy(filenames[filenames_arr++], tmp_str);

					memset(tmp_str, 0, sizeof(tmp_str));
					tmp_str_arr = 0;
					i++;
				}
				// appending character
				else
				{
					tmp_str[tmp_str_arr++] = sd_msg.string[sd_msg_arr];
				}
			}

			i = 0;
			while (strcmp(filenames[i], "") != 0)
			{
				size_total += look_for_file(disk_mount_pt, filenames[i]);
				i++;
			}
			// how many "jumps" between messages
			interval = ((size_total - size_total_prev) / json_size) / msg_total;
			// guard in case interval is rounded to zero.
			if (!interval)
			{
				interval = 1;
			}
			// printk("\n%d", interval);

			i = 0;
			while (strcmp(filenames[i], "") != 0)
			{
				create_file_path(file_path, filenames[i]);
				printk("\n%s", file_path);
				read_JSON(file_path, &cursor, interval, &current_msg_total, msg_total);
				i++;
			}
			size_total_prev = size_total;

			break;
		case READ_FILE:
			printk("\nReading file: %s\n", sd_msg.filename);
			// create file path
			create_file_path(file_path, sd_msg.filename);
			read_file(file_path);
			// printk("\nFile content:\n\n%s", file_text);

			break;
		default:
			printk("\nUnknown SD event type");

		} // switch case end
		copy_sd_msg(&sd_msg_prev, &sd_msg);
	}
}
