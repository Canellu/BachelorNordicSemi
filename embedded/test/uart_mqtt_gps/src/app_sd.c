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
#include <logging/log.h>
#include <cJSON.h>

#include "app_button.h"
#include "app_uart.h"
#include "app_sd.h"

LOG_MODULE_REGISTER(app_sd);

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
static const int json_size = 70; // rough length (bytes) of each json object
								 // larger value -> lower interval
static int lsdir(const char *path)
{
	int res;
	struct fs_dir_t dirp;
	static struct fs_dirent entry;

	/* Verify fs_opendir() */
	res = fs_opendir(&dirp, path);
	if (res)
	{
		LOG_ERR("Error opening dir %s [%d]", path, res);
		set_LED(21, 0);
		return res;
	}

	LOG_INF("Listing dir %s ...", log_strdup(path));
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
			LOG_INF("[DIR ] %s", log_strdup(entry.name));
		}
		else
		{
			LOG_INF("[FILE] %s (size = %zu)",
					log_strdup(entry.name), entry.size);
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
		LOG_ERR("Error opening dir %s [%d]", log_strdup(path), res);
		set_LED(21, 0);
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
			// LOG_INF("%s", log_strdup(entry.name));

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
	uint8_t tmp_str[16] = "";

	res = fs_opendir(&dirp, path);
	if (res)
	{
		LOG_ERR("Error opening dir %s [%d]", log_strdup(path), res);
		set_LED(21, 0);
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
			snprintf(tmp_str, sizeof(tmp_str), ":%u", entry.size);
			strcat(file_data, tmp_str);

			LOG_INF("%s", log_strdup(file_data));
			k_sleep(K_MSEC(10)); // TODO

			uart_send(UART_2, file_data, strlen(file_data));
		}
	}

	/* Verify fs_closedir() */
	fs_closedir(&dirp);

	return 0;
}

// Create absolute path for filename
static int create_file_path(char *file_path, char *filename)
{
	if (strlen(filename) != 0)
	{
		// empty file_path
		strcpy(file_path, "");

		strcat(file_path, disk_mount_pt);
		strcat(file_path, "/");
		strcat(file_path, filename);

		// LOG_INF("file path: %s", log_strdup(file_path));
	}
	else
	{
		LOG_ERR("filename empty, unable to create file path");
		return -1;
	}

	return 0;
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
		uint8_t line[256] = "";
		while (1)
		{
			uint8_t buf[2] = "";
			ret = fs_read(&file, &buf, 1);

			// test for EOF
			if (ret == 0)
			{
				// if any remaining strings, send
				if (strlen(line) != 0)
				{
					uart_send(UART_2, line, strlen(line));
					memset(line, 0, sizeof(line));
				}
				break;
			}
			// delimiter
			else if (buf[0] == '\r')
			{
				// LOG_INF("line: %s %d", log_strdup(line), strlen(line));
				uart_send(UART_2, line, strlen(line));
				memset(line, 0, sizeof(line));
			}
			// if buffer full, send
			else if (strlen(line) >= sizeof(line) - 3)
			{
				strcat(line, buf);
				uart_send(UART_2, line, strlen(line));
				memset(line, 0, sizeof(line));
			}
			// add to string
			else
			{
				strcat(line, buf);
			}

			// //LOG_INF("%s", log_strdup(buffer));
			// //strcat(buffer, ";");
			// uart_send(UART_2, buffer, strlen(buffer));

			// memset(buffer, 0, sizeof(buffer));
			// k_sleep(K_MSEC(20));
		}
		k_sleep(K_MSEC(10)); // TODO
		uart_send(UART_2, "EOF", strlen("EOF"));

		LOG_INF("Finished reading file");

		fs_close(&file);
	}
	else
	{
		LOG_ERR("Error in reading file");
		set_LED(21, 0);
	}

	return 0;
}

