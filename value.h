#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <unordered_map>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

namespace json
{
    struct value
    {
        virtual int type() = 0;
        virtual operator std::string() = 0;
        virtual ~value() = 0;
    };

    value::~value() {}
    static const std::string _type[6] = { "null","bool","num", "str", "arr", "obj"};

    enum {
        j_NULL,
        j_BOOl,
        j_NUM,
        j_STR,
        j_ARR,
        j_OBJ,
    };

    struct str : public value
    {
        std::string val;

        str() = default;
        str(std::string _v) :val(_v) {}
        ~str() {}

        operator std::string()
        {
            return '"' + val + '"';
        }

        int type()
        {
            return 3;
        }


    };

    template <class name>
    struct num : public value
    {
        using T = typename std::enable_if<std::is_same_v<name, int> || std::is_same_v<name, double>, name>::type;
        T val;

        num() = default;
        num(T _v) :val(_v) {}
        ~num() {};


        operator std::string()
        {
            return std::to_string(val);
        }

        operator T()
        {
            return val;
        }

        int type()
        {
            return 2;
        }
    };

    struct obj : public value
    {
        std::map<std::string,  value* > val;

        obj() = default;
        obj(const std::map<std::string, value*>& _v) :val(_v) {}
        ~obj() {
            for (auto i : val)
            {
                delete(i.second);
            }
        }

        auto operator[](const std::string& index)
        {
            return val[index];
        }

        operator std::string()
        {
            std::stringstream _t;
            _t << '{';
            for (auto i = val.begin(); i != --val.end(); i++)
            {
                _t << '"' + i->first + '"' << ':' << std::string(*i->second) << ',';
            }
            _t << '"' + val.rbegin()->first + '"' << ':' << std::string(*val.rbegin()->second) << '}';
            return _t.str();
        }

        int type()
        {
            return 5;
        }
    };

    struct arr : public value
    {
        std::vector<value*> val;

        arr() = default;
        arr(const std::vector<value*>& _v) :val(_v) {}
        ~arr() {
            for (auto i : val)
            {
                delete (i);
            }
        }

        auto operator [](size_t index)
        {
            return val[index];
        }

        operator std::string()
        {
            std::stringstream _t;
            _t << '[';
            for (auto i = val.begin(); i != --val.end(); i++)
            {
                _t << (std::string) * *i << ',';
            }
            _t << (std::string)*val.back() << ']';
            return _t.str();
        }

        int type()
        {
            return 4;
        }
    };

    struct j_bool :public value
    {
        bool val;

        j_bool() = default;
        j_bool(bool _val) :val(_val) {}
        ~j_bool() {}

        operator bool()
        {
            return val;
        }

        operator std::string()
        {
            return val ? "true" : "false";
        }

        int type()
        {
            return 1;
        }
    };

    struct null : public value
    {

        null() = default;
        ~null() {};

        operator bool() {
            return false;
        }

        operator std::string()
        {
            return "null";
        }

        int type()
        {
            return 0;
        }
    };
}
