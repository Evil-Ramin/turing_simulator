#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined(_WIN32)
#include <windows.h>
#define SLEEP_FUNCTION(x) Sleep((x) / 1000)
#elif defined(__unix__) || defined(__APPLE__)
#include <unistd.h>
#define SLEEP_FUNCTION(x) usleep(x)
#endif

struct tstate {

  bool write0;
  bool write1;
  char direction0;
  char direction1;
  int state0;
  int state1;
};

int main() {

  printf("Copyright (c) 2024, EvilRamin\n");
  printf("MIT license: https://mit-license.org/\n");
  printf("Turing machine simulator\n");
  printf("Use reasonable inputs or expect unexpected behavior\n");
  bool ff = false;
#if defined(__unix__) || defined(__APPLE__)
  ff = true;
  printf("Running on a Unix-like OS (full functionality)\n");
#else
  ff = false;
  printf("Running on a non Unix-like OS (some functionality is lost)\n");
#endif

  int m, p;
  printf("\n Initializing the tape\n");
  printf("The tape will be indexed 0 to m-1 and so m the length os the tape\n");
  printf("p is the index of the cell on the tape on which the head starts\n");
  printf("If the tape is not round and the head tries to go out of indexed "
         "cells, the program terminates with an error\n");
  printf("For tapes with length 50 or shorter, you can watch in real time\n");
  printf("All cells are initialized to be 0\n");
  if (ff) {
    printf("1 ≤ (m and p) ≤ 2^31-1\n");
  } else {
    printf("1 =< (m and p) =< 2^31-1\n");
  }
  printf("m = ");
  scanf("%d", &m);
  if (m < 1) {
    return 1;
  }
  printf("p = ");
  scanf("%d", &p);
  if (!((m > p) && (p >= 0))) {
    return 1;
  }
  bool *cell = calloc(m, sizeof(bool));
  char rc;
  bool r = 0;
  printf("Round tape? (y/aNy) ");
  scanf(" %c", &rc);
  if (rc == 'y' || rc == 'Y') {
    r = 1;
  }

  int n;
  if (ff) {
    printf("\nEnter the number of states (1 ≤ n ≤ 2^31-1): ");
  } else {
    printf("\nEnter the number of states (1 =< n =< 2^31-1): ");
  }
  scanf("%d", &n);
  if (n < 0) {
    return 1;
  }
  struct tstate state[n];

  for (int i = 0; i < n; i++) {
    printf("\n Initializing state number %d\n", i + 1);
    if (i == 0) {
      printf(" Note: this is the starting state\n");
    }
    printf("What should the head write when it reads 0? (bool 0/1) ");
    scanf("%hhd", &state[i].write0);
    printf("What should the head write when it reads 1? (bool 0/1) ");
    scanf("%hhd", &state[i].write1);
    printf("What direction should the head go when it reads 0? (L/R) ");
    scanf(" %c", &state[i].direction0);
    if (!(state[i].direction0 == 'L' || state[i].direction0 == 'R')) {
      return 1;
    }
    printf("What direction should the head go when it reads 1? (L/R) ");
    scanf(" %c", &state[i].direction1);
    if (!(state[i].direction1 == 'L' || state[i].direction1 == 'R')) {
      return 1;
    }
    printf(
        "In what state should the head go when it reads 0? (int)(0 for halt) ");
    scanf("%d", &state[i].state0);
    state[i].state0--;
    if (!((-1 <= state[i].state0) && (state[i].state0 < n))) {
      return 1;
    }
    printf(
        "In what state should the head go when it reads 1? (int)(0 for halt) ");
    scanf("%d", &state[i].state1);
    state[i].state1--;
    if (!((-1 <= state[i].state1) && (state[i].state1 < n))) {
      return 1;
    }
  }

  printf("\n All values initialized!\n");
  printf("Try the \"help\" command!\n");
  char cmd[50];
  int csta = 0;
  int cste = 0;
  int sb;
  int coun;
  int mws;
  int wsc;
  bool alrph = false;
  if (n == 0) {
    csta--;
  }

  while (true) {
    printf("(turing) command: ");
    scanf("%s", cmd);
    if (strcmp(cmd, "quit") == 0) {
      break;
    }
    if (strcmp(cmd, "help") == 0) {
      printf("help -> prints this text\nquit -> quits the app\nprint_tape (or) "
             "p -> prints the whole tape (on Unix-like OS' cells with indices "
             "divisible by 5 "
             "will be colored green and the cell the head is on will be colord "
             "red)\nprint_cell -> prints a specified cell\nflip_cell -> flips "
             "a specified cell\nstep_one (or) s -> the head performs one "
             "single step\nsp -> performs s and then p\nstep_many -> the head "
             "performs a specified number "
             "of steps\nhead_pos -> prints the index of the cell the head is "
             "on\nhead_mov -> moves the head to the a specified cell\nwatch_rt "
             "-> watch the machine run in real time\nno_steps -> prints the "
             "number of steps taken so far\nno_ones -> prints the number of "
             "ones on the tape\nno_zeros -> prints the number of zeros on the "
             "tape\nprint_state -> prints the sate the current state\nch_state "
             "-> changes the state to what you specify\nclear_tape -> sets all "
             "the cells on tape back to zero\nzero_the_steps -> sets the step "
             "count back to zero\n");
    }
    if (strcmp(cmd, "print_tape") == 0 || strcmp(cmd, "p") == 0) {
      for (int i = 0; i < m; i++) {
        if ((i == p) && ff) {
          printf("\033[31m%d\033[0m", cell[i]);
        } else if ((i % 5 == 0) && ff) {
          printf("\033[32m%d\033[0m", cell[i]);
        } else {
          printf("%d", cell[i]);
        }
      }
      printf("\n");
    }
    if (strcmp(cmd, "flip_cell") == 0) {
      while (true) {
        printf(
            "Enter the index of a cell you want flipped, enter -1 to skip: ");
        scanf("%d", &sb);
        if (!((m > sb) && (sb >= -1))) {
          return 1;
        }
        if (sb == -1) {
          break;
        } else {
          cell[sb] = !(cell[sb]);
        }
      }
    }
    if (strcmp(cmd, "print_cell") == 0) {
      while (true) {
        printf(
            "Enter the index of a cell you want to print, enter -1 to skip: ");
        scanf("%d", &sb);
        if (!((m > sb) && (sb >= -1))) {
          return 1;
        }
        if (sb == -1) {
          break;
        } else {
          printf("%d\n", cell[sb]);
        }
      }
    }
    if (strcmp(cmd, "print_state") == 0) {
      if (csta == -1) {
        printf("Halt!\n");
      } else {
        printf("%d\n", csta + 1);
      }
    }
    if (strcmp(cmd, "ch_state") == 0) {
      printf("Enter the state you want to switch to, enter -1 to skip: ");
      scanf("%d", &sb);
      if (!((-1 <= sb) && (sb <= n))) {
        return 1;
      }
      if (sb == -1) {
        break;
      } else {
        csta = sb - 1;
      }
    }
    if (strcmp(cmd, "head_pos") == 0) {
      printf("%d\n", p);
    }
    if (strcmp(cmd, "head_mov") == 0) {
      printf("Enter the index of the cell you want to move the head to, enter "
             "-1 to skip: ");
      scanf("%d", &sb);
      if (!((m > sb) && (sb >= -1))) {
        return 1;
      }
      if (sb == -1) {
        break;
      } else {
        p = sb;
      }
    }
    if (strcmp(cmd, "no_steps") == 0) {
      printf("%d\n", cste);
    }
    if (strcmp(cmd, "no_ones") == 0) {
      coun = 0;
      for (int i = 0; i < m; i++) {
        if (cell[i] == 1) {
          coun++;
        }
      }
      printf("%d\n", coun);
    }
    if (strcmp(cmd, "no_zeros") == 0) {
      coun = 0;
      for (int i = 0; i < m; i++) {
        if (cell[i] == 0) {
          coun++;
        }
      }
      printf("%d\n", coun);
    }
    if (strcmp(cmd, "step_one") == 0 || strcmp(cmd, "s") == 0) {
      if (csta == -1) {
        printf("Halt!\n");
        alrph = true;
      } else {
        if (cell[p]) {
          cell[p] = state[csta].write1;
          if (state[csta].direction1 == 'R') {
            p++;
          } else {
            p--;
          }
          if (p == m) {
            if (r) {
              p = 0;
            } else {
              return 1;
            }
          }
          if (p == -1) {
            if (r) {
              p = m - 1;
            } else {
              return 1;
            }
          }
          csta = state[csta].state1;
        } else {
          cell[p] = state[csta].write0;
          if (state[csta].direction0 == 'R') {
            p++;
          } else {
            p--;
          }
          if (p == m) {
            if (r) {
              p = 0;
            } else {
              return 1;
            }
          }
          if (p == -1) {
            if (r) {
              p = m - 1;
            } else {
              return 1;
            }
          }
          csta = state[csta].state0;
        }
        cste++;
      }
      if ((csta == -1) && !(alrph)) {
        printf("Halt!\n");
        alrph = false;
      }
    }
    if (strcmp(cmd, "sp") == 0) {
      if (csta == -1) {
        printf("Halt!\n");
        alrph = true;
      } else {
        if (cell[p]) {
          cell[p] = state[csta].write1;
          if (state[csta].direction1 == 'R') {
            p++;
          } else {
            p--;
          }
          if (p == m) {
            if (r) {
              p = 0;
            } else {
              return 1;
            }
          }
          if (p == -1) {
            if (r) {
              p = m - 1;
            } else {
              return 1;
            }
          }
          csta = state[csta].state1;
        } else {
          cell[p] = state[csta].write0;
          if (state[csta].direction0 == 'R') {
            p++;
          } else {
            p--;
          }
          if (p == m) {
            if (r) {
              p = 0;
            } else {
              return 1;
            }
          }
          if (p == -1) {
            if (r) {
              p = m - 1;
            } else {
              return 1;
            }
          }
          csta = state[csta].state0;
        }
        cste++;
      }
      if ((csta == -1) && !(alrph)) {
        printf("Halt!\n");
        alrph = false;
      }
      for (int i = 0; i < m; i++) {
        if ((i == p) && ff) {
          printf("\033[31m%d\033[0m", cell[i]);
        } else if ((i % 5 == 0) && ff) {
          printf("\033[32m%d\033[0m", cell[i]);
        } else {
          printf("%d", cell[i]);
        }
      }
      printf("\n");
    }
    if (strcmp(cmd, "step_many") == 0) {
      printf("Enter the number of steps to take: ");
      scanf("%d", &sb);
      for (int i = 0; i < sb; i++) {
        if (csta == -1) {
          printf("Halt!\n");
          break;
        } else {
          if (cell[p]) {
            cell[p] = state[csta].write1;
            if (state[csta].direction1 == 'R') {
              p++;
            } else {
              p--;
            }
            if (p == m) {
              if (r) {
                p = 0;
              } else {
                return 1;
              }
            }
            if (p == -1) {
              if (r) {
                p = m - 1;
              } else {
                return 1;
              }
            }
            csta = state[csta].state1;
          } else {
            cell[p] = state[csta].write0;
            if (state[csta].direction0 == 'R') {
              p++;
            } else {
              p--;
            }
            if (p == m) {
              if (r) {
                p = 0;
              } else {
                return 1;
              }
            }
            if (p == -1) {
              if (r) {
                p = m - 1;
              } else {
                return 1;
              }
            }
            csta = state[csta].state0;
          }
          cste++;
        }
        if (csta == -1) {
          printf("Halt!\n");
          break;
        }
      }
    }
    if (strcmp(cmd, "watch_rt") == 0) {
      if (m < 51) {
        printf("(you won't be able to get back to the turing command line "
               "unless the program halts or the limit is reached!)\nEnter the "
               "max number of steps to take, enter -1 for no limit: ");
        scanf("%d", &mws);
        if (mws < -1) {
          return 1;
        }
        printf("Enter the delay between each step in micro seconds: ");
        scanf("%d", &sb);
        if (sb < 0) {
          return 1;
        }
        printf("head state: %d tape: ", csta + 1);
        for (int i = 0; i < m; i++) {
          if ((i == p) && ff) {
            printf("\033[31m%d\033[0m", cell[i]);
          } else if ((i % 5 == 0) && ff) {
            printf("\033[32m%d\033[0m", cell[i]);
          } else {
            printf("%d", cell[i]);
          }
        }
        printf("\n");
        wsc = 0;
        while (true) {
          SLEEP_FUNCTION(sb);
          if (csta == -1) {
            printf("Halt!\n");
            break;
          } else if ((wsc >= mws) && (mws != -1)) {
            printf("Step limit reached!\n");
            break;
          } else {
            if (cell[p]) {
              cell[p] = state[csta].write1;
              if (state[csta].direction1 == 'R') {
                p++;
              } else {
                p--;
              }
              if (p == m) {
                if (r) {
                  p = 0;
                } else {
                  return 1;
                }
              }
              if (p == -1) {
                if (r) {
                  p = m - 1;
                } else {
                  return 1;
                }
              }
              csta = state[csta].state1;
            } else {
              cell[p] = state[csta].write0;
              if (state[csta].direction0 == 'R') {
                p++;
              } else {
                p--;
              }
              if (p == m) {
                if (r) {
                  p = 0;
                } else {
                  return 1;
                }
              }
              if (p == -1) {
                if (r) {
                  p = m - 1;
                } else {
                  return 1;
                }
              }
              csta = state[csta].state0;
            }
            cste++;
            wsc++;
          }
          printf("head state: %d tape: ", csta + 1);
          for (int i = 0; i < m; i++) {
            if ((i == p) && ff) {
              printf("\033[31m%d\033[0m", cell[i]);
            } else if ((i % 5 == 0) && ff) {
              printf("\033[32m%d\033[0m", cell[i]);
            } else {
              printf("%d", cell[i]);
            }
          }
          printf("\n");
          if (csta == -1) {
            printf("Halt!\n");
            break;
          }
        }
      } else {
        printf("Error: m is greater than 50");
      }
    }
    if (strcmp(cmd, "clear_tape") == 0) {
      for (int i = 0; i < m; i++) {
        cell[i] = 0;
      }
    }
    if (strcmp(cmd, "zero_the_steps") == 0) {
      cste = 0;
    }
  }

  free(cell);
  return 0;
}

// YES! I DO KNOW THIS IS AN INEFFICIENT IMPLEMENTATION AND THAT I COULD HAVE
// HAD BASIC OPERATIONS BE FUNCTIONS AND AVOID REWRITING THEM. YES! I DO KNOW I
// COULD HAVE HAD COLOR FORMATTING IN WINDOWS TOO IF I HAD USED A CERTAIN
// LIBRARY.
