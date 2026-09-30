# Cuestionario: Práctica Productor Consumidor

## 1. prodcons_01.c (Condición de carrera)
**Pregunta:** Explique qué problema de sincronización presenta esta implementación.

**Respuesta:** El problema radica en el orden de las instrucciones dentro del ciclo del consumidor. El hilo realiza la evaluación de la variable compartida `if (n == 0)` **después** de haber liberado la exclusión mutua mediante `sem_post(&s)`. Al estar fuera de la sección crítica, se abre una ventana temporal o "condición de carrera". Si justo en ese instante el núcleo le otorga el procesador al hilo productor, este podría incrementar `n` a 1 y ejecutar `sem_post(&delay)`. Como el consumidor aún no había evaluado su `if`, esa señal *wake-up* enviada al `delay` se pierde en el vacío. Cuando el consumidor finalmente recupere el procesador, evaluará erróneamente un estado antiguo y quedará suspendido infinitamente en el `delay`.

## 2. prodcons_01_buggy_01.c (Error en la operación de un semáforo)
**Pregunta:** Identifique la instrucción incorrecta y explique cómo esta modificación afecta la sincronización entre el productor y el consumidor. ¿Qué sucede si el consumidor intenta retirar un elemento antes de que el productor haya incorporado el primero?

**Respuesta:** 
* **Instrucción incorrecta:** La instrucción problemática es `sem_post(&delay);` ubicada al inicio de la función del consumidor.
* **Afectación y consecuencias:** En la lógica correcta, el consumidor debería usar una instrucción de espera (`sem_wait`) para quedarse inactivo hasta que exista al menos un dato. Al cambiarla por `sem_post`, el consumidor está incrementando artificialmente el semáforo y autorizándose el paso de manera automática. Si el hilo consumidor es despachado por el procesador antes que el productor, entrará libremente a la sección crítica, retirará un elemento que aún no existe, consumirá "basura" y provocará que la variable de conteo de ítems `n` adquiera un valor negativo (-1), corrompiendo de manera irreparable el estado del programa.

## 3. prodcons_01_buggy_02.c (Interbloqueo / Deadlock)
**Pregunta:** Analice cómo estas modificaciones (cambio en el orden de los semáforos) pueden ocasionar un interbloqueo (deadlock) entre ambos hilos.

**Respuesta:** El interbloqueo sucede debido a un estado de **espera circular**. Al invertir el orden, el consumidor ejecuta primero `sem_wait(&s)`, bloqueando exitosamente el acceso a todos los demás hilos sobre el recurso compartido. Seguidamente, al notar que no hay elementos producidos, ejecuta `sem_wait(&delay)` y se queda esperando a que el productor despierte al sistema. El problema es que cuando el productor finalmente intenta generar datos, se queda bloqueado esperando poder acceder a `sem_wait(&s)`, un candado que actualmente está secuestrado por el consumidor dormido. El consumidor no despertará hasta que el productor actúe, y el productor no actuará hasta que el consumidor suelte la llave. El resultado es un cuelgue total (*deadlock*).

## 4. prodcons_01_OK.c (Solución con variable local)
**Pregunta:** Compare esta implementación con `prodcons_01.c` y explique cómo la incorporación de dicha variable `m` permite corregir el problema identificado.

**Respuesta:** En la primera versión (`prodcons_01.c`), el consumidor consultaba la variable `n` en un momento de vulnerabilidad concurrente. En esta solución correcta, se introduce la variable local `m` dentro de la función del consumidor. Antes de liberar la sección crítica de exclusión mutua, se ejecuta `m = n;`. Esto significa que el hilo toma una fotografía del estado *exacto y seguro* del sistema y lo guarda en su propia pila de memoria privada, que no es compartida con el productor. Cuando posteriormente el programa ejecuta el chequeo fuera de la sección crítica `if (m == 0)`, basa su decisión de dormirse o no, en ese estado previamente salvaguardado, logrando esquivar cualquier modificación asíncrona no deseada del productor y solucionando la condición de carrera originaria.