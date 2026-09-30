# Práctica: Problema Productor-Consumidor con Hilos POSIX

## Descripción
Este proyecto implementa y analiza el clásico problema de concurrencia "Productor-Consumidor" en lenguaje C. Mediante el uso de la biblioteca `pthread` y semáforos POSIX en un entorno Linux, el proyecto demuestra cómo coordinar el acceso a recursos compartidos. Además, incluye un análisis progresivo de implementaciones para comprender y diagnosticar problemas comunes como condiciones de carrera (*race conditions*), operaciones semánticas incorrectas y bloqueos mutuos (*deadlocks*), culminando en la implementación de un sistema seguro para múltiples productores y consumidores.

## Objetivos
- Instanciar múltiples hilos en un solo proceso.
- Proteger secciones críticas y coordinar eventos mediante el uso de exclusión mutua y Semáforos POSIX (`sem_wait` y `sem_post`).
- Identificar el impacto de una mala sincronización y solucionar interbloqueos lógicos (*deadlocks*).
- Implementar un búfer circular seguro gestionado por varios productores y consumidores simultáneamente.

## Tecnologías Utilizadas
- **Lenguaje:** C
- **Compilador:** GCC
- **Bibliotecas:** POSIX Threads (`<pthread.h>`), Semáforos (`<semaphore.h>`)
- **SO objetivo:** Linux / UNIX-like

## Estructura del Repositorio
- `/src/`: Contiene los distintos archivos de código fuente analizados en la práctica (ej. `prodcons_01.c`, versiones *buggy*, versión *OK* y el ejercicio final `prodcon.c`).
- `/evidencias/`: Capturas de pantalla que demuestran la compilación y los distintos comportamientos observados en terminal (congelamientos, condiciones de carrera, flujo correcto).
- `preguntas.md`: Análisis de resultados y respuestas a las interrogantes teóricas sobre cada implementación.

## Instrucciones de Compilación
El proyecto incluye diferentes versiones del código en la carpeta `src/`. Para compilar cualquiera de ellos, abre la terminal y asegúrate de incluir la bandera `-lpthread` para el manejo de hilos y `-lrt` para las rutinas de semáforos.

Ejemplo básico de compilación para la solución final:
```bash
gcc -o prodcons_01_OK src/prodcons_01_OK.c -lpthread -lrt
```

Si deseas mantener un orden utilizando un directorio para binarios (`bin/`), el comando sería:
```bash
gcc -o bin/prodcons_01_OK src/prodcons_01_OK.c -lpthread -lrt
```
*(Nota: Asegúrate de crear el directorio `bin/` previamente si decides usar esta estructura).*

## Ejecución
Una vez compilado, puedes ejecutar el programa directamente desde la terminal:
```bash
./prodcons_01_OK
```