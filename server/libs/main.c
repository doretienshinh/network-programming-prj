#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ws.h>

#define ONE_GAME_TIME 900  // 15 minutes

typedef struct {
  char *socketId;
  char *name;
  int charactorId;
  int roomId;
} User;

typedef struct {
  int id;
  User users[4];
  int timeRemaining;
} Room;

Room rooms[50];

User *findUser(char *socketId) {
  for (int i = 0; i < 50; i++) {
    for (int j = 0; j < 4; j++) {
      if (rooms[i].users[j].socketId != NULL &&
          strcmp(rooms[i].users[j].socketId, socketId) == 0) {
        return &rooms[i].users[j];
      }
    }
  }
  return NULL;
}

bool addUserToRoom(User *user, int roomId) {
  for (int i = 0; i < 50; i++) {
    if (rooms[i].id == roomId) {
      for (int j = 0; j < 4; j++) {
        if (rooms[i].users[j].socketId == NULL) {
          rooms[i].users[j] = *user;
          return true;
        }
      }
    }
  }
  return false;
}

Room *createRoom(User *user) {
  for (int i = 0; i < 50; i++) {
    if (rooms[i].id == 0) {
      rooms[i].id = i + 1;
      rooms[i].users[0] = *user;
      rooms[i].timeRemaining = ONE_GAME_TIME;
      return &rooms[i];
    }
  }
  return NULL;
}

void removeUserFromRoom(char *socketId) {
  for (int i = 0; i < 50; i++) {
    for (int j = 0; j < 4; j++) {
      if (rooms[i].users[j].socketId != NULL &&
          strcmp(rooms[i].users[j].socketId, socketId) == 0) {
        rooms[i].users[j].socketId = NULL;
        rooms[i].users[j].name = NULL;
        rooms[i].users[j].charactorId = 0;
        rooms[i].users[j].roomId = 0;
      }
    }
  }
}

void onopen(int fd) {
  char *cli;
  cli = ws_getaddress(fd);
#ifndef DISABLE_VERBOSE
  printf("Connection opened, client: %d | addr: %s\n", fd, cli);
#endif
  free(cli);
}

void onclose(int fd) {
  char *cli;
  cli = ws_getaddress(fd);
#ifndef DISABLE_VERBOSE
  printf("Connection closed, client: %d | addr: %s\n", fd, cli);
#endif
  free(cli);
}

void onmessage(int fd, const unsigned char *msg, uint64_t size, int type) {
  char *cli;
  cli = ws_getaddress(fd);
#ifndef DISABLE_VERBOSE
  printf("I receive a message: %s (size: %" PRId64 ", type: %d), from: %s/%d\n",
         msg, size, type, cli, fd);
#endif
  free(cli);

  ws_sendframe(fd, (char *)msg, size, true, type);
}

int main(void) {
  struct ws_events evs;
  evs.onopen = &onopen;
  evs.onclose = &onclose;
  evs.onmessage = &onmessage;
  ws_socket(&evs, 8080, 0);

  return (0);
}