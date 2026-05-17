#include "sys.h"
#include "../../app/state.h"

void cmd_sys_help(AppState *app) {
    app->show_help = !app->show_help;
}
