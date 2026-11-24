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

void EditorVersionado::olvidar_version(const string &nombre) {
	map<string, id_version>::iterator it = _version_de_nombre.find(nombre);
	if (it == _version_de_nombre.end()) return;

	id_version id = it->second;

	_version_de_nombre.erase(it);
	_nombres.erase(nombre);
	_texto_de_version.erase(id);

	if (id < cantidad_versiones()) {
		_versiones[id].nombre = "";
	}
}

void EditorVersionado::insertar_palabra(const string& palabra, int pos) {
	_palabras.push_back("");
	for (int i = _palabras.size() - 1; i > pos; i--) {
		_palabras[i] = _palabras[i-1];
	}
	_palabras[pos] = palabra;

	for (int j = _versiones.size() - 1; j > _version_actual; j--) {
		if (!_versiones.back().nombre.empty()) olvidar_version(_versiones.back().nombre);
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
		if (!_versiones.back().nombre.empty()) olvidar_version(_versiones.back().nombre);
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
	return _version_actual;
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

int EditorVersionado::ir_a_version(int ver) {
	int pos_resultado;
	if (ver < _version_actual) {
		pos_resultado = _versiones[ver + 1].pos;
	} else {
		pos_resultado = _versiones[ver].pos;
	}

	if (!_versiones[ver].nombre.empty()) {
		_palabras = _texto_de_version[ver];
		_version_actual = ver;
	} else {
		while (_version_actual > ver) deshacer();
		while (_version_actual < ver) rehacer();
	}

	return pos_resultado;
}


int EditorVersionado::ir_a_version_con_nombre(const string & nombre) {
	return ir_a_version(_version_de_nombre[nombre]);
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
	*this = EditorVersionado();

	ifstream ver(archivo_versiones);
	if (!ver) {
		ifstream txt(archivo);
		string palabra;
		while (txt >> palabra) insertar_palabra(palabra, longitud());
		return _versiones[_version_actual].pos;
	}

	string linea, nombre;

	getline(ver, linea);
	stringstream cero(linea);
	if (cero >> nombre) nombrar_version_actual(nombre);

	while (getline(ver, linea)) {
		stringstream ss(linea);
		int pos, es_insercion;
		string palabra;
		ss >> pos >> es_insercion >> palabra;

		if (es_insercion == 1) insertar_palabra(palabra, pos);
		else borrar_palabra(pos);

		if (ss >> nombre) nombrar_version_actual(nombre);
	}

	return _versiones[_version_actual].pos;
}

void EditorVersionado::guardar_archivo(const string &archivo,
                                       const string &archivo_versiones) const {
	vector<string> texto = _palabras;
	for (int i = _version_actual + 1; i < cantidad_versiones(); i++) {
		const version &v = _versiones[i];
		if (v.es_insercion) texto.insert(texto.begin() + v.pos, v.palabra);
		else texto.erase(texto.begin() + v.pos);
	}

	ofstream txt(archivo);
	for (const string &palabra : texto) txt << palabra << " ";

	ofstream ver(archivo_versiones);
	ver << _versiones[0].nombre << "\n";
	for (int i = 1; i < cantidad_versiones(); i++) {
		const version &v = _versiones[i];
		ver << v.pos << " " << (v.es_insercion ? 1 : 0) << " " << v.palabra;
		if (!v.nombre.empty()) ver << " " << v.nombre;
		ver << "\n";
	}
}