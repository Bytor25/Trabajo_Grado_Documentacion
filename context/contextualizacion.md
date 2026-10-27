
# Plataforma web para la visualización informativa de una iniciativa IoT

**Documento:** contextualización del proyecto (documento vivo).
**Estado:** borrador en construcción mientras se terminan de levantar los requerimientos y de resolver las dudas con los investigadores.
**Documentos relacionados:**

- [plataforma_web_descripcion.md](plataforma_web_descripcion.md): especificación funcional (vistas, requisitos `RF`, reglas de negocio `RN`, requisitos no funcionales `RNF` y puntos abiertos `PA`).
- [gestion_cambios.md](gestion_cambios.md): registro de los cambios de este documento y de la especificación funcional.

Los elementos marcados como **(propuesto)** son sugerencias para completar el contexto y deben validarse con el director del trabajo de grado y con los investigadores del macroproyecto. Los temas pendientes de este documento se identifican con el prefijo `PC-xx` (pendiente de contexto) para no confundirlos con los `PA-xx` de la especificación funcional.

---

## 1. Contexto

Dentro de la Universidad Católica de Oriente (UCO) se presentó un macroproyecto de investigación denominado *"Implementación de un sistema IoT experimental con sensores de bajo costo para el monitoreo de calidad del aire en el municipio de Rionegro, Antioquia"*, con identificador **IDI-FR-05**. Esta iniciativa busca, en conjunto con los programas de Ingeniería Ambiental, Ingeniería Electrónica e Ingeniería de Sistemas, desarrollar estaciones de monitoreo de variables ambientales que permitan recolectar, procesar y validar los datos que capturan, y así aportar a los procesos investigativos de la Universidad.

El macroproyecto, dentro de sus objetivos específicos, presenta los dos siguientes:

Objetivo específico número 2: *"Desarrollar y aplicar una metodología de calibración y validación en campo (co-ubicación) utilizando modelos avanzados de Aprendizaje Automático (ANN), comparando su desempeño con modelos de regresión lineal múltiple, para garantizar la calidad de los datos"*.

Objetivo específico número 3: *"Realizar mediciones de calidad del aire con los sensores calibrados y compararlas con mediciones en paralelo de microplásticos en ambientes interiores y exteriores"*, cuyo producto asociado tiene el subtipo *"Reportes interactivos y plataforma web de reporte tipo dashboard"*.

A partir de estas necesidades se vinculan dos estudiantes de Ingeniería de Sistemas como auxiliares de investigación, con el fin de dar cumplimiento a estos productos, desarrollar su trabajo de grado y poner en práctica los conocimientos adquiridos durante su proceso formativo.

### 1.1 Participación de cada programa en el macroproyecto

| Programa | Aporte al macroproyecto | Relación con la plataforma web |
|----------|-------------------------|--------------------------------|
| Ingeniería Electrónica | Diseño y montaje de las estaciones, firmware, reloj de las estaciones y comunicación (WiFi en la primera fase; LoRa en fases futuras). | Define el firmware y el mensaje que las estaciones publican por MQTT, incluida la marca de tiempo: es la **contraparte del contrato de integración**. |
| Ingeniería Ambiental | Interpretación de las variables, umbrales de calidad del aire, mediciones de microplásticos y análisis de resultados. | Define, con el investigador principal, los umbrales, niveles y colores de alerta de las variables, y los criterios de anomalía; valida los resultados de la calibración. Es el **principal usuario investigador**. Las mediciones de microplásticos no pasan por la plataforma. |
| Ingeniería de Sistemas | Plataforma web, contrato de integración por MQTT y metodología de calibración de los sensores (Winsen ZPHS01B, Honeywell HPMA115S0-XXX, AM2305B y los que se agreguen) mediante modelos de aprendizaje automático (este proyecto). | Recibe, valida, almacena, visualiza y exporta los datos, y entrena, compara y exporta los modelos de calibración y de estimación de variables. |

La distribución exacta de responsabilidades entre programas está por confirmar (`PC-01`).

### 1.2 Problema que atiende la plataforma

Las estaciones de monitoreo generan datos de forma continua, pero sin una plataforma que los centralice y los analice:

- los sensores de bajo costo entregan lecturas que se alejan de los valores reales si no se calibran frente a una referencia;
- los investigadores no tienen un único lugar donde consultar, comparar, calibrar y descargar los datos de todas las estaciones;
- no hay forma de detectar a tiempo estaciones desconectadas, lecturas anómalas o valores que superan los umbrales de calidad del aire;
- la ciudadanía no tiene acceso a información comprensible sobre la calidad del aire de Rionegro;
- el macroproyecto no cuenta con los productos comprometidos para los objetivos específicos número 2 y 3.

### 1.3 ¿Por qué construir una interfaz?

La plataforma no es solo un repositorio técnico de datos: es el medio de **conexión con las comunidades** y parte del **proceso de apropiación social del conocimiento** del macroproyecto. Por eso:

- la consulta es libre, sin registro, para que cualquier persona pueda acceder a ella;
- las vistas públicas usan un lenguaje no técnico, de modo que cualquier persona entienda lo que la página presenta;
- la información aporta al conocimiento, a la investigación y al medio ambiente, en parte concientizando a las personas sobre la calidad del aire que respiran.

### 1.4 Funcionamiento definido de las estaciones

**Alcance de la primera fase:** el proyecto se enfoca en las **primeras 5 estaciones, conectadas por WiFi**, con los sensores ya definidos. Las estaciones con LoRa quedan para fases futuras.

**Protocolo de envío de datos:** MQTT. Las estaciones publican sus lecturas en un bróker MQTT administrado (proveedor por definir, `PC-07`), al que la plataforma se suscribe.

**Redes de comunicación:** WiFi. En fases futuras, algunas estaciones tendrán LoRa como red principal o de respaldo; en ese caso, las estaciones con LoRa enviarán sus datos a una estación con WiFi, y esta los publicará en el bróker.

```
Primera fase:   Estación con WiFi ──WiFi / MQTT──► Bróker MQTT ──► Plataforma web
Fases futuras:  Estación con LoRa ──LoRa──► Estación con WiFi ──WiFi / MQTT──► Bróker MQTT
```

**Sensores:** inicialmente cada estación tendrá los siguientes tres sensores; en futuras mejoras se agregarán sensores diferentes, y la plataforma debe permitir registrarlos con las variables que miden y sus unidades.

| Sensor | Variables |
|--------|-----------|
| AM2305B | Temperatura y humedad. |
| Winsen Multi-in-One ZPHS01B | Temperatura, humedad, CO₂, CO, O₃, CH₂O, NO₂, TVOC, PM10, PM2.5 y PM1. |
| Honeywell HPM Series HPMA115S0-XXX | PM10, PM2.5 y PM1. |

La estación envía **todas las variables que capturan sus sensores**. Las unidades de las variables actuales están confirmadas, pero falta registrarlas en la especificación (`PC-02`, `PA-04`).

**Frecuencia de envío:** cada estación envía **un mensaje cada 30 segundos**, con las lecturas de todos sus sensores.

**Contenido de cada mensaje (JSON):**

- código de la estación;
- marca de tiempo de la lectura;
- identificador de cada sensor;
- por cada sensor, los valores de las variables que mide (por ejemplo, el sensor de material particulado envía PM10, PM2.5 y PM1).

El mensaje de ejemplo, la estructura de tópicos y el nivel de QoS están por acordar con Ingeniería Electrónica (`PC-02`). No hay un contrato único: el administrador puede configurar el contrato de cada estación desde la plataforma.

**Marca de tiempo:** la pone la estación con un reloj de hardware (dispositivo por definir). La plataforma valida su formato; si no cumple el formato esperado, lo corrige y genera una alerta para que los administradores corrijan el error en la estación. Se propone lo siguiente **(propuesto)**:

