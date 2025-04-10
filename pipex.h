#ifndef PIPEX_H
#define PIPEX_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

typedef struct s_pipex
{
	char    **argv;     // Komut argümanları
	char    **envp;     // Ortam değişkenleri
	int     infile;     // Giriş dosyası
	int     outfile;    // Çıkış dosyası
	int     pipe_fd[2]; // Pipe dosya tanımlayıcıları
} t_pipex;

void    error_exit(char *msg);
void    open_files(t_pipex *pipex, char *infile, char *outfile);
void    execute(t_pipex *pipex, char *cmd);
char    *get_path(char **envp);
char    **get_path_array(t_pipex *pipex);
void    ft_free(char **arr);
char    *ft_strjoin(char *s1, char *s2);
int     ft_strncmp(const char *s1, const char *s2, size_t n);
char	**ft_split(char *s, char c);


#endif
