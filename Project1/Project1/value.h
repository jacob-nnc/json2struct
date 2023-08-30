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

    union val_type;

    struct value
    {

        virtual int type() = 0;
        virtual std::string toJson() = 0;
        virtual ~value() = 0;
    };

    value::~value() {}
    static const std::string _type[6] = {"null", "bool", "num", "str", "arr", "obj"};

    enum
    {
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
        str(std::string _v) : val(_v) {}
        str(const str &item) : val(item.val) {}
        ~str() {}

        std::string toJson()
        {
            return '"' + val + '"';
        }

        int type()
        {
            return 3;
        }

        str &operator=(const str &item)
        {
            this->val = item.val;
            return *this;
        }
    };

    struct numi : public value
    {
        int val;

        numi() = default;
        numi(int _v) : val(_v) {}
        numi(const numi &item) : val(item.val) {}
        ~numi(){};

        std::string toJson()
        {
            return std::to_string(val);
        }

        int type()
        {
            return 2;
        }
    };

    struct numf : public value
    {
        double val;

        numf() = default;
        numf(double _v) : val(_v) {}
        numf(const numf &item) : val(item.val) {}
        ~numf() {}

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
    };

    struct obj : public value
    {
        std::map<std::string, value *> val;

        obj() = default;
        obj(const std::vector<std::string> &a, const std::map<std::string, value *> &_v) : _insert(a), val(_v) {}
        obj(const obj &item) : val(item.val), _insert(item.keys()) {}
        ~obj()
        {
            for (auto i : val)
            {
                delete (i.second);
            }
        }

        operator std::map<std::string, value *>()
        {
            return val;
        }

        std::string toJson()
        {
            std::stringstream _t;
            _t << '{';
            for (auto i = _insert.begin(); i != --_insert.end(); i++)
            {
                _t << '"' + *i + '"' << ':' << val[*i]->toJson() << ',';
            }
            _t << '"' + _insert.back() + '"' << ':' << val[_insert.back()]->toJson() << '}';
            return _t.str();
        }

        int type()
        {
            return 5;
        }

        const std::vector<std::string> &keys() const
        {
            return _insert;
        }

        std::vector<std::string> &keys()
        {
            return const_cast<std::vector<std::string> &>(static_cast<const obj *>(this)->keys());
        }

    private:
        std::vector<std::string> _insert;
    };

    struct arr : public value
    {
        std::vector<value *> val;

        arr() = default;
        arr(const std::vector<value *> &_v) : val(_v) {}
        arr(const arr &item) : val(item.val) {}

        ~arr()
        {
            for (auto i : val)
            {
                delete (i);
            }
        }

        auto operator[](size_t index)
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
    };

    struct j_bool : public value
    {
        bool val;

        j_bool() = default;
        j_bool(bool _val) : val(_val) {}
        j_bool(const j_bool &item) : val(item.val) {}

        ~j_bool() {}

        std::string toJson()
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
        ~null(){};

        std::string toJson()
        {
            return "null";
        }

        int type()
        {
            return 0;
        }
    };
}
