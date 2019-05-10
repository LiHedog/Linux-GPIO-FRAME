#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <asm/arch/regs-gpio.h>
#include <asm/hardware.h>

/* 创建一个类 */
static struct class *firstdrv_class;
/* 创建一个设备类 */
static struct class_device	*firstdrv_class_dev;
/* GPIO 配置寄存器 */
volatile unsigned long *gpbcon = NULL;
/* GPIO 配置数据器 */
volatile unsigned long *gpiodat = NULL;

/* gpio 驱动 open 函数，与应用层的 open 函数逻辑相关 */
static int gpio_drv_open(struct inode *inode, struct file *file)
{
	/*
	 * 根据 datasheet 配置 gpiocon
	 * 设置为 输入 | 输出 模式
	 */
	*gpiocon &= ;
	*gpiocon |= ;
	return 0;
}

/* gpio 驱动 write 函数，与应用层的 write 函数逻辑相关 */
static ssize_t gpio_drv_write(struct file *file, const char __user *buf, size_t count, loff_t * ppos)
{
	int val;

	//printk("gpio_drv_write\n");

	/*
	 * 用户于应用程序传 val 参数进驱动程序
	 */
	copy_from_user(&val, buf, count); //	copy_to_user();

	if (val == 1)
	{
		// 根据 datasheet 置 1 | 0
		*gpiodat &= ;
	}
	else
	{
		// 根据 datasheet 置 1 | 0
		*gpiodat |= ;
	}
	
	return 0;
}

/*
 * 应用层与驱动层链接通道 
 */
static struct file_operations gpio_drv_fops = {
	 /* 这是一个宏，推向编译模块时自动创建的__this_module变量 */
    .owner  =   THIS_MODULE,   
    .open   =   gpio_drv_open,     
	.write	=	gpio_drv_write,	   
};

//主设备号
int major;

//驱动函数入口
static int gpio_drv_init(void)
{
	// 注册, 告诉内核，返回主设备号
	major = register_chrdev(0, "gpio_drv", &gpio_drv_fops); 
	//创建类
	gpiodrv_class = class_create(THIS_MODULE, "gpiodrv");
	//创建设备类
	gpiodrv_class_dev = class_device_create(gpiodrv_class, NULL, MKDEV(major, 0), NULL, "/dev/gpio"); 
	//映射内存
	gpiocon = (volatile unsigned long *)ioremap(gpbcon_address, sizeof(gpbcon_address));
	gpiodat = gpiocon + 1;

	return 0;
}

//驱动函数出口
static void gpio_drv_exit(void)
{
	// 卸载
	unregister_chrdev(major, "gpio_drv"); 
	class_device_unregister(gpiodrv_class_dev);
	class_destroy(gpiodrv_class);
	iounmap(gpiocon);
}

//链接内核
module_init(gpio_drv_init);
module_exit(gpio_drv_exit);


MODULE_LICENSE("GPL");

