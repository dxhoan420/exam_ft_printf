//
// Created by Demeter Xhoan on 1/24/21.
//

#include <stdarg.h>
#include <unistd.h>

int		g_accuracy;
int		g_width;
char	g_type;

int micro_atoi(char **string)
{
	int result = -1;
	int temp;
	char numbers[11] = "0123456789";
	int i = 0;

	while (numbers[i] && **string)
		if (numbers[i] == **string)
		{
			result = i;
			(*string)++;
			break;
		}
		else
			i++;
	if(result == -1)
		return (-1);
	temp = micro_atoi(string);
	if (temp == -1)
		return (result);
	return (result * 10 + temp);
}

int ft_strlen(char *string)
{
	return (*string ? 1 + ft_strlen(string + 1) : 0);
}

int s_write(char *string)
{
	if (string == NULL)
		string = "(null)";

	int temp = ft_strlen(string);
	int len = g_accuracy < temp && g_accuracy >= 0 ? g_accuracy : temp;
	int result = g_width > len ? g_width : len;

	while (g_width-- > len)
		write(1, " ", 1);
	write(1, string, len);
	return (result);
}

int get_num_len(long long num, unsigned int base)
{
	if (num == 0)
	{
		if (g_accuracy == 0)
			return (0);
		else
			return (1);
	}
	return (num / base == 0 ? 1 : 1 + get_num_len(num/base, base));
}

void put_num(unsigned int hex, unsigned int base, char *alphabet)
{
	char temp;

	if (g_accuracy == 0 && hex == 0)
		return;
	temp = alphabet[hex % base];
	if (hex < base)
	{
		write(1, &temp, 1);
		return;
	}
	put_num(hex / base, base, alphabet);
	write(1, &temp, 1);
}

int d_write(long long digit)
{
	char *alphabet = "0123456789";
	long long positive = digit < 0 ? -1 : 1;
	digit = digit * positive;
	unsigned int base = ft_strlen(alphabet);
	int len = get_num_len(digit, base);
	int len_with_0 = (positive == -1 ? 1 : 0) + (g_accuracy > len ? g_accuracy
			: len);
	int	result = g_width > len_with_0 ? g_width : len_with_0;

	while (g_width-- > len_with_0)
		write(1, " ", 1);
	write(1, "-", positive == -1 ? 1 : 0);
	while (g_accuracy > len++)
		write(1, "0", 1);
	put_num(digit, base, alphabet);
	return (result);
}

int x_write(unsigned int hex)
{
	char *alphabet = "0123456789abcdef";
	unsigned int base = ft_strlen(alphabet);
	int len = get_num_len(hex, base);
	int len_with_0 = g_accuracy > len ? g_accuracy : len;
	int	result = g_width > len_with_0 ? g_width : len_with_0;

	while (g_width-- > len_with_0)
		write(1, " ", 1);
	while (len_with_0-- > len)
		write(1, "0", 1);
	put_num(hex, base, alphabet);
	return (result);
}


int ft_printf(const char *origin, ... )
{
	va_list	args;
	int		result = 0;
	char	*format = (char *)origin;

	va_start(args, origin);
	while (*format)
	{
		if (*format == '%')
		{
			if (*++format == '%')
				result += write(1, format++, 1);
			else if (*format)
			{
				g_accuracy = -1;
				g_width = -1;
				g_type = -1;
				g_width = micro_atoi(&format);
				if (*format == '.')
				{
					format++;
					g_accuracy = micro_atoi(&format);
					if (g_accuracy == -1)
						g_accuracy = 0;
				}
				g_type = *format++;
				if (g_type == 's')
					result += s_write(va_arg(args, char *));
				if (g_type == 'x')
					result += x_write(va_arg(args, unsigned int));
				if (g_type == 'd')
					result += d_write(va_arg(args, int));
			}
		}
		else
			result += write(1, format++, 1);
	}
	va_end(args);
	return (result);
}
