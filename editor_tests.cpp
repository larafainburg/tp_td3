#include <gtest/gtest.h>
#include <string>

#include "EditorVersionado.h"

using namespace std;

TEST(editor_tests, test_al_iniciar_no_tiene_palabras_ni_versiones) {
  EditorVersionado e;
  ASSERT_EQ(e.longitud(), 0);
  ASSERT_EQ(e.cantidad_versiones(), 1);
  ASSERT_EQ(e.version_actual(), 0);
  ASSERT_EQ(e.nombre_version_actual(), "");
  ASSERT_EQ(e.versiones_con_nombre().size(), 0);
}

TEST(editor_tests, test_permite_construir_un_editor_con_un_texto_inicial) {
  EditorVersionado e = EditorVersionado::crear_con_texto("gato sol árbol libro");

  ASSERT_EQ(e.longitud(), 4);

  ASSERT_EQ(e.palabra_en(0), "gato");
  ASSERT_EQ(e.palabra_en(1), "sol");
  ASSERT_EQ(e.palabra_en(2), "árbol");
  ASSERT_EQ(e.palabra_en(3), "libro");

  ASSERT_EQ(e.version_actual(), 4);
  ASSERT_EQ(e.cantidad_versiones(), 5);
  ASSERT_EQ(e.versiones_con_nombre().size(), 0);
  ASSERT_EQ(e.nombre_version_actual(), "");
}

TEST(editor_tests, test_insertar_una_palabra_modifica_el_texto_y_la_longitud_y_versiones) {
  EditorVersionado e = EditorVersionado::crear_con_texto("Hola");
  e.insertar_palabra("mundo", 1);

  ASSERT_EQ(e.longitud(), 2);
  ASSERT_EQ(e.palabra_en(0), "Hola");
  ASSERT_EQ(e.palabra_en(1), "mundo");

  ASSERT_EQ(e.version_actual(), 2);
  ASSERT_EQ(e.versiones_con_nombre().size(), 0);
  ASSERT_EQ(e.cantidad_versiones(), 3);
  ASSERT_EQ(e.nombre_version_actual(), "");
}

TEST(editor_tests, test_borrar_una_palabra_modifica_el_texto_y_la_longitud_y_versiones) {
  EditorVersionado e = EditorVersionado::crear_con_texto("Hola mundo");
  e.borrar_palabra(1);

  ASSERT_EQ(e.longitud(), 1);
  ASSERT_EQ(e.palabra_en(0), "Hola");

  ASSERT_EQ(e.version_actual(), 3);
  ASSERT_EQ(e.versiones_con_nombre().size(), 0);
  ASSERT_EQ(e.cantidad_versiones(), 4);
  ASSERT_EQ(e.nombre_version_actual(), "");
}

TEST(editor_tests, test_deshacer_retrocede_version_y_texto) {
  EditorVersionado e = EditorVersionado::crear_con_texto("Hola mundo");
  ASSERT_EQ(e.version_actual(), 2);
  ASSERT_EQ(e.versiones_con_nombre().size(), 0);
  ASSERT_EQ(e.cantidad_versiones(), 3);
  ASSERT_EQ(e.nombre_version_actual(), "");
  
  ASSERT_EQ(e.deshacer(),1);

  ASSERT_EQ(e.longitud(), 1);
  ASSERT_EQ(e.palabra_en(0), "Hola");

  ASSERT_EQ(e.version_actual(), 1);
  ASSERT_EQ(e.versiones_con_nombre().size(), 0);
  ASSERT_EQ(e.cantidad_versiones(), 3);
  ASSERT_EQ(e.nombre_version_actual(), "");
}

TEST(editor_tests, test_rehacer_avanza_version_y_texto) {
  EditorVersionado e = EditorVersionado::crear_con_texto("Hola mundo");
  e.deshacer();
  ASSERT_EQ(e.version_actual(), 1);
  ASSERT_EQ(e.versiones_con_nombre().size(), 0);
  ASSERT_EQ(e.cantidad_versiones(), 3);
  ASSERT_EQ(e.nombre_version_actual(), "");
  
  ASSERT_EQ(e.rehacer(),1);

  ASSERT_EQ(e.longitud(), 2);
  ASSERT_EQ(e.palabra_en(0), "Hola");
  ASSERT_EQ(e.palabra_en(1), "mundo");

  ASSERT_EQ(e.version_actual(), 2);
  ASSERT_EQ(e.versiones_con_nombre().size(), 0);
  ASSERT_EQ(e.cantidad_versiones(), 3);
  ASSERT_EQ(e.nombre_version_actual(), "");
}

