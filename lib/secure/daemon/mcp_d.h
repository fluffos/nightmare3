#ifndef __MCPD_H
#define __MCPD_H

#include <dirs.h>

void listen_callback(int fd);
void write_callback(int fd);
void read_callback(int fd, string str);
void close_connection(int fd);
static private void retrieve_file(int fd, string file);
static private void store_file(int fd, string file, string *lines);
void write_tick();

#endif /* __MCPD_H */
