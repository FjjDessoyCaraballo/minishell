/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 11:39:22 by fdessoy-          #+#    #+#             */
/*   Updated: 2023/11/07 11:57:49 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strncat(char *dest, const char *src, unsigned int nb)
{
	unsigned int	i;
	unsigned int	j;

	i = 0;
	j = 0;
	while (dest[i] != '\0')
	{
		i++;
	}
	while (src[j] != '\0' && j < nb)
	{
		dest[i] = src[j];
		i++;
		j++;
	}
	dest[i] = '\0';
	return (dest);
}

size_t	ft_strlcat(char *dst, const char *src, size_t n)
{
	size_t	dst_len;
	size_t	src_len;
	size_t	total_len;
	size_t	chars_copy;

	src_len = 0;
	dst_len = 0;
	if (src != NULL && dst != NULL)
	{
		src_len = ft_strlen(src);
		dst_len = ft_strlen(dst);
	}
	total_len = dst_len + src_len;
	chars_copy = 0;
	if (n > dst_len)
		chars_copy = n - dst_len - 1;
	else
		chars_copy = 0;
	if (chars_copy > 0)
		ft_strncat(dst, src, chars_copy);
	if (n >= dst_len)
		total_len = dst_len + src_len;
	else
		total_len = n + src_len;
	return (total_len);
}
