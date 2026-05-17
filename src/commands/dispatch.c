#include "commands.h"
#include "../input/input.h"

void command_dispatch(AppState *app, int key) {
    if (app->search_failed) {
        app->search_failed = false;
        // Optional: consume the key or process it? 
        // Standard pager behavior: first key clears the message.
        // Let's process it anyway if it's not a dummy key.
    }

    if (app->show_help) {
        if (key == KEY_HELP || key == KEY_ESC) {
            app->show_help = false;
        } else if (key == KEY_QUIT) {
            cmd_sys_quit(app);
        }
        return;
    }

    switch (key) {
        case KEY_UP:              cmd_nav_up(app); break;
        case KEY_DOWN:            cmd_nav_down(app); break;
        case KEY_PAGE_UP:         cmd_nav_page_up(app); break;
        case KEY_PAGE_DOWN:       cmd_nav_page_down(app); break;
        case KEY_HALF_UP:         cmd_nav_half_up(app); break;
        case KEY_HALF_DOWN:       cmd_nav_half_down(app); break;
        case KEY_HOME:            cmd_nav_home(app); break;
        case KEY_END:             cmd_nav_end(app); break;
        case KEY_SEARCH_FORWARD:  cmd_search_forward(app); break;
        case KEY_SEARCH_BACKWARD: cmd_search_backward(app); break;
        case KEY_SEARCH_NEXT:     cmd_search_next(app); break;
        case KEY_SEARCH_PREV:     cmd_search_prev(app); break;
        case KEY_HELP:            cmd_sys_help(app); break;
        case KEY_ESC:             app->last_pattern[0] = '\0'; break;
        case KEY_QUIT:            cmd_sys_quit(app); break;
    }
}