TEST(editor_tests, test_insertar_en_version_intermedia_borra_posteriores) {
  EditorVersionado e = EditorVersionado::crear_con_texto("Hola mundo");
  e.deshacer();
  e.deshacer();
  ASSERT_EQ(e.version_actual(), 0);
  ASSERT_EQ(e.versiones_con_nombre().size(), 0);
  ASSERT_EQ(e.cantidad_versiones(), 3);
  ASSERT_EQ(e.nombre_version_actual(), "");
  
  e.insertar_palabra("chau",0);

  ASSERT_EQ(e.longitud(), 1);
  ASSERT_EQ(e.palabra_en(0), "chau");

  ASSERT_EQ(e.version_actual(), 1);
  ASSERT_EQ(e.versiones_con_nombre().size(), 0);
  ASSERT_EQ(e.cantidad_versiones(), 2);
  ASSERT_EQ(e.nombre_version_actual(), "");
}

TEST(editor_tests, test_borrar_en_version_intermedia_borra_posteriores) {
  EditorVersionado e = EditorVersionado::crear_con_texto("Hola mundo chau");
  ASSERT_EQ(e.version_actual(), 3);
  ASSERT_EQ(e.versiones_con_nombre().size(), 0);
  ASSERT_EQ(e.cantidad_versiones(), 4);
  ASSERT_EQ(e.nombre_version_actual(), "");

  // Retrocede dos versiones a version 1
  e.deshacer();
  e.deshacer();
  
  ASSERT_EQ(e.longitud(), 1);
  ASSERT_EQ(e.palabra_en(0), "Hola");

  ASSERT_EQ(e.version_actual(), 1);
  ASSERT_EQ(e.versiones_con_nombre().size(), 0);
  ASSERT_EQ(e.cantidad_versiones(), 4);
  ASSERT_EQ(e.nombre_version_actual(), "");

  // borra una palabra. Esto elimina versiones futuras  
  e.insertar_palabra("chau",1);

  ASSERT_EQ(e.longitud(), 2);
  ASSERT_EQ(e.palabra_en(0), "Hola");
  ASSERT_EQ(e.palabra_en(1), "chau");

  ASSERT_EQ(e.version_actual(), 2);
  ASSERT_EQ(e.versiones_con_nombre().size(), 0);
  ASSERT_EQ(e.cantidad_versiones(), 3);
  ASSERT_EQ(e.nombre_version_actual(), "");
}

TEST(editor_tests, test_ir_a_version_0) {
  EditorVersionado e = EditorVersionado::crear_con_texto("Hola amigos chau");

  ASSERT_EQ(e.version_actual(), 3);
  ASSERT_EQ(e.versiones_con_nombre().size(), 0);
  ASSERT_EQ(e.cantidad_versiones(), 4);
  ASSERT_EQ(e.nombre_version_actual(), "");
  
  ASSERT_EQ(e.ir_a_version(0),0);

  ASSERT_EQ(e.longitud(), 0);
  ASSERT_EQ(e.version_actual(), 0);
  ASSERT_EQ(e.versiones_con_nombre().size(), 0);
  ASSERT_EQ(e.cantidad_versiones(), 4);
  ASSERT_EQ(e.nombre_version_actual(), "");
  
}

TEST(editor_tests, test_ir_a_ultima_version) {
  EditorVersionado e = EditorVersionado::crear_con_texto("Hola amigos chau");

  ASSERT_EQ(e.cantidad_versiones(), 4);
  ASSERT_EQ(e.version_actual(), 3);
  ASSERT_EQ(e.longitud(), 3);
  ASSERT_EQ(e.palabra_en(0), "Hola");
  ASSERT_EQ(e.palabra_en(1), "amigos");
  ASSERT_EQ(e.palabra_en(2), "chau");
  
  ASSERT_EQ(e.ir_a_version(0),0);
  ASSERT_EQ(e.longitud(), 0);
  ASSERT_EQ(e.ir_a_version(3),2);
  ASSERT_EQ(e.longitud(), 3);
  ASSERT_EQ(e.version_actual(), 3);
  ASSERT_EQ(e.palabra_en(0), "Hola");
  ASSERT_EQ(e.palabra_en(1), "amigos");
  ASSERT_EQ(e.palabra_en(2), "chau");  
}

