#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>

class NeispravnoZadataOpcija : public exception {
public:
	NeispravnoZadataOpcija(const char* msg) : exception(msg) {};
};

class Neuspesno : public exception {
public:
	Neuspesno(const char* msg) : exception(msg) {};
};

class Nepostojece : public exception {
public:
	Nepostojece(const char* msg) : exception(msg) {};
};
#endif
