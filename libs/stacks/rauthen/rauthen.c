#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <ctype.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>

#include "rauthen.h"
#include "linkedlist.h"

int
rauthen_init(HOST_EQUIV **hl, char *error)
{
#define _IS_EMPTY(res,p)										\
    register char *_p##res = (char *)(p);						\
    int res = 0;												\
    while (*_p##res && isspace(*_p##res)) {						\
        ++_p##res;												\
    }															\
    res = (*_p##res == '\0' || *_p##res == '#') ? 1:0 ;			\

    int ret = 1;
    FILE *hostf;
    struct passwd *pwd;
    uid_t uid = 0;
    struct stat sbuf;
    char pbuf[INIT_RHOST_BUFFER];
    HOST_EQUIV *he;

    //cleanup memory
    while ((he = *hl) != NULL)
    {
        LINKEDLIST_REMOVE((*hl), he);
        free(he);
    }

    //try system /etc/host.equiv
    hostf = fopen (_PATH_HEQUIV, "r");
    if (!hostf)
    {
        uid = getuid();
        pwd = getpwuid(uid);
        if (pwd != NULL)
        {
            if (strlen(pwd->pw_dir) + sizeof(USER_RHOSTS_FILE) + 2 >= INIT_RHOST_BUFFER)
            {
                if (error!=NULL) sprintf(error, "home directory for `%s' is too long", pwd->pw_name);
                goto exit_function;
            }

            (void) strcpy(pbuf, pwd->pw_dir);
            (void) strcat(pbuf, USER_RHOSTS_FILE);

            uid = geteuid();
            (void) seteuid(pwd->pw_uid);
            hostf = fopen(pbuf, "r");
            if( hostf )
            {
                if (lstat(pbuf, &sbuf) < 0 || !S_ISREG(sbuf.st_mode))
                {
                    if (error!=NULL) sprintf(error, ".rhosts not regular file");
                    goto exit_function;
                }
                else if (fstat(fileno(hostf), &sbuf) < 0)
                {
                    if (error!=NULL) sprintf(error, ".rhosts fstat failed");
                    goto exit_function;
                }
                else if (sbuf.st_uid && sbuf.st_uid != pwd->pw_uid)
                {
                    if (error!=NULL) sprintf(error, "bad .rhosts owner");
                    goto exit_function;
                }
                else if (sbuf.st_mode & S_IWOTH)
                {
                    if (error!=NULL) sprintf(error, ".rhosts writable by other!");
                    goto exit_function;
                }
                else if (sbuf.st_mode & S_IWGRP)
                {
                    /* private group caveat */
                    struct group *grp = getgrgid(sbuf.st_gid);

                    if (NULL == grp || NULL == grp->gr_name || strcmp(pwd->pw_name,grp->gr_name))
                    {
                        if (error!=NULL) sprintf(error, ".rhosts writable by public group");
                        goto exit_function;
                    }
                    else if (grp->gr_mem)
                    {
                        int gcount;

                        /* require at most one member (luser) of this group */
                        for (gcount=0; grp->gr_mem[gcount]; ++gcount)
                        {
                            if (strcmp(grp->gr_mem[gcount], pwd->pw_name))
                            {
                                gcount = -1;
                                break;
                            }
                        }
                        if (gcount < 0)
                        {
                            if (error!=NULL) sprintf(error, ".rhosts writable by other members of group");
                            goto exit_function;
                        }
                    }
                } //check group
            }//file instance
        }//pwd result
    }//not open  /etc/host.equiv

    if(hostf)
    {
        while (fgets(pbuf, sizeof(pbuf), hostf) != NULL)
        {
            char *p = pbuf;

            _IS_EMPTY(li_emp,p)
            if (li_emp)  continue;

            //Skip lines that are too long.
            if (strchr(p, '\n') == NULL)
            {
                int ch = getc(hostf);
                while (ch != '\n' && ch != EOF)
                    ch = getc(hostf);
                continue;
            }

            for (;*p && !isspace(*p); ++p)
            {
                *p = tolower(*p);
            }

            if (*p == ' ' || *p == '\t') {

                //<nul> terminate hostname and skip spaces
                for (*p++='\0'; *p && isspace(*p); ++p);

                while (*p && !isspace(*p))
                    ++p;                    // find end of user's name
            }
            *p = '\0';
            HOST_EQUIV * he =  (HOST_EQUIV*)malloc(sizeof(HOST_EQUIV));
            strcpy(he->data, pbuf);
            LINKEDLIST_APPEND((*hl), he);
        }
    }

exit_function:
    if( uid != 0 )
        (void)seteuid(uid);
    if (hostf != NULL)
        (void) fclose(hostf);

    return ret;
}

void
rauthen_free(HOST_EQUIV *hl, char *error)
{
    HOST_EQUIV *he;

    //cleanup memory
    while ((he = hl) != NULL)
    {
        LINKEDLIST_REMOVE(hl, he);
        free(he);
    }
}

static int
rauthen_checkhost ( unsigned int raddr, register char *lhost, const char *rhost)
{
    struct hostent *hp;
    unsigned int laddr;
    int negate=1;    /* Multiply return with this to get -1 instead of 1 */
    char **pp;

    if (strncmp("+@",lhost,2) == 0)
        return(innetgr(&lhost[2],rhost,NULL,NULL));

    if (strncmp("-@",lhost,2) == 0)
        return(-innetgr(&lhost[2],rhost,NULL,NULL));

    /* -host */
    if (strncmp("-",lhost,1) == 0)
	{
		negate=-1;
		lhost++;
    }
	else if (strcmp("+",lhost) == 0)
	{
		//if (opts->opt_promiscuous)
		//	return (1);   /* asking for trouble, but ok.. */
		/* If not promiscuous: handle as negative */
		//return (-1);

		return (1);
    }

    /* Try for raw ip address first. */
    if (isdigit(*lhost) && (in_addr_t)(laddr = inet_addr(lhost)) != -1)
		return (negate*(! (raddr ^ laddr)));

    /* Better be a hostname. */
    hp = gethostbyname(lhost);
    if (hp == NULL)
        return (0);

    /* Spin through ip addresses. */
    for (pp = hp->h_addr_list; *pp; ++pp)
	{
		if (!memcmp (&raddr, *pp, sizeof (unsigned int)))
			return (negate);
	}

    /* No match. */
    return (0);
}

int
rauthen_verify(HOST_EQUIV *hl, unsigned int raddr , const char *rhost, char *message)
{
    HOST_EQUIV *he_first = NULL;
    HOST_EQUIV *he = hl;
    int res = 0;

    if (!he) return RAUTHEN_FAIL;
    while(he)
    {
        res = rauthen_checkhost (raddr, he->data, rhost);
        if (message!=NULL) sprintf(message, "[remote authentication check] res = %d longip=%d cmp [%s = %s]", res, raddr, he->data, rhost);

        if(res==1) return RAUTHEN_PASS;

        if(!he_first) he_first = he;  //prevent infinite loop
        he = he->next;
        if(he ==  he_first) break;	//prevent infinite loop
    }
    
	return RAUTHEN_FAIL; //1 = pass , 0 = false
}


