#ifndef __MORE_H
#define __MORE_H

varargs int more(mixed what, string cl, function endmore, mixed args);
static void do_more(string cmd);
void do_help();
static void regexp_fwd(string str);
static void regexp_bkwd(string str);
string getenv(string val);

#endif /* __MORE_H */
