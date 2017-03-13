#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <limits.h>

struct CommandArgv
{
	char arguments[50][50];
};

struct process
{
	int pid;
	int job_id;
	int pstate; //-1 stopped FG 1 BG 2 
};

struct process processes[100];

void pip_command_exec( char* );

int insert_pid( int pid, int pstate )
{
	int i=1;
	while ( processes[i].job_id != 0 )
	{
		i++;
	}
	//printf( "Inserted at location %d\n", i );
	if( i <= 100 )
	{
		processes[i].job_id = i;
		processes[i].pid = pid;
		processes[i].pstate = pstate;  
	}
	else
	{
		printf( "No space left\n" );
	}

	return i;
}

void remove_pid( int pid )
{
	int i = 1;
	int flag = 0;
	while ( processes[i].pid != pid )
	{
		if ( i > 100 )
		{
			flag = 1;
			break;
		}
		i++;		
	}

	if ( flag )
	{
		fprintf(stderr, "No Such Process Found\n");
	}
	else
	{
		//printf( "Removed from location %d\n", i );
		processes[i].job_id = 0;
		processes[i].pid = 0;
		processes[i].pstate = 0;
	}
}

int get_pid( int job_id )
{
	int i = 1, pid;
	int flag = 0;
	while ( processes[i].job_id != job_id )
	{
		if ( i > 100 )
		{
			flag = 1;
			break;
		}
		i++;		
	}

	if ( flag )
	{
		//fprintf(stderr, "No Such Job Found\n");
		pid = -1;
	}
	else
	{
		//printf( "Job at location %d\n", i );
		pid = processes[i].pid;
	}

	return pid;
}

int get_jobid( int pid )
{
	int i = 1, job_id;
	int flag = 0;
	while ( processes[i].pid != pid )
	{
		if ( i > 100 )
		{
			flag = 1;
			break;
		}
		i++;		
	}

	if ( flag )
	{
		//fprintf(stderr, "No Such Job Found\n");
		job_id = -1;
	}
	else
	{
		//printf( "Job at location %d\n", i );
		job_id = processes[i].job_id;
	}

	return job_id;
}

int get_pstate( int pid )
{
	int i = 1, pstate;
	int flag = 0;
	while ( processes[i].pid != pid )
	{
		if ( i > 100 )
		{
			flag = 1;
			break;
		}
		i++;		
	}

	if ( flag )
	{
		//fprintf(stderr, "No Such Job Found\n");
		pstate = -1;
	}
	else
	{
		//printf( "Job at location %d\n", i );
		pstate = processes[i].pstate;
	}

	return pstate;
}

int get_current_pid()
{
	int i=1, cpid;
	int flag = 0;
	while ( processes[i].pstate != 1 )
	{
		if ( i > 100 )
		{
			flag = 1;
			break;
		}
		i++;		
	}

	if ( flag )
	{
		//fprintf(stderr, "No Such Job Found\n");
		cpid = -1;
	}
	else
	{
		//printf( "Job at location %d\n", i );
		cpid = processes[i].pid;
	}
	return cpid;
}

void change_pid_status( int pid, int new_pstate )
{
	int i = 1;
	int flag = 0;
	while ( processes[i].pid != pid )
	{
		if ( i > 100 )
		{
			flag = 1;
			break;
		}
		i++;		
	}

	if ( flag )
	{
		//fprintf(stderr, "No Such Job Found\n");
		//job_id = -1;
	}
	else
	{
		//printf( "Job at location %d\n", i );
		processes[i].pstate = new_pstate;
	}
}

char * trim_whitespaces( char * string )
{
	int first_i = 0;
	int last_i = strlen( string ) - 1;
	while( string[first_i] == ' ' )
	{
		first_i++;
	}

	if( first_i > 0 )
	{
		int i;
		for( i=0; i<=last_i; i++ )
		{	
			string[i] = string[i+first_i];
		}
	}

	if( last_i > 0 && string[last_i] == '\n')
	{
		string[last_i] = '\0';
	}

	while( string[last_i] == ' ' )
	{
		string[last_i] = '\0';
		last_i = last_i-1;
	}

	return string;
}

