
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
| Ingeniería Ambiental | Interpretación de las variables, umbrales de calidad del aire, mediciones de microplásticos y análisis de resultados. | Define, con el investigador principal, los umbrales, niveles y colores de alerta de las variables, y los criterios de anomalía; interpreta los resultados de la calibración. Es el **principal usuario investigador**. Las mediciones de microplásticos no pasan por la plataforma. |
| Ingeniería de Sistemas | Plataforma web, contrato de integración por MQTT y módulo de calibración de los sensores (Winsen ZPHS01B, Honeywell HPMA115S0-XXX, AM2305B y los que se agreguen) mediante modelos de aprendizaje automático (este proyecto). | Recibe, valida, almacena, visualiza y exporta los datos; entrena y compara modelos de calibración y permite descargar el modelo elegido. La estimación de variables está por confirmar (`PC-18`). |

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

La estación envía **todas las variables que capturan sus sensores**. Las unidades de las variables actuales están confirmadas y validadas en la sección 2.3 de la especificación (`PA-04`).

**Frecuencia de envío:** cada estación envía **un mensaje cada 30 segundos**, con las lecturas de todos sus sensores.

**Contenido de cada mensaje (JSON):**

- código de la estación;
- marca de tiempo de la lectura;
- identificador de cada sensor;
- por cada sensor, los valores de las variables que mide (por ejemplo, el sensor de material particulado envía PM10, PM2.5 y PM1).

**No hay un formato único:** cada estación puede tener su propio formato JSON, incluidas las estaciones caseras que construyan los administradores, y el administrador configura el formato de cada estación desde la plataforma (V-27). La estructura concreta del JSON, el mensaje de ejemplo, la estructura de tópicos y el nivel de QoS aún se deben definir con Ingeniería Electrónica (`PC-02`, `PA-50`).

**Sensores con falla:** los códigos de error de los sensores aún se deben definir con una **matriz de valores y umbrales** por sensor que permita identificar y marcar los sensores con fallas (`PA-32`, `PA-33`).

**Marca de tiempo:** la pone la estación con un reloj de hardware (dispositivo por definir). La plataforma la valida así:

- los formatos que se pueden manejar dependen del reloj que se use en las estaciones y se definirán cuando se conozca ese dispositivo; mientras tanto se toma como referencia ISO 8601 en UTC (por ejemplo, `2026-10-26T14:30:00Z`) **(propuesto)**;
- un formato distinto pero legible se convierte al formato esperado;
- una marca que no se puede interpretar, o que está desajustada (en el futuro o muy atrasada), se reemplaza por la **hora de recepción** del servidor y la lectura se marca como **corregida**; la diferencia tolerada antes de considerarla desajustada está por definir (`PC-02`);
- se genera **una sola alerta por episodio**, que se mantiene hasta que la estación vuelva a enviar marcas válidas (`PA-34`).

**Pérdida de conexión:** si una estación pierde la conexión a internet, no envía datos al bróker. Los datos de ese periodo no se guardan en la estación ni se reenvían: **se acepta esa pérdida**. Los huecos y la desconexión los mide el **servidor** con la **hora de llegada** de los mensajes, no con la marca de tiempo de la estación, porque esa marca puede estar mal.

- **Hueco "sin datos":** se pierden 2 envíos seguidos (más de 60 s sin mensajes). La plataforma muestra ese periodo como "sin datos", sin interpolarlo.
- **Desconexión:** al superar el **tiempo máximo sin mensajes** (por defecto, **10 minutos**; configurable desde la plataforma), la estación se considera desconectada y pasa al estado *Sin datos*; ese mismo criterio la pone en gris en el mapa y genera la alerta *Tiempo sin conexión* (`RN-34`). Las estaciones recién creadas o recién habilitadas también quedan en *Sin datos* hasta que llega su primer mensaje.
- **Estaciones deshabilitadas, ocultas o eliminadas:** la plataforma deja de escuchar sus mensajes en el bróker, por lo que no recibe ni almacena sus datos ni genera alertas sobre ellas (`RN-38`).
- **Conexión de la plataforma con el bróker:** la plataforma detecta cuando su API se desconecta o se desincroniza del bróker y genera una alerta (`PA-49`).

### 1.5 Funcionamiento definido del módulo de calibración

Los requisitos de la analítica los entregaron los investigadores y están en [requerimientos_modulo_calibracion.md](requerimientos_modulo_calibracion.md); sus dudas se resolvieron en la [Reunión 26/10](<reuniones/Reunión 261026.md>) (`PCAL-xx`).

- **Usuarios:** solo administradores (`PCAL-02`).
- **Datos:** toda comparación se hace entre estaciones registradas en la plataforma; no se usan redes externas (`PC-10`). No se marca ninguna estación como "de referencia": en cada calibración el administrador elige cualquier estación registrada. Las estaciones **no tienen que estar en el mismo lugar**, y se puede calibrar y validar con 2 o más estaciones. Las lecturas se emparejan con **promedios por minuto**.
- **Ejecución:** el administrador elige la variable a calibrar, la variable de referencia, las variables auxiliares y uno o varios modelos, y ejecuta la calibración cuando lo decide (`PCAL-05`). Los modelos se entrenan en la plataforma.
- **Modelos:** regresión lineal, redes neuronales artificiales, Random Forest, SVM, AdaBoost y regresión polinomial; ElasticNet está por revisar (`PCAL-08`). Los modelos mínimos de la primera versión están por definir (`PCAL-09`).
- **Resultados:** una gráfica por modelo con los datos crudos y calibrados frente a la referencia, la línea g(x) = x y el **R² de los datos calibrados** (`PCAL-10`, `PCAL-12`). No se fija un valor de aceptación (`PCAL-11`) ni se guarda historial de calibraciones (`PCAL-13`).
- **Uso del modelo:** los modelos **no se aplican** a los datos de la plataforma: el público ve los datos crudos, y los calibrados solo sirven para comparar y elegir modelos (`PCAL-03`). El administrador **descarga** el modelo elegido para luego calibrar las estaciones (`PCAL-01`), en formato `.keras` (`PCAL-14`; ese formato solo sirve para redes neuronales, ver `PC-18`).

> **Nota de riesgo — calidad de la referencia:** el equipo de desarrollo no puede validar ni responder por la calidad de los datos que se usen como referencia. El requisito lo planteó el investigador, que decide con qué datos y cómo valida, calibra, compara y ejecuta el análisis. Quedan documentadas dos limitaciones: si la estación de referencia tiene sensores de bajo costo, el resultado es una intercalibración y no una calibración frente a un equipo más confiable; y si las estaciones no están en el mismo lugar, las diferencias pueden deberse a la ubicación y no al error del sensor (`RG-11`).

---

## 2. Objetivos

### 2.1 Objetivo general