- formato esperado: ISO 8601 en UTC (por ejemplo, `2026-10-26T14:30:00Z`);
- un formato distinto pero legible (por ejemplo, `26/10/2026 14:30:00`) se convierte al formato esperado;
- una marca que no se puede interpretar, o que tiene una hora imposible (en el futuro o muy atrasada), se reemplaza por la hora de recepción del servidor y la lectura se marca como *corregida*;
- se genera **una sola alerta por episodio**, que no se repite hasta que la estación vuelva a enviar marcas válidas (`PA-34`).

**Pérdida de conexión:** si una estación pierde la conexión a internet, no envía datos al bróker. Los datos de ese periodo no se guardan en la estación ni se reenvían: **se acepta esa pérdida**.

- **Hueco "sin datos":** se pierden 2 envíos seguidos (más de 60 s sin mensajes). La plataforma muestra ese periodo como "sin datos", sin interpolarlo.
- **Desconexión:** si después de esos 2 envíos perdidos siguen sin llegar mensajes, la estación se considera desconectada. Se propone fijarlo en **5 minutos sin mensajes** **(propuesto)**; ese mismo criterio pone la estación en gris en el mapa y genera la alerta *Tiempo sin conexión* (`PA-31`).
- Los huecos y la desconexión se miden con la **hora de llegada** al servidor, no con la marca de tiempo de la estación, porque esa marca puede estar mal **(propuesto)**.

---

## 2. Objetivos

### 2.1 Objetivo general

Desarrollar una plataforma web y una metodología de calibración y validación en campo, aplicando el ciclo de vida del software, que reciba, valide, almacene, compare, calibre y visualice la información generada por la red de sensores de bajo costo del macroproyecto **IDI-FR-05**, como producto de los objetivos específicos número 2 y 3, y como aporte al proceso investigativo.

### 2.2 Objetivos específicos

1. Definir las etapas del ciclo de vida del software que se seguirán durante el desarrollo de la plataforma y los artefactos que se entregarán en cada una, de modo que sustenten el proceso investigativo, formativo y práctico.
2. Diseñar e implementar la integración por MQTT, con un contrato de integración configurable por estación desde la plataforma, que permita recibir, validar y almacenar los datos enviados por las estaciones de la primera fase (5 estaciones con WiFi, con un mensaje cada 30 segundos) y por las que se agreguen después, y que funcione igual para datos simulados y reales.
3. Desplegar la plataforma, con la visualización y exportación de las series históricas de las variables medidas con acceso libre para investigadores y ciudadanía, y con una estrategia de operación, seguridad y respaldo que permita mantenerla en funcionamiento durante 24 meses dentro de un presupuesto de $2.000.000 COP.
4. Definir y aplicar una metodología de calibración y validación en campo (co-ubicación) de los sensores que compare modelos de regresión lineal múltiple y redes neuronales artificiales (ANN) frente a otras estaciones registradas en la plataforma, permita estimar variables a partir de variables correlacionadas, seleccione el modelo con mejor equilibrio entre precisión y costo computacional y permita exportar el modelo calibrado.

**Sugerencias sobre la redacción de los objetivos:**

- Los estudiantes no estarán vinculados durante los 24 meses de operación; por eso el cumplimiento del objetivo 3 se demuestra con un documento de operación (costos, estrategia, riesgos y su mitigación) y no con la operación misma (ver sección 6 y `PC-06`).
- El objetivo 4 está en espera de un documento de los investigadores con lo que se espera de la analítica (`PC-17`); su redacción se refinará con ese documento.
- En el objetivo 4, si la estación de referencia usa sensores de bajo costo como las demás, el resultado es un ajuste entre sensores (intercalibración) y no una calibración frente a un equipo de referencia; la redacción debe reflejarlo cuando se aclare `PC-10`.
- Cada objetivo debería tener al menos un indicador o entregable verificable (ver sección 6).

### 2.3 Trazabilidad de los objetivos con la especificación funcional

| Objetivo específico | Dónde se aborda | Estado |
|---------------------|-----------------|--------|
| 1. Etapas y artefactos | Sección 5 de este documento. Metodología Scrum (`PC-11`). | Parcial: falta validar los artefactos con el director. |
| 2. Contrato de integración | Sección 1.4 de este documento. Especificación: credenciales por estación (V-14, V-28), formato del mensaje configurable (V-27), sensores por estación (V-25, V-26), `RN-28` a `RN-30`. | Parcial: contenido del mensaje, frecuencia, marca de tiempo y manejo de la desconexión definidos; falta el JSON de ejemplo, los tópicos, el QoS y el tiempo exacto de desconexión (`PC-02`). La especificación (V-27, `RN-29`) aún habla de un formato único y no incluye la validación de la marca de tiempo. LoRa queda para fases futuras (`PA-51`). |
| 3. Series históricas y exportación | V-02, V-03, V-04 y V-05 de la especificación. | Cubierto. Falta actualizar en la especificación el color del mapa, que pasa a depender de los umbrales de las variables y no del estado de la estación (`PC-22`). |
| 3. Acceso para la ciudadanía | Consulta y descarga públicas, sin registro (`RF-G07`, `RN-27`, `RNF-03`). | Cubierto. |
| 3. Despliegue y operación durante 24 meses | Secciones 7 y 8 de este documento. | Parcial: Oracle Cloud Always Free y bróker administrado (`PC-07`); falta el responsable de la operación (`PC-06`). |
| 4. Calibración y analítica de datos | V-29 a V-32 de la especificación (preliminares), `RN-31`, `RN-32`. | En espera del documento de analítica (`PC-17`, `PC-18`, `PC-19`). V-29 debe limitarse a estaciones registradas, sin fuentes externas (`PC-10`). |

El levantamiento y la validación de requerimientos dejan de ser un objetivo específico, pero siguen dentro del alcance (sección 3.1) y de la etapa 2 del ciclo de vida (sección 5).

---

## 3. Alcance

El proyecto recorre el ciclo de vida del software de la plataforma web —levantamiento de requerimientos, diseño, desarrollo, pruebas y despliegue— y define y aplica la metodología de calibración de los sensores mediante modelos de aprendizaje automático, integrada en la plataforma como módulo de analítica de datos. El resultado es una plataforma funcional, preparada para integrarse con las 5 estaciones de la primera fase a medida que se completen los demás objetivos y etapas del macroproyecto.

### 3.1 Incluye

**Gestión y documentación**

- Planeación del proyecto: cronograma, plan de gestión de riesgos y plan de gestión de recursos y costos, con la estimación de cómo varían los costos al agregar estaciones, sensores o variables.
- Levantamiento, documentación y validación de requerimientos con los investigadores.
- Los entregables técnicos que define Scrum (`PC-11`), junto con la documentación técnica: arquitectura, modelo de datos, contrato de integración, manual de despliegue y manual de usuario.
- Documentación de la metodología de calibración: datos usados, modelos, métricas y resultados.

**Recepción y almacenamiento de datos**

- Definición e implementación del contrato de integración por MQTT: bróker, estructura de tópicos, formato del mensaje JSON (configurable por estación desde la plataforma), frecuencia, credenciales por estación y manejo de mensajes inválidos.
- Recepción de los datos de las 5 estaciones con WiFi de la primera fase y de las que se registren después.
- Validación de los datos recibidos: estructura del mensaje, estación registrada, sensores y variables asignados, formato de la marca de tiempo (con corrección y alerta), rangos físicos y valores centinela.
- Detección de huecos "sin datos" y de estaciones desconectadas, sin interpolar los datos faltantes.
- Almacenamiento de las lecturas y de su historial durante el periodo de operación.
- Un **simulador de estaciones** que publique datos en el bróker con el mismo contrato de integración, para desarrollar y probar la plataforma sin depender de las estaciones físicas.

**Funcionalidades de la plataforma** (detalladas en la especificación funcional)

