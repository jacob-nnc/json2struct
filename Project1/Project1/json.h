#include "value.h"
using namespace std;

#define error(message)                              \
	{                                               \
		cout << string("error ") + message << endl; \
		exit(1);                                    \
	}

struct json2struct
{
	using value = json::value;

	string json_str;

	json2struct(string filename)
	{
		json_str = chuli(getCode(filename));
	}

	json::obj *from()
	{
		return make_obj(json_str.substr(1, json_str.size() - 2));
	}

private:
	value *make_val(string _t)
	{
		value *_val = nullptr;

		if (_t == "null")
		{
			_val = new json::null();
		}
		else if (_t == "false")
		{
			_val = new json::j_bool(0);
		}
		else if (_t == "true")
		{
			_val = new json::j_bool(1);
		}
		else if (_t[0] == '"')
		{
			_val = new json::str(_t.substr(1, _t.size() - 2));
		}
		else if (_t[0] == '{')
		{
			_val = make_obj(_t.substr(1, _t.size() - 2));
		}
		else if (_t[0] == '[')
		{
			_val = make_arr(_t.substr(1, _t.size() - 2));
		}
		else if (isdigit(_t[0]))
		{
			if (_t.find('.') != -1)
			{
				_val = new json::numf(stod(_t));
			}
			else
			{
				_val = new json::numi(stoi(_t));
			}
		}
		else
		{
			error("Strange value");
		}

		return _val;
	}
	string chuli(string _val)
	{
		auto noeff_char = string(" \n\t");
		string ret;
		bool flag = 0;
		for (auto i : _val)
		{
			if (i == '"')
				flag = !flag;
			if (flag || noeff_char.find(i) == -1)
			{
				ret += i;
			}
		}
		return ret;
	}
	string getCode(string filename)
	{
		return (stringstream() << fstream(filename).rdbuf()).str();
	}
	json::obj *make_obj(string json_str)
	{
		map<string, value *> temp_data;
		vector<string> _temp_data;
		const string spa_char = "{}[]\":,";
		string key;
		string _str;
		int i = 0;
		int _t = 0;

		enum
		{
			KEY = 0,
			VAL,
		};

		int flag = KEY;

		json::value *_val = nullptr;
		for (; i != json_str.size();)
		{
			if (json_str[i] == ':')
			{
				flag = VAL;
				i++;
			}
			else if (json_str[i] == ',')
			{
				temp_data[key] = _val;
				_temp_data.push_back(key);
				flag = KEY;
				key = "";
				_val = nullptr;
				i++;
			}

			if (flag == KEY)
			{
				int j = i;
				for (; json_str[i] != ':'; i++)
					;
				key = json_str.substr(j + 1, i - j - 2);
			}
			else
			{
				int j = i;
				if (json_str[j] == '{' || json_str[j] == '[')
				{
					int _t = 0;
					for (;; j++)
					{
						if (json_str[i] == json_str[j])
						{
							_t++;
						}
						else if (json_str[i] + 2 == json_str[j])
						{
							_t--;
							if (_t == 0)
								break;
						}
					}
					j++;
				}
				else
				{
					for (; json_str[j] != ',' && j != json_str.size(); j++)
						;
				}
				_val = make_val(json_str.substr(i, j - i));
				i = j;
			}
		}
		temp_data[key] = _val;
		_temp_data.push_back(key);
		return new json::obj(_temp_data, temp_data);
	}
	json::arr *make_arr(string json_str)
	{
		auto ret = new json::arr();

		string _str;
		int i = 0;
		int _t = 0;

		json::value *_val;

		for (; i != json_str.size();)
		{
			if (json_str[i] == ',')
			{
				ret->val.push_back(_val);
				_val = nullptr;
				i++;
			}
			else
			{
				int j = i;
				if (json_str[j] == '{' || json_str[j] == '[')
				{
					int _t = 0;
					for (; j < json_str.size(); j++)
					{
						if (json_str[i] == json_str[j])
						{
							_t++;
						}
						else if (json_str[i] + 2 == json_str[j])
						{
							_t--;
							if (_t == 0)
								break;
						}
					}
					j++;
				}
				else
				{
					for (; json_str[j] != ',' && j != json_str.size(); j++)
						;
				}
				_val = make_val(json_str.substr(i, j - i));
				i = j;
			}
		}
		ret->val.push_back(_val);
		return ret;
	}
};

