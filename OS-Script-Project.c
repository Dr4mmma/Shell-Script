/*
** Name: Mohammad Tayseer Mohammad Abu Mailiesh
** Date: October, 23th 2019
** Project: Writing my own shell script
** Overview: A program that works as a standalone shell that acts like linux shell
*/

#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <signal.h>
#include <math.h>
#include <sys/wait.h>
#include <sys/types.h>


int my_atoi(const char* snum)
{    //Converting ASCII to Integer
     int nInt = 0;
     int index = 0;
     while(snum[index])
 {
     if(!nInt)
     nInt= ( (int) snum[index]) - 48;
     else
     nInt = (nInt *= 10) + ((int) snum[index] - 48);
     
     index++;
 }
 return(nInt);
}

//Built in commands
void gtexit()
{
    exit(0); //Terminates the shell
}

void gtpid()
{
    printf("Current process ID is: %d\n", getpid()); //Returns working process ID
}

void gtppid()
{
    printf("Parent process ID is: %d\n", getppid()); //Returns parent process ID
}

int stcd( int argc, char* path)
{   //Changes the directory 
    if( argc < 0 ) 
    {
        fprintf( stderr, "Use: cd <directory>\n" );
        return EXIT_FAILURE;
    }

    if( chdir( path ) == 0 )
    {
        printf("Directory has been changed to %s\n", path);
        return EXIT_SUCCESS;
    }
    else 
    {
        perror( path );
        return EXIT_FAILURE;
    }
}

void gtpwd()
{ 
    printf(" %s \n",  (char*)get_current_dir_name() ); //Print the working directory
}

void stset( const char *name, const char *value)
{   //Sets a new command in the enviroment
    if( setenv( name, value, 1) == 0)
    printf("This %s has been saved to the enviroment \n", name );
    else
    printf("This %s has not been saved to the enviroment, please try again! \n", name );
}

void gtget( const char *name)
{   //Returns a command from the enviroment
    printf("%s= %s\n", name, getenv( name ));
}

//Defining functions that works exactly like in the terminal in linux and run in background
int gtps()
{   //Opens processes status
    printf("\n");
    int e = 0;

    char *const parameterlist[] = {"ps", "-o", "pid, ppid, time", NULL, NULL};
    e = execvp("/bin/ps", parameterlist);

    return e;
}

int gtls()
{    //Listing information about files and directories within the file system
     char* curr_dir = NULL;
     DIR* dp = NULL;
     struct dirent* dptr = NULL;
     unsigned int count = 0;

     curr_dir = (char*)get_current_dir_name();
     if(NULL == curr_dir)
     {
          printf("\n ERROR : Could not get the working directory\n");
          return -1;
     }

     dp = opendir((const char*)curr_dir);
     if(NULL == dp)
     {
          printf("\n ERROR : Could not open the working directory\n");
          return -1;
     }

     for(count = 0; NULL != (dptr = readdir(dp)); count++)
     {
          printf("%s  ",dptr->d_name);
     }
     printf("  [ %u ]\n", count);

     return 0;

}

void not_found(char *command)
{   //Printing error for the user that he entered a wrong command
    printf("Can't execute %s; No such file or directory \n", command);
}

int stsleep(int seconds)
{   //Making the system sleep
    return sleep(seconds);
}

void gtkill(int pid)
{   //Terminating the working process
    kill(pid, SIGTERM);
    printf("[%d] has been killed \n", pid);
}