- Consulta pública, sin registro: mapa de estaciones coloreadas según el nivel de alerta de los umbrales de sus variables (en gris y opacas si están sin datos o desconectadas), con las variables en alerta; listado y detalle de estaciones, gráficos históricos, comparación entre sensores y tablas de datos.
- Generación de archivos de descarga en CSV y JSON, sin historial de archivos, con la cita del macroproyecto.
- Inicio de sesión solo para administradores (correo y contraseña), registro de administradores por otro administrador (nombre, correo y contraseña), recuperación y cambio de contraseña, perfil y eliminación de cuenta.
- Gestión de estaciones: crear, editar, habilitar/deshabilitar, ocultar/mostrar y eliminar.
- Catálogo de sensores ampliable, con las variables que mide cada sensor y sus unidades, y asignación de sensores a cada estación.
- Configuración del formato del mensaje MQTT de cada estación y regeneración de las credenciales de conexión de las estaciones con el bróker.
- Cálculo automático del estado de estaciones y sensores.
- Alertas por umbrales, anomalías, problemas de conexión, marcas de tiempo inválidas y acciones de los administradores (alertas *Informative* con el usuario que hizo la acción).
- Configuración de umbrales, intervalo de actualización y parámetros de conexión.
- Correo de contacto en el pie de página para la comunicación con la ciudadanía.

**Analítica de datos** (preliminar, `PC-17`)

- Selección de estaciones registradas en la plataforma como referencia (por ejemplo, una estación casera construida por un administrador) y comparación entre estaciones de zonas distintas.
- Calibración por co-ubicación, entrenando y comparando modelos de regresión lineal múltiple y redes neuronales artificiales, con métricas de precisión y de costo computacional.
- Estimación de variables a partir de variables correlacionadas, eligiendo las variables de entrada, la variable a estimar y el modelo.
- Almacenamiento, comparación y exportación de los modelos calibrados.

**Calidad, despliegue y cierre**

- Atributos de calidad que guían el diseño, el desarrollo y las pruebas: disponibilidad, rendimiento, usabilidad, seguridad, accesibilidad, compatibilidad, mantenibilidad y escalabilidad.
- Elementos de seguridad que la plataforma debe cubrir, sin suponer todavía qué servicios los proveerán: seguridad del software (protección mediante firewall, escaneo y eliminación de malware, actualizaciones periódicas y análisis de tráfico anti-DDoS), certificado SSL, copias de seguridad y restauraciones, protección DDoS, supervisión de redes, soporte CDN y validación de formularios contra la inyección de consultas.
- Plan de pruebas y su ejecución: pruebas unitarias, de integración, funcionales (con el simulador) y de aceptación con los investigadores.
- Validación de los modelos con datos de prueba separados de los de entrenamiento.
- Despliegue en máquinas virtuales de Oracle Cloud (plan Always Free), accesible por internet con el dominio que se acuerde con su propietario (`PC-07`).
- Estrategia de respaldos y de monitoreo básico de la plataforma.
- Transferencia de conocimiento a quien operará la plataforma después del trabajo de grado, incluida la entrega de las cuentas de los servicios.

### 3.2 No incluye

- Diseño, construcción, montaje, instalación o mantenimiento de las estaciones y de sus sensores, y su calibración física (de hardware). La calibración del proyecto se hace sobre los datos, mediante modelos.
- Desarrollo o modificación del firmware de las estaciones, incluido el reloj que pone la marca de tiempo. La plataforma define el contrato de integración por MQTT, pero su implementación en las estaciones corresponde a Ingeniería Electrónica.
- Estaciones con LoRa y la retransmisión de LoRa a WiFi, que quedan para fases futuras.
- Almacenamiento local y reenvío de los datos de una estación durante una desconexión; esa pérdida se acepta. Tampoco se interpolan los periodos sin datos.
- Conectividad de las estaciones (redes WiFi y LoRa, planes de datos, gateways, energía).
- Compra de hardware, incluidos los equipos de referencia.
- Análisis científico de los datos o conclusiones sobre la calidad del aire de Rionegro. Los modelos se limitan a la calibración de los sensores y a la estimación de variables.
- Índice de Calidad del Aire (ICA): no forma parte de la plataforma, del proyecto ni de las estaciones (`PC-04`).
- Datos de microplásticos: la plataforma no los almacena, muestra ni compara (`PC-08`).
- Informes de cualquier tipo, ni de datos ambientales (PDF o resúmenes por periodo) ni de los cambios o acciones en la plataforma; basta con los dashboards y la descarga de datos (`PC-13`).
- Uso o integración de redes y plataformas externas de monitoreo (por ejemplo, SIATA o CORNARE), incluso como fuentes de referencia; toda comparación se hace entre estaciones registradas en la plataforma (`PC-10`).
- Cuentas para visitantes y gestión de PQRS; la comunicación con la ciudadanía se hace por el correo de contacto.
- Aplicaciones móviles nativas; la plataforma será web y adaptable a celulares.
- Notificaciones por correo o *push* de las alertas (`RN-22`).
- Los demás objetivos del macroproyecto distintos de los objetivos específicos número 2 y 3.
- Soporte y mantenimiento evolutivo después de la entrega del trabajo de grado, salvo lo que se acuerde en el plan de operación (`PC-06`).

### 3.3 Supuestos

Condiciones que se dan por ciertas para planear el proyecto; si alguna deja de cumplirse, se debe revisar el alcance, el cronograma o el presupuesto.

| ID | Supuesto |
|----|----------|
| `S-01` | Las 5 estaciones de la primera fase podrán conectarse a internet por WiFi y publicar sus datos en el bróker MQTT siguiendo el contrato de integración que se acuerde. |
| `S-02` | Ingeniería Electrónica adaptará el firmware de las estaciones al contrato de integración por MQTT y definirá e instalará el reloj que pone la marca de tiempo, con un mecanismo para ponerlo en hora (por ejemplo, sincronización NTP al conectarse a WiFi). |
| `S-03` | Mientras no existan estaciones reales en funcionamiento, el desarrollo y las pruebas se harán con datos simulados, y el paso a datos reales no exigirá cambios en la plataforma. |
| `S-04` | Las estaciones iniciarán con los sensores AM2305B, Winsen ZPHS01B y Honeywell HPMA115S0-XXX, con las variables y unidades de la sección 2.3 de la especificación. Los sensores que se agreguen en el futuro se podrán registrar desde el catálogo de la plataforma, con sus variables y unidades, sin cambios en el código, siempre que sus datos lleguen en el formato de mensaje configurado para su estación. |
| `S-05` | El investigador principal y el investigador de Ingeniería Ambiental definirán los umbrales, niveles y colores de alerta de las variables y los criterios para considerar un dato como anómalo, e Ingeniería Ambiental validará los resultados de la calibración. |
| `S-06` | Los investigadores estarán disponibles para validar requerimientos, mockups y entregas parciales con una frecuencia acordada. |
| `S-07` | La primera fase tiene 5 estaciones, cada una con un mensaje cada 30 segundos que incluye las lecturas de todos sus sensores. El número de estaciones de fases posteriores será pequeño (del orden de unidades o pocas decenas; por confirmar, `PC-03`). |
| `S-08` | El presupuesto de $2.000.000 COP se destina a los costos de operación de la plataforma (infraestructura y servicios) y no incluye el trabajo de los estudiantes. |
| `S-09` | La capacidad gratuita del plan Always Free de Oracle Cloud alcanza para la aplicación y la base de datos con el volumen de la primera fase (por verificar, `PC-07`), y existe un bróker MQTT administrado compatible con el presupuesto. |
| `S-10` | La Universidad, titular de los datos, definirá con su área legal la licencia de publicación y la cita de los archivos descargados antes del despliegue, y aprobará los documentos legales (términos y condiciones, política de tratamiento de datos). |
| `S-11` | Se contará con una estación registrada en la plataforma que sirva de referencia para la calibración (por ejemplo, una estación casera de un administrador), co-ubicada con la estación a calibrar durante un periodo suficiente (`PC-10`, `PC-19`). Por validar por los estudiantes. |
| `S-12` | El entrenamiento de los modelos se puede ejecutar con los recursos de cómputo cubiertos por el presupuesto o con equipos de la Universidad, dado que se privilegian modelos de bajo costo computacional. |

