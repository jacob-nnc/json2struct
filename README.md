# json2struct
c++解析**json**  
~~~c++
    json2struct a("1.json");
    auto json=a.from();
    //json 就是解析得到的 json 结构体指针
 
    cout<<json->toJson()<<endl;
    //输出,且输出是符合 json 格式的
    fstream("2.json")<<json->toJson();
    //将输出写入另一个 json 文件

    auto t=json["name"];// 返回指针
    cout<< (json::str*)(t)->val<<endl;
    //获取 json 的 某一个key 对应的 value

    if(t.type==json::OBJ)
    {
        //判断 t 的类型 是否是 object
        // 类型定义为
        // enum {
        // j_NULL,
        // j_BOOl,
        // j_NUM,
        // j_STR,
        // j_ARR,
        // j_OBJ,
        // }
    }

    cout << json::_type[t.type()];
    //输出类型

    
~~~