Desarrollar una plataforma web y una metodología de calibración y validación en campo, aplicando el ciclo de vida del software, que reciba, valide, almacene, compare, calibre y visualice la información generada por la red de sensores de bajo costo del macroproyecto **IDI-FR-05**, como producto de los objetivos específicos número 2 y 3, y como aporte al proceso investigativo.

### 2.2 Objetivos específicos

1. Definir las etapas del ciclo de vida del software que se seguirán durante el desarrollo de la plataforma y los artefactos que se entregarán en cada una, de modo que sustenten el proceso investigativo, formativo y práctico.
2. Diseñar e implementar la integración por MQTT, con un contrato de integración configurable por estación desde la plataforma, que permita recibir, validar y almacenar los datos enviados por las estaciones de la primera fase (5 estaciones con WiFi, con un mensaje cada 30 segundos) y por las que se agreguen después, y que funcione igual para datos simulados y reales.
3. Desplegar la plataforma, con la visualización y exportación de las series históricas de las variables medidas con acceso libre para investigadores y ciudadanía, y con una estrategia de operación, seguridad y respaldo que permita mantenerla en funcionamiento durante 24 meses dentro de un presupuesto de $2.000.000 COP.
4. Definir y aplicar una metodología de calibración de los sensores mediante modelos de aprendizaje automático, integrada en la plataforma como un módulo para administradores, que entrene y compare modelos de regresión lineal múltiple, redes neuronales artificiales (ANN) y otros modelos de regresión con los datos de dos o más estaciones registradas, presente para cada modelo una gráfica comparativa con su R² y permita descargar el modelo elegido.

**Sugerencias sobre la redacción de los objetivos:**

- Los estudiantes no estarán vinculados durante los 24 meses de operación; por eso el cumplimiento del objetivo 3 se demuestra con un documento de operación (costos, estrategia, riesgos y su mitigación) y no con la operación misma (ver sección 6 y `PC-06`).
- El objetivo 4 se redactó con el documento de analítica y la reunión del 26/10 (sección 1.5). Ya no promete "validación en campo (co-ubicación)" ni elegir automáticamente el mejor modelo, porque las estaciones no tienen que estar juntas, no se fija un valor de aceptación y la elección la hace el administrador. Como el objetivo específico 2 del macroproyecto sí menciona la co-ubicación, conviene que el investigador confirme esta diferencia (`PC-10`).
- El objetivo anterior incluía la estimación de variables a partir de variables correlacionadas y el costo computacional como criterio de selección. Ninguno de los dos aparece en el documento de analítica ni en la reunión del 26/10; si se confirman (`PC-18`), se vuelven a agregar.
- Cada objetivo debería tener al menos un indicador o entregable verificable (ver sección 6).

### 2.3 Trazabilidad de los objetivos con la especificación funcional

| Objetivo específico | Dónde se aborda | Estado |
|---------------------|-----------------|--------|
| 1. Etapas y artefactos | Sección 5 de este documento. Metodología Scrum (`PC-11`). | Parcial: falta validar los artefactos con el director. |
| 2. Contrato de integración | Sección 1.4 de este documento. Especificación: credenciales por estación (V-14, V-28), formato del mensaje configurable (V-27), sensores por estación (V-25, V-26), `RN-28` a `RN-30`. | Parcial: contenido del mensaje, frecuencia, formato por estación, validación de la marca de tiempo, desconexión (10 minutos) y unidades definidos; falta la estructura concreta del JSON, los tópicos, el QoS, los formatos de marca de tiempo admitidos y la matriz de valores y umbrales para los códigos de error (`PC-02`). La especificación ya incluye los formatos por estación (V-27, `RN-29`), la validación de la marca de tiempo (`RN-33`) y los huecos y la desconexión (`RN-34`). LoRa queda para fases futuras (`PA-51`). |
| 3. Series históricas y exportación | V-02, V-03, V-04 y V-05 de la especificación. | Cubierto. La exportación tiene *captcha*, retención máxima de 12 horas y límites por IP (`RN-40`), y no incluye los datos de estaciones ocultas o eliminadas (`RN-39`); faltan los valores de los límites (`PA-46`). El color del mapa ya depende de los umbrales de las variables en la especificación (V-02, `RF-11`, `RN-37`); faltan las reglas de detalle (`PC-22`, `PA-56`). |
| 3. Acceso para la ciudadanía | Consulta y descarga públicas, sin registro (`RF-G07`, `RN-27`, `RNF-03`). | Cubierto. |
| 3. Despliegue y operación durante 24 meses | Secciones 7 y 8 de este documento. | Parcial: Oracle Cloud Always Free y bróker administrado (`PC-07`); falta el responsable de la operación (`PC-06`). |
| 4. Calibración y analítica de datos | Sección 1.5 de este documento y [requerimientos_modulo_calibracion.md](requerimientos_modulo_calibracion.md). Especificación: sección 2.8, V-30, V-31 (por confirmar), `C-11`, `RN-31`, `RN-32`, `RN-35` y `RN-36`. | Parcial: requisitos recibidos y aclarados (`PC-17`, `PC-18`) e incorporados a la especificación (se retiran V-29 y V-32); faltan los modelos mínimos, el formato de exportación de los modelos que no son redes neuronales y la decisión sobre la estimación de variables (`PA-52`, `PA-54`). |

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
- Validación de los datos recibidos: estructura del mensaje según el formato de su estación, estación registrada, sensores y variables asignados, marca de tiempo (conversión de formatos legibles; hora de recepción y marca de *corregida* si no se puede interpretar o está desajustada; una alerta por episodio), rangos físicos y códigos de error de los sensores (según la matriz de valores y umbrales que se defina).
- Detección de huecos "sin datos" (más de 60 s sin mensajes) y de estaciones desconectadas (tiempo máximo sin mensajes, 10 minutos por defecto, que deja la estación en *Sin datos*), medidos con la hora de llegada al servidor y sin interpolar los datos faltantes, y detección de la desconexión o desincronización entre la API y el bróker.
- Suspensión de la escucha en el bróker de las estaciones deshabilitadas, ocultas o eliminadas.
- Almacenamiento de las lecturas y de su historial durante el periodo de operación.
- Un **simulador de estaciones** que publique datos en el bróker con el mismo contrato de integración, para desarrollar y probar la plataforma sin depender de las estaciones físicas.

**Funcionalidades de la plataforma** (detalladas en la especificación funcional)

