#pragma once

#include "EditorVersionado.h"

#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>
#include <sstream>

using namespace std;
using namespace ftxui;

class UI {
public:
  UI(EditorVersionado &editor)
      : _editor(editor), _pos(0), _reset_position(""),
        _screen(Screen::Create(Dimension::Full(), Dimension::Full())) {}

  void ejecutar() {
    while (true) {
      render();
      ejecutar_comando();
    }
  }

private:
  EditorVersionado _editor;
  int _pos;

  string _reset_position;
  Screen _screen;

  void ejecutar_comando() {
    string tmp, tmp2;
    int version = -1;

    char ch;
    cin >> ch;
    switch (ch) {
    case 'q':
      exit(0);
      break;
    case 'i': {
      getline(cin, tmp);
      stringstream ss(tmp);
      int pos0 = _pos;
      while (ss >> tmp2)
        _editor.insertar_palabra(tmp2, _pos++);
      _pos = pos0;
    } break;
    case 'e':
      if (_pos < _editor.longitud())
        _editor.borrar_palabra(_pos);
      break;
    case 'd':
      _pos = _editor.deshacer();
      break;
    case 'r':
      _pos = _editor.rehacer();
      break;
    case 'C':
      cin >> tmp;
      _pos = _editor.cargar_archivo(tmp,tmp+".ver");
      break;
    case 'S':
      cin >> tmp;
      _editor.guardar_archivo(tmp,tmp+".ver");
      break;
    case 'n':
      cin >> tmp;
      _editor.nombrar_version_actual(tmp);
      break;
    case 'o':
      cin >> tmp;
      _editor.olvidar_version(tmp);
      break;
    case 'v':
      cin >> version;
        _pos = _editor.ir_a_version(version);
      break;
    case 'V':
      cin >> tmp;
      _pos = _editor.ir_a_version_con_nombre(tmp);
      break;
    case 'h':
      _pos = (_pos - 1) % (_editor.longitud() + 1);
      break;
    case 'l':
      _pos = (_pos + 1) % (_editor.longitud() + 1);
    default:
      break;
    }
  }

  void render() {
    stringstream status_posicion, status_versiones, instrucciones, ft;
    std::vector<Element> palabras;
    status_posicion << "Posición: " << _pos << " de " << _editor.longitud();
    status_versiones << "Versiones: " << _editor.cantidad_versiones()
                     << " Con nombre: " << _editor.versiones_con_nombre().size()
                     << " Actual: " << _editor.version_actual() << " nombres:{ " <<
                     _editor.nombre_version_actual() << " " << "}" << endl;
    instrucciones
        << "teclas: [i]nsertar y [e]liminar palabra, [d]eshacer, [r]ehacer, [h/l] "
           "mover a der/izq, [C]argar/[S]alvar texto, [n]ombrar/[o]lvidar "
           "version,\nir a [v]ersion (por numero)/ ir a [V]ersion (por nombre)"
        << endl;

    for (int i = 0; i < _editor.longitud(); ++i) {
      stringstream txt;
      if (_pos == i)
        txt << "[";
      txt << _editor.palabra_en(i);
      if (_pos == i)
        txt << "]";
      txt << " ";

      palabras.push_back(text(txt.str()));
    }

    if (_pos == _editor.longitud()) {
      palabras.push_back(text(">"));
    }

    std::vector<Element> versiones;
    // {
    //   for (auto c: _editor.(_pos)) {
    //     ft << "[" << c << "]:" << _editor.texto_comentario(c);
    //     comentarios.push_back(text(ft.str()));
    //     ft = stringstream();
    //   }
    // }

    auto document = vbox({
        vbox({
            text(status_posicion.str()),
            text(status_versiones.str()),
        }),
        hflow(palabras) | border | flex,
        vbox(versiones) | flex,
        text(instrucciones.str()),
    });

    _screen.Clear();
    Render(_screen, document);

    cout << _reset_position;
    _screen.Print();
    _reset_position = _screen.ResetPosition();
  }
};
