#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include "tokenize.h"
#include "svec.h"
#include "ast.h"
#include "nush.h"
#include "hashmap.h"


// base case for tree exc
// handles special cases: exit, cd, empty
int
execute(svec* cmd)
{
	char* first = svec_get(cmd, 0);
	char* last = svec_get(cmd, cmd->size - 1);
	
	if (strcmp(first, "exit") == 0) {
		exit(1);
	}

	if (strcmp(first, "cd") == 0) {
		//$ cd ,credit for getting to home dir: ernest friedman-hill
		//https://stackoverflow.com/questions/9493234/chdir-to-home-directory
		if (cmd->size == 1) {
			return(chdir(getenv("HOME")));
		}
		//$ cd /path
		else {
			return(chdir(svec_get(cmd, 1)));
		}
	}
	
	// last token is &, run command in background
	if (strcmp(last, "&") == 0) {
		// remove & from token and exec command
		svec_remove(cmd, cmd->size - 1);
    	int cpid;
		if ((cpid = fork())) {
    		// do not wait
		}
		else {
			execvp(svec_get(cmd, 0), cmd->data);
		}
	}
	// otherwise wait on child
	else {
    	int cpid;
		if ((cpid = fork())) {
			int status;
			waitpid(cpid, &status, 0);
			//printf("status: %d\n", status);
			return status;
    	}
		else {
			execvp(svec_get(cmd,0), cmd->data);
		}
	}
}

/*
 * Recursively evaluate ast* of operators and commands.
 */
int
exec_tree(ast* at)
{
	// we have operator
	if (strcmp(at->op, "<") == 0) {
		return redir_in(at);
	}
	else if (strcmp(at->op, ">") == 0) {	
		return redir_out(at);
	}
	else if (strcmp(at->op, ";") == 0) {
		return exec_semi(at);
	}
	else if (strcmp(at->op, "&&") == 0) {
		return exec_and(at);
	}
	else if (strcmp(at->op, "||") == 0) {
		return exec_or(at);
	}
	else if (strcmp(at->op, "|") == 0) {
		return exec_pipe(at);
	}
	// base case
	if (strcmp(at->op, "=") == 0) {
		return execute(at->cmd);
	}
}

/* op "<"
*  ex. $ sort < foo.txt
*  credit (modified): Nat Tuck
*  https://github.com/NatTuck/scratch-2020-09/blob/682c8a64774068b0f8938d46b54300b2724b3a90/3650/v06/redir.c
*/
int
redir_in(ast* at)
{
    int cpid;
    if ((cpid = fork())) {
		int status;
		waitpid(cpid, &status, 0);
		return status;
	}
    else {
		int fd = open(at->right->cmd->data[0], O_RDONLY);
		close(0);
		dup(fd);
		close(fd);		
		exit(exec_tree(at->left));	
    }
}

/* op ">"
* ex. $ sort foo.txt > output.txt
* credit (modified): Nat Tuck
* https://github.com/NatTuck/scratch-2020-09/blob/682c8a64774068b0f8938d46b54300b2724b3a90/3650/v06/redir.c
*/
int
redir_out(ast* at)
{
	int cpid;
	// I dont know why last line is sometimes read in twice here
	// since the parent fd I'm not changing and child is being exit
	if ((cpid = fork())) {
		int status;
		waitpid(cpid, &status, 0);
		return status;
	}
	else {
		int fd = open(at->right->cmd->data[0], O_CREAT | O_APPEND | O_WRONLY, 0644);
		close(1);
		dup(fd); 
		close(fd);
		_exit(exec_tree(at->left));
	}
}

/*
 * Credit (modified) : Nat Tuck
 * pipe.c
 * https://github.com/NatTuck/scratch-2020-09/blob/682c8a64774068b0f8938d46b54300b2724b3a90/3650/v06/sort-pipe.c
 * the pipe for some reason will usually seg fault
 * still can't figure out after lots of debugging
 */
int
exec_pipe(ast* at)
{
	int rv;
	int pipe_fds[2];
	// pipe fills in two spots of fill descriptor table
	
	rv = pipe(pipe_fds); 
	if (rv == -1) {
		perror("fail");
		exit(1);
	}

	int p_read = pipe_fds[0];
	int p_write = pipe_fds[1];

	int cpid;
	
	if ((cpid = fork())) {	
		close(p_write);  // close write end of pipe
		close(0); 	    // close stdin
		dup(p_read);   // p_read is now stdin

		int status;
		waitpid(cpid, &status, 0);
		return status;
	}
	else {
		close(1);   // close stdout
		dup(p_write); // p__write now stdout
		_exit(exec_tree(at->right));
	}
}

// op is ";"
// execute left command then right command
int
exec_semi(ast* at)
{
	exec_tree(at->left);
	return(exec_tree(at->right));
}

// op is "&&"
// execute left and check status before executing right
int
exec_and(ast* at)
{
	if (exec_tree(at->left) == 0) {
		return exec_tree(at->right);
	}
}

//op is "||"
//execute left and check status before executing right
int
exec_or(ast* at)
{
	if (exec_tree(at->left) != 0) {
		return(exec_tree(at->right));
	}
}

int
main(int argc, char* argv[])
{
    char cmd[256];
	FILE* script_file;
	// load script file
	if (argc != 1) {
		script_file = fopen(argv[1], "r");
		if (!script_file) {
			printf("Error: Failed to open script file.\n");
			exit(1);
		}
	}

	// map $FOO to value
	// when echo is called look in hashmap
	// for value
	// when = is found in token store
	// was interfering tests
	//static hashmap* vars = make_hashmap();

	// collect input until end of file or exit
	while(1) {
		// show prompt and read input from keyboard
		if (argc == 1) {
			printf("nush$ ");
			fflush(stdout);
			if (!fgets(cmd, 256, stdin)) {
				printf("\n");
				break;
			}
		}
		// read in line from script file
		else {
			if (!fgets(cmd, 256, script_file)) {
				break;
			}
		}
		// not responsible for freeing this svec
		// since it used directly for ast
		svec* tok_cmd = tokenize(cmd);
		//print_tokens(tok_cmd);
		ast* at = parse(tok_cmd);
		//ast_print(at);
		exec_tree(at);
		free_ast(at);
	}
	if (argc != 1) {
		fclose(script_file);
	}
}
