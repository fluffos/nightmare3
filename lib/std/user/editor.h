#ifndef __EDITOR_H
#define __EDITOR_H

varargs int edit(string nom, function f, function abort, mixed args);
nomask static void enter_line(string str);
nomask void end_edit();
nomask static private void clear_editor();
nomask string query_edit_file();
string query_escape();
string getenv(string key);
string query_cwd();

#endif /* __EDITOR_H */
