### Descomposición y Mapeo MPI

En la parte distribuida del programa, utilizamos como unidad de descomposición los mapas, es decir, los procesos se distribuyen los mapas a procesar. Para mapear esta unidad de descomposición utilizamos mapeo ciclico, con ayuda del Rank de los procesos, primero se leen los nombres de los mapas y el número de noches de cada uno en el archivo ```job``` correspondiente, y estos datos se guardan en un ```Vector<String>```, seguidamente se entra en un ciclo que va desde el rank hasta el tamaño del vector, este ciclo aumenta sumando el index mas el tamaño del vector, luego el proceso realiza los calculos sobre el mapa guardado en el indice del vector.

La ventaja de utilizar esta implementacion es que no requiere de paso de mensajes, eliminando transferecias de datos por red que potencialmente podrían ser de gran tamaño (si un mapa fuera muy pesado) que podrían ser afectadas por latencia en la red. Para entregar los resultados tampoco es necesaria la transferencia ya que los resultados se guardan en archivos distintos, independientes entre ellos.

Como desventaja está el hecho de que, sin paso de mensajes, todos los procesos requieren tener acceso a los mapas, lo que convierte el tener un sistema de archivos compartido en una necesidad. Además, como desventaja del mapeo cíclico tenemos que, si se diera el caso de que los mapas más pesados fueran asignados al mismo proceso, esto equivale a una mala distribución de trabajo, y se daría una situación de "somos tan rápidos como el más lento de nosotros".

### Descomposición y Mapeo OpenMP

En el caso de OpenMp la unidad de trabajo son las celdas, por lo que los hilos se dividen las celdas que procesar, esto se logra utilizando la clausulo ```Collapse(2)``` que lo que hace es aplanar la matriz para que OpenMP pueda trabajar en ella como si de un arreglo se tratara. Luego de varias pruebas con todos los mapeos disponibles, vimos que el que mejor nos funcionaba era el bloque ciclico, concretamente con bloques de 8. Sin embargo, la diferencia con los otros mapeos es de apenas milisegundos, a excepcion del mapeo dinamico que nos tardo casi el doble, por lo que creemos que este comportamiento podria deberse a los fallos de cache.

### Entrada y salida de datos

Como se mencionó anteriormente, la entrada de los datos la realizan todos los procesos, cada uno lee su vector de mapas el cual es usado para posteriormente procesarlos. Para la salida de datos, cada proceso crea sus archivos de salida, ya sean para mapas con noches postivas o negativas.