- Consulta pública, sin registro: mapa de estaciones coloreadas según el nivel de alerta de los umbrales de sus variables (en gris y opacas si están sin datos o desconectadas), con las variables en alerta; listado y detalle de estaciones, gráficos históricos, comparación entre sensores y tablas de datos.
- Generación de archivos de descarga en CSV y JSON, con la cita del macroproyecto, *captcha*, límites de solicitudes y descargas por IP (visitantes y administradores) y sin historial: cada archivo se conserva un máximo de 12 horas y solo el último de cada persona. Los datos de las estaciones ocultas o eliminadas no se grafican ni se descargan.
- Inicio de sesión solo para administradores (correo y contraseña); gestión de administradores por otro administrador (registrar con nombre y correo, listar, editar y eliminar), con activación de la cuenta mediante un enlace enviado al correo, en el que el nuevo administrador define su contraseña; recuperación y cambio de contraseña, perfil y eliminación de la propia cuenta.
- Gestión de estaciones: crear, editar, habilitar/deshabilitar, ocultar/mostrar y eliminar.
- Catálogo de sensores ampliable, con las variables que mide cada sensor y sus unidades, y asignación de sensores a cada estación.
- Configuración del formato del mensaje MQTT de cada estación y regeneración de las credenciales de conexión de las estaciones con el bróker.
- Cálculo automático del estado de estaciones (incluido el estado *Sin datos*) y sensores.
- Alertas por umbrales, anomalías, problemas de conexión, marcas de tiempo inválidas y acciones de los administradores (alertas *Informative* con el usuario que hizo la acción).
- Configuración de umbrales, intervalo de actualización y tiempo máximo sin mensajes de las estaciones.
- Correo de contacto en el pie de página para la comunicación con la ciudadanía.

**Módulo de calibración** (solo administradores; sección 1.5)

- Registro de estaciones caseras construidas por los administradores como cualquier otra estación, con su propio formato de mensaje.
- Selección de datos de 2 o más estaciones registradas, sin marcar ninguna como referencia ni exigir que estén en el mismo lugar, y emparejamiento de sus lecturas con promedios por minuto.
- Selección de la variable a calibrar, la variable de referencia, las variables auxiliares y uno o varios modelos.
- Entrenamiento en la plataforma de los modelos elegidos: regresión lineal, redes neuronales artificiales, Random Forest, SVM, AdaBoost y regresión polinomial (ElasticNet por revisar; modelos mínimos por definir, `PCAL-09`).
- Una gráfica comparativa por modelo, con los datos crudos y calibrados frente a la referencia, la línea g(x) = x y el R² de los datos calibrados.
- Descarga del modelo elegido (`.keras` para redes neuronales; formato de los demás modelos por definir, `PC-18`).
- Comparación de estaciones de zonas distintas.
- Estimación de variables a partir de variables correlacionadas: **por confirmar** (`PC-18`).

**Calidad, despliegue y cierre**

- Atributos de calidad que guían el diseño, el desarrollo y las pruebas: disponibilidad, rendimiento, usabilidad, seguridad, accesibilidad, compatibilidad, mantenibilidad y escalabilidad.
- Elementos de seguridad que la plataforma debe cubrir, sin suponer todavía qué servicios los proveerán: seguridad del software (protección mediante firewall, escaneo y eliminación de malware, actualizaciones periódicas y análisis de tráfico anti-DDoS), certificado SSL, copias de seguridad y restauraciones, protección DDoS, supervisión de redes, soporte CDN y validación de formularios contra la inyección de consultas.
- Plan de pruebas y su ejecución: pruebas unitarias, de integración, funcionales (con el simulador) y de aceptación con los investigadores.
- Validación de los modelos con datos de prueba separados de los de entrenamiento por bloques de tiempo **(propuesto**, sección 6.4 de [requerimientos_modulo_calibracion.md](requerimientos_modulo_calibracion.md); por confirmar sobre qué datos se calcula el R², `PC-18`**)**.
- Despliegue en máquinas virtuales de Oracle Cloud (plan Always Free), accesible por internet con el dominio que se acuerde con su propietario (`PC-07`).
- Estrategia de respaldos y de monitoreo básico de la plataforma.
- Transferencia de conocimiento a quien operará la plataforma después del trabajo de grado, incluida la entrega de la cuenta del proyecto (monitoringiotsystemuco@gmail.com) con la que se crean los servicios.

### 3.2 No incluye

- Diseño, construcción, montaje, instalación o mantenimiento de las estaciones y de sus sensores, y su calibración física (de hardware). La calibración del proyecto se hace sobre los datos, mediante modelos.
- Desarrollo o modificación del firmware de las estaciones, incluido el reloj que pone la marca de tiempo. La plataforma define el contrato de integración por MQTT, pero su implementación en las estaciones corresponde a Ingeniería Electrónica.
- Estaciones con LoRa y la retransmisión de LoRa a WiFi, que quedan para fases futuras.
- Almacenamiento local y reenvío de los datos de una estación durante una desconexión; esa pérdida se acepta. Tampoco se interpolan los periodos sin datos.
- Conectividad de las estaciones (redes WiFi y LoRa, planes de datos, gateways, energía).
- Compra de hardware, incluidos los equipos de referencia.
- Análisis científico de los datos o conclusiones sobre la calidad del aire de Rionegro. El módulo se limita a entrenar y comparar modelos de calibración (y, si se confirma, a la estimación de variables).
- Validar la calidad de los datos que se usen como referencia o decidir con qué datos se calibra: lo decide el investigador (sección 1.5).
- Aplicar los modelos a los datos de la plataforma, mostrar o descargar datos calibrados y guardar el historial de calibraciones o los modelos entrenados (`PCAL-01`, `PCAL-03`, `PCAL-13`).
- El uso del modelo descargado fuera de la plataforma (por ejemplo, cargarlo en las estaciones), que corresponde al administrador; falta aclarar en qué consiste (`PC-18`).
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
| `S-05` | El investigador principal y el investigador de Ingeniería Ambiental definirán los umbrales, niveles y colores de alerta de las variables y los criterios para considerar un dato como anómalo, y participarán en la matriz de valores y umbrales que define los códigos de error de los sensores (`PC-02`). |
| `S-06` | Los investigadores estarán disponibles para validar requerimientos, mockups y entregas parciales con una frecuencia acordada. |
| `S-07` | La primera fase tiene 5 estaciones, cada una con un mensaje cada 30 segundos que incluye las lecturas de todos sus sensores. El número de estaciones de fases posteriores será pequeño (del orden de unidades o pocas decenas; por confirmar, `PC-03`). |
| `S-08` | El presupuesto de $2.000.000 COP se destina a los costos de operación de la plataforma (infraestructura y servicios) y no incluye el trabajo de los estudiantes. |
| `S-09` | La capacidad gratuita del plan Always Free de Oracle Cloud alcanza para la aplicación y la base de datos con el volumen de la primera fase (por verificar, `PC-07`), y existe un bróker MQTT administrado compatible con el presupuesto. |
| `S-10` | La Universidad, titular de los datos, definirá con su área legal la licencia de publicación y la cita de los archivos descargados antes del despliegue, y aprobará los documentos legales (términos y condiciones, política de tratamiento de datos). |
| `S-11` | El investigador decide con qué estaciones y datos se calibra, valida y compara, y asume la responsabilidad sobre la calidad de la referencia; el equipo de desarrollo entrega la herramienta y documenta sus limitaciones (sección 1.5, `PC-10`). Habrá datos de al menos 2 estaciones registradas (reales, caseras o simuladas) para probar el módulo. |
| `S-12` | El entrenamiento de los modelos en la plataforma se puede ejecutar con los recursos de la máquina de Oracle o con otros recursos cubiertos por el presupuesto, porque solo se ejecuta cuando un administrador lo decide (`PCAL-05`) y con el volumen de datos de la primera fase. |
| `S-13` | Las estaciones caseras que construyan los administradores enviarán sus datos por MQTT y su formato de mensaje se podrá describir con la configuración por estación de la plataforma (V-27). |