### 3.4 Restricciones

| ID | Tipo | Restricción |
|----|------|-------------|
| `R-01` | Presupuesto | La plataforma debe funcionar durante 24 meses con un presupuesto total de $2.000.000 COP, es decir, en promedio unos **$83.000 COP al mes** para toda la operación (bróker MQTT, dominio, correo transaccional, almacenamiento de archivos, respaldos, cómputo para entrenar modelos y servicios externos, además de lo que exceda el plan gratuito del servidor). El equipo define el presupuesto; quién lo aprueba y quién hace los pagos se acordará con el investigador (`PC-07`). |
| `R-02` | Tiempo de operación | La plataforma debe mantenerse disponible durante 24 meses desde su puesta en producción (fecha de inicio por definir, `PC-07`). Como los estudiantes no estarán vinculados durante todo ese periodo, el proyecto entrega el plan de operación que lo hace posible, y la operación queda a cargo de quien se defina en `PC-06`. |
| `R-03` | Tiempo de desarrollo | El desarrollo debe ajustarse al calendario académico del trabajo de grado (fechas por definir, `PC-12`). |
| `R-04` | Equipo | El equipo de desarrollo son dos estudiantes de Ingeniería de Sistemas, con dedicación parcial como auxiliares de investigación, que presentan la plataforma como un trabajo de grado conjunto. |
| `R-05` | Alcance | Solo se atienden los objetivos específicos número 2 y 3 del macroproyecto, con las 5 estaciones con WiFi de la primera fase. |
| `R-06` | Dependencia | La integración con datos reales depende del avance de las estaciones (Ingeniería Electrónica) y de las definiciones de Ingeniería Ambiental; la calibración depende además de la disponibilidad de una estación de referencia y del documento de analítica (`PC-17`). |
| `R-07` | Hardware | Los sensores son de bajo costo: sus datos son indicativos y no reemplazan a los equipos de referencia, aun después de calibrarlos. La plataforma debe comunicarlo así al mostrar los datos a la ciudadanía. |
| `R-08` | Normativa | Tratamiento de los datos personales de los administradores conforme a la Ley 1581 de 2012; licencia de publicación y cita de los datos definidas por el área legal de la Universidad (`PC-14`). |
| `R-09` | Idioma y zona horaria | Interfaz en español; fechas y horas en la zona horaria de Colombia (UTC−5). |
| `R-10` | Protocolo | La recepción de datos de las estaciones se hace por MQTT, mediante un bróker administrado. |
| `R-11` | Infraestructura | La Universidad no provee servidores: la plataforma se aloja en máquinas virtuales de Oracle Cloud en el plan Always Free, con sus límites de procesador, memoria, disco y transferencia. El dominio pertenece a un docente de la Universidad (Juan Camilo) y su configuración se acuerda con él. |

Debido a las restricciones `R-01` y `R-11`, es necesario elaborar un **plan de gestión de riesgos** y un **plan de gestión de recursos y costos**, y elegir un stack tecnológico que permita implementar la plataforma de forma efectiva sin exceder el presupuesto ni los límites del plan gratuito (ver sección 7).

---

## 4. Interesados

| Interesado | Interés o rol en el proyecto | Nombre |
|------------|------------------------------|--------|
| Director del trabajo de grado | Guía académica, aprobación de entregables. | Por definir |
| Investigador principal del macroproyecto IDI-FR-05 | Dueño del producto; valida requerimientos y entregas; define con Ingeniería Ambiental los umbrales del mapa; acuerda la aprobación y el pago del presupuesto. | Por definir |
| Investigadores de Ingeniería Ambiental | Usuarios de los datos; definen umbrales, niveles de alerta y criterios de anomalía, y validan la calibración. | Por definir |
| Investigadores de Ingeniería Electrónica | Responsables de las estaciones, del firmware, del reloj y del envío de datos por MQTT. | Por definir |
| Estudiantes de Ingeniería de Sistemas (2) | Desarrollo de la plataforma y de la metodología de calibración; definición del presupuesto y del nombre de la plataforma. | Por definir |
| Administradores de la plataforma | Operan la plataforma: estaciones, sensores, credenciales, formato del mensaje MQTT, alertas, configuración y analítica de datos; pueden construir y registrar estaciones caseras de referencia. | Por definir |
| Propietario del dominio | Docente de la Universidad; define el proceso, los formatos, el subdominio y la configuración del dominio. | Juan Camilo |
| Ciudadanía de Rionegro | Consulta libre, sin registro, de la calidad del aire; contacto por correo. | — |
| Universidad Católica de Oriente | Titular de los datos; su área legal define la licencia de publicación y la cita de los archivos descargados. | — |

---

## 5. Ciclo de vida y entregables

Se usa **Scrum** adaptado a un equipo de dos personas, con Sprints de una semana y Releases cada dos Sprints, porque los requerimientos siguen abiertos y las estaciones reales no estarán disponibles desde el inicio. Solo se esperan los entregables técnicos que define Scrum; no se pide ningún otro artefacto ni un formato especial (`PC-11`). Los artefactos de la tabla son los que el equipo propone para sustentar cada etapa.

| Etapa | Actividades principales | Artefactos |
|-------|------------------------|------------|
| 1. Planeación | Alcance, cronograma, riesgos, costos. | Este documento, cronograma, plan de gestión de riesgos, plan de gestión de recursos y costos. |
| 2. Requerimientos | Entrevistas con investigadores, especificación funcional, priorización, validación. | Especificación funcional, Product Backlog Items (PBI) priorizados en Azure DevOps, actas de las reuniones. |
| 3. Diseño | Mockups, arquitectura, modelo de datos, contrato de integración MQTT, diseño de la metodología de calibración (datos, modelos y métricas), selección del stack y del bróker. | Mockups, documento de arquitectura, modelo entidad-relación, especificación del contrato MQTT (tópicos, mensaje de ejemplo, QoS, marca de tiempo y credenciales), diseño experimental de la calibración, análisis comparativo de stacks, brókeres y costos. |
| 4. Desarrollo | Implementación por incrementos, simulador de estaciones, módulo de analítica, integración continua. | Repositorio de código, simulador, incrementos de cada sprint. |
| 5. Pruebas | Pruebas unitarias, de integración, funcionales, de carga básicas y de aceptación; validación de los modelos. | Plan de pruebas, casos de prueba trazados a los `RF`, informe de resultados, informe de validación de los modelos. |
| 6. Despliegue | Puesta en producción en Oracle Cloud, bróker MQTT administrado, dominio (con su propietario), respaldos, monitoreo. | Manual de despliegue, plan de respaldos, plataforma en producción. |
| 7. Operación y cierre | Integración con las estaciones reales, calibración con datos reales, capacitación, transferencia (incluidas las cuentas de los servicios). | Manual de usuario, manual técnico, plan de operación de 24 meses, informe final del trabajo de grado. |

**Priorización sugerida del desarrollo** (para asegurar primero lo que da cumplimiento a los productos del macroproyecto):

1. Contrato de integración MQTT, simulador, almacenamiento y validación de datos (incluida la marca de tiempo).
2. Consulta pública: mapa con colores por umbrales, listado, detalle de estación, gráficos y tablas.
3. Exportación de archivos.
4. Autenticación de administradores, gestión de estaciones, catálogo y asignación de sensores, credenciales y formato del mensaje MQTT.
5. Analítica de datos: estaciones de referencia, calibración (RLM y ANN), estimación de variables y exportación de modelos (en cuanto se precisen sus requisitos, `PC-17`).
6. Estados automáticos, alertas y configuración.
7. Funcionalidades complementarias (perfil, eliminación de cuenta, estaciones eliminadas).

---

## 6. Criterios de éxito (propuesto)

