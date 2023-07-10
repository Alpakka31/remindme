#include <ncurses.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <mpv/client.h>
#include "config.h"

static inline void check_error(int status) {
    if (status < 0) {
        mvprintw(0, 0, "mpv API error: %s\n", mpv_error_string(status));
        refresh();
        exit(1);
    }
}

int main(void) {
    // These will contain current amount of rows and columns
    int row, col;

    // Define messages to show
    char msg[] = ":)";
    char exit_msg[] = "Press 'q' to exit...";

    // Convert time variables to seconds
    // Time variables are defined in 'config.h'
    int seconds = hour * 3600 + min * 60 + sec;

    // Initialize ncurses
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);

    // Set amount of rows and columns to the variables 'row' and 'col'
    getmaxyx(stdscr, row, col);

    // Calculate rows and cols to the middle of the screen
    int midrow = row / 2;
    int midcol = (col / 2) - 4;
    int secs_hour, secs_min, secs_seconds;

    // Set bold text and hide the blinking cursor
    attron(A_BOLD);
    curs_set(0);

    // Start the countdown
    while (seconds >= 0) {
        fflush(stdout);
        sleep(1);

        // Calculate seconds to time variables
        secs_hour = seconds / 3600;
        secs_min = (seconds / 60) % 60;
        secs_seconds = seconds % 60;

        // Show countdown in the middle of the screen
        mvprintw(midrow, midcol, "%02d:%02d:%02d", secs_hour, secs_min, secs_seconds);
        refresh();
        seconds--;
    }

    // Show the messages defined before
    mvprintw(midrow + 1, midcol + 3, "%s", msg);
    mvprintw(midrow + 4, midcol - 5, "%s", exit_msg);

    refresh();

    // Play an audio file using MPV C API
    // Create MPV instance
    mpv_handle *ctx = mpv_create();
    if (!ctx) {
        mvprintw(0, 0, "%s", "failed creating context");
        refresh();
        return 1;
    }

    // Loop the audio file
    mpv_set_option_string(ctx, "loop-file", "yes");

    // Initialize MPV instance
    check_error(mpv_initialize(ctx));

    // Load audio file
    const char *cmd[] = {"loadfile", AUDIO_FILE_PATH, NULL};
    check_error(mpv_command(ctx, cmd));
    
    // Press 'q' to exit the program
    int ch;
    while (1) {
        ch = getch();
        if (ch == 113) {
            break;
        }
    }

    mpv_terminate_destroy(ctx);

    // Disable bold text and show the blinking cursor
    attroff(A_BOLD);
    curs_set(1);

    endwin();
    return 0;
}