### 3.4 Restricciones

| ID | Tipo | Restricción |
|----|------|-------------|
| `R-01` | Presupuesto | La plataforma debe funcionar durante 24 meses con un presupuesto total de $2.000.000 COP, es decir, en promedio unos **$83.000 COP al mes** para toda la operación (bróker MQTT, dominio, correo transaccional, almacenamiento de archivos, respaldos, cómputo para entrenar modelos y servicios externos, además de lo que exceda el plan gratuito del servidor). El equipo define el presupuesto; quién lo aprueba y quién hace los pagos se acordará con el investigador (`PC-07`). |
| `R-02` | Tiempo de operación | La plataforma debe mantenerse disponible durante 24 meses desde su puesta en producción (fecha de inicio por definir, `PC-07`). Como los estudiantes no estarán vinculados durante todo ese periodo, el proyecto entrega el plan de operación que lo hace posible, y la operación queda a cargo de quien se defina en `PC-06`. |
| `R-03` | Tiempo de desarrollo | El desarrollo debe ajustarse al calendario académico del trabajo de grado (fechas por definir, `PC-12`). |
| `R-04` | Equipo | El equipo de desarrollo son dos estudiantes de Ingeniería de Sistemas, con dedicación parcial como auxiliares de investigación, que presentan la plataforma como un trabajo de grado conjunto. |
| `R-05` | Alcance | Solo se atienden los objetivos específicos número 2 y 3 del macroproyecto, con las 5 estaciones con WiFi de la primera fase. |
| `R-06` | Dependencia | La integración con datos reales depende del avance de las estaciones (Ingeniería Electrónica) y de las definiciones de Ingeniería Ambiental; la calibración con datos reales depende además de que haya datos de al menos 2 estaciones registradas y de que se definan los modelos mínimos y el formato de exportación (`PC-18`). |
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
| Investigador principal del macroproyecto IDI-FR-05 | Dueño del producto; valida requerimientos y entregas; define con Ingeniería Ambiental los umbrales del mapa; decide con qué datos y cómo se calibra, valida y compara; acuerda la aprobación y el pago del presupuesto. | Por definir |
| Investigadores de Ingeniería Ambiental | Usuarios de los datos; definen umbrales, niveles de alerta y criterios de anomalía, e interpretan los resultados de la calibración. | Por definir |
| Investigadores de Ingeniería Electrónica | Responsables de las estaciones, del firmware, del reloj y del envío de datos por MQTT. | Por definir |
| Estudiantes de Ingeniería de Sistemas (2) | Desarrollo de la plataforma y de la metodología de calibración; definición del presupuesto y del nombre de la plataforma. | Por definir |
| Administradores de la plataforma | Operan la plataforma: estaciones, sensores, credenciales, formato del mensaje MQTT, alertas, configuración, módulo de calibración y cuentas de los demás administradores; pueden construir y registrar estaciones caseras y descargar los modelos que elijan. | Por definir |
| Propietario del dominio | Docente de la Universidad; define el proceso, los formatos, el subdominio y la configuración del dominio. Los estudiantes agendan la reunión con él. | Juan Camilo |
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
5. Módulo de calibración: selección de estaciones y variables, emparejamiento por minuto, entrenamiento y comparación de modelos (empezando por los mínimos que se definan en `PCAL-09`), gráficas con R² y descarga del modelo. La estimación de variables, solo si se confirma (`PC-18`).
6. Estados automáticos, alertas y configuración.
7. Funcionalidades complementarias (perfil, eliminación de cuenta, estaciones eliminadas).

---

## 6. Criterios de éxito (propuesto)

| Objetivo específico | Criterio verificable |
|---------------------|----------------------|
| 1 | Documento de etapas y artefactos aprobado por el director. |
| 2 | El simulador y al menos una estación real (si está disponible) publican un mensaje cada 30 segundos por MQTT, cada uno según el contrato configurado para su estación; la plataforma los almacena, rechaza los mensajes inválidos, guarda con la hora de recepción y marca como corregidas las lecturas con marca de tiempo ilegible o desajustada (una alerta por episodio), marca los huecos "sin datos" (más de 60 s), detecta las estaciones desconectadas (10 minutos sin mensajes por defecto) y las deja en *Sin datos*, deja de escuchar las estaciones deshabilitadas, y permite configurar el formato del mensaje de cada estación y regenerar sus credenciales. |
| 3 | La plataforma está desplegada en Oracle Cloud, con respaldos automáticos probados (restauración exitosa). Cualquier persona, sin iniciar sesión, puede consultar el mapa con los colores por umbrales y las series históricas y exportarlas en CSV y JSON; los casos de prueba asociados se ejecutan con éxito. Se entrega un documento de operación para 24 meses con la proyección de costos dentro de $2.000.000 COP (incluida la variación de costos si se agregan estaciones, sensores o variables), la estrategia de operación y mantenimiento, los riesgos y su mitigación, y la demás información necesaria para que la plataforma se pueda usar, mantener y conservar en funcionamiento durante ese periodo. |
| 4 | Para al menos una variable (por ejemplo, PM2.5), un administrador elige los datos de 2 o más estaciones registradas (simuladas o reales), entrena en la plataforma al menos los modelos mínimos que se definan (`PCAL-09`; como base, RLM y ANN), obtiene una gráfica por modelo con los datos crudos y calibrados, la línea g(x) = x y el R², y descarga el modelo elegido. La metodología queda documentada con sus limitaciones (sección 1.5). No se fija un valor de aceptación del R² (`PCAL-11`). |

---

## 7. Consideraciones de costos y stack tecnológico

### 7.1 Costos a cubrir con el presupuesto

