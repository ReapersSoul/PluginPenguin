#pragma once
#include <functional>
#include <string>
#include <typeindex>
#include <stdarg.h>
#include <variant>
class Script;

using Variant = std::variant<int, float, double, std::string, bool, void*>;

class Language
{
public:
	virtual void Initialize() = 0;
	virtual void Finalize() = 0;
	virtual void Reset() = 0;
    virtual void expose_no_ret_no_args(Script* script,std::string name, std::function<void()> func)=0;
	virtual void expose_no_ret_args(Script* script,std::string name, std::function<void()> func, std::type_index args...)=0;
	virtual void expose_ret_no_args(Script* script,std::string name, std::function<void()> func, std::type_index returnType)=0;
    virtual void expose_ret_args(Script* script,std::string name, std::function<void()> func, std::type_index returnType, std::type_index args...)=0;
    void expose_no_ret_no_args(Script* script,std::string name, void(*func)()){
        expose_no_ret_no_args(script,name,std::function<void()>(func));
    }    
    template <typename... Args>
    void expose_no_ret_args(Script* script, std::string name, std::function<void(Args...)> func){
        expose_no_ret_args(script, name, (std::function<void()>)func, typeid(Args)...);    
    }
    template <typename... Args>
    void expose_no_ret_args(Script* script, std::string name, void(*func)(Args...)){
        expose_no_ret_args(script, name, std::function<void(Args...)>(func));
    }
	template <typename ReturnType>
	void expose_ret_no_args(Script* script, std::string name, std::function<ReturnType()> func){
        expose_ret_no_args(script, name, func, typeid(ReturnType));
	}
	template <typename ReturnType>
	void expose_ret_no_args(Script* script, std::string name, ReturnType(*func)()){
        expose_ret_no_args(script, name, std::function<ReturnType()>(func), typeid(ReturnType));
	}
    template <typename ReturnType, typename... Args>
    void expose_ret_args(Script* script, std::string name, std::function<ReturnType(Args...)> func){
        expose_ret_args(script, name, func, typeid(ReturnType), typeid(Args)...);
    }
    template <typename ReturnType, typename... Args>
    void expose_ret_args(Script* script, std::string name, ReturnType(*func)(Args...)){
        expose_ret_args(script, name, std::function<ReturnType(Args...)>(func));
    }

	virtual void call_no_ret_no_args(Script* script,std::string name)=0;
    virtual void call_no_ret_args(Script* script,std::string name, Variant args...)=0;
    virtual void call_ret_no_args(Script* script,std::string name, Variant& Ret)=0;
    virtual void call_ret_args(Script* script,std::string name, Variant& Ret, Variant args...)=0;
    template <typename... Args>
    void call_no_ret_args(Script* script,std::string name, Args... args){
        call_no_ret_args(script,name,Variant(args)...);
    }
    template <typename ReturnType>
    ReturnType call_ret_no_args(Script* script,std::string name){
        Variant ret;
        call_ret_no_args(script,name,ret);
        return std::get<ReturnType>(ret);
    }
    template <typename ReturnType, typename... Args>
    ReturnType call_ret_args(Script* script,std::string name, Args... args){
        Variant ret;
        call_ret_args(script,name,ret,Variant(args)...);
        return std::get<ReturnType>(ret);
    }

    virtual bool has(Script* script,std::string name)=0;
    virtual void load(Script* script)=0;
    virtual void unload(Script* script)=0;
    virtual void reload(Script* script)=0;
    virtual void reset()=0;
    virtual void set(std::string name, Variant value)=0;
    virtual Variant get(std::string name)=0;
    virtual std::vector<std::string> getExtensions()=0;
    virtual std::string getName()=0;
};