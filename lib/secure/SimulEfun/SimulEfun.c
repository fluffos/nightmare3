/*    /secure/obj/SimulEfun.c
 *    from Nightmare IV
 *    the mud SimulEfun object
 *    created by Descartes of Borg 940213
 */

#include <daemons.h>
#include <objects.h>
#include <security.h>
#include "SimulEfun.h"

#include "/secure/SimulEfun/identify.c"
#include "/secure/SimulEfun/misc.c"
#include "/secure/SimulEfun/absolute_value.c"
#include "/secure/SimulEfun/alignment.c"
#include "/secure/SimulEfun/base_name.c"
#include "/secure/SimulEfun/communications.c"
#include "/secure/SimulEfun/convert_name.c"
#include "/secure/SimulEfun/copy.c"
#include "/secure/SimulEfun/creator_file.c"
#include "/secure/SimulEfun/distinct_array.c"
#include "/secure/SimulEfun/domains.c"
#include "/secure/SimulEfun/economy.c"
#include "/secure/SimulEfun/english.c"
#include "/secure/SimulEfun/events.c"
#include "/secure/SimulEfun/exclude_array.c"
#include "/secure/SimulEfun/files.c"
#include "/secure/SimulEfun/format_page.c"
#include "/secure/SimulEfun/format_string.c"
#include "/secure/SimulEfun/get_object.c"
#include "/secure/SimulEfun/idle.c"
#include "/secure/SimulEfun/interact.c"
#include "/secure/SimulEfun/interface.c"
#include "/secure/SimulEfun/light.c"
#include "/secure/SimulEfun/load_object.c"
#include "/secure/SimulEfun/log_file.c"
#include "/secure/SimulEfun/mud_info.c"
#include "/secure/SimulEfun/ordinal.c"
#include "/secure/SimulEfun/parse_objects.c"
#include "/secure/SimulEfun/path_file.c"
#include "/secure/SimulEfun/percent.c"
#include "/secure/SimulEfun/personal_log.c"
#include "/secure/SimulEfun/pointers.c"
#include "/secure/SimulEfun/read_database.c"
#include "/secure/SimulEfun/absolute_path.c"
#include "/secure/SimulEfun/security.c"
#include "/secure/SimulEfun/strings.c"
#include "/secure/SimulEfun/substr.c"
#include "/secure/SimulEfun/time.c"
#include "/secure/SimulEfun/to_object.c"
#include "/secure/SimulEfun/translate.c"
#include "/secure/SimulEfun/user_exists.c"
#include "/secure/SimulEfun/user_path.c"
#include "/secure/SimulEfun/visible.c"

int destruct(object ob) {
    string *privs;
    string tmp;
   
    if(previous_object(0) == ob) return efun::destruct(ob);
    if(!(tmp = query_privs(previous_object(0)))) return 0;
    if(member_array(PRIV_SECURE, explode(tmp, ":")) != -1)
      return efun::destruct(ob);
    privs = ({ file_privs(file_name(ob)) });
    if((int)master()->valid_apply(({ "ASSIST" }) + privs))
      return efun::destruct(ob);
    else return 0;
}
    
varargs void shutdown(int code) {
    if(!((int)master()->valid_apply(({})))) return;
    if(this_player())
      log_file("shutdowns", (string)this_player()->query_CapName()+
        " shutdown "+mud_name()+" at "+ctime(time())+"\n");
    else log_file("shutdowns", "Game shutdown by "+
      file_name(previous_object(0))+" at "+ctime(time())+"\n");
    catch(NETWORK_D->send_shutdown());
    efun::shutdown(code);
}

varargs object snoop(object who, object target) {
    if(!target) return efun::snoop(who);
    if(!creatorp(who)) return 0;
    if(!((int)master()->valid_apply(({ "ASSIST" })))) {
        if(!((int)target->query_snoopable())) return 0;
        else return efun::snoop(who, target);
    }
    else if(member_group(target, PRIV_SECURE)) {
        message("system", (string)who->query_CapName()+" is now snooping "
          "you.", target);
        return efun::snoop(who, target);
    }
    else return efun::snoop(who, target);
}

object query_snoop(object ob) {
    if(!userp(previous_object(0))) return 0;
    return efun::query_snoop(ob);
}

object query_snooping(object ob) {
    if(!((int)master()->valid_apply(({})))) return 0;
    else return efun::query_snooping(ob);
}

int exec(object target, object src) {
    if(base_name(previous_object(0)) != OB_LOGIN) return 0;
    return efun::exec(target, src);
}

void write(string str) {
    if(this_player()) message("my_action", str, this_player());
    else efun::write(str);
}

object *livings() {
    return efun::livings() - (efun::livings() - objects());
}

void set_privs(object ob, string str) { return; }

void set_eval_limit(int x) {
    if(previous_object() != master()) return;
    efun::set_eval_limit(x);
}

int seteuid(string str) { return 1; }

string getuid(object ob) { return "nothing"; }

string geteuid(object ob) { return "nothing"; }
