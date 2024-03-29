#include <linux/module.h>
#include <asm/uaccess.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Luca Borzacchiello");

#define MODNAME "MAIL_SPOT_PROC_METADATA"

int get_users_mail_spot(int);
int get_max_mex_len_mail_spot(int);
int get_number_messages_mail_spot(int);

static struct proc_dir_entry *proc_parent;
static struct proc_dir_entry *get_users_count;
static struct proc_dir_entry *get_max_mex_len;
static struct proc_dir_entry *get_number_messages;

static ssize_t get_users_count_read(struct file *file, char *buf, size_t length, loff_t *offset)  {

    if (*offset > 255)
        return 0;

    int len, tot_len = 0, ret;
    char tmp[100];
    int i = *offset;
    for (; i<256; i++) {
        len = sprintf(tmp, "%d:\t%d\n", i, get_users_mail_spot(i));

        if (tot_len + len > length) {
            break;
        } else {
            ret = copy_to_user(buf+tot_len, tmp, len);
            if (ret != 0) {
                printk("%s: copy_to_user failed, pid=%d\n", MODNAME, current->pid);
                return -1;
            }
            tot_len += len;
        }
    }

    *offset = i;

    return tot_len;
}

static struct file_operations f_ops_get_users_count = {  
    .owner = THIS_MODULE,
    .read = get_users_count_read,
};

static ssize_t get_max_mex_len_read(struct file *file, char *buf, size_t length, loff_t *offset)  {

    if (*offset > 255)
        return 0;

    int len, tot_len = 0, ret;
    char tmp[100];
    int i = *offset;
    for (; i<256; i++) {
        len = sprintf(tmp, "%d:\t%d\n", i, get_max_mex_len_mail_spot(i));

        if (tot_len + len > length) {
            break;
        } else {
            ret = copy_to_user(buf+tot_len, tmp, len);
            if (ret != 0) {
                printk("%s: copy_to_user failed, pid=%d\n", MODNAME, current->pid);
                return -1;
            }
            tot_len += len;
        }
    }

    *offset = i;

    return tot_len;
}

static struct file_operations f_ops_get_max_mex_len = {  
    .owner = THIS_MODULE,
    .read = get_max_mex_len_read,
};

static ssize_t get_number_messages_mail_spot_read(struct file *file, char *buf, size_t length, loff_t *offset)  {

    if (*offset > 255)
        return 0;

    int len, tot_len = 0, ret;
    char tmp[100];
    int i = *offset;
    for (; i<256; i++) {
        len = sprintf(tmp, "%d:\t%d\n", i, get_number_messages_mail_spot(i));

        if (tot_len + len > length) {
            break;
        } else {
            ret = copy_to_user(buf+tot_len, tmp, len);
            if (ret != 0) {
                printk("%s: copy_to_user failed, pid=%d\n", MODNAME, current->pid);
                return -1;
            }
            tot_len += len;
        }
    }

    *offset = i;

    return tot_len;
}

static struct file_operations f_ops_get_number_messages = {  
    .owner = THIS_MODULE,
    .read = get_number_messages_mail_spot_read,
};

int init_module(void) {

    proc_parent = proc_mkdir("mail_spot_metadata",NULL);
    if(!proc_parent) {
        printk("Error creating proc directory");
        return -1;
    }

    get_users_count = proc_create("get_users_count", 0444, proc_parent, &f_ops_get_users_count);
    if (get_users_count == NULL) {
        remove_proc_entry("get_users_count", proc_parent);
        remove_proc_entry("mail_spot_metadata",NULL);
        printk("Error creating get_users_count\n");
        return -1;
    }

    get_max_mex_len = proc_create("get_max_mex_len", 0444, proc_parent, &f_ops_get_max_mex_len);
    if (get_users_count == NULL) {
        remove_proc_entry("get_max_mex_len", proc_parent);
        remove_proc_entry("get_users_count", proc_parent);
        remove_proc_entry("mail_spot_metadata",NULL);
        printk("Error creating get_max_mex_len\n");
        return -1;
    }

    get_number_messages = proc_create("get_number_messages", 0444, proc_parent, &f_ops_get_number_messages);
    if (get_users_count == NULL) {
        remove_proc_entry("get_number_messages", proc_parent);
        remove_proc_entry("get_max_mex_len", proc_parent);
        remove_proc_entry("get_users_count", proc_parent);
        remove_proc_entry("mail_spot_metadata",NULL);
        printk("Error creating get_number_messages\n");
        return -1;
    }

    printk(KERN_INFO "%s: module initialized\n", MODNAME);
    return 0;
}

void cleanup_module(void) {
    remove_proc_entry("get_number_messages", proc_parent);
    remove_proc_entry("get_users_count",proc_parent);
    remove_proc_entry("get_max_mex_len",proc_parent);
    remove_proc_entry("mail_spot_metadata",NULL);
    printk(KERN_INFO "%s: module cleaned\n", MODNAME);
}