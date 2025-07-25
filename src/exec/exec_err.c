/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_err.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 09:56:52 by arocca            #+#    #+#             */
/*   Updated: 2025/07/13 10:33:38 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>
#include "minishell.h"

static void	enoent_err(t_ctx *ctx, const char *value)
{
	ctx->status = 127;
	if (value && ft_strchr(value, '/'))
	{
		ft_dprintf(2, "minishell: ");
		perror(value);
	}
	else if (value)
	{
		ft_dprintf(2, "minishell: ");
		ft_dprintf(2, "%s: command not found\n", value);
	}
	else
		ft_dprintf(2, "minishell: command not found\n");
}

static void	eacces_err(t_ctx *ctx, const char *value)
{
	struct stat	sb;

	ctx->status = 126;
	if (value && (!stat(value, &sb) && S_ISDIR(sb.st_mode)))
	{
		ft_dprintf(2, "minishell: ");
		ft_dprintf(2, "%s: Is a directory\n", value);
	}
	else if (value && *value)
	{
		ft_dprintf(2, "minishell: ");
		perror(value);
	}
	else
		perror("minishell: ");
}

int	execve_err(t_ctx *ctx, char **value)
{
	int	saved_errno;

	ctx->status = 1;
	saved_errno = errno;
	if (*value && !**value)
		saved_errno = ENOENT;
	if (*value && !ft_strcmp(*value, "."))
	{
		ft_dprintf(2, "minishell: ");
		ft_dprintf(2, ".: filename argument required\n");
		return (exit_with_code(ctx, 2));
	}
	if (saved_errno == ENOENT || (*value && !ft_strcmp(*value, "..")))
		enoent_err(ctx, *value);
	else if (saved_errno == EACCES)
		eacces_err(ctx, *value);
	else
	{
		if (value && *value)
			perror(*value);
		else
			perror("minishell");
	}
	return (ctx->status);
}

int	redir_err(t_ctx *ctx, t_ast *ast, int exit_code)
{
	ft_dprintf(2, "minishell: ");
	perror(ast->childs[0]->value);
	ctx->status = 1;
	if (exit_code < 0)
		return (exit_code);
	return (1);
}

int	perror_code(const char *error, int exit_code)
{
	perror(error);
	return (exit_code);
}
