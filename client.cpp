/* На этой странице содержится клиентская программа, запрашивающая файл у серверной
программы, расположенной на следующей странице. */
/* Сервер в ответ на запрос высылает файл.*/

#include <cstring>
#include <cstdio>
#include <stdlib.h> /* exit */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <unistd.h> /* read, write, open, close */

#define SERVER_PORT 12345 /* По договоренности между клиентом и сервером */
#define BUF_SIZE 4096 /* Размер передаваемых блоков */

void fatal(const char *string);

int main(int argc, char *argv[])
{
    int c,s,bytes;
    char buf[BUF_SIZE]; /*буфер для входящего файла */
    struct hostent *h; /*информация о сервере */
    struct sockaddr_in channel; /*хранит IP=адрес */
    if (argc!=3) fatal("Для запуска введите: client имя_сервера имя_файла");
    h = gethostbyname(argv[1]); /* поиск IP-адреса хоста */
    if(!h) fatal("Ошибка выполнения gethostbyname")
    s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (s<0) fatal("Сокет");
    memset(&channel, 0, sizeof(channel));
    channel.sin_family=AF_INET;
    memcpy(&channel.sin_addr.s_addr,h>h_addr,h>h_length);
    channel.sin_port=htons(SERVER_PORT);
    c = connect(s,(struct sockaddr *) &channel, sizeof(channel));
    if (c<0) fatal("Ошибка соединения");
    /* Соединение установлено. Посылается имя файла с нулевым байтом на конце */
    write*s, argv[2], strlen(argv[2])+1);
    /* Получить файл, записать на стандартное устройство вывода */
    while (1) {
        bytes = read(s, buf, BUF_SIZE); /* Читать из сокета */
        if (bytes <= 0) exit(0); /* Проверка конца файла */
        write(1, buf, bytes); /* Записать на стандартное устройство вывода */
    }
}

void fatal(const char *string)
{
    printf("%s\n", string);
    exit(1);
}