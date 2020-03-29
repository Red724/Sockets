/* Код программы для сервера */
#include <cstring>
#include <cstdio>
#include <stdlib.h> /* exit */

#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <unistd.h> /* read, write, open, close */

#define SERVER_PORT 12345 /* По договоренности между клиентом и сервером */
#define BUF_SIZE 4096 /* Размер передаваемых блоков */
#define QUEUE_SIZE 10

void fatal(const char *string);

int main(int argc, char *argv[])
{
    int s, b, l, fd, sa, bytes, on = 1;
    char buf[BUF_SIZE]; /* буфер для исходящего файла */
    struct sockaddr_in channel; /* содержит IP-адрес */
    /* Создать структуру адреса для привязки к сокету */
    memset(&channel,
           0, sizeof(channel)); /* Обнуление channel */
    channel.
            sin_family = AF_INET;
    channel.sin_addr.
            s_addr = htonl(INADDR_ANY);
    channel.
            sin_port = htons(SERVER_PORT);
    /* Пассивный режим. Ожидание соединения */
    s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); /* создать сокет */
    if (s < 0) fatal("ошибка сокета");
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR,
               (char *) &on, sizeof(on));
    b = bind(s, (struct sockaddr *) &channel, sizeof(channel));
    if (b < 0) fatal("Ошибка связывания");
    l = listen(s, QUEUE_SIZE); /* Определение размера очереди */
    if (l < 0) fatal("Ошибка ожидания");
    /* Теперь сокет установлен и привязан. Ожидание и обработка соединения */
    while (1) {
        sa = accept(s, 0, 0); /* Блокировка в ожидании запроса соединения*/
        if (sa < 0) fatal("Ошибка доступа");
        read(sa, buf,
             BUF_SIZE); /* считать имя файла из сокета */
        /* Получить и вернуть файл */
        fd = open(buf, O_RDONLY); /* Открыть файл для отсылки */
        if (fd < 0) fatal("Ошибка открытия файла");
        while (1) {
            bytes = read(fd, buf, BUF_SIZE); /* Читать из файла */
            if (bytes <= 0) break; /* Проверка конца файла */
            write(sa, buf, bytes
            ); /* Записать байты в сокет */
        }
        close(fd); /* Закрыть файл */
        close(sa); /* Разорвать соединение */
    }
}

void fatal(const char *string)
{
    printf("%s\n", string);
    exit(1);
}