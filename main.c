#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/fs.h>
#include <linux/seq_file.h>
#include <linux/slab.h>

static struct proc_dir_entry *entry; // holds the info about /proc file
static struct proc_dir_entry *prnt;
//
// we will define "/proc/try/show"
//
#define PROC_SUB_DIRECTORY "odev4"
#define PROC_ENTRY "durum"
#define MAX_MODULE_BUFFER 256

static char module_buffer[MAX_MODULE_BUFFER];
static unsigned long module_message_len = 0;  // size of buffer
static int offt=1000;

//
// The function is called when /proc/try/show is read
//
void reverse(char str[], int length)
{
    int start = 0;
    int end = length -1;
    while (start < end)
    {
        swap(*(str+start), *(str+end));
        start++;
        end--;
    }
}
// Implementation of itoa()   int to string
char* itoa(int num, char* str, int base)
{
    int i = 0;
    bool isNegative = false;

    /* Handle 0 explicitly, otherwise empty string is printed for 0 */
    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }
    // In standard itoa(), negative numbers are handled only with
    // base 10. Otherwise numbers are considered unsigned.
    if (num < 0 && base == 10)
    {
        isNegative = true;
        num = -num;
    }

    // Process individual digits
    while (num != 0)
    {
        int rem = num % base;
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
        num = num/base;
    }

    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';

    str[i] = '\0'; // Append string terminator

    // Reverse the string
    reverse(str, i);

    return str;
}
int myAtoi(const char* str) // string to int
{
    int sign = 1, base = 0, i = 0;

    // if whitespaces then ignore.
    while (str[i] == ' ')
    {
        i++;
    }

    // sign of number
    if (str[i] == '-' || str[i] == '+')
    {
        sign = 1 - 2 * (str[i++] == '-');
    }

    // checking for valid input
    while (str[i] >= '0' && str[i] <= '9')
    {
        // handling overflow test case
        if (base > INT_MAX / 10
            || (base == INT_MAX / 10
            && str[i] - '0' > 7))
        {
            if (sign == 1)
                return INT_MAX;
            else
                return INT_MIN;
        }
        base = 10 * base + (str[i++] - '0');
    }
    return base * sign;
}
static ssize_t module_file_read(struct file *file,char __user *buffer,size_t buf_len,loff_t *offset)
{
	if ( buf_len < MAX_MODULE_BUFFER || *offset > 0) {
		// finished

		return 0;
	}
// copy message to user space then buffer size is returned
	if(copy_to_user(buffer, module_buffer, module_message_len)) {
		return -EFAULT;
	}
	*offset = module_message_len;
	return module_message_len;
}

//
// The function is called when /proc/try/show is written to
//
static ssize_t module_file_write(struct file* file,const char __user *buffer,size_t msg_len,loff_t *f_pos)
{
	char *tmp_buf;
	int cpy_len;
	// kernel string allocation
	if(!(tmp_buf = kzalloc((msg_len+1),GFP_KERNEL))) {  // not enough memory
		return -ENOMEM;
		}

	if(copy_from_user(tmp_buf, buffer, msg_len+1)){
		kfree(tmp_buf);
		return EFAULT;
	}

	// calculating the number of bytes that our buffer can hold
	offt = offt+myAtoi(tmp_buf);
	itoa(offt,tmp_buf,10); // copy value to tmp_buf as string

	cpy_len = (msg_len < MAX_MODULE_BUFFER) ? msg_len+2 : MAX_MODULE_BUFFER+1;
	memcpy(&module_buffer,tmp_buf,cpy_len); // copy temporary buffer to module_buffer
	kfree(tmp_buf); // we no longer need
	module_message_len = cpy_len;
	return cpy_len;
}

static int module_file_durum(struct seq_file *m,void *v){
    static char *str = NULL;
    seq_printf(m,"hello %s\n",str);
    return 0;
}

// this function is called when entry is opened by a user app
static int module_file_open(struct inode *inode,struct file *file)
{
	return single_open(file,module_file_durum,NULL);
}
static struct proc_ops fops = {
	.proc_lseek = seq_lseek,
	.proc_open = module_file_open,
	.proc_read = module_file_read,
	.proc_release = single_release,
	.proc_write = module_file_write,
};
static int __init module_init_func(void)
{
	printk(KERN_INFO "Try is started!!\n");
	// make a directory at /proc with name procfs
	prnt = proc_mkdir(PROC_SUB_DIRECTORY,NULL);

	// under /proc/procfs we will create a directory entry with name show
	if((entry = proc_create(PROC_ENTRY, 0777, prnt, &fops))) {
	return 0;
	}
	return -1;
}

static void __exit module_exit_func(void)
{
	remove_proc_entry(PROC_ENTRY, prnt);
	remove_proc_entry(PROC_SUB_DIRECTORY, NULL);
	printk(KERN_INFO "Try is exiting!\n");
}

module_init(module_init_func);
module_exit(module_exit_func);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kerem Sarı");
MODULE_DESCRIPTION("Example for /proc file system");
