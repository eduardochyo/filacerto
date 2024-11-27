
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/__assert.h>
#include <string.h>

/* size of stack area used by each thread */
#define STACKSIZE 1024

/* scheduling priority used by each thread */
#define PRIORITY 7

K_FIFO_DEFINE(printk_fifo);

    struct data_item_t  {
	void *fifo_reserved; /* 1st word reserved for use by fifo */
	uint32_t valor;
	};
//
struct data_item_t tx_data1;
struct data_item_t tx_data2;

void uart_out(int id)
{
	if (id == 0){
		tx_data1.valor = 45;
		k_fifo_put(&printk_fifo, &tx_data1);
		printk("id 0 \n");
	}
	if (id == 1){
		tx_data2.valor = 37;
		k_fifo_put(&printk_fifo, &tx_data2);
		printk("id 1 \n");
		}
	}



void uart_in(int id)
{
		if(id == 0){
		struct data_item_t *rx_data = k_fifo_get(&printk_fifo, K_FOREVER);
		printk("id 0 chamado\n");
		printk("Received data: %d id 0 \n", rx_data->valor);
		//k_free(rx_data);
		}
		if(id == 1){
		printk("id 1 chamado\n");
		struct data_item_t *rx_data = k_fifo_get(&printk_fifo, K_FOREVER);
		printk("Received data: %d  id 1  \n", rx_data->valor);
		//k_free(rx_data);
		}
}
void writer0 (void){
	while(1){
	uart_out(0);
	k_msleep(1000);
	}
}
void writer1 (void){
	while(1){
	uart_out(1);
	k_msleep(1000);
	}
}
void reader0 (void){
	while(1){
	uart_in(0);
	k_msleep(1000);
	}
	
}
void reader1 (void){
	while(1){
	uart_in(1);
	k_msleep(1000);
	}
}



K_THREAD_DEFINE(uart_in_0, STACKSIZE, reader0, NULL, NULL, NULL,
		4, 0, 0);
K_THREAD_DEFINE(uart_in_1, STACKSIZE, reader1, NULL, NULL, NULL,
		3, 0, 0);
K_THREAD_DEFINE(uart_out_0, STACKSIZE, writer0, NULL, NULL, NULL,
		1, 0, 0);
K_THREAD_DEFINE(uart_out_1, STACKSIZE, writer1, NULL, NULL, NULL,
		2, 0, 0);