//Calling and executing the above functions
void run( char* command )
{
    int i = 0;
    char *chrcs[9];
    char *chrc;
    char *spacing = command;

    while ((chrc = strtok_r(spacing, "\n  ", &spacing)))
    {
        chrcs[i] = chrc;
        i++;
    }

    if(strcmp( chrcs[0], "exit" ) == 0) 
    {
        gtexit();
    }
    else 
    if(strcmp( chrcs[0], "pid" ) == 0)
    {
        gtpid();
    }
    else 
    if(strcmp( chrcs[0], "ppid" ) == 0)
    {
        gtpid();
    }    
    else 
    if(strcmp( chrcs[0], "cd" ) == 0)
    {
        stcd(i, chrcs[1]);
    }    
    else 
    if(strcmp( chrcs[0], "pwd" ) == 0)
    {
        gtpwd();
    }
    else 
    if(strcmp( chrcs[0], "set" ) == 0)
    {
        stset(chrcs[1], chrcs[2]);
    }
    else 
    if(strcmp( chrcs[0], "get" ) == 0)
    {
        gtget(chrcs[1]);
    }
    else 
    if(strcmp( chrcs[0], "ls" ) == 0)
    {
        int ret = fork();

        if (ret == 0)
        {
             
             printf("\n[%d] ls \n",getpid());
             int e = gtls() ;

             if(e == 0)
             printf("[%d] ls Exit %d",getpid(), EXIT_SUCCESS);
             else if (e == -1)
             printf("[%d] ls Exit %d",getpid(), EXIT_FAILURE);


        }
        else
        {


             if(i >= 2 && strcmp(chrcs[1], "&" ) == 0 )
             return; else wait(NULL);

             return;

        }
        exit (0);

    }
    else 
    if(strcmp( chrcs[0], "ps" ) == 0)
    {
        
        int ret = fork();

        if (ret == 0)
        {
             int e = gtps() ;

             
             printf("\n[%d] ps \n",getpid());

             if(e != -1)
             printf("[%d] ps Exit %d",getpid(), EXIT_SUCCESS);
             else if (e == -1)
             printf("[%d] ps Exit %d",getpid(), EXIT_FAILURE);


        }
        else
        {
             if(i >= 2 && strcmp(chrcs[1], "&" ) == 0 )
             return; else wait(NULL);

             return;

        }
        exit (0);

    }
    else 
    if(strcmp( chrcs[0], "sleep" ) == 0)
    {
        int ret = fork();

        if (ret == 0)
        {
             int e = 0 ;
      
             printf("\n[%d] sleep \n", getpid());
             e = stsleep( my_atoi( chrcs[1]) );

             if(e == 0)
             printf("[%d] sleep Exit %d", getpid(), EXIT_SUCCESS);
             else if (e != 0)
             printf("[%d] sleep Killed (%d)", getpid(), e);


        }
        else
        {

             if(i >= 3 && strcmp(chrcs[2], "&" ) == 0 )
             return; else wait(NULL);
 
             return;

        }
        exit (0);

    }
    else 
    if(strcmp( chrcs[0], "sleep" ) == 0)
    {
        int ret = fork();

        if (ret == 0)
        {
             int e = 0 ;
      
             printf("\n[%d] sleep \n", getpid());
             e = stsleep( my_atoi( chrcs[1]) );

             if(e == 0)
             printf("[%d] sleep Exit %d", getpid(), EXIT_SUCCESS);
             else if (e != 0)
             printf("[%d] sleep Killed (%d)", getpid(), e);


        }
        else
        {

             if(i >= 3 && strcmp(chrcs[2], "&" ) == 0 )
             return; else wait(NULL);

             return;

        }
        exit (0);

     }
     else 
     if(strcmp( chrcs[0], "kill" ) == 0)
     {

        int ret = fork();

        if (ret == 0)
        {
             int e = 0 ;
        
             printf("\n[%d] kill \n", getpid());
             gtkill( my_atoi(chrcs[1]) );
             printf("[%d] kill Exit %d", getpid(), EXIT_SUCCESS);

        }
        else
        {

             if(i >= 3 && strcmp(chrcs[2] ,"&" ) == 0 )
             return; else wait(NULL);

             return;

        }
        exit (0);


     }
     else
     {
        
        int ret = fork();    
        
        if(ret == 0)
        {    
             int e = 0;
             printf("\n[%d] %s \n", getpid(), chrcs[0]);
             not_found( chrcs[0] );
             printf("[%d] %s Exit %d",getpid(), chrcs[0], EXIT_SUCCESS);

        }
        else
        {


             if(i >= 3 && strcmp(chrcs[2] ,"&" ) == 0 )
             return; else wait(NULL);

             return ;

        }
        exit (0);

     }

}

//This is where the code runs
int main(int argc, char **argv)
{

      char *user_prompt = "451sh>" ;

      int  MAX_LEN = 5000 ;
      char *command=malloc(sizeof(char)*MAX_LEN);

      if(argc == 3 && strcmp(argv[1] ,"-p" ) == 0)
              user_prompt = strcat( argv[2] , ">" );

      while (1)
      {
              printf("\n%s ", user_prompt);
              fgets(command, MAX_LEN, stdin);

              if(strcmp(command, " " ) > 0)
              run(command);
      }


      return 0;

}
