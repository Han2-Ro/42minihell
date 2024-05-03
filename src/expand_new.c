/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_new.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrother <hrother@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 15:49:00 by hrother           #+#    #+#             */
/*   Updated: 2024/05/03 15:23:33 by hrother          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*expand_status(char *str, int i, const int status, int *expand_len)
{
	char	*status_str;
	char	*new_str;

	status_str = ft_itoa(status);
	if (!status_str)
		return (NULL);
	*expand_len = ft_strlen(status_str);
	new_str = str_insert(status_str, str, i, i + 2);
	free(status_str);
	return (new_str);
}

char	*expand_var_new(char *str, int i, const t_list *env_list,
		int *expand_len)
{
	char	*new_str;
	char	*env_key;
	char	*env_val;
	int		j;

	j = i + 1;
	while (ft_strchr(SPECIAL_CHARS WHITESPACE, str[j]) == NULL)
		j++;
	env_key = ft_substr(str, i + 1, j - i - 1);
	env_val = ft_getenv(env_list, env_key);
	free(env_key);
	if (!env_val)
		env_val = "";
	*expand_len = ft_strlen(env_val);
	new_str = str_insert(env_val, str, i, j);
	return (new_str);
}

int	handle_dollar(char **str, int i, const t_evars evars)
{
	char	*new_value;
	int		expand_len;

	log_msg(DEBUG, "handle_dollar: '%s' at %i", *str, i);
	expand_len = 1;
	if (ft_strchr(WHITESPACE, (*str)[i + 1]) != NULL)
		return (1);
	else if ((*str)[i + 1] == '?')
		new_value = expand_status(*str, i, evars.status, &expand_len);
	else
		new_value = expand_var_new(*str, i, evars.envp, &expand_len);
	free(*str);
	*str = new_value;
	if (!new_value)
		return (FAILURE);
	return (expand_len);
}

int	split_token(t_list *list, int from, int to)
{
	t_token	*token;
	int		i;
	char	*str;
	t_list	*next;

	token = (t_token *)list->content;
	if (token->type != CMD && token->type != ARG)
		return (SUCCESS);
	next = list->next;
	i = from;
	from = 0;
	str = token->value;
	while (i < to)
	{
		if (ft_strchr(WHITESPACE, str[i]) != NULL)
		{
			token->value = ft_substr(str, from, i - from);
			token = malloc(sizeof(t_token));
			if (!token)
				return (FAILURE);
			token->type = ARG;
			list->next = ft_lstnew(token);
			list = list->next;
			while (ft_strchr(WHITESPACE, str[i]) != NULL)
				i++;
			from = i;
		}
		i++;
	}
	token->value = ft_substr(str, from, ft_strlen(str) - from);
	list->next = next;
	free(str);
	return (SUCCESS);
}

int	expand_token(t_list *list, const t_evars evars)
{
	t_token			*token;
	int				quote;
	unsigned int	i;
	int				expand_len;

	i = 0;
	quote = 0;
	token = (t_token *)list->content;
	while (token->value[i] != '\0')
	{
		if (ft_strchr("\"\'", token->value[i]) != NULL)
			handle_quote(&i, &token->value, &quote);
		else if (token->value[i] == '$' && quote != 1
			&& token->type != R_HEREDOC && token->type != R_QUOTEDOC)
		{
			expand_len = handle_dollar(&token->value, i, evars);
			if (quote == 0 && expand_len > 1)
				split_token(list, i, i + expand_len);
			i += expand_len;
		}
		else
			i++;
		if (token->value == NULL)
			return (FAILURE);
	}
	if (quote != 0)
		return (log_msg(ERROR, "Syntax Error: Quote not closed"), FAILURE);
	return (SUCCESS);
}

int	expand_heredoc(char **str, t_evars evars)
{
	unsigned int	i;
	int				expand_len;

	i = 0;
	while ((*str)[i] != '\0')
	{
		if ((*str)[i] == '$')
		{
			expand_len = handle_dollar(str, i, evars);
			i += expand_len;
		}
		else
			i++;
	}
	return (SUCCESS);
}

int	expand_tokens_new(t_list *token_lst, const t_evars evars)
{
	t_list *current;
	t_token *token;
	int ret;

	ret = SUCCESS;
	current = token_lst;
	while (current)
	{
		token = (t_token *)current->content;
		if (token->type != PIPE)
		{
			ret |= expand_token(current, evars);
		}
		current = current->next;
	}
	return (ret);
}