| Concepto | Observación |
|----------|-------------|
| Servidor o plataforma de aplicación | Máquinas virtuales de Oracle Cloud en el plan Always Free: backend, frontend y tareas en segundo plano (generación de archivos, cálculo de estados, alertas y entrenamiento de modelos). Sin costo mientras se respeten los límites del plan; hay que verificar que alcanzan y documentar el riesgo de que Oracle recupere instancias que considere inactivas (`RG-15`). |
| Bróker MQTT | Servicio administrado (proveedor por definir, `PC-07`); debe soportar autenticación por estación y cifrado TLS. Probablemente con costo (por mensaje o por conexión; las capas gratuitas suelen durar 12 meses). La primera fase genera unos 432.000 mensajes al mes. Alternativa: bróker propio en la máquina de Oracle. |
| Base de datos | Es el recurso que más crece con el tiempo, porque depende del volumen de lecturas (ver 7.2); debe caber en el almacenamiento del plan Always Free o planear su crecimiento. |
| Almacenamiento de archivos | Archivos CSV/JSON generados de forma temporal (sin historial; máximo 12 horas y solo el último archivo de cada persona, `RN-40`) y archivos temporales de los modelos para su descarga (no se guardan modelos ni historial de calibraciones, `PCAL-13`). |
| Respaldos | Copias periódicas de la base de datos fuera del servidor principal. |
| Dominio y certificado SSL/TLS | El dominio pertenece a Juan Camilo (docente); el costo, el subdominio y la configuración DNS se acuerdan con él. Certificado para el sitio web y para el bróker MQTT. |
| Seguridad | Protección mediante firewall, escaneo y eliminación de malware, actualizaciones periódicas, protección DDoS y análisis de tráfico, supervisión de redes y soporte CDN. Los servicios que los cubran están por definir. |
| Correo | Enlaces de activación de cuenta y de recuperación de contraseña de los administradores (volumen muy bajo) y buzón del correo de contacto. |
| Servicios externos | Mapas y geocodificación (`PA-45`) y *captcha* para la generación de archivos de descarga (`PA-46`), generalmente gratuitos dentro de ciertos límites. |
| Reserva para imprevistos | Se recomienda reservar entre el 10 % y el 15 % del presupuesto. |

Las cuentas de todos los servicios (Oracle, bróker y demás) se crean con la cuenta de Gmail del proyecto, **monitoringiotsystemuco@gmail.com**, que se entrega como parte de la entrega de la plataforma (`PC-07`, `PC-06`). Los costos se definirán cuando se validen por completo los servicios necesarios.

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
- Bibliotecas de aprendizaje automático que permitan entrenar los modelos definidos (regresión lineal y polinomial, ANN, Random Forest, SVM, AdaBoost y, si se incluye, ElasticNet) con un costo computacional compatible con la máquina de Oracle, y exportarlos: `.keras` para las redes neuronales y un formato por definir para los demás (`PC-18`).
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
| `RG-10` | Los detalles pendientes del módulo de calibración (modelos mínimos, formato de exportación de los modelos que no son redes neuronales, estimación de variables) no se definen a tiempo. | Media | Medio | Construir primero la base común (selección de datos, emparejamiento por minuto, entrenamiento de RLM y ANN, gráfica con R²) y diseñar la lista de modelos para que se puedan agregar sin rediseñar el módulo (`PC-18`). |
| `RG-11` | Los resultados de la calibración no son confiables porque la referencia usa sensores de bajo costo (intercalibración) o porque las estaciones están en lugares distintos y las diferencias se deben a la ubicación. | Media | Alto | Riesgo que asume el investigador, que decide con qué datos y cómo se calibra (sección 1.5). El equipo deja la nota de riesgo en la documentación y muestra en el módulo la estación de origen de cada variable (`PC-10`). |
| `RG-12` | Abuso de las funciones públicas (descargas masivas o consultas excesivas) que afecte el rendimiento o el costo. | Media | Medio | *Captcha* para generar archivos, límites de solicitudes y de descargas por IP (también para administradores), retención máxima de 12 horas y solo el último archivo de cada persona (`RN-40`); falta definir los valores de los límites y si hay rango máximo de fechas o tamaño máximo de archivo (`PA-46`). |
| `RG-13` | Un cambio en el mensaje que envía el firmware, o un error al configurar el formato de una estación (cada estación puede tener el suyo), interrumpe la recepción de datos. | Media | Medio | Formato del mensaje configurable por estación desde la plataforma, con prueba previa con un mensaje de ejemplo y versiones (V-27). |
| `RG-14` | Se pierden los datos de una estación mientras está sin conexión a internet (pérdida aceptada). | Alta | Bajo | Aceptar el riesgo; mostrar los periodos "sin datos" sin interpolarlos, generar la alerta *Tiempo sin conexión* y documentar los huecos para la analítica (`PC-02`). |
| `RG-15` | El plan Always Free de Oracle no alcanza para el volumen de datos, o Oracle recupera las instancias que considera inactivas. | Media | Alto | Verificar los límites del plan con el volumen de la sección 7.2 antes de elegir el stack; agregación y retención de datos; monitoreo de uso; respaldos fuera de Oracle; despliegue reproducible (por ejemplo, con contenedores) para restaurar en otra instancia o proveedor. |
| `RG-16` | El reloj de una estación se desajusta y envía marcas de tiempo inválidas o imposibles. | Media | Medio | Validar y corregir la marca de tiempo con la hora de recepción, marcar la lectura como corregida, una sola alerta por episodio y medir los huecos con la hora de llegada (sección 1.4); sincronización del reloj por NTP (`S-02`). |
| `RG-17` | Se pierde el acceso a las cuentas de los servicios (Oracle, bróker) al terminar el trabajo de grado, o la cuenta del proyecto queda comprometida. | Baja | Alto | Las cuentas se crean con la cuenta del proyecto (monitoringiotsystemuco@gmail.com), que se entrega con la plataforma; definir quién guarda las credenciales, activar la verificación en dos pasos con un método de recuperación que también se entregue (`PC-06`, `PC-07`). |

---

## 9. Pendientes de contexto

Temas que no están resueltos en ninguno de los dos documentos y que afectan el alcance o la planeación. Los puntos abiertos de detalle funcional están en la sección 7 de la especificación (`PA-01` a `PA-56`).

