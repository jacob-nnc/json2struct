#include"json.h"

int main()
{
	json2struct a("1.json");
	auto t = a.from();
	auto s = t->toJson();
	ofstream("2.json") << s;
}