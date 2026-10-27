# Cambios, correciones y puntos abiertos

## Cambios y correcciones

1.2 En el rol de administrador también pueden administrar de manera completa a los otros administradores:
    - Registrar
    - Actualizar
    - Listar
    - Eliminar

De acá nacen otras vistas para la administración de otros administradores.

2.1 Se agrega un nuevo estado para los administradores:

***New***

Funciona solo a la hora de registrar al administrador y cambia a Active al administrador establecer su contraseña en el enlace de activación.

## Puntos abiertos

***PA-04***

- **Respuesta:** 
Se reviso las unidades y todas corresponden con la hoja técnica de los sensores. 

***PA-30***

- **Respuesta:** 
    - Estaciones recién creadas o que no reciben datos desde el broker se actualizan en un estado nuevo a Sin datos
    Diferencia con estados como Inactive, hidden, deleted. Al estar en estos estados se lanza una instrucción para que no se escuchen o no se consulten los datos al broker.

***PA-36***

- **Respuesta:** 
    - Los históricos de las estaciones con estado hidden y deleted no se deben graficar ni poder descargar a partir del momento que se cambie para alguno de los dos estados.

***PA-46***

- **Respuesta:** 

    - Debe haber un captcha para la descarga del archivo para evitar descargas y generaciones masivas de archivos o ataques.

    - El archivo se mantiene en el servidor 12 horas máximo si se descarga o no se descarga.

    - Si un usuario genera un archivo de descarga antes de las 12 horas de que se elimine el archivo anterior o de manera continua, solo se conserva el último archivo generado y se establece un límite de solicitudes de descargas.

    - Se debe establecer un limite de descargas por IP en el caso de ususarios generales y administradores

***PA-49*** 

- **Respuesta:** 

    - Siguiendo la recomendación se debe cambiar por el tiempo máximo sin mensajes por cada estación y cuando se detecta que hay una desconexión o desincronización de la api con el broker.

