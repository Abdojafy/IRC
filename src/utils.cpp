#include"server/server.hpp"

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

void remove_new_line(std::string &msg){
	int pos = -1;
	pos = msg.find('\r');
	if (pos != -1)
		msg.erase(pos);
	pos = msg.find('\n');
	if (pos != -1)
		msg.erase(pos);

}

void	ft_upper(std::string &msg){
	int i = 0;
	while(msg[i]){
		if (std::islower(msg[i]))
			msg[i] -= 32;
		i++;
	}
}

std::vector<std::string> split(std::string str, char c){
	std::stringstream			ss;
	std::vector<std::string>	vec;
	std::string					tmp;
	
	ss << str;
	while(std::getline(ss, tmp, c))
	{
		if (tmp[0] != 0)
			vec.push_back(tmp);
	}
	return (vec);
}

std::string trim_spaces(std::string str){
	while(1){
		if (std::isspace(str.front()))
			str.erase(0, 1);
		if (std::isspace(str.back()))
			str.erase(str.length() - 1, str.length());
		else if(!std::isspace(str.front()) && !std::isspace(str.back()))
			break;
	}
	return (str);
}