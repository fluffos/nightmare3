#ifndef __BBOARD_H
#define __BBOARD_H

void create();
void init();
private int valid_edit(string author);
int cmd_post(string str);
protected void begin_post(string cmd, string subj, string file, function f);
void continue_post(string subj, string file);
void abort_edit();
void end_post(mixed *args);
int cmd_read(string str);
int cmd_followup_and_respond(string str);
void continue_followup(mapping post, string subj, string file);
void continue_mail(mapping post, string subj, string file);
protected void check_include_text(string ans, string subj, string file, mapping post, int mail);
int cmd_remove(string str);
int cmd_edit(string str);
void end_edit(mixed *args);
varargs string query_long(string str);
void set_board_id(string str);
string query_board_id();
string query_board_time(int x);

#endif /* __BBOARD_H */
