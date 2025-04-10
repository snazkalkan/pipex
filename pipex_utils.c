#include "pipex.h"

void	error_exit(char *msg)
{
	perror(msg);
	exit(1);
}

char	*ft_strjoin(char *s1, char *s2)
{
	char	*str;
	int		i;
	int		j;

	str = malloc(strlen(s1) + strlen(s2) + 1);
	if (!str)
		return (NULL);
	i = -1;
	while (s1[++i])
		str[i] = s1[i];
	j = 0;
	while (s2[j])
		str[i++] = s2[j++];
	str[i] = '\0';
	return (str);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && s1[i] && s2[i])
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	if (i < n)
		return ((unsigned char)s1[i] - (unsigned char)s2[i]);
	return (0);
}

void	ft_free(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

char	*get_path(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}

char	**get_path_array(t_pipex *pipex)
{
	char	*path;

	path = get_path(pipex->envp);
	if (!path)
		error_exit("Path not found\n");
	return (ft_split(path, ':'));
}

void	execute(t_pipex *pipex, char *cmd)
{
	char	**cmd_args;
	char	**paths;
	char	*cmd_path;
	char	*tmp;
	int		i;

	if (!cmd || cmd[0] == '\0')
	{
		perror("Command not found");
		exit(1);
	}
	cmd_args = ft_split(cmd, ' ');
	if(!*cmd_args)
	{
		ft_free(cmd_args);
		error_exit("Command not found");
	}
	paths = get_path_array(pipex);
	i = -1;
	while (paths[++i])
	{
		cmd_path = ft_strjoin(paths[i], "/");
		tmp = cmd_path;
		cmd_path = ft_strjoin(tmp, cmd_args[0]);
		free(tmp);
		if (access(cmd_path, F_OK | X_OK) == 0)
			execve(cmd_path, cmd_args, pipex->envp);
		free(cmd_path);
	}
	ft_free(cmd_args);
	ft_free(paths);
	error_exit("Command not found");
}


char	*ft_strndup(const char *s, size_t n)
{
    char *dup;
    size_t i;

    dup = malloc(n + 1);
    if (!dup)
        return NULL;
    i = 0;
    while (i < n)
    {
        dup[i] = s[i];
        i++;
    }
    dup[i] = '\0';
    return dup;
}

char	**ft_split(char *s, char c)
{
    char	**result;
    int		i;
    int		j;
    int		k;
    int		words;

    i = 0;
    j = 0;
    k = 0;
    words = 0;
    while (s[i])
    {
        if (s[i] != c && (s[i + 1] == c || s[i + 1] == '\0'))
            words++;
        i++;
    }
    result = malloc((words + 1) * sizeof(char *));
    if (!result)
        return NULL;
    i = 0;
    while (s[i] && k < words)
    {
        while (s[i] == c)
            i++;
        j = i;
        while (s[i] && s[i] != c)
            i++;
        result[k] = ft_strndup(&s[j], i - j);
        k++;
    }
    result[k] = NULL;
    return (result);
}