| Objetivo específico | Criterio verificable |
|---------------------|----------------------|
| 1 | Documento de etapas y artefactos aprobado por el director. |
| 2 | El simulador y al menos una estación real (si está disponible) publican un mensaje cada 30 segundos por MQTT, cada uno según el contrato configurado para su estación; la plataforma los almacena, rechaza los mensajes inválidos, corrige las marcas de tiempo inválidas generando una alerta, marca los huecos "sin datos" y detecta las estaciones desconectadas, y permite modificar el contrato y regenerar las credenciales de una estación. |
| 3 | La plataforma está desplegada en Oracle Cloud, con respaldos automáticos probados (restauración exitosa). Cualquier persona, sin iniciar sesión, puede consultar el mapa con los colores por umbrales y las series históricas y exportarlas en CSV y JSON; los casos de prueba asociados se ejecutan con éxito. Se entrega un documento de operación para 24 meses con la proyección de costos dentro de $2.000.000 COP (incluida la variación de costos si se agregan estaciones, sensores o variables), la estrategia de operación y mantenimiento, los riesgos y su mitigación, y la demás información necesaria para que la plataforma se pueda usar, mantener y conservar en funcionamiento durante ese periodo. |
| 4 | Para al menos un sensor y una variable (por ejemplo, PM2.5) se entrenan y comparan modelos de RLM y ANN frente a otra estación registrada y co-ubicada, se documentan sus métricas de precisión y su costo computacional, y el modelo elegido se exporta. Los valores de aceptación se definen con los investigadores (`PC-17`). |

---

## 7. Consideraciones de costos y stack tecnológico

### 7.1 Costos a cubrir con el presupuesto

| Concepto | Observación |
|----------|-------------|
| Servidor o plataforma de aplicación | Máquinas virtuales de Oracle Cloud en el plan Always Free: backend, frontend y tareas en segundo plano (generación de archivos, cálculo de estados, alertas y entrenamiento de modelos). Sin costo mientras se respeten los límites del plan; hay que verificar que alcanzan y documentar el riesgo de que Oracle recupere instancias que considere inactivas (`RG-15`). |
| Bróker MQTT | Servicio administrado (proveedor por definir, `PC-07`); debe soportar autenticación por estación y cifrado TLS. Probablemente con costo (por mensaje o por conexión; las capas gratuitas suelen durar 12 meses). La primera fase genera unos 432.000 mensajes al mes. Alternativa: bróker propio en la máquina de Oracle. |
| Base de datos | Es el recurso que más crece con el tiempo, porque depende del volumen de lecturas (ver 7.2); debe caber en el almacenamiento del plan Always Free o planear su crecimiento. |
| Almacenamiento de archivos | Archivos CSV/JSON generados de forma temporal (sin historial) y modelos entrenados. |
| Respaldos | Copias periódicas de la base de datos fuera del servidor principal. |
| Dominio y certificado SSL/TLS | El dominio pertenece a Juan Camilo (docente); el costo, el subdominio y la configuración DNS se acuerdan con él. Certificado para el sitio web y para el bróker MQTT. |
| Seguridad | Protección mediante firewall, escaneo y eliminación de malware, actualizaciones periódicas, protección DDoS y análisis de tráfico, supervisión de redes y soporte CDN. Los servicios que los cubran están por definir. |
| Correo | Recuperación de contraseña de los administradores (volumen muy bajo) y buzón del correo de contacto. |
| Servicios externos | Mapas y geocodificación (`PA-45`), generalmente gratuitos dentro de ciertos límites. |
| Reserva para imprevistos | Se recomienda reservar entre el 10 % y el 15 % del presupuesto. |

Las cuentas de todos los servicios (Oracle, bróker y demás) deberían crearse con un correo institucional o del proyecto, no con uno personal, para poder entregarlas al terminar el trabajo de grado (`PC-07`, `PC-06`).

### 7.2 Estimación del volumen de datos

Cada mensaje de una estación con los tres sensores iniciales tiene 16 valores (11 del Winsen, 3 del Honeywell y 2 del AM2305B). Con un mensaje cada 30 segundos, cada estación envía 2.880 mensajes al día. La primera fila corresponde a la primera fase; las demás son ilustrativas, porque aún no se sabe cuántas estaciones habrá en fases posteriores (`PC-03`):

| Estaciones | Mensajes por día | Valores por día | Mensajes en 24 meses | Valores en 24 meses |
|-----------:|-----------------:|----------------:|---------------------:|--------------------:|
| 5 (primera fase) | 14.400 | ≈ 230.000 | ≈ 10,5 millones | ≈ 168 millones |
| 10 | 28.800 | ≈ 461.000 | ≈ 21 millones | ≈ 336 millones |
| 20 | 57.600 | ≈ 922.000 | ≈ 42 millones | ≈ 673 millones |

Si se agregan sensores, el número de valores por mensaje crece en la misma proporción. Como aún no se sabe qué sensores se agregarán (`PC-20`), el plan de costos debe incluir una **estimación de cómo varían los costos** según las variables, sensores o estaciones que se agreguen. Este dato es clave para elegir la base de datos y para verificar que el almacenamiento del plan Always Free alcanza. Las agregaciones (promedios por minuto, hora o día) y una política de retención de los datos crudos son necesarias para cargar los gráficos de 2 años con buen rendimiento (`RNF-04`) y para no exceder el almacenamiento disponible ni el presupuesto (`R-01`, `R-11`, `PC-15`).

### 7.3 Criterios para elegir el stack

- Costo mensual predecible y compatible con `R-01`, incluyendo el crecimiento de la base de datos.
- Funcionamiento dentro de los límites de las máquinas virtuales de Oracle Cloud Always Free (`R-11`).
- Soporte para series de tiempo o, al menos, índices y agregaciones eficientes por estación, variable y fecha.
- Soporte para un bróker MQTT con autenticación por estación y control de acceso por tópico. El mecanismo de autenticación del bróker (usuario y contraseña o certificados por estación, como en AWS IoT Core) define cómo se registra una estación y cómo se regeneran sus credenciales (V-28), así que el proveedor debe elegirse antes de diseñar esa vista.
- Bibliotecas de aprendizaje automático que permitan entrenar modelos de RLM y ANN con bajo costo computacional y exportarlos.
- Facilidad de mantenimiento por un equipo pequeño y por quien opere la plataforma después del trabajo de grado.
- Conocimiento previo del equipo en las tecnologías.

La comparación de alternativas (por ejemplo, bróker administrado frente a bróker propio en la máquina de Oracle, o base de datos relacional frente a base de datos de series de tiempo) debe documentarse en la etapa de diseño con su costo proyectado a 24 meses.

---

## 8. Riesgos preliminares

Insumo inicial para el plan de gestión de riesgos.

