#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <time.h>

#define MON_SERVER     "baidu.com"

int GetLocalIp(char *szBuf, int nLen)
{
	struct hostent *host;
	int i,sock,err;
	char server[16];
	const char *p;
	struct sockaddr_in serv;
	struct sockaddr_in local;
	socklen_t local_len = sizeof(local);

	if ((host = gethostbyname(MON_SERVER)) == NULL) 
	{
		return -1;
	}

	for (i = 0; host->h_addr_list[i]; i++) 
	{
		sock = socket(AF_INET, SOCK_STREAM, 0);

		if (sock < 0) continue;

		memset(&serv, 0, sizeof(serv));
		serv.sin_family = AF_INET;
		serv.sin_addr = *((struct in_addr *)host->h_addr_list[i]);
		serv.sin_port = htons(80);

		err = connect(sock, (struct sockaddr*)&serv, sizeof(serv));

		if (err < 0) 
		{
			close(sock);
			continue;
		}

		err = getsockname(sock, (struct sockaddr*)&local, &local_len);

		if (err < 0) 
		{
			close(sock);
			continue;
		}

		p = inet_ntop(AF_INET, &local.sin_addr, szBuf, nLen);

		if (!p) 
		{
			close(sock);
			continue;
		}

		close(sock);
		return 0;
	}

	printf("can't get local address\n");

	return -1;
}

int main()
{
	char priip[32] = { '\0' };
	if (GetLocalIp(priip, sizeof(priip)) < 0)
	{
		printf("Get local address error\n");
		return -1;
	}

	printf("%s\n", priip);

	return 0;
}
