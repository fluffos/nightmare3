#ifndef __SHUTDOWN_H
#define __SHUTDOWN_H

void create();
void reboot_mud(int x);
static void countdown(int x);
static void final_warning();
static void reboot();
static private void notify_listeners(int x);
void notify_armageddon(object ob);

#endif /* __SHUTDOWN_H */