| ID | Riesgo | Probabilidad | Impacto | Respuesta propuesta |
|----|--------|:------------:|:-------:|---------------------|
| `RG-01` | Las estaciones reales no están listas durante el trabajo de grado. | Alta | Medio | Desarrollar y probar con el simulador; validar el contrato de integración con Ingeniería Electrónica desde el inicio. |
| `RG-02` | Los costos de operación superan el presupuesto antes de los 24 meses. | Alta | Alto | Proyectar costos antes de elegir el stack con el volumen de la primera fase (sección 7.2) y el costo del bróker administrado después de su capa gratuita; configurar alertas de facturación; políticas de retención y agregación de datos; reserva para imprevistos. |
| `RG-03` | Los requerimientos cambian o siguen abiertos durante el desarrollo. | Alta | Medio | Desarrollo iterativo, priorización del backlog y control de cambios en [gestion_cambios.md](gestion_cambios.md). |
| `RG-04` | No hay un responsable de la plataforma cuando los estudiantes terminan el trabajo de grado. | Media | Alto | Definir el responsable de la operación; entregar el documento de operación a 24 meses (costos, estrategia, riesgos y mitigación), los manuales técnicos, las cuentas de los servicios y la transferencia de conocimiento (`PC-06`). |
| `RG-05` | Pérdida de datos por fallas del servidor o errores de operación. | Baja | Alto | Respaldos automáticos fuera del servidor y pruebas periódicas de restauración. |
| `RG-06` | Los datos de sensores de bajo costo, aun calibrados, se interpretan como oficiales. | Media | Medio | Aviso visible sobre el carácter indicativo de los datos y de los colores del mapa. |
| `RG-07` | El alcance funcional (consulta pública, administración, integración MQTT y analítica de datos) excede el tiempo disponible. | Media | Alto | Priorizar el núcleo del producto (sección 5) y acordar qué funcionalidades pueden quedar para una fase posterior. |
| `RG-08` | Cambios en los planes gratuitos o precios de proveedores de nube o servicios externos (Oracle Always Free, capa gratuita del bróker). | Media | Medio | Preferir tecnologías portables (por ejemplo, contenedores) que permitan cambiar de proveedor; mantener como alternativa un bróker propio en la máquina de Oracle. |
| `RG-09` | Incidentes de seguridad (acceso indebido, envío de datos falsos al bróker, credenciales de estaciones expuestas, inyección de consultas, malware o ataques DDoS). | Baja | Alto | Credenciales por estación con posibilidad de regenerarlas, SSL/TLS y control de acceso por tópico en el bróker, validación en el servidor y de formularios contra la inyección de consultas, límites de frecuencia, firewall, escaneo de malware, actualizaciones periódicas, protección DDoS, supervisión de redes y CDN (sección 3.1). |
| `RG-10` | Los requisitos de la analítica de datos no se precisan a tiempo. | Alta | Alto | Construir primero la base común (estaciones de referencia, entrenamiento y comparación de RLM y ANN) y acordar métricas mínimas con los investigadores (`PC-17`). |
| `RG-11` | No hay una referencia adecuada para calibrar: no hay periodo de co-ubicación, o la estación de referencia usa sensores de bajo costo y el resultado es solo una intercalibración. | Media | Alto | Definir desde el inicio la estación de referencia, su precisión y el periodo de co-ubicación, y documentar las limitaciones (`PC-10`, `PC-19`). |
| `RG-12` | Abuso de las funciones públicas (descargas masivas o consultas excesivas) que afecte el rendimiento o el costo. | Media | Medio | Límites de frecuencia por IP, rango máximo de fechas y tamaño máximo de archivo (`PA-46`). |
| `RG-13` | Un cambio en el mensaje que envía el firmware interrumpe la recepción de datos. | Media | Medio | Formato del mensaje configurable por estación desde la plataforma, con prueba previa y versiones (V-27). |
| `RG-14` | Se pierden los datos de una estación mientras está sin conexión a internet (pérdida aceptada). | Alta | Bajo | Aceptar el riesgo; mostrar los periodos "sin datos" sin interpolarlos, generar la alerta *Tiempo sin conexión* y documentar los huecos para la analítica (`PC-02`). |
| `RG-15` | El plan Always Free de Oracle no alcanza para el volumen de datos, o Oracle recupera las instancias que considera inactivas. | Media | Alto | Verificar los límites del plan con el volumen de la sección 7.2 antes de elegir el stack; agregación y retención de datos; monitoreo de uso; respaldos fuera de Oracle; despliegue reproducible (por ejemplo, con contenedores) para restaurar en otra instancia o proveedor. |
| `RG-16` | El reloj de una estación se desajusta y envía marcas de tiempo inválidas o imposibles. | Media | Medio | Validar y corregir la marca de tiempo con la hora de recepción, marcar la lectura como corregida, una sola alerta por episodio y medir los huecos con la hora de llegada (sección 1.4); sincronización del reloj por NTP (`S-02`). |
| `RG-17` | Las cuentas de los servicios (Oracle, bróker, dominio) quedan a nombre de un estudiante y se pierde el acceso al terminar el trabajo de grado. | Media | Alto | Crear las cuentas con un correo institucional o del proyecto y planear su entrega en el cierre (`PC-06`, `PC-07`). |

---

## 9. Pendientes de contexto

Temas que no están resueltos en ninguno de los dos documentos y que afectan el alcance o la planeación. Los puntos abiertos de detalle funcional están en la sección 7 de la especificación (`PA-01` a `PA-55`).

