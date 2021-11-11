#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

/*defines */

#define CTRL_KEY(k) ((k)&0x1f)
#define BARACLE_VERSION "1.0"

/*data */

struct editorConfig {
  int cx, cy; // move the cursor
  int screenrows;
  int screencols;
  struct termios orig_termios; // global variable to store attributes
};

struct editorConfig E;

/*terminal */

void die(const char *s) {             // error handling
  write(STDOUT_FILENO, "\x1b[2J", 4); // clear the screen on exit
  write(STDOUT_FILENO, "\x1b[H", 3);

  perror(s);
  exit(1);
}

void disableRawMode() { // disable raw mode at exit
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.orig_termios) == -1)
    die("tcsetattr");
}

void enableRawMode() {
  if (tcgetattr(STDIN_FILENO, &E.orig_termios) == -1)
    die("tcgetattr");

  atexit(disableRawMode);

  struct termios raw = E.orig_termios;
  raw.c_iflag &= ~(BRKINT | ICRNL | // fix ctrl-m
                   INPCK | ISTRIP |
                   IXON);  // disable ctrl-s/ctrl-q : XON/XOFF (freeze process)
  raw.c_oflag &= ~(OPOST); // turn of all output processing
  raw.c_cflag |= (CS8);    // 8 bit per bite
  raw.c_lflag &=
      ~(ECHO | ICANON | // disable canonical mode
        IEXTEN |        // disable ctrl-v
        ISIG);        // disable ctrl-z / ctrl-c : SIGTSTP/SIGINT (stop process)
  raw.c_cc[VMIN] = 0; // minimum value to start read
  raw.c_cc[VTIME] = 1; // max wait before read

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
    die("tcgetattr");
}

int getWindowSize(int *rows, int *cols) { // window size, the "easy" way
  struct winsize ws;

  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
    return -1;
  } else {
    *cols = ws.ws_col;
    *rows = ws.ws_row;
    return 0;
  }
}

char editorReadKey() {
  int nread;
  char c;
  while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
    if (nread == -1 && errno != EAGAIN)
      die("read");
  }
  return c;
}

/* append buffer */

struct abuf {
  char *b;
  int len;
};

#define ABUF_INIT                                                              \
  { NULL, 0 }

void abAppend(struct abuf *ab, const char *s, int len) {
  char *new = realloc(ab->b, ab->len + len);
  if (new == NULL)
    return;
  memcpy(&new[ab->len], s, len);
  ab->b = new;
  ab->len += len;
}
void abFree(struct abuf *ab) { free(ab->b); }

/*output */
void editorDrawRows(struct abuf *ab) {
  int y;
  for (y = 0; y < E.screenrows; y++) {
    if (y == E.screenrows / 3) {
      char welcome[80];
      int welcomelen =
          snprintf(welcome, sizeof(welcome), "Baracle editor -- version %s",
                   BARACLE_VERSION);
      if (welcomelen > E.screencols)
        welcomelen = E.screencols;
      int padding = (E.screencols - welcomelen) / 2;
      if (padding) {
        abAppend(ab, "~", 1);
        padding--;
      }
      while (padding--)
        abAppend(ab, " ", 1);
      abAppend(ab, welcome, welcomelen);
    } else {
      abAppend(ab, "~", 1);
    }
    abAppend(ab, "\x1b[K", 3);
    if (y < E.screenrows - 1) {
      abAppend(ab, "\r\n", 2);
    }
  }
}

void editorRefreshScreen() {
  struct abuf ab = ABUF_INIT;

  abAppend(&ab, "\x1b[?25l", 6);
  abAppend(&ab, "\x1b[H", 3);

  editorDrawRows(&ab);

  char buf[32];
  snprintf(buf, sizeof(buf), "\x1b[%d;%dH", E.cy + 1, E.cx + 1);
  abAppend(&ab, buf, strlen(buf));

  abAppend(&ab, "\x1b[H", 3);
  abAppend(&ab, "\x1b[?25l", 6);

  write(STDOUT_FILENO, ab.b, ab.len);
  abFree(&ab);
}

/*input */

void editorMoveCursor(char key) { // move the cursor rules
  switch (key) {
  case 'a':
    E.cx--;
    break;
  case 'd':
    E.cx++;
    break;
  case 'w':
    E.cy--;
    break;
  case 's':
    E.cy++;
    break;
  }
}

char editorProcessKeyPress() {

  char c = editorReadKey();
  switch (c) {
  case CTRL_KEY('q'):
    write(STDOUT_FILENO, "\x1b[2J", 4); // clewar the screen on exit
    write(STDOUT_FILENO, "\x1b[H", 3);
    exit(0);
    break;
  case 'w':
  case 's':
  case 'a':
  case 'd':
    editorMoveCursor(c);
    break;
  }
}

/* init */

void initEditor() {
  E.cx = 0;
  E.cy = 0;
  if (getWindowSize(&E.screenrows, &E.screencols) == -1)
    die("getWindowSize");
}

int main() {
  enableRawMode();
  initEditor();

  while (1) {
    editorRefreshScreen();
    editorProcessKeyPress();
  }

  return 0;
}
