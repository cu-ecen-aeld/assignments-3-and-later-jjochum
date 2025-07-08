#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>

int main(int argc, char** argv)
{
	const char* output = argv[1];
	const char* text = argv[2];

	int rc = 0;
	openlog("finder", 0, 0);

	if (argc != 3)
	{
		fprintf(stderr, "Please pass a file and a text\n");
		rc = 1;
		goto exit;
	}
	else
	{
		syslog(LOG_DEBUG|LOG_USER, "Writing %s to %s", text, output);
	}

	int fd = open(output, O_WRONLY|O_CREAT);
	if (fd == -1)
	{
		syslog(LOG_ERR|LOG_USER, "Unable to open %s: %s", output, strerror(errno));
	}
	int written = write(fd, text, strlen(text));
	close(fd);
	if (written != strlen(text))
	{
		syslog(LOG_ERR|LOG_USER, "Unable to write complete string");
	}
exit:
	closelog();
	return rc;
}
