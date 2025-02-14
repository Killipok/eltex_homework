#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/time.h>
#include <linux/ktime.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Log module load/unload times to a file");

static char *log_file_path = "/var/log/kernel_times.log";
module_param(log_file_path, charp, 0644);
MODULE_PARM_DESC(log_file_path, "Path to the log file");

static int write_time(const char *event) {
    struct timespec64 ts;
    struct tm tm;
    char buf[128];
    struct file *file;
    loff_t pos;
    ssize_t ret;
    int len;

    ktime_get_real_ts64(&ts);
    time64_to_tm(ts.tv_sec, 0, &tm);

    len = snprintf(buf, sizeof(buf), "%04ld-%02ld-%02ld %02ld:%02ld:%02ld - %s\n",
                   (long)tm.tm_year + 1900, (long)tm.tm_mon + 1, (long)tm.tm_mday,
                   (long)tm.tm_hour, (long)tm.tm_min, (long)tm.tm_sec, event);
    if (len < 0 || len >= sizeof(buf)) {
        printk(KERN_ERR "Time formatting error\n");
        return -EIO;
    }

    file = filp_open(log_file_path, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (IS_ERR(file)) {
        printk(KERN_ERR "Cannot open file %s\n", log_file_path);
        return PTR_ERR(file);
    }

    pos = 0;
    ret = kernel_write(file, buf, len, &pos);
    if (ret != len) {
        printk(KERN_ERR "Write error to %s\n", log_file_path);
        filp_close(file, NULL);
        return -EIO;
    }

    filp_close(file, NULL);
    return 0;
}

static int __init mod_init(void) {
    int ret = write_time("Module loaded");
    if (ret)
        return ret;

    printk(KERN_INFO "Module loaded. Log written to %s\n", log_file_path);
    return 0;
}

static void __exit mod_exit(void) {
    write_time("Module unloaded");
    printk(KERN_INFO "Module unloaded. Log updated in %s\n", log_file_path);
}

module_init(mod_init);
module_exit(mod_exit);