TEST(editor_tests, test_ir_a_version_intermedia) {
  EditorVersionado e = EditorVersionado::crear_con_texto("Hola amigos chau");

  ASSERT_EQ(e.version_actual(), 3);
  ASSERT_EQ(e.versiones_con_nombre().size(), 0);
  ASSERT_EQ(e.cantidad_versiones(), 4);
  ASSERT_EQ(e.nombre_version_actual(), "");
  
  ASSERT_EQ(e.ir_a_version(0),0);
  ASSERT_EQ(e.ir_a_version(2),1);
  
  ASSERT_EQ(e.longitud(), 2);
  ASSERT_EQ(e.version_actual(), 2);
  ASSERT_EQ(e.versiones_con_nombre().size(), 0);
  ASSERT_EQ(e.cantidad_versiones(), 4);
  ASSERT_EQ(e.nombre_version_actual(), "");  
}

TEST(editor_tests, test_nombrar_version_0) {
  EditorVersionado e;
  ASSERT_EQ(e.longitud(), 0);
  ASSERT_EQ(e.version_actual(), 0);
  ASSERT_EQ(e.versiones_con_nombre().size(), 0);
  ASSERT_EQ(e.cantidad_versiones(), 1);
  ASSERT_EQ(e.nombre_version_actual(), "");

  e.nombrar_version_actual("prueba1");
  
  ASSERT_EQ(e.longitud(), 0);
  ASSERT_EQ(e.version_actual(), 0);
  ASSERT_EQ(e.versiones_con_nombre().size(),1);
  ASSERT_EQ(e.versiones_con_nombre().count("prueba1"),1);
  ASSERT_EQ(e.cantidad_versiones(), 1);
  ASSERT_EQ(e.nombre_version_actual(), "prueba1");
  
}

TEST(editor_tests, test_nombrar_version_N) {
  EditorVersionado e = EditorVersionado::crear_con_texto("Hola mundo chau");
  ASSERT_EQ(e.longitud(), 3);
  ASSERT_EQ(e.version_actual(), 3);
  ASSERT_EQ(e.versiones_con_nombre().size(), 0);
  ASSERT_EQ(e.cantidad_versiones(), 4);
  ASSERT_EQ(e.nombre_version_actual(), "");

  e.nombrar_version_actual("prueba1");
  
  ASSERT_EQ(e.longitud(), 3);
  ASSERT_EQ(e.version_actual(), 3);
  ASSERT_EQ(e.versiones_con_nombre().size(),1);
  ASSERT_EQ(e.versiones_con_nombre().count("prueba1"),1);
  ASSERT_EQ(e.cantidad_versiones(), 4);
  ASSERT_EQ(e.nombre_version_actual(), "prueba1");
  
}

TEST(editor_tests, test_nombrar_version_intermedia) {
  EditorVersionado e = EditorVersionado::crear_con_texto("Hola mundo chau");
  ASSERT_EQ(e.longitud(), 3);
  ASSERT_EQ(e.version_actual(), 3);
  ASSERT_EQ(e.versiones_con_nombre().size(), 0);
  ASSERT_EQ(e.cantidad_versiones(), 4);
  ASSERT_EQ(e.nombre_version_actual(), "");

  e.deshacer();
  e.nombrar_version_actual("prueba1");
  
  ASSERT_EQ(e.longitud(), 2);
  ASSERT_EQ(e.version_actual(), 2);
  ASSERT_EQ(e.versiones_con_nombre().size(),1);
  ASSERT_EQ(e.versiones_con_nombre().count("prueba1"),1);
  ASSERT_EQ(e.cantidad_versiones(), 4);
  ASSERT_EQ(e.nombre_version_actual(), "prueba1");
}

TEST(editor_tests, test_nombrar_version_y_deshacer_cambia_nombre_version_actual) {
  EditorVersionado e = EditorVersionado::crear_con_texto("Hola mundo chau");
  ASSERT_EQ(e.longitud(), 3);
  ASSERT_EQ(e.version_actual(), 3);
  ASSERT_EQ(e.versiones_con_nombre().size(), 0);
  ASSERT_EQ(e.cantidad_versiones(), 4);
  ASSERT_EQ(e.nombre_version_actual(), "");

  e.nombrar_version_actual("prueba2");
  e.deshacer();
  e.nombrar_version_actual("prueba1");
  
  ASSERT_EQ(e.longitud(), 2);
  ASSERT_EQ(e.version_actual(), 2);
  ASSERT_EQ(e.versiones_con_nombre().size(),2);
  ASSERT_EQ(e.versiones_con_nombre().count("prueba1"),1);
  ASSERT_EQ(e.versiones_con_nombre().count("prueba2"),1);
  ASSERT_EQ(e.cantidad_versiones(), 4);
  ASSERT_EQ(e.nombre_version_actual(), "prueba1");
}

