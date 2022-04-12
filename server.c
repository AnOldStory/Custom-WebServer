/*
   A simple server in the internet domain using TCP
   Usage:./server port (E.g. ./server 10000 )
*/
#include <stdio.h>
#include <sys/types.h>  // definitions of a number of data types used in socket.h and netinet/in.h
#include <sys/socket.h> // definitions of structures needed for sockets, e.g. sockaddr
#include <netinet/in.h> // constants and structures needed for internet domain addresses, e.g. sockaddr_in
#include <stdlib.h>
#include <strings.h>

#include <unistd.h> // write read close
#include <string.h> // strtok

char *DIRECTORY;

int appendFile(char *directory, char *msg, int newsockfd)
{
   printf("--------------------return appendBinary-----------------------------------\n");
   printf("directory : %s, msg: %s\n", directory, msg);
   FILE *file = fopen(directory + 1, "r"); // read with non binary
   if (file)
   {
      fseek(file, 0, SEEK_END);
      int file_size = ftell(file); // get file size
      fseek(file, 0, SEEK_SET);

      strcat(msg, "\n");

      char *response = (char *)malloc(strlen(msg) + file_size);
      memcpy(response, msg, strlen(msg) + 1);

      int read = 0; // file count
      while (1)
      {
         unsigned char buff[file_size + 1]; // temp buff
         bzero(buff, file_size + 1);
         int n = fread(buff, sizeof(char), file_size, file); // read file

         if (n > 0)
            memcpy(response + strlen(response) + read, buff, sizeof(char) * file_size); // append to response
         read += n;

         if (n <= file_size)
         {
            if (feof(file))
               printf("End of file\n");
            if (ferror(file))
               printf("Error reading\n");
            break;
         }
      }
      fclose(file);
      printf("response %s", response);
      fflush(stdin);
      write(newsockfd, response, strlen(response));
      free(response);
      fflush(stdin);

      return 0;
   }
   else
   {
      printf("File error\n\n");
      return 2;
   }
}

int appendBinary(char *directory, char *msg, int newsockfd)
{
   printf("--------------------return appendBinary-----------------------------------\n");
   printf("directory : %s, msg: %s\n", directory, msg);
   FILE *file = fopen(directory + 1, "rb"); // read with binary
   if (file)
   {
      fseek(file, 0, SEEK_END);
      int file_size = ftell(file); // get file size
      fseek(file, 0, SEEK_SET);

      strcat(msg, "\n");

      char *response = (char *)malloc(file_size + strlen(msg) + 10);
      memcpy(response, msg, strlen(msg) + 1);

      int read = 0; // file count
      while (1)
      {
         unsigned char buff[file_size + 1]; // temp buff
         bzero(buff, file_size + 1);
         int n = fread(buff, sizeof(char), file_size, file); // read file
         if (n > 0)
            memcpy(response + strlen(response) + read, buff, sizeof(char) * file_size + 1); // append to response
         read += n;
         if (n <= file_size)
         {
            if (feof(file))
               printf("End of file\n");
            if (ferror(file))
               printf("Error reading\n");
            break;
         }
      }
      fclose(file);

      write(newsockfd, response, strlen(response) + file_size);
      free(response);
      return 0;
   }
   else
   {
      /* error */
      printf("File error\n\n");
      return 2;
   }
}
int parseContentType(char *directory, char *msg, int newsockfd)
{
   printf("--------------------parseContentType-----------------------------------\n");
   if (!strcmp(directory, "/"))
   {
      /* if default directory */
      char *new_directory = "/index.html";
      strcat(msg, "text/html\n");
      int retval = appendFile(new_directory, msg, newsockfd);
      return retval;
   }
   else
   {
      /* parse extension of directory */
      char *temp = strdup(directory); // strcpy(malloc(1+strlen(directory)), directory);
      temp = strtok(temp, ".");
      char *file_ext = temp;
      while ((temp = strtok(NULL, ".")) != NULL)
      {
         if (temp != NULL)
            file_ext = temp;
      }

      if (file_ext == NULL)
      {
         strcat(msg, "text/html\n");
         int retval = appendFile(directory, msg, newsockfd);
         free(temp);
         return retval;
      }
      else if (!strcmp(file_ext, "html"))
      {
         strcat(msg, "text/html\n");
         int retval = appendFile(directory, msg, newsockfd);
         free(temp);
         return retval;
      }
      else if (!strcmp(file_ext, "css"))
      {
         strcat(msg, "text/css\n");
         int retval = appendFile(directory, msg, newsockfd);
         free(temp);
         return retval;
      }
      else if (!strcmp(file_ext, "js"))
      {
         strcat(msg, "application/javascript\n");
         int retval = appendFile(directory, msg, newsockfd);
         free(temp);
         return retval;
      }
      else if (!strcmp(file_ext, "txt"))
      {
         strcat(msg, "text/plain\n");
         int retval = appendBinary(directory, msg, newsockfd);
         free(temp);
         return retval;
      }
      else if (!strcmp(file_ext, "jpg") || !strcmp(file_ext, "jpeg"))
      {
         strcat(msg, "image/jpeg\n");
         int retval = appendBinary(directory, msg, newsockfd);
         free(temp);
         return retval;
      }
      else if (!strcmp(file_ext, "png"))
      {
         strcat(msg, "image/png\n");
         int retval = appendBinary(directory, msg, newsockfd);
         free(temp);
         return retval;
      }
      else if (!strcmp(file_ext, "gif"))
      {
         strcat(msg, "image/gif\n");
         int retval = appendBinary(directory, msg, newsockfd);
         free(temp);
         return retval;
      }
      else if (!strcmp(file_ext, "ico"))
      {
         strcat(msg, "image/x-icon\n");
         int retval = appendBinary(directory, msg, newsockfd);
         free(temp);
         return retval;
      }
      else if (!strcmp(file_ext, "pdf"))
      {
         strcat(msg, "application/pdf\n");
         int retval = appendBinary(directory, msg, newsockfd);
         free(temp);
         return retval;
      }
      else if (!strcmp(file_ext, "mp4"))
      {
         strcat(msg, "video/mp4\n");
         int retval = appendBinary(directory, msg, newsockfd);
         free(temp);
         return retval;
      }
      else if (!strcmp(file_ext, "mp3"))
      {
         strcat(msg, "audio/mpeg\n");
         int retval = appendBinary(directory, msg, newsockfd);
         free(temp);
         return retval;
      }
      else if (!strcmp(file_ext, "wav"))
      {
         strcat(msg, "audio/wav\n");
         int retval = appendBinary(directory, msg, newsockfd);
         free(temp);
         return retval;
      }
      else if (!strcmp(file_ext, "ogg"))
      {
         strcat(msg, "audio/ogg\n");
         int retval = appendBinary(directory, msg, newsockfd);
         free(temp);
         return retval;
      }
      else if (!strcmp(file_ext, "webm"))
      {
         strcat(msg, "video/webm\n");
         int retval = appendBinary(directory, msg, newsockfd);
         free(temp);
         return retval;
      }
      else if (!strcmp(file_ext, "svg"))
      {
         strcat(msg, "image/svg+xml\n");
         int retval = appendBinary(directory, msg, newsockfd);
         free(temp);
         return retval;
      }
      else if (!strcmp(file_ext, "mpg"))
      {
         strcat(msg, "video/mpeg\n");
         int retval = appendBinary(directory, msg, newsockfd);
         free(temp);
         return retval;
      }
      else
      {
         strcat(msg, "text/html\n");
         int retval = appendFile(directory, msg, newsockfd);
         free(temp);
         return retval;
      }
      return 2;
   }
}

