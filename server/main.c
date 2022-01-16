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
int registerAcc(node *head, char *username, char *password)
{
  head = AddHead(head, username, password, 0);
  reWriteFile(head);
  return 0;
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
  int loginScore, roar = 0;
  char *cli;
  cli = ws_getaddress(fd);
#ifndef DISABLE_VERBOSE
  strcpy(buffer, msg);
  if (buffer[0] == '1') // đăng nhập
  {
    handlCliMes(buffer); // lấy được username password của người dùng input
    loginScore = logIn(root, usernameCli, passwordCli);
  }
  if (buffer[0] == '2')
  { // update điểm
    roar = 1;
    handlCliMes(buffer);
    int x = atoi(passwordCli);
    updateScore(root, usernameCli, x);
    loginScore = x;
  }
  if (buffer[0] == '0')
  { //đăng ký
    handlCliMes(buffer);
    loginScore = registerAcc(root, usernameCli, passwordCli);
  }
  if (buffer[0] == '3')
  {
    // gáy
    roar = 1;
    handlCliMes(buffer); // lấy được username password của người dùng input
    loginScore = logIn(root, usernameCli, passwordCli);
  }
  printf("I receive a message: %s (size: %" PRId64 ", type: %d), from: %s/%d\n", msg, size, type, cli, fd);
#endif
  free(cli);
  if (loginScore > -1) // trả về lớn hơn -1 tức là đã có tài khoản
  {
    if (roar == 0)
    {
      char text[20];
      sprintf(text, "%d", loginScore);
      ws_sendframe_txt(fd, text, false);
    }
    else if (roar == 1)
    {
      char text[100];
      sprintf(text, "%s được %d điểm và đang gáy rất tooo!!!", usernameCli, loginScore);
      ws_sendframe_txt(fd, text, true);
      roar = 0;
    }
  }
  else if (loginScore == -1) // trả về -1 tức là chưa có tài khoản
  {
    ws_sendframe_txt(fd, "Tài khoản sai", false);
  }
  else
  {
    if (roar == 1)
    {
      char text[100];
      sprintf(text, "%s vừa đạt được %d điểm và đang gáy rất tooo!!!", usernameCli, loginScore);
      ws_sendframe_txt(fd, text, true);
      roar = 0;
    }
    else
      ws_sendframe(fd, (char *)msg, size, true, type); // gửi lại tin nhắn cho client
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