#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// info about mounted volumes
#include <sys/param.h>
#include <sys/ucred.h>
#include <sys/mount.h>

// spawning processes
#include <unistd.h>
#include <errno.h>


const char *filename = "/.dril";

const char *open = "/usr/bin/open";
const char *iterm = "iTerm";

int main(int argc, char* argv[]) {

    int cnt = getfsstat(NULL, 0, MNT_WAIT);

    long bufsize = cnt * sizeof(struct statfs);
    struct statfs *buf = (struct statfs*) malloc(bufsize);

    cnt = getfsstat(buf, bufsize, MNT_WAIT);

    for(int i = 0; i < cnt; ++i) {
        char *mnt_name = buf[i].f_mntonname;

        char *mnt_name_cpy;
        strcpy(mnt_name_cpy, mnt_name);
        char *mnt_root = strtok(mnt_name_cpy, "/");

        // root mounted directory must be Volumes
        if(!mnt_root || strcmp(mnt_root, "Volumes") != 0)
            continue;

        char *filepath;
        strcpy(filepath, mnt_name);
        strcat(filepath, filename);

        printf("scanning %s \n", mnt_name);
        FILE* fp = fopen(filepath, "r");

        if(!fp)
            continue;

        printf("found %s \n", filepath);
        fclose(fp);
    }

    pid_t pid = fork();
    
    // if(pid == -1)
    if(pid == -1) {
        printf("Failed to spawn.");
        return 1;
    }

    else if(pid == 0) { // child
        execl(open, "-W", "-a", iterm, NULL);

        // execve failed to change the process
        char* err_desc = strerror(errno);
        FILE* fp = fopen("/tmp/com.dril.agent.err", "a");
        fprintf(fp, "err: %s \n", err_desc);
        fclose(fp);

        exit(1);
    }
    
    int status = 0;
    while( wait(&status) > 0 );
    
    exit(0); 
}