TEST(editor_tests, test_nombrar_version_y_rehacer_cambia_nombre_version_actual) {
  EditorVersionado e = EditorVersionado::crear_con_texto("Hola mundo chau");
  ASSERT_EQ(e.longitud(), 3);
  ASSERT_EQ(e.version_actual(), 3);
  ASSERT_EQ(e.versiones_con_nombre().size(), 0);
  ASSERT_EQ(e.cantidad_versiones(), 4);
  ASSERT_EQ(e.nombre_version_actual(), "");

  e.nombrar_version_actual("prueba2");
  e.deshacer();
  e.nombrar_version_actual("prueba1");
  e.rehacer();
  
  ASSERT_EQ(e.longitud(), 3);
  ASSERT_EQ(e.version_actual(), 3);
  ASSERT_EQ(e.versiones_con_nombre().size(),2);
  ASSERT_EQ(e.versiones_con_nombre().count("prueba1"),1);
  ASSERT_EQ(e.versiones_con_nombre().count("prueba2"),1);
  ASSERT_EQ(e.cantidad_versiones(), 4);
  ASSERT_EQ(e.nombre_version_actual(),"prueba2");
}

TEST(editor_tests, test_renombrar_version) {
  EditorVersionado e = EditorVersionado::crear_con_texto("Hola chau");
  ASSERT_EQ(e.longitud(), 2);
  ASSERT_EQ(e.version_actual(), 2);
  ASSERT_EQ(e.versiones_con_nombre().size(), 0);
  ASSERT_EQ(e.cantidad_versiones(), 3);
  ASSERT_EQ(e.nombre_version_actual(), "");

  e.nombrar_version_actual("prueba1");

  ASSERT_EQ(e.versiones_con_nombre().size(),1);
  ASSERT_EQ(e.versiones_con_nombre().count("prueba1"),1);
  ASSERT_EQ(e.nombre_version_actual(), "prueba1");
  ASSERT_EQ(e.cantidad_versiones(), 3);
  
  e.nombrar_version_actual("prueba2");
  
  ASSERT_EQ(e.versiones_con_nombre().size(),1);
  ASSERT_EQ(e.versiones_con_nombre().count("prueba2"),1);
  ASSERT_EQ(e.nombre_version_actual(),"prueba2");
  ASSERT_EQ(e.cantidad_versiones(), 3); 
}

TEST(editor_tests, test_ir_a_version_con_nombre) {
  EditorVersionado e = EditorVersionado::crear_con_texto("1 2 3 4");
  e.nombrar_version_actual("nombre1"); // v4 -> "nombre1"
  e.ir_a_version(0);
  ASSERT_EQ(e.longitud(), 0);

  e.nombrar_version_actual("nombre2"); // v0 -> "nombre2"

  ASSERT_EQ(e.ir_a_version_con_nombre("nombre1"),3);
  
  ASSERT_EQ(e.version_actual(), 4);
  ASSERT_EQ(e.nombre_version_actual(),"nombre1");
  ASSERT_EQ(e.longitud(), 4);
  ASSERT_EQ(e.palabra_en(0), "1");
  ASSERT_EQ(e.palabra_en(1), "2");
  ASSERT_EQ(e.palabra_en(2), "3");
  ASSERT_EQ(e.palabra_en(3), "4");
  
  ASSERT_EQ(e.ir_a_version_con_nombre("nombre2"),0);
  ASSERT_EQ(e.version_actual(), 0);
  ASSERT_EQ(e.longitud(), 0);
  ASSERT_EQ(e.nombre_version_actual(), "nombre2");
}

TEST(editor_tests, test_olvidar_version_con_nombre) {
  EditorVersionado e;
  ASSERT_EQ(e.longitud(), 0);
  ASSERT_EQ(e.version_actual(), 0);
  ASSERT_EQ(e.versiones_con_nombre().size(), 0);
  ASSERT_EQ(e.cantidad_versiones(), 1);
  ASSERT_EQ(e.nombre_version_actual(), "");

  e.nombrar_version_actual("prueba1");
  
  ASSERT_EQ(e.longitud(), 0);
  ASSERT_EQ(e.version_actual(), 0);
  ASSERT_EQ(e.versiones_con_nombre().size(),1);
  ASSERT_EQ(e.versiones_con_nombre().count("prueba1"),1);
  ASSERT_EQ(e.cantidad_versiones(), 1);
  ASSERT_EQ(e.nombre_version_actual(), "prueba1");

  e.olvidar_version("prueba1");
  
  ASSERT_EQ(e.longitud(), 0);
  ASSERT_EQ(e.version_actual(), 0);
  ASSERT_EQ(e.versiones_con_nombre().size(),0);
  ASSERT_EQ(e.cantidad_versiones(), 1);
  ASSERT_EQ(e.nombre_version_actual(), "");
}

// TEST(editor_tests, test_) {
  
// }



