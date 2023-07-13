#include"server.hpp"

std::string to_upper(const char *str)
{
	std::string s;
	int i = 0;
	while (str[i])
	{
		s.push_back(std::toupper(str[i]));
		i++;
	}
	return (s);
}

std::vector<std::string> ft_split(char const *s, char c)
{
	std::vector<std::string> vec;
	int	i;
	int	j;
	std::string str = s;

	i = 0;
	while (s[i] && (s[i] == c))
		i++;
	while (*(s + i) != '\0')
	{
		j = i;
		while (s[j] != c && s[j] != '\0')
			j++;
		if (s[i] != c)
		{
			vec.push_back(str.substr(i, j - i));
		}
		if (s[j] == '\0')
			i = j;
		else
			i = j + 1;
	}
	return (vec);
}