| ID | Tema pendiente | A quién consultar |
|----|----------------|-------------------|
| `PC-01` | Confirmar las responsabilidades de cada programa (Ambiental, Electrónica, Sistemas) y el interlocutor de cada uno. | Investigador principal |
| `PC-02` | **Contrato de integración.** **Parcialmente resuelto:** primera fase con 5 estaciones WiFi (LoRa en fases futuras); el mensaje JSON lleva el código de la estación, la marca de tiempo, el identificador de cada sensor y los valores de sus variables; cada estación puede tener su propio formato JSON; la marca de tiempo la pone la estación con un reloj de hardware; si no se puede interpretar o está desajustada, la lectura se guarda con la hora de recepción y se marca como corregida, con una sola alerta por episodio que se mantiene hasta que se corrija; las unidades actuales están validadas (`PA-04`); se acepta la pérdida de datos durante una desconexión; hueco "sin datos" con más de 60 s sin mensajes y desconexión con 10 minutos sin mensajes, medidos por el servidor con la hora de llegada (sección 1.4). **Falta:** los formatos de marca de tiempo admitidos, según el reloj que se use (referencia propuesta: ISO 8601 en UTC); la diferencia tolerada antes de considerar una marca desajustada; el dispositivo de reloj y cómo se pone en hora; la estructura concreta del JSON, con un mensaje de ejemplo, la estructura de tópicos y el nivel de QoS; la matriz de valores y umbrales de los sensores que define los códigos de error y cómo se identifican los sensores con fallas (`PA-32`, `PA-33`); los datos que genera la API, que se definirán al validar la estructura de datos de la plataforma. Relacionado con `PA-09`, `PA-31`, `PA-32`, `PA-33`, `PA-49` y `PA-50`. | Ingeniería Electrónica |
| `PC-03` | **Volumen.** **Resuelto:** 5 estaciones en la primera fase, un mensaje cada 30 segundos por estación, con las lecturas de todos sus sensores (sección 7.2). **Por confirmar:** el número estimado de estaciones de fases posteriores, para la estimación de costos y de capacidad, y que todos los sensores vayan en el mismo mensaje como regla del contrato. | Ingeniería Electrónica e investigador principal |
| `PC-04` | **ICA.** **Resuelto (fuera del alcance):** el Índice de Calidad del Aire no forma parte de la plataforma, del proyecto ni de las estaciones. Los colores del mapa se basan en umbrales de las variables ambientales (`PC-22`). | — |
| `PC-05` | **Acceso de la ciudadanía.** **Resuelto:** cualquier persona consulta y descarga los datos sin registrarse; el inicio de sesión es solo para administradores y la comunicación con la ciudadanía se hace por un correo de contacto. | — |
| `PC-06` | **Operación después del trabajo de grado.** **Parcialmente resuelto:** los estudiantes no estarán vinculados durante los 24 meses; el proyecto entrega un documento de operación (costos, estrategia, riesgos y su mitigación). Falta definir quién administra la plataforma, paga los servicios y atiende fallas durante el resto de ese periodo, y a quién se entregan las cuentas de los servicios. | Director e investigador principal |
| `PC-07` | **Infraestructura y presupuesto.** **Parcialmente resuelto:** la Universidad no provee servidores; la plataforma se aloja en máquinas virtuales de Oracle Cloud (Always Free); el bróker MQTT será un servicio administrado (proveedor por definir); el dominio pertenece a Juan Camilo (docente), con quien se trata todo lo relacionado; el equipo define el presupuesto y su aprobación y pago se acordarán con el investigador; las cuentas de los servicios se crean con la cuenta del proyecto monitoringiotsystemuco@gmail.com, que se entrega con la plataforma; los estudiantes agendan la reunión con Juan Camilo y su resultado se agrega a los documentos; los servicios aún se están buscando y los costos se definirán al validar por completo los servicios necesarios. **Falta:** quién guarda las credenciales de la cuenta del proyecto y activar la verificación en dos pasos; la fecha y las preguntas de la reunión con Juan Camilo (¿subdominio?, ¿quién configura los registros DNS hacia Oracle?, ¿costo o requisitos de seguridad como HTTPS?); verificar que los límites del plan Always Free alcanzan (`RG-15`); comparar el costo de los brókeres administrados con unos 432.000 mensajes al mes, y el bróker propio como alternativa; el mecanismo de credenciales del bróker (afecta V-28); las demás partidas del presupuesto (dominio, correo, copias de seguridad); desde cuándo cuentan los 24 meses. | Investigador principal, Juan Camilo |
| `PC-08` | **Microplásticos y variables.** **Resuelto:** la plataforma no maneja datos de microplásticos. La estación envía todas las variables de sus sensores (material particulado, temperatura y humedad, y gases del Winsen ZPHS01B), y la plataforma debe soportar sensores con otras variables. **Por confirmar:** ¿los gases reciben el mismo tratamiento que las demás variables (se muestran al público, tienen umbrales que colorean el mapa y se pueden descargar), o el foco es el material particulado y los gases solo se almacenan? | Investigador principal e Ingeniería Ambiental |
| `PC-09` | **Validación de datos.** **Parcialmente resuelto:** el módulo de calibración solo muestra gráficas para comparar modelos y los modelos no se aplican a los datos de la plataforma (`PCAL-01`, `PCAL-03`; responde `PA-53`). **Falta:** si se guardan todos los datos capturados o hay datos que no se deben guardar (por ejemplo, valores fuera de rango o con código de error, marcados como tales). El documento de calibración recomienda no eliminar datos de forma automática (secciones 6.3 y 9.10). | Ingeniería Ambiental y Electrónica |
| `PC-10` | **Fuentes de referencia.** **Parcialmente resuelto:** no se usan redes ni plataformas externas (por ejemplo, SIATA); toda comparación se hace entre estaciones registradas. Dos usos: calibración y comparación entre zonas. No se marca ninguna estación como referencia: se elige cualquier estación registrada; las estaciones no tienen que estar en el mismo lugar y se puede calibrar con 2 o más; las lecturas se emparejan con promedios por minuto; la estación casera envía por MQTT y se registra como cualquier estación, con su propio formato JSON. La calidad de la referencia la decide el investigador, con la nota de riesgo de la sección 1.5. **Falta:** cómo se configura y se valida el formato JSON de cada estación (V-27, `PA-50`); qué pasa con los minutos que tienen una sola lectura y si las lecturas con marca de tiempo corregida entran en el promedio; confirmar con el investigador que el objetivo 4 ya no incluye la co-ubicación, que sí aparece en el objetivo específico 2 del macroproyecto. Relacionado con `PA-55`. | Investigador principal e Ingeniería Ambiental |
| `PC-11` | **Metodología y entregables.** **Resuelto:** Scrum; solo se esperan los entregables técnicos que define la metodología, sin otros artefactos ni formatos especiales. | — |
| `PC-12` | Cronograma: fechas de inicio y fin del trabajo de grado, hitos del macroproyecto y fecha esperada de las primeras estaciones reales. | Director e investigador principal |
| `PC-13` | **Reportes.** **Resuelto:** no se generan informes de ningún tipo (ni de datos ambientales ni de las acciones en la plataforma); basta con los dashboards y la descarga de datos. Se mantienen las alertas *Informative* de la especificación (sección 2.6), incluidas las que registran las acciones de los administradores. | — |
| `PC-14` | **Titularidad y licencia de los datos.** **Parcialmente resuelto:** el titular de los datos es la Universidad; los estudiantes y los investigadores consultarán al área legal la titularidad, la licencia y la cita en los archivos descargados. Durante el desarrollo solo se incluye una cita del macroproyecto. **Falta:** el texto de esa cita, y resolver la consulta legal **antes del despliegue** (registrarla en el cronograma con fecha límite). | Investigador principal / área legal de la UCO |
| `PC-15` | **Retención de datos (sin respuesta):** ¿los datos se conservan indefinidamente, o solo durante los 24 meses? ¿Qué pasa con la plataforma y los datos al terminar ese periodo? Con el almacenamiento limitado del plan Always Free y unos 230.000 valores al día (sección 7.2), la respuesta es necesaria para decidir si se agregan o se depuran datos antiguos. | Investigador principal |
| `PC-16` | **Distribución del trabajo.** **Resuelto:** los dos estudiantes presentan la plataforma como un único trabajo de grado conjunto. Las actividades y los entregables se reparten en Azure DevOps según la disponibilidad semanal de cada integrante. | — |
| `PC-17` | **Requisitos de la analítica de datos.** **Resuelto en lo principal:** los investigadores entregaron el documento ([requerimientos_modulo_calibracion.md](requerimientos_modulo_calibracion.md)) y sus dudas se aclararon en la reunión del 26/10 (sección 1.5). Se espera comparar modelos y elegir el que mejor calibra, y concluir qué variables auxiliares afectan a cada sensor (`PCAL-07`); se muestra el R² de los datos calibrados, sin valor de aceptación (`PCAL-10` a `PCAL-12`); no hay variables prioritarias (`PCAL-06`); modelos: regresión lineal, ANN, Random Forest, SVM, AdaBoost y regresión polinomial (`PCAL-08`). **Falta:** los modelos mínimos de la primera versión (`PCAL-09`); si se incluye ElasticNet y si XGBoost, que aparece en el documento fuente, sale de la lista; los casos de estimación de variables, que el documento no menciona; cómo reportar el efecto de cada variable (valor p, que solo aplica a los modelos lineales; importancia de variables para los demás); si el R² se calcula sobre los datos de entrenamiento o sobre un conjunto de prueba. | Investigador principal e Ingeniería Ambiental |
| `PC-18` | **Uso de los modelos.** **Parcialmente resuelto:** la analítica la usan solo los administradores (`PCAL-02`); los modelos se entrenan en la plataforma; no se aplican a los datos de la plataforma ni se muestran datos calibrados al público (`PCAL-03`); el administrador descarga el modelo elegido para luego calibrar las estaciones (`PCAL-01`), en formato `.keras` (`PCAL-14`); no se guarda historial (`PCAL-13`). **Falta:** el formato de exportación de los modelos que no son redes neuronales, porque `.keras` solo sirve para modelos de Keras (por ejemplo, `.joblib` de scikit-learn u ONNX para todos); qué significa "subirlo y calibrar las estaciones" (¿se carga en el microcontrolador?, en ese caso el formato debe ser compatible con el dispositivo); si la estimación de variables sigue en el alcance (afecta V-31). Relacionado con `PA-52` a `PA-54`. | Investigador principal |
| `PC-19` | **Datos para pruebas.** **Parcialmente resuelto:** durante el desarrollo se usarán datos simulados para probar el funcionamiento de la plataforma, incluido el módulo de calibración. Ya no se exige co-ubicación ni una estación de referencia fija (`PC-10`). **Falta:** cuándo habrá datos reales de al menos 2 estaciones para probar la calibración (depende de `PC-12`). | Ingeniería Ambiental y Electrónica |
| `PC-20` | **Sensores futuros.** **Resuelto:** aún no se sabe qué sensores se agregarán; la plataforma debe permitir registrar sensores nuevos con sus variables y unidades (catálogo ampliable, V-25 y V-26), y los entregables finales deben estimar cómo varían los costos según la información adicional que se agregue (ver 7.2). | — |
| `PC-21` | **Azure DevOps.** **Parcialmente resuelto:** el Product Owner es único (el director del proyecto, confirmado con los investigadores); la estructura se reorganiza con una épica por objetivo específico y Features que representan funcionalidades validables de la plataforma (por ejemplo, inicio de sesión o administración de estaciones), cada una con escenarios de validación que aprueba el Product Owner; el código vive en Azure Repos, en un repositorio privado. **Falta:** validar esta estructura y los entregables con el director. | Director y estudiantes |
| `PC-22` | **Colores del mapa.** **Parcialmente resuelto:** el color de cada estación corresponde al nivel de alerta de los umbrales de sus variables ambientales, no a su estado de funcionamiento; se muestran las variables en alerta; una estación sin datos o desconectada aparece en gris y opaca. Los umbrales, niveles y colores se definirán con el investigador principal y el investigador de Ingeniería Ambiental. **Falta:** si se evalúa la última lectura o un promedio (por ejemplo, horario o de 24 h), para evitar cambios de color por picos momentáneos; si la estación toma el color del nivel más alto entre sus variables; qué color tiene una estación sin variables en alerta (por ejemplo, verde) y mientras no haya umbrales definidos; si los umbrales son globales o por estación (`PA-24`); confirmar que las anomalías no cambian el color en el mapa público; si una estación con un hueco corto (entre 60 s y 10 minutos sin mensajes) mantiene su color y solo se pone gris al considerarse desconectada; cómo se muestra una estación con un sensor sin datos y los demás funcionando; qué color tiene una estación *Inactive* en el mapa. La regla general ya está en la especificación (V-02, `RF-11`, `RN-37`); los detalles están en `PA-56`. | Investigador principal e Ingeniería Ambiental |

