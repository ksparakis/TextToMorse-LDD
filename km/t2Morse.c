/* Necessary includes for device drivers */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h> /* printk() */
#include <linux/slab.h> /* kmalloc() */
#include <linux/fs.h> /* everything... */
#include <linux/errno.h> /* error codes */
#include <linux/types.h> /* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h> /* O_ACCMODE */
#include <asm/system.h> /* cli(), *_flags */
#include <asm/uaccess.h> /* copy_from/to_user */
#include <linux/timer.h> 
#include <linux/jiffies.h>
#include <linux/delay.h>
#include <linux/gpio.h> //    lib/MODULES/'uname -r'/build/include
                                // 3.12.somethingg

MODULE_LICENSE("Dual BSD/GPL");


//declaring all functions
static int morse_open(struct inode *inode, struct file *filp);
static int morse_release(struct inode *inode, struct file *filp);
static ssize_t morse_list(struct file *filp,char *buf, size_t count, loff_t *f_pos);
static ssize_t morse_create(struct file *filp,const char *buf, size_t count, loff_t *f_pos);
static void morse_exit(void);
static void inletter(void);
static void dash(void);
static int morse_init(void);
static void dot(void);
static void gpioW(int value);


#define COUNT 100
#define TIME 100 //Time T in milliseconds for time values
//const char* GPIO_OUT_PIN = 17; //CHANGE THIS TO CHANGE OUT PUT GPIO PIN FOR LED
int result;
int dot_on_time = TIME;
int dash_on_time = 3*TIME;
int off_in_letter = TIME;
int between_letters =3*TIME;
int between_words = 7*TIME;

struct file_operations morse_fops = {
	read: morse_list,
	write: morse_create,
	open: morse_open,
	release: morse_release
};



/* Declaration of the init and exit functions */
module_init(morse_init);
module_exit(morse_exit);




static unsigned capacity = 128;
static unsigned bite = 128;
module_param(capacity, uint, S_IRUGO);
module_param(bite, uint, S_IRUGO);


/* Global variables of the driver */
/* Major number */
static int morse_major = 60;

/* Buffer to store data */
static char *morse_buffer;
/* length of the current message */
static int morse_len;
char my_msg[1024];


static void inletter()
{
    gpioW(0);
    msleep(off_in_letter);
}
static void gpioW(int value)
{
	 gpio_set_value(17,value);
    //fprintf(fvalue,"%d",value);
    //fflush(fvalue);
}

static void dot()
{
    gpioW(1);
    msleep(dot_on_time);
}

static void dash()
{
    gpioW(0);
    msleep(dash_on_time);
}



// Executes translation to morse code
// inputs [String to morse]
static ssize_t morse_create(struct file *filp, const char *buf, size_t count, loff_t *f_pos) {
	//get data from user level....
	

	int temp;
	char tbuf[256], *tbptr = tbuf;
	char inputString[256];
    int r=0;
    int i;
    char msg[2500];
    int initialer;

	/* end of buffer reached */
	if (*f_pos >= capacity)
	{
		printk(KERN_INFO"write called: buffer full\n");
		return -ENOSPC;
	}

	/* do not eat more than a bite */
	if (count > bite) count = bite;

	/* do not go over the end */
	if (count > capacity - *f_pos)
		count = capacity - *f_pos;

	if (copy_from_user(morse_buffer + *f_pos, buf, count))
	{
		return -EFAULT;
	}



	for (temp = *f_pos; temp < count + *f_pos; temp++)
	{					  

			tbptr += sprintf(tbptr, "%c", morse_buffer[temp]);
			inputString[r] = morse_buffer[temp];
			r++;
	}
	inputString[r] = '\0';

	//printk(KERN_INFO "name = %s \n",inputString);
	//split inputname into just name and time
	//sscanf(inputString,"%lx %[^\n]s", &l, &inputName);
	sscanf(inputString,"%s",msg);

    strcpy(my_msg,msg);
    
    // Sleep time values
    //value = 0;
    
    
    //initiialer makes sure that the first time running through there is no sleep
    initialer =0;
    
    
    //loops through each character and plays appropriate response
    //using while loop because length of msg is not known and this will loop until null is detected
    i=0;
    while(msg[i]){
        
        
        if(msg[i]==' ')
        {
            //does nothing if space this is taken care of in switch statement
        }
        else if(initialer ==0)
        {
            //does nothing if first time running loop
        }
        else{
            // sleeps bettween letters
            msleep(between_letters);
        }
        initialer =1;
        printk(KERN_ALERT "Blinking Letter: %c\n",msg[i]);
        
        switch(msg[i])
        {
            case' ':
                gpioW(0);
                msleep(between_words);
                break;
                
            case 'a': case 'A':
                dot();
                inletter();
                dash();
                
                break;
                
            case 'b': case'B':
                dash();
                inletter();
                dot();
                inletter();
                dot();
                inletter();
                dot();
                break;
                
            case 'c':case'C':
                dash();
                inletter();
                dot();
                inletter();
                dash();
                inletter();
                dot();
                break;
                
            case 'd':case'D':
                dash();
                inletter();
                dot();
                inletter();
                dot();
                break;
                
            case 'e':case'E':
                dot();
                break;
                
            case 'f':case'F':
                dot();
                inletter();
                dot();
                inletter();
                dash();
                inletter();
                dot();
                break;
                
            case 'g':case'G':
                dash();
                inletter();
                dash();
                inletter();
                dot();
                break;
                
            case 'h':case'H':
                dot();inletter();dot();inletter();dot();inletter();dot();
                break;
                
            case 'i':case'I':
                dot();inletter();dot();
                break;
                
            case 'j':case'J':
                dot();inletter();dash();inletter();dash();inletter();dash();
                break;
                
            case'k':case'K':
                dash();inletter();dot();inletter();dash();
                break;
                
            case'l':case'L':
                dot();inletter();dash();inletter();dot();inletter();dot();
                break;
                
            case'm':case'M':
                dash();inletter();dash();
                break;
                
            case'n':case'N':
                dash();inletter();dot();
                break;
                
            case'o':case'O':
                dash();inletter();dash();inletter();dash();
                break;
                
            case'p':case'P':
                dot();inletter();dash();inletter();dash();inletter();dot();
                break;
                
            case'q':case'Q':
                dash();inletter();dash();inletter();dot();inletter();dash();
                break;
                
            case'r':case'R':
                dot();inletter();dash();inletter();dot();
                break;
                
            case's':case'S':
                dot();inletter();dot();inletter();dot();
                break;
                
            case't':case'T':
                dash();
                break;
                
            case'u':case'U':
                dot();inletter();dot();inletter();dash();
                break;
                
            case'v':case'V':
                dot();inletter();dot();inletter();dot();inletter();dash();
                break;
                
            case'w':case'W':
                dot();inletter();dash();inletter();dash();
                break;
                
            case'x':case'X':
                dash();inletter();dot();inletter();dot();inletter();dash();
                break;
                
            case'y':case'Y':
                dash();inletter();dot();inletter();dash();inletter();dash();
                break;
                
            case'z':case'Z':
                dash();inletter();dash();inletter();dot();inletter();dot();
                break;
                
            default:
                printk(KERN_ALERT "Error: Char %c is not defined.\n", msg[i]);
                break;
        }
        i++;	
    }
    dash();
  	

	*f_pos += count;
	morse_len = *f_pos;

	return count;
}


