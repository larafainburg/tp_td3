#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

// Incluya aquí los headers que necesite


using std::map;
using std::set;
using std::string;
using std::vector;

typedef int id_version;

class EditorVersionado {
public:
  // Constructores
  EditorVersionado();
  // Método estático auxiliar para construir editores de forma conveniente
  static EditorVersionado crear_con_texto(const string &texto);

  // Lee un texto de "archivo", y, si existe, sus versiones de "archivo.ver"
  // Ver enunciado del TP para obtener el formato esperado del archivo .ver
  // - Reemplaza el documento actual por el documento leído del archivo
  // - Devuelve ultima posicion modificada
  int cargar_archivo(const string &archivo, const string &archivo_comentarios);
  // Almacena el texto en "archivo", y sus datos de versiones en "archivo.ver"
  // Ver enunciado del TP para obtener el formato esperado del archivo .ver
  void guardar_archivo(const string &archivo,
                       const string &archivo_comentarios) const;

  // Observadores

  // longitud del texto, medida en palabras
  int longitud() const;
  // i-ésima palabra de texto
  const string &palabra_en(int pos) const;

  // Cuántas versiones tiene el historial de cambiso del documento actual
  int cantidad_versiones() const;
  // Número de versión actual, entre 0 y cantidad_versiones()-1
  id_version version_actual() const;
  // Conjunto de versiones con nombre existentes (de toda la historia del
  // documento)
  const set<string> &versiones_con_nombre() const;
  // Conjunto de nombres de la versión actual
  const string &nombre_version_actual() const;

  // Modificadores

  // Edición del texto.
  // Cada edición genera una versión nueva.
  // Si la versión previa a la inserción/eeliminación no era la última,
  // todas las versiones posteriores al ccambio se descartan y ésta pasa a ser
  // la última versión

  // Agrega una palabra en la posición pos del texto
  void insertar_palabra(const string &palabra, int pos);
  // Quita la palabra de la posición pos del texto
  void borrar_palabra(int pos);

  /// Cambio de versiones
  // Ninguna de estas operaciones altera qué versiones existen en el documento
  // Sólo alteran:
  // - En qué versión está el documento (su texto)

  // Deshace el último cambio (retrocede una versión)
  // Devuelve posicion del texto modificada en esa versión
  int deshacer();
  // Rehace el último cambio (avanza una versión)
  // Devuelve posicion del texto modificada en esa versión
  int rehacer();
  // Ir a una versión anterior o posterior por número de versión (0 a
  // cantidad_versiones()-1). Devuelve la posicion modificada en esa versión
  int ir_a_version(id_version num_version);

  /// Manejo de nombres de versión
  // Estas operaciones no alteran qué versiones existen en el documento. Altera:
  // - Qué nombres tienen las versiones que existen
  // - Cual es la versión actual

  // Le pone o cambia el nombre a la versión actual.
  // El nombre no puede ser vacío y no puede estar en existencia
  void nombrar_version_actual(const string &nombre);
  // Saltar a una versión por nombre
  // Devuelve la posicion del texto modificada en esa version
  int ir_a_version_con_nombre(const string &nombre);
  // Olvida alguno de los nombres que se asignaron a versiones (no
  // necesariamente de la versión actual)
  void olvidar_version(const string &nombre);

private:
  struct version {
    // Posición de la inserción/borrado entre 0 y longitud() del texto previo al cambio
    // La posición -1 representa a una edición vacía, que es la de la versión 0 (unicamente)
    int pos;
    // si fue inserción o borrado
    bool es_insercion;
    // Palabra insertada/borrada en ubicación "pos"
    string palabra;
    // Nombre de la versión (si no es el string vacío)
    string nombre;
  };

  // Palabras del texto. Cada palabra no tiene espacios
  vector<string> _palabras;
  // Secuencia de versiones del texto.
  vector<version> _versiones;
  // 0 <= _version_actual < |_versiones|
  int _version_actual;

  // Conjunto de nombres de versión existentes (para devolver por referencia
  // desde versiones_con_nombre())
  set<string> _nombres;
  // nombre -> número de versión al que corresponde (para encontrar la
  // versión de un nombre en O(log V))
  map<string, id_version> _version_de_nombre;
  // número de versión -> foto completa del texto en esa versión
  // (solo para versiones con nombre; evita reconstruir el texto pasando
  // por todas las versiones intermedias)
  map<id_version, vector<string>> _texto_de_version;
};

// Chequeo de _versiones
// longitud_texto(v: versiones, pos: int) === Sum(i=1...pos) {if v[i].es_insercion then 1 else -1 fi}
// es_secuencia_de_versiones_correcta(v : vector<version>) ===
// |v| > 0 && v[0] = {-1, true, "", ""}
// && (forall i,j: int) 1 <= i,j < |v| => |v[i].palabra| > 0
// && v[i].pos <= longitud_texto(v,i-1) && v[i].nombre == v[j].nombre => i == j || v[i].nombre == "" 

/// Predicado auxiliar dado, no se necesita escribir:
// es_resultado_de_versiones(texto: vector<string>, versiones: vector<version>, version: int)
// Este predicado será VERDADERO si y solo si el texto de la variable `texto`
// es el resultado de aplicar la secuencia de operaciones desde versiones[1]
// hasta versiones[version] inclusive, asumiendo que la secuencia de versiones dada es correcta
// Si version==0, text debe ser "" (el texto vacío)

/// REP de la estructura de representación
// Rep(e: EditorVersionado) ==
// 1. versiones ok
// es_secuencia_de_versiones_correcta(e._versiones) &
// 2. version actual ok
// 0 <= e._version_actual && e._version_actual < |e._versiones|
// 3. palabras ok
// && es_resultado_de_versiones(e._palabras,e._versiones,e._version_actual)

// ... completar el rep cuando esté la estructura definitiva ...
