#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/* Write COUNT copies of MESSAGE to STREAM, pausing for a second
   between each.  */

void writer (const char* message, int count, FILE* stream)
{
    for (; count > 0; --count) {
       /* Write the message to the stream, and send it off immediately.   */
       fprintf (stream, "%s\n", message);
       fflush (stream);
       /* Snooze a while.  */
       sleep (1);
    }
}

/* Read random strings from the stream as long as possible.  */

void reader (FILE* stream)
{
    char buffer[1024];
    /* Read until we hit the end of the stream. fgets reads until
       either a newline or the end-of-file.  */
    while (!feof (stream)
           && !ferror (stream)
           && fgets (buffer, sizeof (buffer), stream) != NULL)
       fputs (buffer, stdout);
}

int main ()
{
    int fds[2];
    pid_t pid;

    char *names[] = {"Hello 5", "Goodbye 6", "No 8", "Goodbye 8"};
    int num = 4;

    /* Create a pipe. File descriptors for the two ends of the pipe are
       placed in fds.  */
    pipe (fds);
    /* Fork a child process.  */
    pid = fork ();
    if (pid == (pid_t) 0) {
      FILE* stream;
      /* This is the child process. Close our copy of the write end of
         the file descriptor.  */
      close (fds[1]);
      /* Convert the read file descriptor to a FILE object, and read
         from it.   */
      stream = fdopen (fds[0], "r");
      reader (stream);
        close (fds[0]);
      }
      else {
        /* This is the parent process.  */
        FILE* stream;
        /* Close our copy of the read end of the file descriptor.  */
        close (fds[0]);
        /* Convert the write file descriptor to a FILE object, and write
           to it.  */
        stream = fdopen (fds[1], "w");

        int n;
   char line[80];
   clrscr();

   fr = fopen ("/Users/Dylan/Desktop/out.txt", "rt");  /* open the file for reading */
   /* elapsed.dta is the name of the file */
   /* "rt" means open the file for reading text */

   while(fgets(line, 80, fr) != NULL)
   {
	 /* get a line, up to 80 chars from fr.  done if NULL */
	 sscanf (line, "%ld", &elapsed_seconds);
	 /* convert the string to a long int */
	 printf ("%ld\n", elapsed_seconds);
   }
   fclose(fr);  /* close the file prior to exiting the routine */

        for (int i = 0; i < num; i++) {

            writer (names[i], 1, stream);
        }

        close (fds[1]);
      }

      return 0;
}
