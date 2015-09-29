#include <assert.h>
#include <stdio.h>
#include <nanomsg/nn.h>
#include <nanomsg/bus.h>

int send(const int argc, const char **argv)
{
  int sock = nn_socket (AF_SP, NN_BUS);
  assert (sock >= 0);
  if (argc >= 2)
    {
      int x=2;
      for(x; x<argc; x++)
        assert (nn_connect (sock, argv[x]) >= 0);
    }
  sleep (1); // wait for connections
  int to = 100;
  assert (nn_setsockopt (sock, NN_SOL_SOCKET, NN_RCVTIMEO, &to, sizeof (to)) >= 0);
  // SEND
  int sz_n = strlen(argv[1]) + 1; // '\0' too
  printf ("%s: SENDING '%s' ONTO BUS\n", argv[1], argv[1]);
  int send = nn_send (sock, argv[1], sz_n, 0);
  assert (send == sz_n);
  return nn_shutdown (sock, 0);
}

int main (const int argc, const char **argv)
{
  if (argc >= 2) send(argc, argv);
  else
    {
      fprintf (stderr, "Usage: send <MSG> <CONNECT_URL> <CONNECT_URL> ...\n");
      return 1;
    }
}