### 9.1 Puntos abiertos de la especificación que afectan el alcance

De los puntos abiertos de [plataforma_web_descripcion.md](plataforma_web_descripcion.md), estos son los que tienen mayor impacto en la planeación y conviene priorizar en la próxima reunión:

- `PA-09` y `PA-50`: procedimiento para que una estación empiece a enviar datos y estructura concreta del mensaje MQTT (dependen de `PC-02` y del bróker elegido en `PC-07`). `PA-49` quedó resuelto (tiempo máximo sin mensajes y alerta de desconexión con el bróker) y `PA-51` (estaciones LoRa) queda aplazado para fases futuras.
- `PA-52` y `PA-54`: modelos mínimos, estimación de variables, valor p, datos para el R² y formato de exportación de los modelos que no son redes neuronales (`PC-17`, `PC-18`). `PA-53` quedó resuelto y `PA-55` descartado.
- `PA-24` y `PA-56`: alcance de los umbrales y reglas de detalle de los colores del mapa (afectan el modelo de datos, la configuración y V-02, `PC-22`).
- `PA-31`, `PA-32`, `PA-33` y `PA-34`: reglas de cálculo del estado de los sensores, matriz de códigos de error, anomalías y alertas repetidas por umbral (afectan la validación de datos del objetivo 2). La desconexión de la estación (10 minutos) y la alerta por episodio para la marca de tiempo ya están en la especificación (`RN-33`, `RN-34`).
- `PA-36`: alcance de la regla sobre los datos de estaciones ocultas o eliminadas (tablas de histórico, detalle de una estación oculta y módulo de calibración). `PA-30` y `PA-35` quedaron resueltos con el estado *Sin datos* y la suspensión de la escucha en el bróker (`RN-38`).
- `PA-45`: servicio de geocodificación y mapas (afecta el costo).
- `PA-46`: valores de los límites de solicitudes y de descargas por IP, y servicio de *captcha* (afectan el costo y el rendimiento). El *captcha*, la retención de 12 horas y el último archivo por persona ya están definidos (`RN-40`).
- `PA-48`: creación del primer administrador, vigencia del enlace de activación, datos editables de otros administradores e ingreso con Microsoft. La activación por enlace y la gestión de administradores ya están definidas (V-33 a V-36).

