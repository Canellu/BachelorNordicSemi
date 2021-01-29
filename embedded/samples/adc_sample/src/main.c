#include <stdio.h>
#include <drivers/adc.h>
#include <zephyr.h>
#include <hal/nrf_saadc.h>

#define ADC_DEVICE_NAME "ADC_0"
#define ADC_RESOLUTION 10
#define ADC_GAIN ADC_GAIN_1_6
#define ADC_REFERENCE ADC_REF_INTERNAL
#define ADC_ACQUISITION_TIME ADC_ACQ_TIME(ADC_ACQ_TIME_MICROSECONDS, 10)
#define ADC_1ST_CHANNEL_ID 0
#define ADC_1ST_CHANNEL_INPUT NRF_SAADC_INPUT_AIN0


const struct device *adc_dev;

static const struct adc_channel_cfg m_1st_channel_cfg = {
	.gain = ADC_GAIN,
	.reference = ADC_REFERENCE,
	.acquisition_time = ADC_ACQUISITION_TIME,
	.channel_id = ADC_1ST_CHANNEL_ID,
#if defined(CONFIG_ADC_CONFIGURABLE_INPUTS)
	.input_positive = ADC_1ST_CHANNEL_INPUT,
#endif
};

#define BUFFER_SIZE 1
static int16_t m_sample_buffer[BUFFER_SIZE];

static int adc_sample(void)
{
	int err;

	const struct adc_sequence sequence = {
		.channels = BIT(ADC_1ST_CHANNEL_ID),
		.buffer = m_sample_buffer,
		.buffer_size = sizeof(m_sample_buffer),
		.resolution = ADC_RESOLUTION,
	};

	if (!adc_dev) {
		return -1;
	}


	err = adc_read(adc_dev, &sequence);
	if(err) {
		printk("ADC read err: %d\n", err);
	}
	


	/* Print the AIN0 values */
	for (int i = 0; i < BUFFER_SIZE; i++) {
		
		
		
		printk("ADC raw value: %d\n" , m_sample_buffer[i]);
		
		//TODO: CONVERT ALGORITHM FROM RAW!
		
		// err = adc_raw_to_millivolts(ADC_REFERENCE, ADC_GAIN, ADC_RESOLUTION, &m_sample_buffer[i]);
		// printf("Measured voltage: %d mV\n", m_sample_buffer[i]);

	}

	return err;
}



int main(void)
{
	int err;

	printk("******** Sampling from P0.13 test ********\n\n\n");
	

	adc_dev = device_get_binding(ADC_DEVICE_NAME);
	if (!adc_dev) {
		printk("device_get_binding ADC_0 failed\n");
	}
	
	err = adc_channel_setup(adc_dev, &m_1st_channel_cfg);
	if (err) {
		printk("Error in adc setup: %d\n", err);
	}

	while (1) {
		err = adc_sample();
		if (err) {
			printk("Error in adc sampling: %d\n", err);
		}
		k_msleep(500);

	}
}