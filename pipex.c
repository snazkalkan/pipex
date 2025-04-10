#include "pipex.h"

void	open_files(t_pipex *pipex, char *infile, char *outfile)
{
	pipex->infile = open(infile, O_RDONLY);
	if (pipex->infile == -1)
		error_exit("infile");
	pipex->outfile = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pipex->outfile == -1)
		error_exit("outfile");
}
void init_pipex(t_pipex *pipex, char **av, char **envp)
{
	pipex->argv = av;
	pipex->envp = envp;
	open_files(pipex, av[1], av[4]);
	if (pipe(pipex->pipe_fd) == -1)
		error_exit("pipe");
}
void child_process(t_pipex *pipex, char *cmd)
{
	dup2(pipex->infile, 0); // infile ->> stdin
	dup2(pipex->pipe_fd[1], 1); // pipex yazma ucu ->> stdout
	close(pipex->pipe_fd[0]);  // okuma ucu kapandı
	close(pipex->pipe_fd[1]); // yazma ucu kapandı
	close(pipex->infile);
	close(pipex->outfile);
	execute(pipex, cmd);
}
void parent_process(t_pipex *pipex, char *cmd)
{
	dup2(pipex->pipe_fd[0], 0); // pipex okuma ucu ->> stdin
	dup2(pipex->outfile, 1); // outfile ->> stdout
	close(pipex->pipe_fd[0]);       // ✅ artık stdin'e yönlendirildi, orijinali gereksiz
	close(pipex->pipe_fd[1]);       // ❌ parent sadece okuyacak, yazma ucuna ihtiyacı yok
	close(pipex->infile);           // ❌ sadece child kullanmıştı, parent için gereksiz
	close(pipex->outfile);          // ✅ stdout’a yönlendirildi, orijinal fd artık gereksiz
	execute(pipex, cmd);
}

int	main(int ac, char **av, char **envp)
{
	t_pipex	pipex;
	pid_t	pid;
	int		status;

	if (ac != 5)
		error_exit("usage: ./pipex infile cmd1 cmd2 outfile");
	init_pipex(&pipex, av, envp);
	pid = fork();
	if (pid == -1)
		error_exit("fork");
	if (pid == 0) //cocuk surec
		child_process(&pipex, av[2]);
	waitpid(pid, &status, 0);
	parent_process(&pipex, av[3]);
	return (0);
}
