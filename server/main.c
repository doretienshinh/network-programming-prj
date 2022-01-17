#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ws.h>

typedef struct LinkedList
{
  char *username;
  char *password;
  int highscore;
  struct LinkedList *next;
} node;

char buffer[100]; // buffer[0]: 1-login, 0-register, 2-update,
char usernameCli[100];
char passwordCli[100];
node *root;

void reWriteFile(node *head);

node *CreateNode(char *username, char *password, int highscore)
{
  node *temp;                          // declare a node
  temp = (node *)malloc(sizeof(node)); // Cấp phát vùng nhớ dùng malloc()
  temp->username = (char *)malloc(sizeof(char) * 1000);
  temp->password = (char *)malloc(sizeof(char) * 1000);
  temp->next = NULL;                // Cho next trỏ tới NULL
  strcpy(temp->username, username); // Gán giá trị cho Node
  strcpy(temp->password, password);
  temp->highscore = highscore;
  return temp; // Trả về node mới đã có giá trị
}

node *AddHead(node *head, char *username, char *password, int highscore)
{
  // printf("%s %s \n", username, password);
  node *temp = CreateNode(username, password, highscore); // Khởi tạo node temp với data = value
  if (head == NULL)
  {
    head = temp; // //Nếu linked list đang trống thì Node temp là head luôn
  }
  else
  {
    temp->next = head; // Trỏ next của temp = head hiện tại
    head = temp;       // Đổi head hiện tại = temp(Vì temp bây giờ là head mới mà)
  }
  return head;
}
node *registerAcc(node *head, char *username, char *password)
{
  // Traverser(head);
  head = AddHead(head, username, password, 0);
  // Traverser(head);
  reWriteFile(head);
  return head;
}
void updateScore(node *head, char *username, int highscore)
{
  for (node *p = head; p->next != NULL; p = p->next)
  {
    if (strcmp(p->username, username) == 0)
    {
      p->highscore = highscore;
      reWriteFile(head);
      return;
    }
  }
  return;
}

int checkDupAcc(node *head, char *username)
{
  for (node *p = head; p->next != NULL; p = p->next)
  {
    if (strcmp(p->username, username) == 0)
    {
      return -1;
    }
  }
  return 0;
}

int logIn(node *head, char *username, char *password)
{
  for (node *p = head; p->next != NULL; p = p->next)
  {
    if (strcmp(p->username, username) == 0 && strcmp(p->password, password) == 0)
    {
      return p->highscore;
    }
  }
  return -1;
}
void Traverser(node *head)
{
  printf("\n-------\n");
  for (node *p = head; p->next != NULL; p = p->next)
  {
    puts(p->username);
    puts(p->password);
    printf("%d\n-------\n", p->highscore);
  }
}
int top1Score(node *head)
{
  node *p = head;
  int Max = head->highscore;
  while (p != NULL)
  {
    if (Max < p->highscore)
    {
      Max = p->highscore;
    }
    p = p->next;
  }
  return Max;
}
void rank(node *head, char *cpyString)
{
  int scores[3] = {0,0,0};
  char users[3][100] = {"none", "none", "none"};
  char top3OnRank[1000];
  // strcpy(scores[0], head->username);
  // for (int i = 0; i < 3; i++)
  // {
  //   scores[i] = head->highscore;
  //   strcpy(users[i], head->username);
  // }
  node *p = head;
  while (p != NULL)
  {
    if (scores[0] < p->highscore)
    {
      scores[0] = p->highscore;
      strcpy(users[0], p->username);
    }
    p = p->next;
  }
  node *k = head;
  while (k != NULL)
  {
    if (scores[1] < k->highscore && strcmp(users[0], k->username) != 0)
    {
      scores[1] = k->highscore;
      strcpy(users[1], k->username);
    }
    k = k->next;
  }
  node *l = head;
  while (l != NULL)
  {
    if (scores[2] < l->highscore && strcmp(users[1], l->username) != 0 && strcmp(users[0], l->username) != 0)
    {
      scores[2] = l->highscore;
      strcpy(users[2], l->username);
    }
    l = l->next;
  }
  sprintf(top3OnRank, "%s_%d_%s_%d_%s_%d", users[0], scores[0], users[1], scores[1], users[2], scores[2]);
  strcpy(cpyString, top3OnRank);
  // int Max = head->highscore;
  // while (p != NULL)
  // {
  //   if (Max < p->highscore)
  //   {
  //     Max = p->highscore;
  //   }

  //   p = p->next;
  // }
}
void reWriteFile(node *head)
{
  FILE *fWrite;
  fWrite = fopen("account.txt", "w");
  for (node *p = head; p->next != NULL; p = p->next)
  {
    fputs(p->username, fWrite);
    fputs(" ", fWrite);
    fputs(p->password, fWrite);
    fputs(" ", fWrite);
    fprintf(fWrite, "%d", p->highscore);
    fputs("\n", fWrite);
  }
  fclose(fWrite);
}
void handlCliMes(char *mes)
{
  int i = 0;
  char *p;
  p = strtok(mes, "_");
  while (p != NULL)
  {
    // Chỉ dịnh đối số NULL trong hàm strtok để tiếp tục tách chuỗi ban đầu
    p = strtok(NULL, "_");

    if (p != NULL)
    {
      if (i == 1)
      {
        strcpy(passwordCli, p); // get password client
      }
      else
        strcpy(usernameCli, p); // get username client
    }
    i++;
  }
}
void onopen(int fd)
{
  char *cli;
  cli = ws_getaddress(fd);
#ifndef DISABLE_VERBOSE
  printf("Connection opened, client: %d | addr: %s\n", fd, cli);
#endif
  free(cli);
}

