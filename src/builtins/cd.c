/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabidi <yabidi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 18:25:56 by yabidi            #+#    #+#             */
/*   Updated: 2023/02/20 22:53:21 by yabidi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/header.h"

void	new_node(t_env *env, char *value, char *property)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		exit(1);
	new->value = ft_strdup(value);
	new->property = ft_strdup(property);
	new->next = NULL;
	while (env && env->next)
		env = env->next;
	env->next = new;
}

void	set_env_var(char *property, char *value, t_env *env, int i)
{
	t_env	*temp;
	char	*temp2;

	temp = env;
	while (temp && ft_strncmp(property, temp->property, ft_strlen(property)))
		temp = temp->next;
	if (temp && !ft_strncmp(property, temp->property, ft_strlen(property)))
	{
		temp2 = temp->value;
		if (value && i == 0)
			temp->value = ft_strdup(value);
		else if (value && i == 1)
			temp->value = ft_strjoin(temp->value, value);
		free(temp2);
	}
	else
	{
		new_node(env, value, property);
	}
}

char	*get_env_var(char *property, t_env *env)
{
	while (env && ft_strncmp(property, env->property, ft_strlen(property)))
		env = env->next;
	if (!env)
		return (NULL);
	else
		return (env->value);
}

int	normal_cd(char **args, t_env *env)
{
	char	cwd[1024];

	getcwd(cwd, 1024);
	if (!chdir(*args))
	{
		set_env_var("OLDPWD", cwd, env, 0);
		getcwd(cwd, 1024);
		set_env_var("PWD", cwd, env, 0);
		return (0);
	}
	else
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(*args, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (1);
	}
}

int	just_cd(t_env *env)
{
	char	cwd[1024];

	getcwd(cwd, 1024);
	if (chdir(get_env_var("HOME", env)))
		return (1);
	set_env_var("OLDPWD", cwd, env, 0);
	getcwd(cwd, 1024);
	set_env_var("PWD", cwd, env, 0);
	return (0);
}

int	previous_one(t_env *env)
{
	char	cwd[1024];

	getcwd(cwd, 1024);
	if (get_env_var("OLDPWD", env))
	{
		if (chdir(get_env_var("OLDPWD", env)))
			return (1);
		set_env_var("OLDPWD", cwd, env, 0);
		getcwd(cwd, 1024);
		set_env_var("PWD", cwd, env, 0);
		ft_putstr_fd(cwd, 1);
		write(1, "\n", 1);
		return (0);
	}
	else
	{
		ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
		return (1);
	}
}

int	ft_cd(char **args, t_env *env)
{
	if (!env)
		return (1);
	if (!(*args))
		return (just_cd(env));
	else if (*(args) && (**args) == '-')
		return (previous_one(env));
	else
		return (normal_cd(args, env));
	return (0);
}