void split_tream_input( char *input_command, struct CommandArgv *com_argv, char * symbol )
{
	int i = 0;
	char *temp_arr;

	temp_arr = strtok( input_command, symbol );

	while( temp_arr != NULL )
	{
		strcpy(com_argv->arguments[i], trim_whitespaces(temp_arr) );
		//printf("%s", arguments[i] );
		//printf("%s\n", "|" );
		i++;
		temp_arr = strtok( NULL, symbol ); 
	}
}

void split_input( char *input_command, struct CommandArgv *com_argv )
{
	int i=0;
	char *temp_arr;
	temp_arr = strtok( input_command, " " );
	// for( i=0; i<2; i++ )
	// {
	// 	com_argv->arguments[i] = temp_arr[i];
	// }
	while( temp_arr != NULL )
	{
		//printf( "%s\n", temp_arr );
		strcpy(com_argv->arguments[i], temp_arr);
		i++;
		temp_arr = strtok( NULL, " " );
	}
	//char t_arr[50] = {0};
	//strcpy(com_argv->arguments[i], t_arr);
}

void command_exec(char *input_command)
{
	int err, i=0;
	//char *env[1] = {0};
	char *argv[50];
	struct CommandArgv com_argv;
	split_input(input_command, &com_argv);
	//com_argv.arguments = {input_command, 0 };
	char str[] = "/bin/";
	strcat( str, com_argv.arguments[0]);
	while( strcmp(com_argv.arguments[i], "" ) != 0)
	{
		argv[i] = com_argv.arguments[i];
		i++;
	}
	argv[i] = NULL;
	err = execvp( com_argv.arguments[0], argv );
	exit(err);
}

void redi_command_exec( char* input_command, int inout )
{
	int error, i=0, has_pipe, has_inpcomm;
	char *arguments[50];
	char *arguments2[50];
	char *temp_arr;
	char* symb;
	int output_file_fd;
	int input_file_fd;
	if( inout )
	{
		symb = ">";
	}
	else
	{
		symb = "<";
	}
	temp_arr = strtok( input_command, symb );

	while( temp_arr != NULL )
	{
		arguments[i] = trim_whitespaces(temp_arr);
		i++;
		temp_arr = strtok( NULL, symb ); 
	}
	char * filename = arguments[i-1];
	i=0;

	char *argzero = arguments[0];
	has_pipe = command_pipe_status( arguments[0] );
	has_inpcomm = command_rif_status( arguments[0], 0 );
	if( !has_pipe && !has_inpcomm )
	{
		struct CommandArgv redi_com_argv;
		split_input(arguments[0], &redi_com_argv);
		while( strcmp(redi_com_argv.arguments[i], "" ) != 0)
		{
			arguments2[i] = redi_com_argv.arguments[i];
			//printf("%s", arguments[i] );
			//printf("%s\n", "|" );
			i++;
		}

		arguments2[i] = NULL;
	}
	 // printf("%s", trim_whitespaces(str) );
	 // printf("%s\n", "|" );
	char stro[] = "/bin/";
	char stri[] = "/usr/bin/";
	if( inout )
	{
		strcat( stro, arguments2[0]);
		//printf("%d\n",inout );
		output_file_fd = open(filename, O_WRONLY | O_TRUNC | O_CREAT, 0770 );
		if( output_file_fd < 0 )
		{
			perror( input_command );
			exit(1);
		}

		dup2( output_file_fd, 1 );
		if( has_pipe )
		{
			pip_command_exec( argzero );
		}
		else if( has_inpcomm )
		{
			redi_command_exec( argzero, 0 );
		}
		else
		{
			error = execvp( arguments2[0], arguments2 );
			//close( output_file );
			exit(error);
		}
	}
	else
	{
		strcat( stri, arguments2[0]);
		input_file_fd = open( filename, O_RDONLY );
		if( input_file_fd == -1 )
		{
			fprintf( stderr, "%s: No such file or directory.\n", filename );
			exit(0);
		}

		dup2( input_file_fd, 0 );
		close( input_file_fd );
		error = execvp( arguments2[0], arguments2 );
		exit(error);
	}
}

