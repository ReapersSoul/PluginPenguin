#pragma once
#include <filesystem>
#include <typeindex>
#include <functional>
#include <Language/Language.hpp>

class Script {
    Language *language;
    std::filesystem::path path;
    void* state;
public:
    Script();
    Script(Language *language, std::filesystem::path path);
    virtual ~Script(){};


    void expose_no_ret_no_args(std::string name, std::function<void()> func);
	void expose_no_ret_args(std::string name, std::function<void()> func, std::type_index args...);
	void expose_ret_no_args(std::string name, std::function<void()> func, std::type_index returnType);
    void expose_ret_args(std::string name, std::function<void()> func, std::type_index returnType, std::type_index args...);
    void expose_no_ret_no_args(std::string name, void(*func)());
    template <typename... Args>
    void expose_no_ret_args( std::string name, std::function<void(Args...)>* func){
        expose_no_ret_args( name, (std::function<void()>)func, typeid(Args)...);    
    }
    template <typename... Args>
    void expose_no_ret_args( std::string name, void(*func)(Args...)){
        expose_no_ret_args( name, std::function<void(Args...)>(func));
    }
	template <typename ReturnType>
	void expose_ret_no_args( std::string name, std::function<ReturnType()>* func){
        expose_ret_no_args( name, func, typeid(ReturnType));
	}
	template <typename ReturnType>
	void expose_ret_no_args( std::string name, ReturnType(*func)()){
        expose_ret_no_args( name, std::function<ReturnType()>(func), typeid(ReturnType));
	}
    template <typename ReturnType, typename... Args>
    void expose_ret_args( std::string name, std::function<ReturnType(Args...)>* func){
        expose_ret_args( name, func, typeid(ReturnType), typeid(Args)...);
    }
    template <typename ReturnType, typename... Args>
    void expose_ret_args( std::string name, ReturnType(*func)(Args...)){
        expose_ret_args( name, std::function<ReturnType(Args...)>(func));
    }

	void call_no_ret_no_args(std::string name);
    void call_no_ret_args(std::string name, Variant args...);
    void call_ret_no_args(std::string name, Variant& Ret);
    void call_ret_args(std::string name, Variant& Ret, Variant args...);
    template <typename... Args>
    void call_no_ret_args(std::string name, Args... args){
        call_no_ret_args(name,Variant(args)...);
    }
    template <typename ReturnType>
    ReturnType call_ret_no_args(std::string name){
        Variant ret;
        call_ret_no_args(name,ret);
        return std::get<ReturnType>(ret);
    }
    template <typename ReturnType, typename... Args>
    ReturnType call_ret_args(std::string name, Args... args){
        Variant ret;
        call_ret_args(name,ret,Variant(args)...);
        return std::get<ReturnType>(ret);
    }

    bool has(std::string name);

    void load();

    void unload();

    void reload();

    void reset();

    void set(std::string name, Variant value);

    Variant get(std::string name);

    void* getState();

    void setState(void* state);

    std::filesystem::path getPath();

    Language *getLanguage();

    void ExposeDefault();
};
