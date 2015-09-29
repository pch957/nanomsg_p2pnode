#include <assert.h>
#include <stdio.h>
#include <nanomsg/nn.h>
#include <nanomsg/bus.h>

int node (const int argc, const char **argv)
{
  int sock = nn_socket (AF_SP, NN_BUS);
  assert (sock >= 0);
  assert (nn_bind (sock, argv[2]) >= 0);
  sleep (1); // wait for connections
  if (argc >= 3)
    {
      int x=3;
      for(x; x<argc; x++)
        assert (nn_connect (sock, argv[x]) >= 0);
    }
  sleep (1); // wait for connections
  int to = 100;
  assert (nn_setsockopt (sock, NN_SOL_SOCKET, NN_RCVTIMEO, &to, sizeof (to)) >= 0);
  char verify[100];
  strcpy(verify, "");
  while (1)
    {
      // RECV
      char *buf = NULL;
      int recv = nn_recv (sock, &buf, NN_MSG, 0);
      if (recv >= 0 && strcmp(buf, verify))
      {
          strcpy(verify, buf);
          printf ("%s: RECEIVED '%s' FROM BUS\n", argv[1], buf);

          // SEND
          int sz_n = strlen(buf) + 1; // '\0' too
          int send = nn_send (sock, buf, sz_n, 0);
          assert (send == sz_n);
          nn_freemsg (buf);
      }
    }
  printf ("out \n");
  return nn_shutdown (sock, 0);
}

int main (const int argc, const char **argv)
{
  if (argc >= 3) node (argc, argv);
  else
    {
      fprintf (stderr, "Usage: bus <NODE_NAME> <BIND_URL> <CONNECT_URL>  <CONNECT_URL>...\n");
      return 1;
    }
}