void pip_command_exec_pro( char **current, char * redi_flag, int last )
{
	int process[2], pid, status, process_in = 0, process_no = 1;

    while( *current != NULL )
    {
    	if ( pipe( process ) == -1) 
    	{
            perror( "pipe" );
            exit(1);
	    }

	    pid = fork();
		if( pid < 0) 
		{
			perror( "fork" );
			exit(1);
	    }

	    if ( pid == 0 ) {
	        
	        dup2( process_in, 0 );
	        if ( *(current + 1) != NULL)
	        {
	        	dup2( process[1], 1 );   /* make stdout same as process[1] */
	        }
	        close( process[0] ); /* we don't need the read end -- process[0] */

	        if( ( process_no == 1 ) && ( ( strcmp( redi_flag, "i" ) == 0 ) || ( strcmp( redi_flag, "b" ) == 0 ) ) )
			{
				redi_command_exec(*current, 0);
			}

			if( ( process_no == last ) && ( ( strcmp( redi_flag, "o" ) == 0 ) || ( strcmp( redi_flag, "b" ) == 0 ) ) )
			{
				redi_command_exec(*current, 1);
			}

	        command_exec(*current);

			/* control never reaches here */
	    }
	    else
	    {
	    	waitpid( pid, &status, 0 );
	    	close( process[1] );
	    	process_in = process[0];
	    	*current++;
	    	process_no++;
	   	}     	
    }
}

void pip_command_exec( char* input_command )
{
	int redirection_i = command_rif_status( input_command, 0 ); // in
	int redirection_o = command_rif_status( input_command, 1 ); // out
	char* redi_flag;
	if( redirection_i && redirection_o )
	{
		redi_flag = "b"; //both
	}
	else if( redirection_i )
	{
		redi_flag = "i"; //only input
	}
	else if( redirection_o )
	{
		redi_flag = "o"; //only output
	}
	else
	{
		redi_flag = "n"; //none
	}
	
	int i=0;
	char *commands[50];
	struct CommandArgv pip_com_argv;
	char* symb = "|";
	split_tream_input( input_command, &pip_com_argv, symb );
	while( strcmp(pip_com_argv.arguments[i], "" ) != 0)
	{
		commands[i] = pip_com_argv.arguments[i];
		i++;
	}
	commands[i] = NULL;
	pip_command_exec_pro( commands, redi_flag, i-1 );
	exit(0);	

}

void jobs_exec()
{
	int i=1; char * status;
	fprintf( stdout, "%3s\t%8s\t%s\n", "JOB_id", "PID", "JOB_status" );
	
	for( i=1; i<100; i++ )
	{
		if( processes[i].pid != 0 && processes[i].job_id != 0 )
		{
			if( processes[i].pstate == 1 )
			{
				status = "Foreground";
			}
			else if( processes[i].pstate == 2  )
			{
				status = "Background";
			}
			else
			{
				status = "Stopped";
			}
			fprintf( stdout, "%3d\t%8d\t%s\n", processes[i].job_id, processes[i].pid, status );
		}
	}
}

void fg_command_exec( char* input_command )
{
	int pid = 0, status, pstate;
	int last_i = strlen( input_command ) - 1;
	int job_id = atoi(&input_command[last_i]);
	pid = get_pid( job_id );
	if( pid == -1 )
	{
		printf( "fg: No such job.\n"  );
	}
	else
	{
		int pstatus = get_pstate( pid );
		//pstate = get_pstate( pid );
		change_pid_status( pid, 1 );
		killpg( pid, SIGCONT );
		waitpid( pid, &status, WUNTRACED );
		//waitpid( pid );
		if( pstatus == -1 )
		{
			remove_pid( pid );
		}
		//remove_pid( pid );
	}
}