---

## 10. Sugerencias

1. **Congelar una versión mínima del producto.** La especificación tiene 29 vistas vigentes, incluidas las de analítica y las de gestión de administradores. Conviene acordar con el investigador principal qué es imprescindible para los productos del macroproyecto (recepción por MQTT, consulta pública con el mapa por umbrales, exportación y módulo de calibración con los modelos mínimos) y qué es deseable (modelos adicionales, estimación de variables, versiones del formato del mensaje, eliminación de cuenta), para proteger el cronograma.
2. **Cerrar el contrato de integración MQTT cuanto antes** con Ingeniería Electrónica: la estructura concreta del JSON con un mensaje de ejemplo, los tópicos, el QoS, los formatos de marca de tiempo según el reloj elegido y la matriz de valores y umbrales para los códigos de error, incluso antes de terminar los mockups. Es la pieza que más depende de terceros y la que permite construir el simulador. Como cada estación puede tener su propio formato, conviene acordar un formato base que usen las 5 estaciones de la primera fase y dejar la configuración por estación para las caseras y las futuras.
3. **Cerrar los pendientes del módulo de calibración** en la próxima reunión: modelos mínimos (`PCAL-09`), formato de exportación de los modelos que no son redes neuronales, qué significa "subir el modelo a las estaciones" y si la estimación de variables sigue en el alcance (`PC-17`, `PC-18`). **Resolver también la retención de datos** (`PC-15`), porque condiciona el almacenamiento en Oracle.
4. **Dejar por escrito la nota de riesgo de la calibración** (sección 1.5) y que el investigador la acepte: la calidad de la referencia y la ubicación de las estaciones las decide él, y el objetivo 4 ya no promete la co-ubicación que menciona el macroproyecto (`PC-10`).
5. **Hacer la proyección de costos a 24 meses antes de elegir el stack y el bróker**, con el volumen de la sección 7.2 (unos 432.000 mensajes al mes), los límites del plan Always Free y el costo del bróker después de su capa gratuita, y documentarla como parte del plan de gestión de recursos.
6. **Proteger la cuenta del proyecto** (monitoringiotsystemuco@gmail.com): definir quién guarda las credenciales y activar la verificación en dos pasos. **Agendar la reunión con Juan Camilo** sobre el dominio (`PC-07`).
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
| Estación de referencia | Estación registrada que el administrador elige en una calibración para tomar de ella la variable de referencia. No es una marca permanente: cualquier estación registrada, incluida una casera, puede usarse como referencia. |
| Estación casera | Estación construida por un administrador, que envía sus datos por MQTT y se registra en la plataforma como cualquier otra, con su propio formato de mensaje. |
| Sensor | Dispositivo que mide una o varias variables ambientales (inicialmente Winsen ZPHS01B, Honeywell HPMA115S0-XXX y AM2305B). |
| Variable | Magnitud medida por un sensor (por ejemplo, PM2.5, CO₂, temperatura), con su unidad. |
| Lectura | Conjunto de valores enviados por una estación en un mensaje (uno cada 30 segundos). |
| Marca de tiempo | Fecha y hora de una lectura, que pone la estación con su reloj de hardware y la plataforma valida. |
| Lectura corregida | Lectura cuya marca de tiempo no se pudo interpretar o estaba desajustada, y que se guardó con la hora de recepción del servidor. |
| Hueco "sin datos" | Periodo en el que no llegan mensajes de una estación durante 2 envíos seguidos (más de 60 s), medido con la hora de llegada al servidor; se muestra sin interpolar. |
| Desconexión | Situación de una estación de la que no llegan mensajes durante el tiempo máximo sin mensajes (10 minutos por defecto), medido con la hora de llegada al servidor. La estación pasa al estado *Sin datos*. |
| Sin datos (estado) | Estado de una estación de la que la plataforma no está recibiendo mensajes: recién creada o habilitada y sin su primer mensaje, o desconectada. Se muestra en gris y opaca en el mapa. A diferencia de las estaciones deshabilitadas, ocultas o eliminadas, la plataforma sigue escuchando sus mensajes. |
| Enlace de activación | Enlace de un solo uso que recibe por correo un administrador recién registrado para definir su contraseña y activar su cuenta (estado *New* → *Active*). |
| ICA | Índice de Calidad del Aire: valor adimensional que resume la calidad del aire a partir de las concentraciones de contaminantes. Fuera del alcance del proyecto (`PC-04`). |
| MQTT | Protocolo de mensajería ligero basado en publicación y suscripción, usado por las estaciones para enviar sus datos. |
| Bróker MQTT | Servidor que recibe los mensajes publicados por las estaciones y los entrega a la plataforma, que está suscrita a ellos. |
| Tópico | Canal del bróker MQTT en el que publica cada estación. |
| QoS | Nivel de calidad de servicio de MQTT, que define la garantía de entrega de cada mensaje. |
| Credenciales de estación | Usuario y contraseña, token o certificado con los que una estación se autentica ante el bróker MQTT. |
| LoRa | Tecnología de comunicación inalámbrica de largo alcance y bajo consumo, que algunas estaciones usarán en fases futuras para enviar sus datos a una estación de enlace. |
| Contrato de integración | Acuerdo técnico (protocolo MQTT, tópicos, formato del mensaje, marca de tiempo, autenticación, errores) con el que las estaciones, reales o simuladas, envían datos a la plataforma. No es único: cada estación puede tener su propio formato JSON, que el administrador configura. |
| Simulador | Programa que imita el envío de datos de las estaciones por el contrato de integración, para desarrollar y probar sin hardware. |
| Umbral | Valor de una variable a partir del cual se genera una alerta y cambia el color de la estación en el mapa. |
| Calibración | Proceso de ajustar las lecturas de un sensor de bajo costo para que se acerquen a las de una referencia más confiable. |
| Intercalibración | Ajuste entre sensores de precisión similar; no equivale a una calibración frente a un equipo de referencia. |
| Co-ubicación | Instalación de una estación junto a una estación de referencia durante un periodo, para comparar sus mediciones. En este proyecto no es obligatoria: se puede calibrar con estaciones en lugares distintos, con la limitación descrita en la sección 1.5. |
| Variable auxiliar | Medición adicional que el administrador elige como entrada del modelo, además de la variable a calibrar (por ejemplo, la humedad para el material particulado). |
| R² | Coeficiente de determinación. Indica qué tan cerca quedan los valores calibrados de los de referencia; cuanto más cerca de 1, mejor. |
| Estimación de variables | Cálculo de una variable a partir de otras variables correlacionadas (por ejemplo, temperatura a partir de humedad). Por confirmar si sigue en el alcance (`PC-18`). |
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
