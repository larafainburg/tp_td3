#include "EditorVersionado.h"

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <cassert>

using namespace std;

EditorVersionado::EditorVersionado() {
	_version_actual = 0;
	_versiones = {{0,true,"",""}};
	_palabras={};
}

int EditorVersionado::longitud() const {
	return _palabras.size();
}

EditorVersionado EditorVersionado::crear_con_texto(const string &texto) {
	EditorVersionado res;

  stringstream ts(texto);
	string palabra;
  while (ts >> palabra) res.insertar_palabra(palabra, res.longitud());
  
  return res;
}

const string& EditorVersionado::palabra_en(int pos) const {
	return _palabras[pos];
}

void EditorVersionado::olvidar_version(const string & nombre) {
	//... completar
}

void EditorVersionado::insertar_palabra(const string& palabra, int pos) {
	_palabras.push_back("");
	for (int i = _palabras.size() - 1; i > pos; i--) {
		_palabras[i] = _palabras[i-1];
	}
	_palabras[pos] = palabra;

	for (int j = _versiones.size() - 1; j > _version_actual; j--) {
		_versiones.pop_back();
	}

	version nueva_version = {pos, true, palabra, ""};
	_versiones.push_back(nueva_version);

	_version_actual = _versiones.size() - 1;
}

void EditorVersionado::borrar_palabra(int pos) {
	string palabra_borrada = _palabras[pos]; 
	for (int i = pos; i < _palabras.size() - 1; i++) {
		_palabras[i] = _palabras[i+1]; 
	}
	_palabras.pop_back();

	for (int j = _versiones.size() - 1; j > _version_actual; j--) {
		_versiones.pop_back();
	}

	version nueva_version = {pos, false, palabra_borrada, ""};
	_versiones.push_back(nueva_version);
	_version_actual = _versiones.size() - 1;
}

int EditorVersionado::cantidad_versiones() const {
	return _versiones.size();
}

int EditorVersionado::version_actual() const {
	//... completar: ESTE RETURN ESTÁ MAL!!!!
	return -1;
}

const set<string> & EditorVersionado::versiones_con_nombre() const {
	return _nombres; //devolvemos el set que creamos en el private 
}

const string & EditorVersionado::nombre_version_actual() const {
	return _versiones[_version_actual].nombre;
}

int EditorVersionado::deshacer() {
	version v = _versiones[_version_actual];

	if (v.es_insercion) {
		_palabras.erase(_palabras.begin() + v.pos);
	} else {
		_palabras.insert(_palabras.begin() + v.pos, v.palabra);
	}

	_version_actual--;
	return v.pos;
}

int EditorVersionado::rehacer() {
	version v = _versiones[_version_actual + 1];

	if (v.es_insercion) {
		_palabras.insert(_palabras.begin() + v.pos, v.palabra);
	} else {
		_palabras.erase(_palabras.begin() + v.pos);
	}

	_version_actual++;
	return v.pos;
}

int EditorVersionado::ir_a_version(int ver) { //REVISAR!!!
	if (!_versiones[ver].nombre.empty()) {
		_palabras = _texto_de_version[ver]; 

		int pos_resultado;
		if (ver < _version_actual) {
			pos_resultado = _versiones[ver + 1].pos;
		} else {
			pos_resultado = _versiones[ver].pos;
		}

		_version_actual = ver;
		return pos_resultado;
	} else {
		int pos_resultado = _versiones[ver].pos; 
		while (_version_actual > ver) {
			pos_resultado = deshacer();
		}
		while (_version_actual < ver) {
			pos_resultado = rehacer();
		}
		return pos_resultado;
	}
}

int EditorVersionado::ir_a_version_con_nombre(const string & nombre) {
	//... completar: ESTE RETURN ESTÁ MAL!!!!
	return -1;
}

void EditorVersionado::nombrar_version_actual(const string& nombre) {
	string nombre_anterior = _versiones[_version_actual].nombre;

	if (nombre_anterior.empty()) {
		_texto_de_version[_version_actual] = _palabras; 
	} else {
		_nombres.erase(nombre_anterior);        
		_version_de_nombre.erase(nombre_anterior); 
	}

	_versiones[_version_actual].nombre = nombre;
	_nombres.insert(nombre);                        
	_version_de_nombre[nombre] = _version_actual;    
}

int EditorVersionado::cargar_archivo(const string &archivo,
                                     const string &archivo_versiones) {
	//... completar: ESTE RETURN ESTÁ MAL!!!!
	return -1;
}

void EditorVersionado::guardar_archivo(const string& archivo, const string& archivo_versiones) const
{
	//... completar
}