void kill_command_exec( char* input_command )
{
	int error, pid, job_id;
	struct CommandArgv k_com_argv;
	int i=0;
	char *temp_arr;
	temp_arr = strtok( input_command, " " );
	while( temp_arr != NULL )
	{
		strcpy( k_com_argv.arguments[i], temp_arr );
		i++;		
		temp_arr = strtok( NULL, " " );
	}
	char * id = k_com_argv.arguments[1];
	char * substr;
	substr = strchr( id, '%' );
	if( substr != NULL )
	{
		job_id = atoi( &id[1] );
		pid = get_pid( job_id );
	}
	else
	{
		pid = atoi( id );
		job_id = get_jobid( pid );
		if( job_id == -1 )
		{
			pid = -1;
		}
	}

	if( pid != -1 )
	{
		killpg( pid, SIGKILL );
	}
	else
	{
		printf( "kill: No such job.\n" );
	}
	//remove_pid( pid );
}

char * get_parent_path( char * current_path )
{
	//fprintf(stderr, "%s\n", current_path );
	int i=0, location;
	while( current_path[i] != '\0' )
	{
		if ( current_path[i] == '/' )
		{
			location=i;
			i++;
		}
		else
		{
			i++;
		}
	}
	current_path[location] = '\0';
	return current_path;
}

void cd_command_exec( char * input_command )
{
	int cd_success;
	char * current_path = malloc( PATH_MAX );
	getcwd( current_path, PATH_MAX );
	struct CommandArgv com_argv;
	split_input( input_command, &com_argv );
	char back[2]= "..";
	char * changePath;
	if ( strcmp( com_argv.arguments[1], back ) == 0 )
	{
		char * parent_path = get_parent_path( current_path );
		cd_success = chdir( parent_path );
	}
	else 
	{
		if ( com_argv.arguments[1][0] == '/' )
		{
			changePath = com_argv.arguments[1];
			cd_success = chdir( changePath );
		}	
		else
		{
			changePath = strcat( current_path, "/" );
			changePath = strcat( changePath, com_argv.arguments[1] );
			cd_success = chdir( changePath );
		}
	}

	if( cd_success == -1 )
	{
		printf( "%s: No such file or directory.\n", com_argv.arguments[1] );
	}
}

int command_cd_status( char *input_command )
{
	char * substr;
	char * str2 = "cd ";
	substr = strstr( input_command, str2 );
	if(substr != NULL)
	{
		return 1;
	}

	return 0;
}

int command_kill_status ( char * input_command )
{
	char * substr;
	char * str2 = "kill ";
	substr = strstr( input_command, str2 );
	if(substr != NULL)
	{
		return 1;
	}

	return 0;
}

int command_jobs_status( char * input_command )
{
	char * substr;
	char * str2 = "jobs";
	substr = strstr( input_command, str2 );
	if(substr != NULL)
	{
		return 1;
	}

	return 0;
}

int command_fgc_status( char *input_command )
{
	char * substr;
	char * str2 = "fg ";
	substr = strstr( input_command, str2 );
	if(substr != NULL)
	{
		return 1;
	}

	return 0;
}

int command_nano_status( char *input_command )
{
	char * substr;
	char * str2 = "nano ";
	substr = strstr( input_command, str2 );
	if(substr != NULL)
	{
		return 1;
	}

	return 0;
}

int command_pipe_status( char *input_command )
{
	char * substr;
	substr = strchr( input_command, '|' );
	if(substr != NULL)
	{
		return 1;
	}

	return 0;
}

int command_rif_status( char *input_command, int inout )
{
	char * substr;
	if( inout )
	{
		substr = strchr( input_command, '>' );
	}
	else
	{
		substr = strchr( input_command, '<' );
	}

	if(substr != NULL)
	{
		return 1;
	}

	return 0;
}

void childsignal_handler( int signal_number )
{
	int pid, status;

	pid = waitpid( -1, &status, WNOHANG );

	while( pid > 0 )
	{
		//printf( "The process %d changed its status to %d\n", pid, status );
		//printf( "The process %d changed its status to %d\n", get_current_pid(), status );
		remove_pid( pid );
		pid = waitpid( -1, &status, WNOHANG );
	}
}

