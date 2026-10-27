# Plataforma web

Este documento tiene como fin dar una descripción secuencial de como se requiere que la plataforma web se vea y funcione.

## Nombre de la plataforma

Aún se está a la espera de la definición de la plataforma.

## Definiciones previas

### Estados de los usuarios

|Estado|Caso|
|------|----|
|Pending| Este sera el estado de un usuario al registrar la cuenta sin verificarla con el link de verificacion enviado al correo |
|Active| Este sera el estado de un usuario luego de verificar su cuenta|
|Deleted| Este sera el estado de una cuenta cuando el usuario elimina su cuenta|

### Estados de las estaciones
| Estado | Color |
|--------|-------|
|Active | <span style="display:inline-block;width:1em;height:1em;background:#10B981;border:1px solid #888"></span> `#10B981`|
|Warning | <span style="display:inline-block;width:1em;height:1em;background:#D97706;border:1px solid #888"></span> `#D97706`|
|Error | <span style="display:inline-block;width:1em;height:1em;background:#DC2626;border:1px solid #888"></span> `#DC2626`|
|Inactive | <span style="display:inline-block;width:1em;height:1em;background:#6B7280;border:1px solid #888"></span> `#6B7280`|
| Deleted  | No tendrá color ya que este estado solo se pondra al realizar una eliminación de la vista de los administradores pero la estación se mantendra en la base de datos por tema de trazabilidad y consistencia de datos. (Tema a validar)|
| Hidden  | <span style="display:inline-block;width:1em;height:1em;background:#cccccc;border:1px solid #888"></span> `#cccccc` |


### Sensores y sus variables

Sensores que se van a utilizar:

| Sensor   | Variables |
|----------|-----------|
| Winsen   | Temperatura, Humedad, C02, CO, O3, CH2O, NO2, TVOC, PM 10, PM 2.5, PM 1 |
|Honeywell | PM10, PM 2.5, PM 1 |
| AM2305B  | Temperatura, Humedad |

### Estado de los sensores

| Estado | Color |
|--------|-------|
|Ok | <span style="display:inline-block;width:1em;height:1em;background:#10B981;border:1px solid #888"></span> `#10B981`|
|Warning | <span style="display:inline-block;width:1em;height:1em;background:#DC2626;border:1px solid #888"></span> `#DC2626`|
|Offline | <span style="display:inline-block;width:1em;height:1em;background:#4B5563;border:1px solid #888"></span> `#4B5563`|

### Estado de generación de archivos

| Estado | Color |
|--------|-------|
|Validating | <span style="display:inline-block;width:1em;height:1em;background:#7C3AED;border:1px solid #888"></span> `#7C3AED`|
|Generating | <span style="display:inline-block;width:1em;height:1em;background:#2563EB;border:1px solid #888"></span> `#2563EB`|
|Error | <span style="display:inline-block;width:1em;height:1em;background:#DC2626;border:1px solid #888"></span> `#DC2626`|
|Canceled| <span style="display:inline-block;width:1em;height:1em;background:#4B5563;border:1px solid #888"></span> `#4B5563`|
|Generated|<span style="display:inline-block;width:1em;height:1em;background:#10B981;border:1px solid #888"></span> `#10B981`|

### Estados de alertas

| Estado | Color |
|--------|-------|
|Warning | <span style="display:inline-block;width:1em;height:1em;background:#D97706;border:1px solid #888"></span> `#D97706`|
|Critical | <span style="display:inline-block;width:1em;height:1em;background:#DC2626;border:1px solid #888"></span> `#DC2626`|
|Informative| <span style="display:inline-block;width:1em;height:1em;background:	#2563EB;border:1px solid #888"></span> `	#2563EB`|


## Especificaciones

**Roles:** 

- Administrador
- Usuario general

### Vistas compartidas

Al referirse en vistas compartidas son vistas que van a tener ambos roles y que solo cambiaran aspectos mínimos en base al rol del usuario.

-  **Login:** El login debe permitir iniciar sesión con cuentas como google, microsoft, apple o por medio de ingreso de correo y contraseña. El sistema debe detectar el rol y en base a esto habilitar ciertas funcionalidades.

- **Mapa dinámico principal:** En esta vista se presentara un mapa interactivo el cuál mostrara la ubicación de las estaciones por medio de iconos de ubicación que tendrá un color  de fondo en base al estado en el que se encuentre la estación. Al pasar el mouse sobre un icono deberá mostrar un cuadro pequeño donde muestre información de resumen de la estación como:
    - Nombre
    - ID
    - Estado
    - Dirección
    - Estado de cada uno de los sensores que tiene cada estación
 
    Si presiona un icono, este llevara a ver la vista del detalle de cada estación.

