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
//template<int flag>
//struct __m_if__ {};
//
//template <>
//struct __m_if__ <json::j_OBJ>
//{
//    using type = json::obj;
//    using val_type = map<string, json::value*>;
//};
//
//template <>
//struct __m_if__<json::j_ARR>
//{
//    using type = json::arr;
//    using val_type = vector<json::value*>;
//};
//
//template <>
//struct __m_if__<json::j_NULL>
//{
//    using type = json::null;
//    using val_type = struct {};
//};
//
//template <>
//struct __m_if__<json::j_STR>
//{
//    using type = json::str;
//    using val_type = string;
//};
//
//template <>
//struct __m_if__<json::j_NUM>
//{
//    template <class name>
//    using TT = typename std::enable_if<std::is_same_v<name, int> || std::is_same_v<name, double>, name>::type;
//    template <class name>
//    using type = json::num<TT<name>>;
//    template <class T>
//    using val_type = TT<T>;
//};
//
//template <>
//struct __m_if__<json::j_BOOL>
//{
//    using type = json::j_bool;
//    using val_type = bool;
//};
namespace json
{

    union val_type;

    struct value
    {
        virtual int type() = 0;
        virtual std::string toJson() = 0;
        virtual ~value() = 0;
        virtual val_type getVal() = 0;
    };
    
    union val_type
    {
        std::string* str_v;
        std::map<std::string, value*>* obj_v;
        std::vector< value*>* arr_v;
        int* numi_v;
        double* numf_v;
        bool* bool_v;
        struct {}*null_v;
    };

    value::~value() {}
    static const std::string _type[6] = { "null","bool","num", "str", "arr", "obj"};

    enum {
        j_NULL,
        j_BOOL,
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
            return val;
        }
        std::string toJson()
        {
            return '"' + val + '"';
        }

        int type()
        {
            return 3;
        }

        val_type getVal()
        {
            val_type a;
            a.str_v = &val;
            return a;
        }
    };

    struct numi : public value
    {
        int val;

        numi() = default;
        numi(int _v) :val(_v) {}
        ~numi() {};


        std::string toJson()
        {
            return std::to_string(val);
        }

        operator int()
        {
            return val;
        }

        int type()
        {
            return 2;
        }

        val_type getVal()
        {
            val_type a;
            a.numi_v = &val;
            return a;
        }
    };

    struct numf : public value
    {
        double val;

        numf() = default;
        numf(double _v) :val(_v) {}
        ~numf() {};


        std::string toJson()
        {
            return std::to_string(val);
        }

        operator double()
        {
            return val;
        }

        int type()
        {
            return 2;
        }

        val_type getVal()
        {
            val_type a;
            a.numf_v = &val;
            return a;
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

        operator std::map<std::string, value*>()
        {
            return val;
        }

        auto operator[](const std::string& index)
        {
            return val[index];
        }

        std::string toJson()
        {
            std::stringstream _t;
            _t << '{';
            for (auto i = val.begin(); i != --val.end(); i++)
            {
                _t << '"' + i->first + '"' << ':' << (*i->second).toJson() << ',';
            }
            _t << '"' + val.rbegin()->first + '"' << ':' << (*val.rbegin()->second).toJson() << '}';
            return _t.str();
        }

        int type()
        {
            return 5;
        }

        val_type getVal()
        {
            val_type a;
            a.obj_v = &val;
            return a;
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

        operator std::vector<value*>()
        {
            return val;
        }

        auto operator [](size_t index)
        {
            return val[index];
        }

        std::string toJson()
        {
            std::stringstream _t;
            _t << '[';
            for (auto i = val.begin(); i != --val.end(); i++)
            {
                _t << (*i)->toJson() << ',';
            }
            _t << val.back()->toJson() << ']';
            return _t.str();
        }

        int type()
        {
            return 4;
        }

        val_type getVal()
        {
            val_type a;
            a.arr_v = &val;
            return a;
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

        std::string toJson()
        {
            return val ? "true" : "false";
        }

        int type()
        {
            return 1;
        }

        val_type getVal()
        {
            val_type a;
            a.bool_v = &val;
            return a;
        }
    };

    struct null : public value
    {

        null() = default;
        ~null() {};

        std::string toJson()
        {
            return "null";
        }

        int type()
        {
            return 0;
        }

        val_type getVal()
        {
            val_type a;
            a.null_v = nullptr;
            return a;
        }
    };
}