//Prints out timers for -l command
static ssize_t morse_list(struct file *filp,char *buf, size_t count, loff_t *f_pos)
{
    

	char tbuf[1024], *tbptr = tbuf;

	
    memset(&tbuf[0], 0, sizeof(tbuf));
	/* end of buffer reached */
	if (*f_pos >= morse_len)
	{
		return 0;
	}

	/* do not go over then end */
	if (count > morse_len - *f_pos)
		count = morse_len - *f_pos;

	/* do not send back more than a bite */
	if (count > bite) count = bite;

	/* Transfering data to user space */ 
	if (copy_to_user(buf, morse_buffer + *f_pos, count))
	{
		return -EFAULT;
	}




		tbptr += sprintf(tbptr,	"Last Msg:%s\t Time Constant:%d\n", my_msg,dot_on_time);
			
			
	printk(KERN_INFO "\n%s\n", tbuf);
	*f_pos += count; 
	return count;//check this out
}



static int morse_open(struct inode *inode, struct file *filp)
{

	return 0;
}

static int morse_release(struct inode *inode, struct file *filp)
{

	return 0;
}



static int morse_init(void)
{
	
	/* Registering device */
	result = register_chrdev(morse_major, "t2Morse", &morse_fops);
	if (result < 0)
	{
		printk(KERN_ALERT
			"t2Morse: cannot obtain major number %d\n", morse_major);
		return result;
	}

	morse_buffer = kmalloc(capacity, GFP_KERNEL); 
	if (!morse_buffer)
	{ 
		printk(KERN_ALERT "Insufficient kernel memory\n"); 
		result = -ENOMEM;
		goto fail; 
	} 

	memset(morse_buffer, 0, capacity);
	morse_len = 0;

	printk(KERN_ALERT "Inserting t2Morse module\n");

	return 0;

fail: 
	morse_exit(); 
	return result;
}



/* 
int procfile_write(struct file *file, const char *buffer, unsigned long count,
           void *data)
{

    procfs_buffer_size = count;
    if (procfs_buffer_size > PROCFS_MAX_SIZE ) {
        procfs_buffer_size = PROCFS_MAX_SIZE;
    }
    
    
    if ( copy_from_user(procfs_buffer, buffer, procfs_buffer_size) ) {
        return -EFAULT;
    }
    
    return procfs_buffer_size;
}
*/


static void morse_exit(void)
{

	/* Freeing the major number */
	unregister_chrdev(morse_major, "morse");

	/* Freeing buffer memory */
	if (morse_buffer)
	{
		kfree(morse_buffer);
	}
    //remove_proc_entry("t2Morse", &proc_root);

	printk(KERN_ALERT "Removing morse module\n");

}