- **Listado de estaciones:** En esta vista se deben ver las estaciones en tarjetas con información básica de las estaciones como:

    - Nombre
    - ID
    - Estado
    - Dirección
    - Estado de cada uno de los sensores que tiene cada estación

    Despues de las tarjetas de las estaciones se vera un grafico el cuál mostrara como ha cambiado x variable de x sensor de una, unas o todas las estaciones durante el tiempo dando la posibilidad de elegir la/s estaciones, el sensor y en base al sensor listar las variables posibles de ver en el gráfico, tambien se puede elegir el rango de tiempo en el cual se quiere visualizar:
        
    - Rangos de tiempo:
        - 24 Horas
        - Ultima semana
        - Ultimo mes
        - Ultimo año
        - 2 años
    
    Para este grafico se debe tener en cuenta que solo se puede ver una variable a la vez de un solo sensor.

    Despues de este gráfico se encontrara una tabla donde se mostrara un historico de datos enviados por las estaciones los cuales solo mostrara los últimos 10, 20, 50 y 100  datos enviados por la estaciones y en la parte superior de esta tabla un boton que abrira la vista para generar el archivo de descarga de datos. Tambien un filtro para filtrar por estacion, sensor o variable.

    - ***Diferencias entre roles:***
        - Administrador:
            - Boton para crear una estación nueva en la parte superior de las tarjetas.
            - Un boton con un lapiz para editar la estacion en cada tarjeta
            - Un boton con un basurero para eliminar la estación en cada tarjeta
            - El administrador en este listado de las estaciones es el unico que podra ver la estaciones ocultas

- **Detalle de cada estación:** En esta vista ya se vera la información mas detallada y completa de la estación. Tendra en la parte superior un contenedor con:
    - ID
    - Nombre
    - Estado
    - Mapa con la ubicación de la estación
    - Descripción
    - Última fecha y hora en la que envió datos la estación

    Despues de este contenedor se encontraran 3 tarjetas dedicadas para cada sensor donde se muestra el nombre del sensor y las variables definidas para ese sensor mostrando el último dato capturado.

    Despues de las tarjetas se vera un grafico el cuál mostrara como ha cambiado x variable de x sensor durante el tiempo dando la posibilidad de elegir el sensor y en base al sensor listar las variables posibles de ver en el gráfico, tambien se puede elegir el rango de tiempo en el cual se quiere visualizar:
        
    - Rangos de tiempo:
        - 24 Horas
        - Ultima semana
        - Ultimo mes
        - Ultimo año
        - 2 años
    
    Para este grafico se debe tener en cuenta que solo se puede ver una variable a la vez o poder mostrar variables que se capturan con varios sensores y comparar entre esos dos sensores por lo que se debe tener un filtro flexible en base a los sensores y variables.

    Despues de este gráfico se encontrara una tabla donde se mostrara un historico de datos enviados por la estacion los cuales solo mostrara los últimos 10, 20, 50 y 100  datos enviados por la estación y en la parte superior de esta tabla un boton que abrira la vista para generar el archivo de descarga de datos. Tambien un filtro para filtrar por  sensor o variable.

    - ***Diferencias entre roles:***
    
        - Administrador:
            - Boton para editar datos de la estación.
            - Boton para habilitar o deshabilitar la estación, este boton debe abrir una ventana emergente para validar si la estación se va a ocultar del mapa al deshabilitar la estación, si se decide ocultar el estado pasa a ser hidden Y el boton de habilitar pasa a ser mostrar para mostrar la estacion en el mapa pero sin habilitar, al dar en mostrar la estacion se muestra en el mapa y el boton vuelve a ser habilitar.
            - Boton para eliminar la estación, debera abrir una ventana emergente de confirmación, motivo de eliminación obligatorio y verificación de seguridad por medio de solicitud de la contraseña.

                Para eliminar una estación el manejo que se dara es que se cambia el estado a deleted pero se deja en la base de datos y el sistema al detectar que esta en este estado no se mostrara en el listado de estaciones, si no en una vista especifica.


- **Ventana para generar archivo de descarga:** Esta ventana emergente mostrara los filtros de descarga de datos los cuales consistiran en lo siguiente:

    - Tipo de archivo a generar (csv y json)
    - Todas las estaciones o estaciones especificas
    - Datos que desea obtener
    - Rangos de fechas en los que desea obtener la información

    Dentro de esta misma vista en la parte inferior se listara el historial de descargar y de archivos generados por el usuario. Dentro de la lista se mostrara información basica como:

    - Nombre del archivo
    - Fecha de descarga
    - Estado de generación del archivo
    - Botón para cancelar (Solo debe aparecer si el estado es diferente a generated o error)
    - Boton de descarga solo activo cuando el archivo esta en estado generated
    - boton para ver el detalle del filtro de descarga utilizado y tambien para visualizar el proceso a la hora de generar el archivo (%)
    - cuando esta en estado generated debe validar el peso del archivo

    Los archivos tienen un tiempo de duracion de un mes.