| ID | Tema pendiente | A quién consultar |
|----|----------------|-------------------|
| `PC-01` | Confirmar las responsabilidades de cada programa (Ambiental, Electrónica, Sistemas) y el interlocutor de cada uno. | Investigador principal |
| `PC-02` | **Contrato de integración.** **Parcialmente resuelto:** primera fase con 5 estaciones WiFi (LoRa en fases futuras); el mensaje JSON lleva el código de la estación, la marca de tiempo, el identificador de cada sensor y los valores de sus variables; la marca de tiempo la pone la estación con un reloj de hardware y la plataforma la valida, corrige y alerta; las unidades actuales están confirmadas; se acepta la pérdida de datos durante una desconexión; hueco "sin datos" con 2 envíos perdidos (más de 60 s) y desconexión si siguen sin llegar mensajes (sección 1.4). **Falta:** confirmar el formato de la marca de tiempo (propuesto ISO 8601 en UTC), el tratamiento de marcas ilegibles o imposibles y la alerta por episodio (propuestos en 1.4); fijar el tiempo exacto de desconexión (propuesto 5 minutos) y medirlo con la hora de llegada; el dispositivo de reloj y cómo se pone en hora; registrar las unidades confirmadas en la especificación (`PA-04`); el JSON de ejemplo, la estructura de tópicos y el nivel de QoS; cómo se reporta un sensor con falla dentro de un mensaje (`null` o código de error en lugar de `-1`, `PA-32`); los datos que genera la API, que se definirán al validar la estructura de datos de la plataforma. Relacionado con `PA-09`, `PA-31`, `PA-32`, `PA-34`, `PA-49` y `PA-50`. | Ingeniería Electrónica |
| `PC-03` | **Volumen.** **Resuelto:** 5 estaciones en la primera fase, un mensaje cada 30 segundos por estación, con las lecturas de todos sus sensores (sección 7.2). **Por confirmar:** el número estimado de estaciones de fases posteriores, para la estimación de costos y de capacidad, y que todos los sensores vayan en el mismo mensaje como regla del contrato. | Ingeniería Electrónica e investigador principal |
| `PC-04` | **ICA.** **Resuelto (fuera del alcance):** el Índice de Calidad del Aire no forma parte de la plataforma, del proyecto ni de las estaciones. Los colores del mapa se basan en umbrales de las variables ambientales (`PC-22`). | — |
| `PC-05` | **Acceso de la ciudadanía.** **Resuelto:** cualquier persona consulta y descarga los datos sin registrarse; el inicio de sesión es solo para administradores y la comunicación con la ciudadanía se hace por un correo de contacto. | — |
| `PC-06` | **Operación después del trabajo de grado.** **Parcialmente resuelto:** los estudiantes no estarán vinculados durante los 24 meses; el proyecto entrega un documento de operación (costos, estrategia, riesgos y su mitigación). Falta definir quién administra la plataforma, paga los servicios y atiende fallas durante el resto de ese periodo, y a quién se entregan las cuentas de los servicios. | Director e investigador principal |
| `PC-07` | **Infraestructura y presupuesto.** **Parcialmente resuelto:** la Universidad no provee servidores; la plataforma se aloja en máquinas virtuales de Oracle Cloud (Always Free); el bróker MQTT será un servicio administrado (proveedor por definir); el dominio pertenece a Juan Camilo (docente), con quien se trata todo lo relacionado; el equipo define el presupuesto y su aprobación y pago se acordarán con el investigador. **Falta:** a nombre de quién se crean las cuentas de los servicios, quién lo decide y para cuándo (se recomienda un correo institucional o del proyecto); quién agenda la reunión con Juan Camilo y para cuándo (¿subdominio?, ¿quién configura los registros DNS hacia Oracle?, ¿costo o requisitos de seguridad como HTTPS?); verificar que los límites del plan Always Free alcanzan (`RG-15`); comparar el costo de los brókeres administrados con unos 432.000 mensajes al mes, y el bróker propio como alternativa; el mecanismo de credenciales del bróker (afecta V-28); las demás partidas del presupuesto (dominio, correo, copias de seguridad); desde cuándo cuentan los 24 meses. | Investigador principal, Juan Camilo |
| `PC-08` | **Microplásticos y variables.** **Resuelto:** la plataforma no maneja datos de microplásticos. La estación envía todas las variables de sus sensores (material particulado, temperatura y humedad, y gases del Winsen ZPHS01B), y la plataforma debe soportar sensores con otras variables. **Por confirmar:** ¿los gases reciben el mismo tratamiento que las demás variables (se muestran al público, tienen umbrales que colorean el mapa y se pueden descargar), o el foco es el material particulado y los gases solo se almacenan? | Investigador principal e Ingeniería Ambiental |
| `PC-09` | **Validación de datos (en espera del documento de analítica):** alcance de la validación para la calibración mediante modelos; si se guardan todos los datos capturados o hay datos que no se deben guardar (por ejemplo, valores fuera de rango); cómo se aplican los modelos a los datos (`PA-53`). La pregunta sobre qué datos guardar no depende de la analítica y se podría responder antes. | Ingeniería Ambiental y Electrónica |
| `PC-10` | **Fuentes de referencia.** **Parcialmente resuelto:** no se usan redes ni plataformas externas (por ejemplo, SIATA); toda comparación se hace entre estaciones registradas. Dos usos: calibración (un administrador construye una estación casera, la registra y usa sus datos junto con los de otra estación) y comparación entre zonas. **Falta:** qué hace más confiable a la estación de referencia (si usa sensores de bajo costo, el resultado es una intercalibración y debe documentarse así); si se puede marcar una estación como "de referencia" para ocultarla del público o distinguirla; el periodo de co-ubicación (`PC-19`); cómo se alinean en el tiempo las lecturas de las dos estaciones (propuesto: promedios por minuto); si la estación casera envía por MQTT con el mismo formato o se cargan sus datos con un archivo. Relacionado con `PA-55`. | Investigador principal e Ingeniería Ambiental |
| `PC-11` | **Metodología y entregables.** **Resuelto:** Scrum; solo se esperan los entregables técnicos que define la metodología, sin otros artefactos ni formatos especiales. | — |
| `PC-12` | Cronograma: fechas de inicio y fin del trabajo de grado, hitos del macroproyecto y fecha esperada de las primeras estaciones reales. | Director e investigador principal |
| `PC-13` | **Reportes.** **Resuelto:** no se generan informes de ningún tipo (ni de datos ambientales ni de las acciones en la plataforma); basta con los dashboards y la descarga de datos. Se mantienen las alertas *Informative* de la especificación (sección 2.6), incluidas las que registran las acciones de los administradores. | — |
| `PC-14` | **Titularidad y licencia de los datos.** **Parcialmente resuelto:** el titular de los datos es la Universidad; los estudiantes y los investigadores consultarán al área legal la titularidad, la licencia y la cita en los archivos descargados. Durante el desarrollo solo se incluye una cita del macroproyecto. **Falta:** el texto de esa cita, y resolver la consulta legal **antes del despliegue** (registrarla en el cronograma con fecha límite). | Investigador principal / área legal de la UCO |
| `PC-15` | **Retención de datos (sin respuesta):** ¿los datos se conservan indefinidamente, o solo durante los 24 meses? ¿Qué pasa con la plataforma y los datos al terminar ese periodo? Con el almacenamiento limitado del plan Always Free y unos 230.000 valores al día (sección 7.2), la respuesta es necesaria para decidir si se agregan o se depuran datos antiguos. | Investigador principal |
| `PC-16` | **Distribución del trabajo.** **Resuelto:** los dos estudiantes presentan la plataforma como un único trabajo de grado conjunto. Las actividades y los entregables se reparten en Azure DevOps según la disponibilidad semanal de cada integrante. | — |
| `PC-17` | **Requisitos de la analítica de datos (en espera):** los investigadores entregarán un documento con el contexto y los requisitos de la analítica, para refinar el objetivo 4. Se necesita: qué se espera conseguir de forma precisa o exacta; métricas y valores de aceptación; variables prioritarias para calibrar; casos de estimación de variables; modelos a comparar además de RLM y ANN. **Falta saber quién entrega el documento y en qué fecha.** | Investigador principal e Ingeniería Ambiental |
| `PC-18` | **Uso de los modelos (en espera de `PC-17`):** ¿quién puede usar la analítica (solo administradores o también el público)? ¿Los modelos se entrenan en la plataforma o fuera de ella? ¿Se aplican a los datos dentro de la plataforma o solo se exportan, y para qué (análisis externo o ejecución en la estación)? ¿En qué formato se exportan? Relacionado con `PA-52` a `PA-54`. | Investigador principal |
| `PC-19` | **Co-ubicación.** **Parcialmente resuelto:** durante el desarrollo se usarán datos simulados para probar el funcionamiento de la plataforma, incluida la analítica. **En espera de `PC-17` y `PC-10`:** qué estación sirve de referencia, dónde se co-ubican las estaciones y durante cuánto tiempo. | Ingeniería Ambiental y Electrónica |
| `PC-20` | **Sensores futuros.** **Resuelto:** aún no se sabe qué sensores se agregarán; la plataforma debe permitir registrar sensores nuevos con sus variables y unidades (catálogo ampliable, V-25 y V-26), y los entregables finales deben estimar cómo varían los costos según la información adicional que se agregue (ver 7.2). | — |
| `PC-21` | **Azure DevOps.** **Parcialmente resuelto:** el Product Owner es único (el director del proyecto, confirmado con los investigadores); la estructura se reorganiza con una épica por objetivo específico y Features que representan funcionalidades validables de la plataforma (por ejemplo, inicio de sesión o administración de estaciones), cada una con escenarios de validación que aprueba el Product Owner; el código vive en Azure Repos, en un repositorio privado. **Falta:** validar esta estructura y los entregables con el director. | Director y estudiantes |
| `PC-22` | **Colores del mapa.** **Parcialmente resuelto:** el color de cada estación corresponde al nivel de alerta de los umbrales de sus variables ambientales, no a su estado de funcionamiento; se muestran las variables en alerta; una estación sin datos o desconectada aparece en gris y opaca. Los umbrales, niveles y colores se definirán con el investigador principal y el investigador de Ingeniería Ambiental. **Falta:** si se evalúa la última lectura o un promedio (por ejemplo, horario o de 24 h), para evitar cambios de color por picos momentáneos; si la estación toma el color del nivel más alto entre sus variables; qué color tiene una estación sin variables en alerta (por ejemplo, verde) y mientras no haya umbrales definidos; si los umbrales son globales o por estación (`PA-24`); confirmar que las anomalías no cambian el color en el mapa público; si una estación con un hueco corto mantiene su color y solo se pone gris al considerarse desconectada; cómo se muestra una estación con un sensor sin datos y los demás funcionando. Afecta V-02 y `RF-11` de la especificación. | Investigador principal e Ingeniería Ambiental |

### 9.1 Puntos abiertos de la especificación que afectan el alcance

De los puntos abiertos de [plataforma_web_descripcion.md](plataforma_web_descripcion.md), estos son los que tienen mayor impacto en la planeación y conviene priorizar en la próxima reunión:

