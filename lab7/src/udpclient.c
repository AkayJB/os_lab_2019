#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define SADDR struct sockaddr
#define SLEN sizeof(struct sockaddr_in)

int main(int argc, char **argv) {
  int sockfd, n;
  int port, bufsize;
  char *sendline, *recvline;
  struct sockaddr_in servaddr;

  if (argc != 4) {
    printf("Usage: %s <IP-address> <Port> <Buffer Size>\n", argv[0]);
    exit(1);
  }

  port = atoi(argv[2]);
  bufsize = atoi(argv[3]);

  sendline = malloc(bufsize);
  recvline = malloc(bufsize + 1);

  if (!sendline || !recvline) {
    perror("Buffer allocation failed");
    exit(1);
  }

  memset(&servaddr, 0, SLEN);
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(port);

  if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) < 0) {
    perror("inet_pton problem");
    exit(1);
  }

  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket problem");
    exit(1);
  }

  write(1, "Enter string\n", 13);

  while ((n = read(0, sendline, bufsize)) > 0) {
    if (sendto(sockfd, sendline, n, 0, (SADDR *)&servaddr, SLEN) == -1) {
      perror("sendto problem");
      exit(1);
    }

    if (recvfrom(sockfd, recvline, bufsize, 0, NULL, NULL) == -1) {
      perror("recvfrom problem");
      exit(1);
    }

    recvline[n] = 0;  // Null-terminate the received string
    printf("REPLY FROM SERVER= %s\n", recvline);
  }

  free(sendline);
  free(recvline);
  close(sockfd);
  exit(0);
}
