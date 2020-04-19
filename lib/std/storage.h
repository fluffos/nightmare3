#ifndef __STORAGE_H
#define __STORAGE_H

static int Put(string str);
static int Get(string str);
static int Open(string str);
static int Close(string str);
static int Lock(string str);
static int Unlock(string str);
int pick_lock();
varargs string query_long(string str);
varargs int receive_objects(object ob);
void set_can_close(int x);
int query_can_close();
void set_can_lock(int x);
int query_can_lock();
void set_closed(int x);
int query_closed();
void set_locked(int x);
int query_locked();
void set_key(string str);
string query_key();

#endif /* __STORAGE_H */