void stopsignal_handler( int signal_number )
{
	int kill_status;
	//printf( " Signal Number %d\n Process id:%d\n Process Parent id: %d\n", signal_number, getpid(), getppid() );
	int current_pid;
	current_pid = get_current_pid();
	//printf( "%d\n", current_pid );
	if( current_pid == -1 )
	{
		return;
	}
	else
	{
		//printf( "Process id: %d is stopped.\n", current_pid );
		kill_status = killpg( current_pid, SIGTSTP );
		//printf( "%d\n", kill_status );
		if( !kill_status )
		{
			change_pid_status( current_pid, -1 );
		}
	}
}

void intsignal_handler( int signal_number )
{
	int kill_status;
	//printf( " Signal Number %d\n Process id:%d\n Process Parent id: %d\n", signal_number, getpid(), getppid() );
	int current_pid;
	current_pid = get_current_pid();
	if( current_pid == -1 )
	{
		return;
	}
	else
	{
		//printf( "Process id: %d is terminated.\n", current_pid );
		kill_status = killpg( current_pid, SIGINT );
		//printf( "%d\n", kill_status );
		if( !kill_status )
		{
			//remove_pid( current_pid );
		}

	}
}

int main()
{
	char input_command[100];
	int background = 0;

	//Signals Initialization
	signal( SIGINT, intsignal_handler );
	signal( SIGTSTP, stopsignal_handler );
	signal( SIGCHLD, childsignal_handler );

	while(1)
	{
		int pid;
		int status;
		printf( "minish> " );
		//scanf( "%s", input_command );
		fgets( input_command, 100, stdin );
		int last_i = strlen( input_command ) - 1;

		strcpy(input_command, trim_whitespaces( input_command ));

		if( last_i > 0 && input_command[last_i-1] == '&' )
		{
			input_command[last_i-1]	= '\0';
			input_command[last_i-2]	= '\0';
			background = 1;
		}
		else
		{
			background = 0;
		}

		if( strcmp( input_command, "exit" ) == 0 )
		{
			break;
		}

		//logic for redirection input file - RIF
		int redirection_i = command_rif_status( input_command, 0 ); // in
		int redirection_o = command_rif_status( input_command, 1 ); // out
		int has_pipe = command_pipe_status( input_command ); // out
		int has_fgc = command_fgc_status( input_command );
		int has_cd = command_cd_status( input_command );
		int has_jobs = command_jobs_status(input_command);
		int has_kill = command_kill_status( input_command );
		int has_nano = command_nano_status( input_command );
		//RIF ends

		if( has_fgc )
		{
			//printf( "%s\n", "fgc found" );
			fg_command_exec( input_command );
		}
		else if ( has_kill )
		{
			kill_command_exec( input_command );
		}
		else if( has_cd )
		{
			cd_command_exec( input_command );
		}
		else if( has_jobs )
		{
			jobs_exec();
		}
		else
		{
			//Child process for normal process
			pid = fork();
			if( !has_nano )
			{
				setpgrp();	
			}
			
			int job_id;
			if( pid < 0 )
			{
				printf( "Error in child process fork\n" );
				exit(1);
			}
			else if( pid == 0 )
			{
				setsid();
				//child process
				//printf( "%s\n", input_command );
				if( redirection_o )
				{
					redi_command_exec(input_command, 1);
				}
				else if( has_pipe )
				{
					pip_command_exec(input_command);
				}
				else if( redirection_i )
				{
					redi_command_exec(input_command, 0);
				} 
				else
				{
					command_exec(input_command);
				}
			}
			else
			{
				if( background )
				{

					job_id = insert_pid( pid, 2 ); // 0 -> add to job id
					printf( "[%d] %d\n", job_id, pid );

					// waitpid( pid, &status, 0 );
					// printf( " Process %d is in foreground\n", pid );
				}
				else
				{
					job_id = insert_pid( pid, 1 ); // 1 -> FG
					waitpid( pid, &status, WUNTRACED );
					int pstatus = get_pstate( pid );
					if( pstatus == 1 )
					{
						remove_pid( pid );
					}
				}
			}
		}
	}
	return 0;
}