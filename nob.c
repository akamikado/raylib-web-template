#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

#define BUILD_DIR "build/"
#define SRC_DIR "src/"

// Add path to raylib webassembly build
#define RAYLIB_DIR

#define HTML_OUTPUT

// Uncomment if you want to use ASYNCIFY feature of emcc
// #define ASYNCIFY

// itch.io username
#define ITCH_USERNAME
// Game name in itch.io
#define GAME

void push_to_itch(Cmd cmd) {
  cmd_append(&cmd, "butler");
  cmd_append(&cmd, "push");
  cmd_append(&cmd, BUILD_DIR);
  cmd_append(&cmd, ITCH_USERNAME"/"GAME":html");
  if (!cmd_run(&cmd)) exit(1);
}

void run_game(Cmd cmd) {
  cmd_append(&cmd, "python3");
  cmd_append(&cmd, "-m");
  cmd_append(&cmd, "http.server");
  cmd_append(&cmd, "8080");
  cmd_append(&cmd, "-d");
  cmd_append(&cmd, BUILD_DIR);
  if (!cmd_run(&cmd)) exit(1);
}

void build_game(Cmd cmd) {
  cmd_append(&cmd, "emcc");
  cmd_append(&cmd, "-o");
  cmd_append(&cmd, BUILD_DIR HTML_OUTPUT);
  cmd_append(&cmd, SRC_DIR"main.c");
  cmd_append(&cmd, "-Os");
  cmd_append(&cmd, "-Wall");
  cmd_append(&cmd, "--shell-file");
  cmd_append(&cmd, SRC_DIR"shell.html");
  cmd_append(&cmd, RAYLIB_DIR"lib/libraylib.web.a");
  cmd_append(&cmd, "-I"RAYLIB_DIR"include/");
  cmd_append(&cmd, "-I"SRC_DIR);
  cmd_append(&cmd, "-s");
  cmd_append(&cmd, "USE_GLFW=3");
#ifdef ASYNCIFY
  cmd_append(&cmd, "-s");
  cmd_append(&cmd, "ASYNCIFY");
#endif
  cmd_append(&cmd, "-DPLATFORM_WEB");
  if (!cmd_run(&cmd)) exit(1);
}

typedef struct {
  const char* flag;
  const char* desc;
  void(*action)(Cmd);
} Flag;

Flag flags[] = {
  {"nobuild", "Do not build the source",  NULL},
  {"run" ,    "Run the game",             run_game},
  {"push",    "Push the game to itch.io", push_to_itch},
  {"help",    "Display usage",            NULL},
};

void print_help(const char* exe) {
  printf("Usage: \n");
  for (int i = 0; i < ARRAY_LEN(flags); i++) {
    printf("        %s -%s:\t%s\n", exe, flags[i].flag, flags[i].desc);
  }
}

int main(int argc, char** argv) {
  NOB_GO_REBUILD_URSELF(argc, argv);

  if (!mkdir_if_not_exists(BUILD_DIR)) return 1;

  Cmd cmd = {0};
  bool build = true;

  for (int i = 1; i < argc; i++) {
    for (int j = 0; j < ARRAY_LEN(flags); j++) {
      if (strcmp(argv[i]+1, "nobuild") == 0) build = false;
      if (strcmp(argv[i]+1, "help") == 0) {
        print_help(argv[0]);
        return 0;
      }
    }
  }

  if (build) build_game(cmd);

  for (int i = 1; i < argc; i++) {
    for (int j = 0; j < ARRAY_LEN(flags); j++) {
      if (strcmp(argv[i]+1, flags[j].flag) == 0) {
        if (flags[j].action != NULL) flags[j].action(cmd);
      }
    }
  }

  return 0;
}
