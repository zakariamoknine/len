#ifndef _LEN_FILE_H_
#define _LEN_FILE_H_

#include <len/internal.h>
#include <len/fs.h>

enum file_types {
	FD_NONE, FD_INODE, FD_PIPE, DF_DEVICE
};

struct file {
	enum file_types type;
	struct inode* ip;
	struct device* dev;
	uint64_t off;
	uint64_t ref;
	char readable, writable;
};

struct file_operations {
	int (*read)(struct file* f, char* buf, int n);
	int (*write)(struct file* f, const char* buf, int n);
	int (*ioctl)(struct file* f, int cmd, void* arg);
	int (*mmap)(struct file* f, void* addr);
};

struct device {
	const char* name;
	struct file_operations* fops;
	int major, minor;
};

struct device_property {
	const char* name;
	const void* value;
	uint32_t length;
	struct device_property* next;
};

struct device_node {
	const char* name;
	const char* type;
	const char* compatible;
	struct device_property* properties;

	struct device_node* parent;
	struct device_node* child;
	struct device_node* sibling;
};

#endif /* _LEN_FILE_H_ */