- `PA-04`: registrar las unidades confirmadas de las variables (depende de `PC-02`).
- `PA-09`, `PA-49` y `PA-50`: procedimiento para que una estación empiece a enviar datos, modelo del mensaje MQTT y significado de los parámetros de conexión con MQTT (dependen de `PC-02` y del bróker elegido en `PC-07`). `PA-51` (estaciones LoRa) queda para fases futuras.
- `PA-52`, `PA-53` y `PA-54`: requisitos de la analítica, aplicación de los modelos a los datos y formato de exportación (dependen de `PC-17` y `PC-18`).
- `PA-55`: fuentes de referencia; ya no incluye fuentes externas (`PC-10`).
- `PA-24`: si los umbrales son globales o por estación (afecta el modelo de datos, la configuración y los colores del mapa, `PC-22`).
- `PA-31`, `PA-32`, `PA-33` y `PA-34`: reglas de cálculo de estados, tiempo de desconexión, valores centinela, anomalías y alertas repetidas (afectan la validación de datos del objetivo 2).
- `PA-35` y `PA-36`: tratamiento de datos de estaciones deshabilitadas, ocultas o eliminadas.
- `PA-45`: servicio de geocodificación y mapas (afecta el costo).
- `PA-46`: límites de las descargas públicas (afecta el costo y el rendimiento).
- `PA-48`: gestión de administradores (creación del primero, comunicación de la contraseña).

---

## 10. Sugerencias

1. **Congelar una versión mínima del producto.** La especificación tiene 27 vistas, incluidas las de analítica. Conviene acordar con el investigador principal qué es imprescindible para los productos del macroproyecto (recepción por MQTT, consulta pública con el mapa por umbrales, exportación y calibración con RLM y ANN) y qué es deseable (estimación de variables, versiones del formato del mensaje, eliminación de cuenta), para proteger el cronograma.
2. **Cerrar el contrato de integración MQTT cuanto antes** con Ingeniería Electrónica: un mensaje JSON de ejemplo, los tópicos, el QoS, el formato de la marca de tiempo y el tiempo de desconexión, incluso antes de terminar los mockups. Es la pieza que más depende de terceros y la que permite construir el simulador.
3. **Fijar quién entrega el documento de analítica y en qué fecha** (`PC-17`), y **resolver la retención de datos** (`PC-15`) en la próxima reunión, porque condicionan el objetivo 4 y el almacenamiento en Oracle.
4. **Definir la estación de referencia y el periodo de co-ubicación** (`PC-10`, `PC-19`) desde el inicio: sin una referencia adecuada no se puede calibrar.
5. **Hacer la proyección de costos a 24 meses antes de elegir el stack y el bróker**, con el volumen de la sección 7.2 (unos 432.000 mensajes al mes), los límites del plan Always Free y el costo del bróker después de su capa gratuita, y documentarla como parte del plan de gestión de recursos.
6. **Crear las cuentas de los servicios con un correo institucional o del proyecto** y agendar la reunión con Juan Camilo sobre el dominio (`PC-07`).
7. **Registrar la consulta al área legal en el cronograma**, con fecha límite anterior al despliegue (`PC-14`).
8. **Llevar actas de cada reunión con los investigadores**, con las decisiones y los `PA`/`PC` resueltos, como evidencia del levantamiento de requerimientos para el trabajo de grado.
9. **Mantener la trazabilidad** objetivo → requisito (`RF`) → caso de prueba, para demostrar en la sustentación el cumplimiento de cada objetivo.
10. **Preparar con evidencia la respuesta a "¿por qué construir una interfaz?"** (sección 1.3) para la sustentación: lenguaje no técnico, acceso libre y pruebas de usabilidad con personas sin formación técnica.

---

## 11. Glosario

| Término | Definición |
|---------|------------|
| Estación / nodo | Equipo instalado en un punto de Rionegro que integra uno o varios sensores y envía sus lecturas a la plataforma. |
| Estación de enlace | Estación con WiFi que recibe por LoRa los datos de otras estaciones y los publica en el bróker MQTT (fases futuras). |
| Estación de referencia | Estación registrada en la plataforma (por ejemplo, una estación casera construida por un administrador) cuyos datos se usan para calibrar otra estación co-ubicada. |
| Sensor | Dispositivo que mide una o varias variables ambientales (inicialmente Winsen ZPHS01B, Honeywell HPMA115S0-XXX y AM2305B). |
| Variable | Magnitud medida por un sensor (por ejemplo, PM2.5, CO₂, temperatura), con su unidad. |
| Lectura | Conjunto de valores enviados por una estación en un mensaje (uno cada 30 segundos). |
| Marca de tiempo | Fecha y hora de una lectura, que pone la estación con su reloj de hardware y la plataforma valida. |
| Hueco "sin datos" | Periodo en el que no llegan mensajes de una estación durante 2 envíos seguidos (más de 60 s); se muestra sin interpolar. |
| Desconexión | Estado de una estación de la que siguen sin llegar mensajes después de un hueco, durante el tiempo que se defina (`PC-02`). |
| ICA | Índice de Calidad del Aire: valor adimensional que resume la calidad del aire a partir de las concentraciones de contaminantes. Fuera del alcance del proyecto (`PC-04`). |
| MQTT | Protocolo de mensajería ligero basado en publicación y suscripción, usado por las estaciones para enviar sus datos. |
| Bróker MQTT | Servidor que recibe los mensajes publicados por las estaciones y los entrega a la plataforma, que está suscrita a ellos. |
| Tópico | Canal del bróker MQTT en el que publica cada estación. |
| QoS | Nivel de calidad de servicio de MQTT, que define la garantía de entrega de cada mensaje. |
| Credenciales de estación | Usuario y contraseña, token o certificado con los que una estación se autentica ante el bróker MQTT. |
| LoRa | Tecnología de comunicación inalámbrica de largo alcance y bajo consumo, que algunas estaciones usarán en fases futuras para enviar sus datos a una estación de enlace. |
| Contrato de integración | Acuerdo técnico (protocolo MQTT, tópicos, formato del mensaje, marca de tiempo, autenticación, errores) con el que las estaciones, reales o simuladas, envían datos a la plataforma. No es único: el administrador lo puede configurar para cada estación. |
| Simulador | Programa que imita el envío de datos de las estaciones por el contrato de integración, para desarrollar y probar sin hardware. |
| Umbral | Valor de una variable a partir del cual se genera una alerta y cambia el color de la estación en el mapa. |
| Calibración | Proceso de ajustar las lecturas de un sensor de bajo costo para que se acerquen a las de una referencia más confiable. |
| Intercalibración | Ajuste entre sensores de precisión similar; no equivale a una calibración frente a un equipo de referencia. |
| Co-ubicación | Instalación de una estación junto a una estación de referencia durante un periodo, para comparar sus mediciones y calibrarla. |
| Estimación de variables | Cálculo de una variable a partir de otras variables correlacionadas (por ejemplo, temperatura a partir de humedad). |
| RLM | Regresión lineal múltiple: modelo estadístico que relaciona una variable con varias variables de entrada mediante una ecuación lineal. |
| ANN | Red neuronal artificial: modelo de aprendizaje automático capaz de representar relaciones no lineales entre variables. |
| Costo computacional | Recursos que exige un modelo: tiempo de entrenamiento, tiempo de cálculo de cada predicción y tamaño del modelo. |
| Oracle Cloud Always Free | Plan gratuito de Oracle Cloud, con límites de procesador, memoria, disco y transferencia, en el que se alojará la plataforma. |
| Visitante | Persona que usa la plataforma sin iniciar sesión. |
| Stakeholder / interesado | Persona o grupo con interés en el proyecto o afectado por él. |

---

## 12. Control de cambios

Los cambios posteriores a la versión 2 se registran en [gestion_cambios.md](gestion_cambios.md). Esta tabla conserva el historial de las versiones anteriores.

| Versión | Fecha | Cambios |
|---------|-------|---------|
| 1 | — | Contexto inicial, objetivos, alcance general y restricción de presupuesto. |
| 2 | 2026-09-25 | Corrección de redacción; participación de cada programa; problema; trazabilidad de objetivos; alcance detallado (incluye, no incluye, supuestos y restricciones); interesados; ciclo de vida y entregables; criterios de éxito; costos y stack; riesgos preliminares; pendientes de contexto (`PC-01` a `PC-16`); sugerencias y glosario. |