// reads JSONs up to requested total
static int read_JSON(char *file_path, int json_total)
{
	int counter = 0;

	// For catching return values from fs_functions
	int ret = 1;

	// Open file for reading, if file doesnt exist, create one.
	struct fs_file_t file;
	ret = fs_open(&file, file_path, FS_O_READ);

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
				break;
			}
			// handling overflow
			else if (strlen(line) >= sizeof(line) - 1)
			{
				LOG_INF("overflow, deleting str");
				memset(line, 0, sizeof(line));
			}
			// delimiter
			else if (buf[0] == '\r')
			{
				if (counter < json_total)
				{
					cJSON *line_JSON = cJSON_Parse(line);
					if (cJSON_IsObject(line_JSON))
					{
						// LOG_INF("%s", log_strdup(line));
						k_msgq_put(&main_msg_q, &line, K_NO_WAIT);
						counter++;
					}
					memset(line, 0, sizeof(line));
					cJSON_Delete(line_JSON);
				}
				else
				{
					break;
				}
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
		}

		fs_close(&file);
	}
	else
	{
		uint8_t sd_msg_response[] = "ERROR";
		k_msgq_put(&main_msg_q, &sd_msg_response, K_NO_WAIT);
		LOG_ERR("Error in reading JSONs");
		set_LED(21, 0);
	}

	return 0;
}

// reads JSON with more parameters specifically for 4G use, DO NOT USE ELSEWHERE
static int read_JSON_4G(char *file_path, uint8_t *param_str, size_t file_size)
{
	uint32_t cursor = 0;
	int counter = 0;
	int interval = 0;
	size_t file_part = 0;

	int msg_max = 0;
	int msg_cur = 0;
	int wp_cur = 0;

	if (strstr(param_str, "max:") != NULL)
	{
		char *ptr = strstr(param_str, "max:");
		static char *eptr;

		msg_max = strtol(ptr + 4, &eptr, 10);
		LOG_INF("message max: %d", msg_max);

		if (msg_max <= 0)
		{
			msg_max = 1;
		}
	}

	if (strstr(param_str, "wp_cur:") != NULL)
	{
		char *ptr = strstr(param_str, "wp_cur:");
		static char *eptr;

		wp_cur = strtol(ptr + 7, &eptr, 10);
		LOG_INF("waypoint: %d", wp_cur);

		if (wp_cur <= 0)
		{
			wp_cur = 1;
		}
	}

	file_part = file_size / wp_cur;

	interval = (file_part / json_size) / msg_max;
	if (interval <= 0)
	{
		interval = 1;
	}
	LOG_INF("interval: %d", interval);

	cursor = file_part - file_size;
	if (cursor < 0)
	{
		cursor = 0;
	}

	// For catching return values from fs_functions
	int ret = 1;

	// Open file for reading
	struct fs_file_t file;
	ret = fs_open(&file, file_path, FS_O_READ);

	if (ret == 0)
	{
		fs_seek(&file, cursor, FS_SEEK_SET);
		uint8_t line[512] = "";
		while (1)
		{
			uint8_t buf[2] = "";
			ret = fs_read(&file, &buf, 1);

			// test for EOF
			if (ret == 0)
			{
				LOG_INF("EOF");
				break;
			}
			// handling overflow
			else if (strlen(line) >= sizeof(line) - 1)
			{
				LOG_INF("overflow, deleting str");
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
						// LOG_INF("line: %s", log_strdup(line));
						k_msgq_put(&main_msg_q, &line, K_NO_WAIT);
						msg_cur++;
					}
					memset(line, 0, sizeof(line));
					cJSON_Delete(line_JSON);

					if (msg_cur == msg_max)
					{
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

		uint8_t sd_msg_response[] = "EOM";
		k_msgq_put(&main_msg_q, &sd_msg_response, K_NO_WAIT);

		fs_close(&file);
	}
	else
	{
		LOG_ERR("Error in reading JSON for 4G");
		uint8_t sd_msg_response[] = "ERROR";
		k_msgq_put(&main_msg_q, &sd_msg_response, K_NO_WAIT);
		set_LED(21, 0);
	}

	return 0;
}

// write into file, creates new file if it doesn't exist
static int write_file(char *file_path, char *data, int size)
{
	int ret = 1;
	static struct fs_file_t file;

	ret = fs_open(&file, file_path, (FS_O_WRITE | FS_O_APPEND | FS_O_CREATE));

	if (ret == 0)
	{
		fs_write(&file, data, size);
		fs_write(&file, "\r\n", strlen("\r\n"));
	}
	else
	{
		LOG_ERR("Error in writing to file");
		return ret;
	}

	fs_close(&file);

	return 0;
}

static int overwrite_file(char *file_path, char *data, int size)
{
	int ret = 1;
	struct fs_file_t file;

	// ret = fs_open(&file, file_path, 0);

	LOG_INF("Overwriting mission parameters");
	fs_unlink(file_path);

	ret = fs_open(&file, file_path, (FS_O_WRITE | FS_O_CREATE));

	if (ret == 0)
	{
		fs_write(&file, data, size);
		fs_write(&file, "\r\n", strlen("\r\n"));
	}
	else
	{
		LOG_ERR("Error writing new parameters");
		return ret;
	}
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
			LOG_ERR("Storage init ERROR!");
			set_LED(21, 0);
			break;
		}

		if (disk_access_ioctl(disk_pdrv,
							  DISK_IOCTL_GET_SECTOR_COUNT, &block_count))
		{
			LOG_ERR("Unable to get sector count");
			set_LED(21, 0);
			break;
		}
		LOG_INF("Block count %u", block_count);

		if (disk_access_ioctl(disk_pdrv,
							  DISK_IOCTL_GET_SECTOR_SIZE, &block_size))
		{
			LOG_ERR("Unable to get sector size");
			set_LED(21, 0);
			break;
		}
		LOG_INF("Sector size %u", block_size);

		memory_size_mb = (uint64_t)block_count * block_size;
		LOG_INF("Memory Size(MB) %u", (uint32_t)(memory_size_mb >> 20));

		mp.mnt_point = disk_mount_pt;

		int res = fs_mount(&mp);

		if (res == FR_OK)
		{
			LOG_INF("Disk mounted.");
			lsdir(disk_mount_pt);
		}
		else
		{
			LOG_ERR("Error mounting disk.");
			set_LED(21, 0);
			return res;
		}
	} while (0);

	return 0;
}