- **Perfil de los usuarios:** En esta vista se mostrara la informacion registrada por los usuarios en base a su rol.

    Tendrá un boton para editar, recuperar contraseña, cerrar sesion y eliminar cuenta.

- **Editar perfil:** En esta vista se abrira una ventana emergente con un formulario permitiendo editar el nombre y apellidos, numero de telefeno y la contraseña verificando la contraseña actual y dando la opcion de olvide mi contraseña. 

- **Recuperacion de contraseña:** Para esta vista se debe presionar en recuperar contraseña o en olvide mi contraseña la cual primero solicitara el correo registrado y el sistema validara si se encuentra registrado el correo, enviara un link a ese correo y en ese link se abrira la vista para restablecer la contraseña. Luego de restablecer la contraseña mostrara contraseña reestablecida exitosamente si es el caso y llevara a la pagina de login.

La contraseña se debe validar que no sea la anterior.

### Vistas exclusivas para usuario general

- **Registro:** Esta vista permite al usuario general registrarse con microsoft, apple, gmail o por medio de correo. Si el registro es por medio de correo debe validar la siguiente información:

    - Nombre
    - Apellidos
    - Número de telefono
    - Correo
    - Contraseña
    - Validacion de contraseña
    - Captcha

    Luego de registrar se debe enviar un correo para verificar y activar la cuenta si se hace registro por medio de correo. Y se le asignara el rol de user.

### Vistas exclusivas para administrador

- **Registrar un nuevo administrador:** En esta vista permite registrar un administrador nuevo por medio de un correo validando la siguiente información:

    - Nombre
    - Apellidos
    - Número de telefono
    - Correo
    - Facultad a la que pertenece
    - Contraseña
    - Validacion de contraseña
    - Captcha

    Luego de registrar se debe enviar un correo para verificar y activar la cuenta si se hace registro por medio de correo. Y se le asignara el rol de admin.

- **Edicion de estaciones:** En esta vista los administradores podran editar información de la estacion como:

    - Nombre
    - Ubicación de la estación
    - Descripción

    puede ser una ventana emergente que cargue los datos de la estación seleccionada.

- **Crear estación:** En esta vista los administradores podran registrar una estación nueva. (Pendiente de revisión de como se hace el registro para que la estación empiece a enviar datos).


- **Configuración:** En esta vista el administrador podra definir elementos como:
    - El umbral para las alertas durante la detección de las variables, definir un umbral para advertencia y otro umbral para estado critico.
    - Intervalo de tiempo de actualización de la pagina con los datos recibidos de las estaciones (5s, 10s, 30s, etc).
    - Timeout de conexion
    - Numero de reintentos de conexión

- **Alertas:** En esta vista se mostraran una lista de alertas de detecciones de anomalias en base a lo siguiente:

    - Mostrara warning cuando el sistema detecte en los datos recibidos que una variable este sobre el umbral de warning definido para esa variable sin sobrepasar el umbral de critical.
    - Mostrara Critical cuando el sistema detecta en los datos recibidos que en una variable se estan evidenciando datos que sobrepasan el humbral definido para critical de esa variable
    - Mostrara informative cuando se detectan casos en las estaciones como:
        - Numeros de reintentos de conexion con una estacion
        - Tiempo sin conexion con una estación
        - Anomalias en algunos de los sensores de las estaciones
        - Edicion de la informacion de una estacion
        - Eliminacion de una estacion
        Creacion de una estacion

    La informacion que debera mostrar sera en tarjetas donde mostrara:
        - El estado de la alerta
        - Que tipo de alerta corresponde 
        - Estacion de donde proviene la alerta
        - fecha y hora en la que se genero la alerta.
        - Usuario involucrado en ciertos casos de alertas informativas.

    Al presionar en una tarjeta debera mostrar la vista con el detalle de la alerta.

    La lista se puede filtrar por:

    - Estación
    - Tipo de estado de la alerta
    - Fecha de la alerta

    Tambien debe haber un boton para marcar como vista la alerta. Esta marca debe ser independiente de cada usuario y solo evidenciara el cambio para el administrador que marque la alerta como vista, para los demas administradores aparecera la alerta asi otro administrador la hubiera marcado como vista.

- **Detalle de alertas:** En esta vista se debera evidenciar una alerta en especifico donde se mostrara:

    - El estado de la alerta
    - Que tipo de alerta corresponde 
    - Estacion de donde proviene la alerta
    - Fecha y hora en la que se genero la alerta.
    - Descripción del hallazgo encontrado
    - Tabla con evidencias de los hallazgos encontrados

    Al entrar en esta vista, en la lista de alertas se marcara como vista.

- **Vista de estaciones eliminadas:** En esta vista los administradores podran ver tarjetas con las estaciones eliminadas donde mostrara:

    - Nombre
    - ID
    - Dirección
    - Fecha de eliminacion
    - Administrador que elimino la estacion
    - Motivo de la eliminacion


