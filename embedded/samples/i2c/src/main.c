
#include <zephyr.h>
#include <drivers/i2c.h>


void main(void)
{
	const struct device *i2c_dev;
	
	k_sleep(K_MSEC(500));

	printk("Starting i2c scanner...\n");

	i2c_dev = device_get_binding("I2C_2");
	if (!i2c_dev) {
		printk("I2C: Device driver not found.\n");
		return;
	}
	
	uint8_t error = 0u;
	
	i2c_configure(i2c_dev, I2C_SPEED_SET(I2C_SPEED_STANDARD));

	for (uint8_t i = 4; i <= 0x77; i++) {
		struct i2c_msg msgs[1];
		uint8_t dst = 1;

		/* Send the address to read from */
		msgs[0].buf = &dst;
		msgs[0].len = 1U;
		msgs[0].flags = I2C_MSG_WRITE | I2C_MSG_STOP;
		
		error = i2c_transfer(i2c_dev, &msgs[0], 1, i);
		if (error == 0) {
			printk("0x%2x FOUND\n", i);
		}
		else {
			printk("error %d \n", error);
		}
	
	}

	// uint8_t addr = 4;
	// 	for (uint8_t i = 2; i <= 0x77; i++)
	// 	{
	// 		uint8_t buf[] = {"test"};

	// 		error = i2c_write(i2c_dev, buf, sizeof(buf), i);
	// 		if (error == 0) {
	// 			printk("device found");
	// 		}
	// 		else {
	// 			printk("error %d \n", error);
	// 		}
	// 	}
	
}