void app_sd_thread(void *unused1, void *unused2, void *unused3)
{
	int ret = 0;

	// sd_msg_t sd_msg_prev;
	sd_msg_t sd_msg;

	char file_path[32] = "";
	// helper variables for event READ_JSON_4G
	size_t file_size = 0;

	set_LED(21, 1);

	mountSD();

	while (1)
	{
		// get msg from main
		k_msgq_get(&sd_msg_q, &sd_msg, K_FOREVER);

		// LOG_INF("\n%s %s\n", log_strdup(sd_msg.filename), log_strdup(sd_msg.string));

		switch (sd_msg.event)
		{
		case WRITE_FILE:
			ret = create_file_path(file_path, sd_msg.filename);
			if (ret == 0)
			{
				write_file(file_path, sd_msg.string, strlen(sd_msg.string));
			}
			break;
		case OVERWRITE_FILE:
			ret = create_file_path(file_path, sd_msg.filename);
			if (ret == 0)
			{
				overwrite_file(file_path, sd_msg.string, strlen(sd_msg.string));
			}

			break;
		case FIND_FILE:
			ret = look_for_file(disk_mount_pt, sd_msg.filename);
			if (ret >= 0)
			{
				k_msgq_put(&main_msg_q, &ret, K_NO_WAIT);
			}
			else
			{
				LOG_ERR("Error locating file");
			}

			break;
		case SEND_FILE_INFO:
			send_all_file_info(disk_mount_pt);

			break;
		case READ_JSON:
			ret = create_file_path(file_path, sd_msg.filename);
			if (ret == 0)
			{
				read_JSON(file_path, 1);
			}
			break;
		case READ_JSON_4G:
			ret = create_file_path(file_path, sd_msg.filename);
			if (ret == 0)
			{
				file_size = look_for_file(disk_mount_pt, sd_msg.filename);
				read_JSON_4G(file_path, sd_msg.string, file_size);
			}
			break;
		case READ_FILE:
			ret = create_file_path(file_path, sd_msg.filename);
			if (ret == 0)
			{
				read_file(file_path);
			}
			break;
		default:
			LOG_ERR("Unknown SD event type");

		} // switch case end
		  // copy_sd_msg(&sd_msg_prev, &sd_msg);
	}
}
