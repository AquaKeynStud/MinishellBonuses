/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 15:04:52 by arocca            #+#    #+#             */
/*   Updated: 2025/07/11 00:37:41 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include <errno.h>
#include "parsing.h"
#include "minishell.h"
#include "sigaction.h"

char	*get_from_env(t_env *env, const char *key)
{
	while (env)
	{
		if (!ft_strcmp(env->key, key))
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

static int	count_argv(t_ast *node)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (i < node->sub_count)
	{
		if (node->childs[i]->type == AST_REDIR)
			i += 2;
		else
		{
			count++;
			i++;
		}
	}
	return (count);
}

char	**ast_to_argv(t_ctx *ctx, t_ast *node)
{
	int		i;
	int		argc;
	char	**argv;
	int		arg_idx;
	t_ast	**childs;

	if (!node)
		return (NULL);
	i = 0;
	arg_idx = 1;
	childs = node->childs;
	argc = count_argv(node);
	argv = s_malloc(ctx, (argc + 2) * sizeof(char *));
	argv[0] = node->value;
	argv[argc + 1] = NULL;
	while (i < node->sub_count)
	{
		if (childs[i]->type == AST_REDIR)
			i += 2;
		else
			argv[arg_idx++] = childs[i++]->value;
	}
	return (argv);
}

char	*get_path(t_ctx *ctx, char *cmd, t_env *env)
{
	int		i;
	char	**paths;
	char	*raw_path;
	char	*full_cmd;

	i = 0;
	full_cmd = NULL;
	if (!cmd)
		return (NULL);
	raw_path = get_from_env(env, "PATH");
	if (!raw_path || !*raw_path || ft_strchr(cmd, '/'))
		return (s_save(ctx, ft_strdup(cmd)));
	paths = ft_split(raw_path, ':');
	if (!paths)
		return (NULL);
	while (paths[i])
	{
		full_cmd = join_with_delim(ctx, paths[i++], cmd, "/");
		if (!full_cmd || !access(full_cmd, X_OK))
			break ;
		s_free(ctx, full_cmd);
		full_cmd = NULL;
	}
	double_free(ctx, (void **)paths, 0);
	return (full_cmd);
}
