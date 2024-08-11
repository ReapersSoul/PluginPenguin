#include "Script.hpp"
#include <Language/Language.hpp>


Script::Script(){
	//ExposeDefault();
}
Script::Script(Language *language, std::filesystem::path path){
	this->language = language;
	this->path = path;
	ExposeDefault();
	load();
}


void Script::expose_no_ret_no_args(std::string name, std::function<void()> func){
	language->expose_no_ret_no_args(this,name,func);
}
void Script::expose_no_ret_args(std::string name, std::function<void()> func, std::type_index args...){
	language->expose_no_ret_args(this,name,func,args);
}
void Script::expose_ret_no_args(std::string name, std::function<void()> func, std::type_index returnType){
	language->expose_ret_no_args(this,name,func,returnType);
}
void Script::expose_ret_args(std::string name, std::function<void()> func, std::type_index returnType, std::type_index args...){
	language->expose_ret_args(this,name,func,returnType,args);
}
void Script::expose_no_ret_no_args(std::string name, void(*func)()){
	expose_no_ret_no_args(name, std::function<void()>(func));
}    

void Script::call_no_ret_no_args(std::string name){
	language->call_no_ret_no_args(this,name);
}
void Script::call_no_ret_args(std::string name, Variant args...){
	language->call_no_ret_args(this,name,args);
}
void Script::call_ret_no_args(std::string name, Variant& Ret){
	language->call_ret_no_args(this,name,Ret);
}
void Script::call_ret_args(std::string name, Variant& Ret, Variant args...){
	language->call_ret_args(this,name,Ret,args);
}

bool Script::has(std::string name){
	return language->has(this,name);
}

void Script::load(){
	language->load(this);
}

void Script::unload(){
	language->unload(this);
}

void Script::reload(){
	language->reload(this);
}

void Script::reset(){
	language->reset();
}

void Script::set(std::string name, Variant value){
	language->set(name,value);
}

Variant Script::get(std::string name){
	return language->get(name);
}

void* Script::getState(){
	return state;
}

void Script::setState(void* state){
	this->state = state;
}

std::filesystem::path Script::getPath(){
	return path;
}

Language *Script::getLanguage(){
	return language;
}

void Script::ExposeDefault(){
	expose_no_ret_no_args("load", [this](){
		load();
	});
	expose_no_ret_no_args("unload", [this](){
		unload();
	});
	expose_no_ret_no_args("reload", [this](){
		reload();
	});
	expose_no_ret_no_args("reset", [this](){
		reset();
	});
}