void onclose(int fd)
{
  char *cli;
  cli = ws_getaddress(fd);
#ifndef DISABLE_VERBOSE
  printf("Connection closed, client: %d | addr: %s\n", fd, cli);
#endif
  free(cli);
}

void onmessage(int fd, const unsigned char *msg, uint64_t size, int type) // viết xử lý vào trong này luôn
{
  char res[100];
  char users[100];
  int loginScore = 0;
  int top1 = 0;
  int state = 0;
  char *cli;
  cli = ws_getaddress(fd);
#ifndef DISABLE_VERBOSE
  strcpy(buffer, msg);

  switch (buffer[0])
  {
  case '0':
  { //đăng ký
    handlCliMes(buffer);
    int check = checkDupAcc(root, usernameCli);
    if(check == -1 ) {
      state = 5;
      break;
    }
    root = registerAcc(root, usernameCli, passwordCli);
    Traverser(root);
    state = 0;
    break;
  }
  case '1':
  {
    Traverser(root);
    handlCliMes(buffer); // lấy được username password của người dùng input
    loginScore = logIn(root, usernameCli, passwordCli);
    state = 1;
    break;
  }
  case '2':
  { // update điểm
    // roar = 1;
    handlCliMes(buffer);
    int newScore = atoi(passwordCli);
    // check top1 or not
    top1 = top1Score(root);
    updateScore(root, usernameCli, newScore);
    loginScore = newScore;
    state = 2;
    break;
  }
  case '3':
  {
    // loginScore = top1Score(root);
    handlCliMes(buffer);
    state = 3;
    break;
  }
  case '4':
  {
    rank(root, users);
    // printf("%s", res);
    state = 4;
    break;
  }
  default:
    state = -1;
    break;
  }
  // if (buffer[0] == '3')
  // {
  //   // gáy
  //   roar = 1;
  //   handlCliMes(buffer); // lấy được username password của người dùng input
  //   loginScore = logIn(root, usernameCli, passwordCli);
  // }
  printf("I receive a message: %s (size: %" PRId64 ", type: %d), from: %s/%d\n", msg, size, type, cli, fd);
#endif
  free(cli);
  switch (state)
  {
  case 0:
    ws_sendframe_txt(fd, "0_1", false);
    break;
  case 1:
    sprintf(res, "1_%d", loginScore);
    ws_sendframe_txt(fd, res, false);
    break;
  case 2:
    if (loginScore >= top1)
    {
      sprintf(res, "2_1_%s_%d", usernameCli, loginScore);
      ws_sendframe_txt(fd, res, true);
    }
    else
    {
      sprintf(res, "2_2_%d", loginScore);
      ws_sendframe_txt(fd, res, false);
    }
    break;
  case 3:
    sprintf(res, "3_%s_%s",usernameCli , passwordCli);
    ws_sendframe_txt(fd, res, true);
    break;
  case 4:
    sprintf(res, "4_%s", users);
    ws_sendframe_txt(fd, res, false);
    break;
  case 5:
    ws_sendframe_txt(fd, "0_0", false);
    break;
  default:
    ws_sendframe_txt(fd, "Lỗi gì đó ngớ ngẩn!!!", false);
    break;
  }
}
int main(void)
{
  struct ws_events evs;
  FILE *fRead;
  root = (node *)malloc(sizeof(node));
  char *usernameBuffer = (char *)malloc(sizeof(char) * 100);
  char *passwordBuffer = (char *)malloc(sizeof(char) * 100);
  int highscoreBuffer;
  fRead = fopen("account.txt", "r");
  while (fscanf(fRead, "%s %s %d", usernameBuffer, passwordBuffer, &highscoreBuffer) != EOF)
  {
    root = AddHead(root, usernameBuffer, passwordBuffer, highscoreBuffer);
  }
  fclose(fRead);
  evs.onopen = &onopen;
  evs.onclose = &onclose;
  evs.onmessage = &onmessage;
  ws_socket(&evs, 8080, 0);

  return (0);
}