// int return appendBinary

void error(char *msg)
{
   perror(msg);
   exit(1);
}

int main(int argc, char *argv[])
{
   int sockfd, newsockfd; // descriptors rturn from socket and accept system calls
   int portno;            // port number
   socklen_t clilen;

   char buffer[256];

   /*sockaddr_in: Structure Containing an Internet Address*/
   struct sockaddr_in serv_addr, cli_addr;

   int n;
   if (argc < 2)
   {
      fprintf(stderr, "ERROR, no port provided\n");
      exit(1);
   }

   /*Create a new socket
       AF_INET: Address Domain is Internet
       SOCK_STREAM: Socket Type is STREAM Socket */
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   if (sockfd < 0)
      error("ERROR opening socket");

   bzero((char *)&serv_addr, sizeof(serv_addr));
   portno = atoi(argv[1]); // atoi converts from String to Integer
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = INADDR_ANY; // for the server the IP address is always the address that the server is running on
   serv_addr.sin_port = htons(portno);     // convert from host to network byte order

   if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) // Bind the socket to the server address
      error("ERROR on binding");

   listen(sockfd, 5); // Listen for socket connections. Backlog queue (connections to wait) is 5

   while (1)
   {
      clilen = sizeof(cli_addr);
      /*accept function:
       1) Block until a new connection is established
       2) the new socket descriptor will be used for subsequent communication with the newly connected client.
     */
      newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
      if (newsockfd < 0)
         error("ERROR on accept");

      bzero(buffer, 256);
      n = read(newsockfd, buffer, 255); // Read is a block function. It will read at most 255 bytes
      if (n < 0)
         error("ERROR reading from socket");

      /* handle empty request */
      if (!strcmp(buffer, "\0"))
      {
         close(newsockfd);
         continue;
      }

      printf("Here is the message:\n%s\n", buffer);

      printf("----------------------strtok start ------------------------\n");

      char *method = strtok(buffer, " \r\n"); // not thread safe
      char *directory = strtok(NULL, " \r\n");
      char *http_version = strtok(NULL, " \r\n");

      printf("method %s\n", method);
      printf("directory %s\n", directory);
      printf("http_version %s\n", http_version);

      char msg[20480] = "HTTP/1.1 200 OK\nAccept-Ranges: bytes\nContent-type:";
      char *temp_msg = strdup(msg);

      char notFound[] = "HTTP/1.1 404 Not Found\nAccept-Ranges: bytes\nContent-type: text/html\n\n\n<html>\n<body>\n<h1>Not Found</h1>\n</body>\n</html>\n";

      if (parseContentType(directory, temp_msg, newsockfd) == 2)
         write(newsockfd, notFound, strlen(notFound));
      /* handle content */
      free(temp_msg);

      if (n < 0)
         error("ERROR writing to socket");

      close(newsockfd);
   }

   close(sockfd);

   return 0;
}
