# Guía de estilo — Casa Nostra

## Índice

1. [Convenciones de nombres](#1-convenciones-de-nombres)
2. [Formato del código](#2-formato-del-código)
3. [Header guards](#3-header-guards)
4. [C++ moderno y buenas prácticas](#4-c-moderno-y-buenas-prácticas)
5. [Formato de commits](#6-formato-de-commits)
6. [Reglas del proyecto](#7-reglas-del-proyecto)

---

## 1. Convenciones de nombres

### Archivos y carpetas

- `snake_case` para archivos `.hpp`, `.cpp` y carpetas.
- Sin espacios ni símbolos especiales (acentos, ñ, etc.) **La excepción a la
  regla es en palabras en español que se le presenten al usuario** (como nombres,
  apellidos, etc.).
- utilizar el prefijo `m_*` (que proviene de member) para nombres de atributos
  de clase. Esto nos permite diferenciar entre los nombres de variables locales
  y atributos de clase.

| Antes              | Después             |
| ------------------ | ------------------- |
| `familyTree.hpp`   | `family_tree.hpp`   |
| `mi-archivo.cpp`   | `mi_archivo.cpp`    |
| `src/MiCódigo.cpp` | `src/mi_codigo.cpp` |

### Clases

PascalCase.

```cpp
// Bien
class FamilyTree { };
class Member {
    m_edad = 12;
};

// Mal
class family_tree { };
class member {
    edad = 12;
};
```

### Funciones y variables

snake_case.

```cpp
// Bien
int get_successor(Member* member);
int member_count = 0;

// Mal
int getSuccessor(Member* member);
int memberCount = 0;
```

### Constantes

UPPER_SNAKE_CASE.

```cpp
constexpr int MAX_AGE = 70;
```

## 2. Formato del código

### Indentación

4 espacios. No tabs.

### Llaves

Estilo K&R (llave en la misma línea).

```cpp
// Bien
class FamilyTree {
    // ...
};

if (condition) {
    // ...
} else {
    // ...
}

// Mal
class FamilyTree
{
    // ...
};

if (condition)
{
    // ...
}
```

### Columnas

Máximo 100 caracteres por línea.

```cpp
// Bien (≤ 100 cols)
Member* find_successor(Member* member, bool only_alive, bool only_free);

// Mal (> 100 cols)
Member* find_successor(Member* member, bool only_alive, bool only_free, bool skip_jailed, bool skip_dead);
```

---

## 3. Header guards

Aprovechando las caracteristicas de C++20, usar `#pragma once`. No usar
`#ifndef` / `#define`.

```cpp
// Bien
#pragma once

// Mal
#ifndef FAMILY_TREE_HPP
#define FAMILY_TREE_HPP
// ...
#endif
```

---

## 4. C++ moderno y buenas prácticas

### Usar `nullptr` en vez de `NULL` o `0`

```cpp
// Bien
Member* root = nullptr;

// Mal
Member* root = NULL;
Member* root = 0;
```

### Listas de inicialización en constructores

Mandar a llamar los constructores de la clase, pasandole el valor a sus miembros
lo antes posible, no en el cuerpo del constructor. En ciertas ocasiones cuando
esto no sea posible (como en el caso de pasar referencias a objetos),
se puede mandar al cuerpo del constructor.

```cpp
// Bien
Member::Member(int id, std::string name)
    : id(id), name(std::move(name)) {}

// Mal
Member::Member(int id, std::string name) {
    this->id = id;
    this->name = name;
}
```

### No usar `auto`

El profesor lo prohíbe. Escribir el tipo explícitamente.

```cpp
// Bien
Member* current = root;
int count = 0;

// Mal
auto current = root;
auto count = 0;
```

### `std::ifstream` correcto

```cpp
// Bien
std::ifstream file("datos.csv");
if (!file.is_open()) {
    std::cerr << "Error: no se pudo abrir el archivo" << std::endl;
    return;
}

// Mal
std::ifstream file("datos.csv");
// no verificar si se abrió
```

### Preferir `std::endl` o `'\n'` según contexto

Usar `'\n'` para rendimiento, `std::endl` cuando se necesite flush inmediato.

```cpp
// Bien (rendimiento)
std::cout << "Linea\n";

// Bien (flush forzado)
std::cout << "Error critico" << std::endl;

// Mal (flush innecesario en cada línea)
std::cout << "Linea 1" << std::endl;
std::cout << "Linea 2" << std::endl;
```

## 6. Formato de commits

Formato visto en clase:

```
tipo(ámbito): descripción en presente
```

### Tipos

| Tipo       | Uso                                               |
| ---------- | ------------------------------------------------- |
| `feat`     | Nueva funcionalidad                               |
| `fix`      | Corrección de bug                                 |
| `refactor` | Cambio que no agrega funcionalidad ni corrige bug |
| `docs`     | Documentación                                     |
| `style`    | Formato, linting, estilo                          |
| `chore`    | Configuración, build, dependencias                |

### Ejemplos

```
feat(parser): implement csv file reader
feat(succession): add automatic boss assignment on death
fix(succession): handle edge case when no successor is alive
refactor(family_tree): extract find_successor method
docs(styleguides): add naming conventions section
style: apply clang-format to all source files
chore: add .gitignore and .clang-format
```

### Reglas de commits

- Usar presente imperativo ("add" no "added" ni "adds").
- Primera línea ≤ 72 caracteres.
- Commits atómicos: un cambio lógico por commit.

---

## 7. Reglas del proyecto

- **No usar `std::vector`** — prohibido explícitamente.
- **No usar `auto`** — escribir el tipo explícitamente.
- **No usar `#ifndef` / `#define`** — usar `#pragma once`.
- **No usar `NULL`** — usar `nullptr`.
- **Máximo 100 columnas** por línea.
- **4 espacios** de indentación (no tabs).
- **snake_case** para archivos, funciones y variables.
- **PascalCase** para clases.
- **Sin vectores** (`std::vector` prohibido).
- **Sin símbolos especiales** en nombres de archivos/carpetas.
- **El programa debe seguir ejecutándose** hasta que el usuario decida salir.
- **Datos de prueba incluidos** en `bin/datos.csv`.
