# Gestión de cambios

Registro de los cambios que se hacen en los documentos del proyecto, con su origen y su justificación. Los documentos no describen sus propios cambios: cada cambio se registra aquí.

**Documentos controlados:**

| Documento | Contenido | Versión vigente |
|-----------|-----------|:---------------:|
| [contextualizacion.md](contextualizacion.md) | Contexto, objetivos, alcance, interesados, ciclo de vida, costos, riesgos y pendientes de contexto (`PC-xx`). | 8 |
| [plataforma_web_descripcion.md](plataforma_web_descripcion.md) | Especificación funcional: vistas (`V-xx`), componentes (`C-xx`), requisitos (`RF`), reglas de negocio (`RN`), requisitos no funcionales (`RNF`) y puntos abiertos (`PA-xx`). | 4 |

El historial anterior a estas versiones está en la sección *Control de cambios* de cada documento.

**Convenciones**

- Cada conjunto de cambios tiene un identificador `GC-xxx`, una fecha, un origen (reunión, acta o solicitud) y la lista de cambios por documento.
- **Tipo de cambio:** *Agregado*, *Modificado*, *Retirado* (el elemento se elimina; su identificador no se reutiliza) o *Corrección* (ortografía o redacción, sin cambio de contenido).
- Cuando un cambio no viene literalmente del origen, sino que es una interpretación o una consecuencia derivada, se indica en la columna *Motivo*.

---

## GC-001 — 2026-10-01 — Reunión del 28/09/2026

**Origen:** [Reunión 280926.md](Reunión%20280926.md).
**Versiones:** contextualizacion.md 2 → 3; plataforma_web_descripcion.md 3 → 4.

### Decisiones de la reunión

| # | Decisión |
|---|----------|
| D-01 | **Nuevo alcance: analítica de datos.** Calibrar los sensores con modelos de aprendizaje automático para que sus mediciones se acerquen al valor real; relacionar sensores de distintas estaciones (del proyecto o de otras personas) como referencia para comparar, validar y calibrar; encontrar un modelo con excelente precisión y menor costo computacional; calcular variables a partir de variables correlacionadas (por ejemplo, temperatura a partir de humedad). Atiende los objetivos específicos 2 y 3 del macroproyecto. |
| D-02 | **Protocolo de envío:** MQTT. |
| D-03 | **Redes:** WiFi; algunas estaciones tendrán en el futuro LoRa como red principal o de respaldo. Las estaciones con LoRa envían sus datos a estaciones con WiFi, y estas al bróker. |
| D-04 | **Sensores iniciales:** AM2305B, Winsen Multi-in-One ZPHS01B y Honeywell HPM Series HPMA115S0-XXX. En futuras mejoras se agregarán sensores diferentes. |
| D-05 | **Vistas nuevas:** agregar la API o estación con la que se compara al calibrar (interna o externa); elegir variables para encontrar otra variable y elegir el modelo; exportar el modelo calibrado; agregar sensores a las estaciones; editar cómo se recibe el JSON por MQTT (administrador); regenerar las credenciales de conexión de las estaciones con el bróker. |
| D-06 | **Inicio de sesión solo para administradores.** Cualquier persona accede a las funciones principales que no son de administración. Datos para crear un administrador: correo, nombre y contraseña. |
| D-07 | **Sin historial de descargas de archivos.** |
| D-08 | **Sin PQRS;** se deja un correo de contacto. |
| D-09 | **Observaciones:** validar el modelo de MQTT para el envío de variables; los requisitos de la analítica de datos están a la espera de que se valide qué se espera conseguir de forma precisa o exacta. |
| D-10 | **Pregunta para la sustentación — ¿por qué construir una interfaz?:** conexión con las comunidades, apropiación social del conocimiento, página no técnica que cualquier persona entienda y que aporte al conocimiento, a la investigación y al medio ambiente, concientizando a las personas. |

### Interpretaciones y decisiones derivadas

Puntos que el acta no dice de forma literal y que se resolvieron así para mantener la coherencia de los documentos. Deben confirmarse con los investigadores.

| # | Interpretación | Dónde queda pendiente |
|---|----------------|-----------------------|
| I-01 | El acta está incompleta en la línea de sensores futuros ("se piensan añadir difere…"); se interpretó como "sensores diferentes". | `PC-20` |
| I-02 | Como los datos para crear un administrador son correo, nombre y contraseña, quien registra define la contraseña: se retiran la contraseña temporal, el correo de activación, el estado *Pending* y la vista de primer ingreso (V-20). Se recomienda evaluar el cambio de contraseña en el primer ingreso. | `PA-48` |
| I-03 | Como solo hay cuentas de administrador con correo y contraseña, se retira el inicio de sesión con Google y Microsoft. | `PA-48` (d) |
| I-04 | Se conservan para los administradores la recuperación de contraseña (V-10), el cambio de contraseña (V-21), el perfil (V-11, V-12) y la eliminación de cuenta (V-19), porque la reunión no los descartó. Se retiran los campos *Apellidos*, *Teléfono* y *Facultad*. | `PA-40`, `PA-48` |
| I-05 | El acceso a las vistas de analítica se definió como de administrador (propuesto), por su costo computacional y porque registrar fuentes externas es una tarea de administración. | `PA-52` (d), `PC-18` |
| I-06 | Como la consulta y las descargas pasan a ser públicas, se agregan límites de uso para evitar abusos. | `PA-46`, `RNF-10` |
| I-07 | Con MQTT la plataforma no se conecta directamente a las estaciones; el *timeout*, los *reintentos de conexión* y la alerta de reintentos se conservan, pero su significado queda por redefinir. | `PA-49` |
| I-08 | Las vistas nuevas se numeran desde V-25 para no reutilizar los identificadores retirados (V-09, V-20, V-22, V-23 y V-24). | — |

---

### Cambios en plataforma_web_descripcion.md (versión 3 → 4)

#### Generales y catálogos

| # | Sección / ID | Tipo | Cambio | Motivo |
|---|--------------|------|--------|--------|
| 1 | Encabezado | Modificado | Versión 4; el historial de cambios pasa a este documento. Se aclara que los identificadores retirados no se reutilizan y se define la marca **(preliminar)** para la analítica. | Organización documental; D-09 |
| 2 | 1.2 Roles | Modificado | El rol *Usuario general* (con cuenta) se reemplaza por *Visitante* (sin sesión). El administrador suma la gestión de sensores, credenciales, formato MQTT y analítica, y pierde la atención de PQRS. Se define que *Ambos* = visitante y administrador. | D-05, D-06, D-08 |
| 3 | 1.3 Mapa de navegación | Modificado | Acceso público a mapa, estaciones, detalle y descargas; el inicio de sesión es solo para administradores. Se quitan registro, primer ingreso y PQRS; se agregan sensores de la estación, regenerar credenciales, analítica, catálogo de sensores y formato MQTT. Solo V-01 y V-10 quedan sin barra lateral. | D-05, D-06, D-08 |
| 4 | 2.1 Estados de las cuentas | Modificado | Solo cuentas de administrador, con estados *Active* y *Deleted*. Se retiran *Pending*, el atributo *Cambio de contraseña obligatorio* y las transiciones de verificación y de Google/Microsoft. El primer administrador se crea en el despliegue (propuesto). | D-06, I-02, I-03 |
| 5 | 2.2 Estados de las estaciones | Modificado | En la tabla de visibilidad, *Listado, usuario general* pasa a *Listado, visitante*. Referencia a `PA-49` en el estado *Error*. | D-06, I-07 |
| 6 | 2.3 Sensores y variables | Modificado | Se agregan los modelos completos (Winsen ZPHS01B, Honeywell HPMA115S0-XXX, AM2305B). Se aclara que el catálogo es ampliable (V-25) y que cada estación puede tener sensores distintos (V-26). Se quita la pregunta sobre si todas tienen los mismos sensores. | D-04, D-05 |
| 7 | 2.4 Estados de los sensores | Corrección | "anomalias" → "anomalías"; "una anomalías" → "anomalías"; "deberian de recibir" → "deberían recibir". | Ortografía |
| 8 | 2.5 Estados de generación de archivos | Modificado | Sin historial: el estado se muestra solo durante la generación en curso y el archivo se elimina del servidor al descargarse o al vencer una retención temporal (propuesto). | D-07 |
| 9 | 2.6 Tipos de alerta | Agregado | Tipos *Mensaje inválido* (propuesto), *Asignación o retiro de sensor* y *Regeneración de credenciales*. Referencia a `PA-49` en *Reintentos de conexión*. | D-02, D-05 |
| 10 | 2.8 PQRS | Retirado | Se elimina el catálogo de tipos y estados de PQRS. | D-08 |
| 11 | 2.8 Analítica de datos | Agregado | Nueva sección preliminar: tipos de análisis (calibración y estimación de variables), fuentes de referencia (interna o externa), modelos (RLM y ANN), criterios de comparación (precisión y costo computacional) y estados de un entrenamiento. | D-01, D-05 |

#### Componentes

| # | ID | Tipo | Cambio | Motivo |
|---|----|------|--------|--------|
| 12 | `C-05` | Modificado | También se usa en V-28. | D-05 |
| 13 | `C-06` | Modificado | Verifica la contraseña del administrador; también se usa en V-28. | D-05, D-06 |
| 14 | `C-07` | Modificado | Se usa en V-10, V-13 y V-21 (ya no en V-09 ni V-20). | D-06, I-02 |
| 15 | `C-09` | Modificado | Opciones según el rol: el visitante ve *Mapa*, *Estaciones*, *Generar archivos de descarga* e *Iniciar sesión*; el administrador ve además el menú de perfil, *Alertas*, *Analítica de datos* (submenú), *Catálogo de sensores*, *Formato del mensaje MQTT*, *Configuración* y *Registro de administradores*. Se quita *PQRS* y la regla de Google/Microsoft. | D-05, D-06, D-08 |
| 16 | `C-10` | Modificado | El pie de página incluye el correo de contacto como canal de comunicación con la ciudadanía. | D-08 |
| 17 | `C-11` | Agregado | Panel de resultados de entrenamiento (comparación de modelos, métricas, gráficos y botón *Guardar modelo*). | D-01, D-05 |
| 18 | `C-12` | Agregado | Visualización de credenciales MQTT, que se muestran una sola vez. | D-05 |

#### Vistas retiradas

| # | Vista | Requisitos retirados | Motivo |
|---|-------|----------------------|--------|
| 19 | V-09 Registro de usuario | `RF-80` a `RF-86` | D-06 |
| 20 | V-20 Establecer contraseña (primer ingreso) | `RF-190` a `RF-192` | I-02 |
| 21 | V-22 PQRS | `RF-210` a `RF-213` | D-08 |
| 22 | V-23 Creación de PQRS | `RF-220`, `RF-221` | D-08 |
| 23 | V-24 Detalle de PQRS | `RF-230` a `RF-233` | D-08 |

#### Vistas modificadas

| # | Vista / ID | Tipo | Cambio | Motivo |
|---|------------|------|--------|--------|
| 24 | Requisitos globales: `RF-G01` a `RF-G04`, `RF-G06` | Modificado | Barra lateral en todas las vistas salvo V-01 y V-10; opciones del visitante y del administrador; menú de perfil solo para administradores y sin regla de Google/Microsoft; pie de página con correo de contacto. | D-06, D-08, I-03 |
| 25 | `RF-G07` | Agregado | Mapa, listado, detalle y descargas disponibles sin registro ni inicio de sesión. | D-06 |
| 26 | V-01 Inicio de sesión | Modificado | Pasa a *Inicio de sesión — administradores*: sin Google/Microsoft ni enlace de registro; con enlace para volver al mapa. `RF-02` a `RF-07` se ajustan al administrador; `RF-07` solo bloquea cuentas *Deleted*. | D-06, I-02, I-03 |
| 27 | `RF-01`, `RF-08` | Retirado | Inicio de sesión con Google/Microsoft y redirección al primer ingreso. | I-02, I-03 |
| 28 | `RF-09` | Agregado | El inicio de sesión es accesible desde el botón *Iniciar sesión* de la barra lateral. | D-06 |
| 29 | V-02 Mapa / `RF-14` | Modificado | Redacción: "ni para el visitante ni para el administrador". | D-06 |
| 30 | V-04 Detalle de estación / `RF-37` | Modificado | Botones de administrador *Sensores* (V-26) y *Regenerar credenciales* (V-28). Las tarjetas de sensores dependen de los sensores asignados. Nota sobre valores calibrados (`PA-53`). | D-01, D-05 |
| 31 | V-05 Generación de archivos | Modificado | Vista pública. El historial se reemplaza por un panel de la generación en curso. `RF-46` muestra estado y avance de la generación en curso; `RF-48` pasa a "no conservar historial y eliminar el archivo tras descargarlo o al vencer la retención temporal". | D-06, D-07 |
| 32 | `RF-42`, `RF-47` | Retirado | Historial de archivos y disponibilidad de un mes. | D-07 |
| 33 | V-10 Recuperación de contraseña | Modificado | Solo para administradores; `RF-90` a `RF-95` con rol *Administrador*; `RF-91` se refiere a cuentas de administrador activas. | D-06, I-04 |
| 34 | `RF-96` | Retirado | Recuperación para cuentas de Google/Microsoft. | I-03 |
| 35 | V-11 Perfil / `RF-100`, `RF-101` | Modificado | Solo administrador; muestra nombre, correo y rol. | D-06, I-04 |
| 36 | V-12 Edición de perfil / `RF-110` | Modificado | Solo se edita el nombre. | D-06, I-04 |
| 37 | V-13 Registro de administradores | Modificado | Formulario con nombre, correo, contraseña y confirmación; sin teléfono, facultad ni captcha. La cuenta queda *Active* de inmediato. `RF-120` y `RF-121` se ajustan. | D-06, I-02 |
| 38 | `RF-123`, `RF-124` | Retirado | Contraseña temporal y activación por enlace. | I-02 |
| 39 | `RF-125` | Agregado | No permitir registrar un correo con cuenta de administrador activa. | D-06 |
| 40 | V-14 Crear estación / `RF-130` | Modificado | Se agregan la selección de sensores del catálogo y la red de comunicación (WiFi o LoRa con estación de enlace, propuesto). | D-03, D-05 |
| 41 | `RF-134`, `RF-135` | Agregado | Generación y visualización única de las credenciales MQTT; indicación de red WiFi o LoRa y estación de enlace (propuesto). | D-03, D-05 |
| 42 | V-15 Configuración | Modificado | Los umbrales cubren las variables del catálogo; los parámetros de conexión quedan sujetos a `PA-49`. | D-02, D-05, I-07 |
| 43 | V-16 Alertas / `RF-152` | Modificado | Incluye las alertas por mensajes inválidos, asignación o retiro de sensores y regeneración de credenciales. | D-05 |
| 44 | V-19 Eliminación de cuenta | Modificado | Solo administrador. La anonimización cubre ID, nombre, correo y contraseña. Estado de vista "último administrador". `RF-180` y `RF-181` se ajustan. | D-06, D-07, I-04 |
| 45 | `RF-182`, `RF-184` | Retirado | Eliminación de los archivos del usuario y desvinculación de Google/Microsoft. | D-07, I-03 |
| 46 | V-21 Restablecer contraseña / `RF-200`, `RF-202` | Modificado | Solo administrador. | D-06 |
| 47 | `RF-201` | Retirado | Ocultar la opción a cuentas de Google/Microsoft. | I-03 |

#### Vistas nuevas

| # | Vista | Requisitos | Motivo |
|---|-------|------------|--------|
| 48 | V-25 Catálogo de sensores | `RF-240` a `RF-244` | D-04, D-05 (agregar sensores) |
| 49 | V-26 Ventana de sensores de la estación | `RF-250` a `RF-253` | D-05 (agregar sensores a las estaciones) |
| 50 | V-27 Formato del mensaje MQTT | `RF-260` a `RF-264` | D-02, D-05 (editar cómo se recibe el JSON), D-09 |
| 51 | V-28 Ventana de regenerar credenciales | `RF-270` a `RF-273` | D-05 (regenerar credenciales con el bróker) |
| 52 | V-29 Fuentes de referencia (preliminar) | `RF-280` a `RF-284` | D-01, D-05 (agregar API o estación de comparación) |
| 53 | V-30 Calibración de sensores (preliminar) | `RF-290` a `RF-296` | D-01 (objetivo específico 2: RLM frente a ANN) |
| 54 | V-31 Estimación de variables (preliminar) | `RF-300` a `RF-304` | D-01, D-05 (elegir variables y modelo para calcular otra variable) |
| 55 | V-32 Modelos (preliminar) | `RF-310` a `RF-314` | D-05 (exportar el modelo calibrado) |

#### Reglas de negocio

| # | ID | Tipo | Cambio | Motivo |
|---|----|------|--------|--------|
| 56 | `RN-01` | Modificado | Sensores del catálogo; los tres iniciales; no todas las estaciones tienen los mismos y se pueden agregar sensores. | D-04 |
| 57 | `RN-03` | Modificado | Redacción con *visitante* y *administrador*. | D-06 |
| 58 | `RN-06` | Modificado | Se agregan las acciones exclusivas nuevas (sensores, credenciales, formato MQTT y analítica) y se quitan las PQRS. | D-05, D-08 |
| 59 | `RN-11` | Modificado | No existe registro público; solo un administrador crea cuentas, todas con rol `admin`. | D-06 |
| 60 | `RN-12` | Modificado | Las cuentas de administrador quedan *Active* al crearse. | I-02 |
| 61 | `RN-19` | Modificado | A los administradores se les pide nombre, correo y contraseña; a los visitantes, ningún dato personal. | D-06 |
| 62 | `RN-20` | Modificado | Aplica a cuentas de administrador; sin eliminación de archivos. | D-07, I-04 |
| 63 | `RN-26` | Modificado | Se agregan la asignación o el retiro de sensores y la regeneración de credenciales. | D-05 |
| 64 | `RN-05`, `RN-08`, `RN-21` | Retirado | Historial de archivos por usuario, disponibilidad de un mes y eliminación al vencer. | D-07 |
| 65 | `RN-17`, `RN-23` | Retirado | Estado *Pending* con enlaces de 48 h y contraseña temporal. | I-02 |
| 66 | `RN-18` | Retirado | Cuentas de Google/Microsoft sin contraseña. | I-03 |
| 67 | `RN-25` | Retirado | PQRS. | D-08 |
| 68 | `RN-27` | Agregado | Consulta y descargas sin registro ni inicio de sesión. | D-06 |
| 69 | `RN-28` | Agregado | Credenciales propias por estación, visibles una sola vez; regenerarlas invalida las anteriores. | D-05 |
| 70 | `RN-29` | Agregado | Envío por MQTT, interpretado según el formato vigente. | D-02, D-05 |
| 71 | `RN-30` | Agregado | Retransmisión LoRa → WiFi; la lectura se asocia a la estación de origen (propuesto). | D-03 |
| 72 | `RN-31` | Agregado | Toda calibración compara al menos RLM y ANN. | D-01 |
| 73 | `RN-32` | Agregado | Los datos originales se conservan; los calibrados o estimados no los reemplazan (propuesto). | D-01 |

#### Requisitos no funcionales

| # | ID | Tipo | Cambio | Motivo |
|---|----|------|--------|--------|
| 74 | `RNF-03` | Modificado | Las vistas V-02 a V-05 son públicas; las demás requieren sesión de administrador. | D-06 |
| 75 | `RNF-06` | Modificado | La trazabilidad incluye la asignación o el retiro de sensores y la regeneración de credenciales. | D-05 |
| 76 | `RNF-07` | Modificado | Incluye las credenciales de las estaciones; se quitan los enlaces de verificación y activación. | D-05, I-02 |
| 77 | `RNF-08` | Modificado | Solo se tratan datos personales de los administradores; se quita la autorización al registrarse. | D-06 |
| 78 | `RNF-09` | Agregado | Seguridad del bróker MQTT (autenticación por estación, TLS, restricción por tópico). | D-02, D-05 |
| 79 | `RNF-10` | Agregado | Límites de uso de las funciones públicas. | D-06, I-06 |
| 80 | `RNF-11` | Agregado | Entrenamientos en segundo plano y con límite de simultaneidad. | D-01 (menor costo computacional) |
| 81 | `RNF-12` | Agregado | Lenguaje claro y no técnico en las vistas públicas. | D-10 |

#### Puntos abiertos

| # | ID | Estado anterior → nuevo | Cambio | Motivo |
|---|----|-------------------------|--------|--------|
| 82 | `PA-05` | Resuelto → Resuelto | Nueva resolución: sin registro público; solo administradores creados por otro administrador. | D-06 |
| 83 | `PA-09` | Parcial → Parcial | Se agregan los avances (sensores y credenciales MQTT) y las dependencias `PA-50` y `PA-51`. | D-02, D-05 |
| 84 | `PA-11`, `PA-18` | Resuelto → Descartado | No hay historial de archivos. | D-07 |
| 85 | `PA-15` | Abierto → Resuelto | Las estaciones pueden tener sensores distintos y se agregarán otros. | D-04 |
| 86 | `PA-19`, `PA-20` | Resuelto → Descartado | No hay registro de usuarios, Google/Microsoft ni estado *Pending*. | D-06, I-02, I-03 |
| 87 | `PA-21` | Resuelto → Resuelto | Nueva resolución: quien registra define nombre, correo y contraseña. | D-06 |
| 88 | `PA-22` | Resuelto → Resuelto | Se quita la eliminación de archivos de la resolución. | D-07 |
| 89 | `PA-38`, `PA-39`, `PA-41` | Abierto → Descartado | Funcionalidades retiradas; lo pendiente pasa a `PA-48`. | D-06, I-02, I-03 |
| 90 | `PA-42` | Abierto → Descartado | No hay PQRS. | D-08 |
| 91 | `PA-32`, `PA-33` | Abierto → Abierto | La recomendación se ajusta al mensaje MQTT configurable y al rango físico del catálogo de sensores. | D-02, D-05 |
| 92 | `PA-36`, `PA-40`, `PA-43`, `PA-46` | Abierto → Abierto | Redacción actualizada: visitante en lugar de usuario general; sin PQRS; correo de contacto; límites de las descargas públicas y retención temporal. | D-06, D-07, D-08 |
| 93 | `PA-48` a `PA-55` | Nuevo (Abierto) | Gestión de administradores, conexión con MQTT, modelo del mensaje MQTT, estaciones LoRa, requisitos de analítica, aplicación de los modelos, exportación de modelos y fuentes externas de referencia. | D-01 a D-06, D-09, I-02, I-05, I-07 |

#### Otros

| # | Sección | Tipo | Cambio | Motivo |
|---|---------|------|--------|--------|
| 94 | 8. Control de cambios | Modificado | Se agrega una nota que remite a este documento; el historial de las versiones 2 y 3 se conserva. | Organización documental |

---

### Cambios en contextualizacion.md (versión 2 → 3)

| # | Sección | Tipo | Cambio | Motivo |
|---|---------|------|--------|--------|
| 1 | Encabezado | Agregado | Enlace a este documento entre los documentos relacionados. | Organización documental |
| 2 | 1. Contexto | Corrección | "especificos" → "específicos"; puntuación de las citas de los objetivos. | Ortografía |
| 3 | 1.1 Participación de cada programa | Modificado | Electrónica: comunicación por WiFi y LoRa y mensaje MQTT. Ambiental: valida la calibración. Sistemas: contrato MQTT, calibración con aprendizaje automático, modelos completos de los sensores y sensores futuros. | D-01, D-02, D-03, D-04 |
| 4 | 1.2 Problema | Modificado | Redacción del problema de calibración; los investigadores también calibran; el macroproyecto carece de los productos de los objetivos 2 y 3. | D-01 |
| 5 | 1.3 ¿Por qué construir una interfaz? | Agregado | Justificación de la interfaz: conexión con las comunidades, apropiación social del conocimiento, lenguaje no técnico y concientización. | D-10 |
| 6 | 1.4 Funcionamiento definido de las estaciones | Agregado | Protocolo MQTT, redes WiFi y LoRa con diagrama de flujo de datos, y tabla de sensores iniciales. | D-02, D-03, D-04, D-09 |
| 7 | 2.1 Objetivo general | Modificado | Se agrega "calibre" y se corrige un espacio antes de la coma. | D-01 |
| 8 | 2.2 Objetivo específico 3 | Modificado | Contrato de integración basado en MQTT, con estaciones WiFi y LoRa. | D-02, D-03 |
| 9 | 2.2 Objetivo específico 4 | Modificado | Acceso libre para investigadores y ciudadanía. | D-06 |
| 10 | 2.2 Objetivo específico 6 | Modificado | Redacción completa: calibración por co-ubicación, comparación RLM y ANN frente a fuentes internas o externas, estimación de variables, equilibrio entre precisión y costo computacional, y exportación del modelo. Corrige "comprandolo". | D-01, D-05 |
| 11 | 2.2 Sugerencias | Agregado | El objetivo 6 depende de los requisitos de analítica (`PC-17`). | D-09 |
| 12 | 2.3 Trazabilidad | Modificado | Objetivo 3 pasa a *Parcial* (MQTT y vistas V-14, V-25 a V-28); acceso ciudadano pasa a *Cubierto*; se agrega la fila del objetivo 6 (V-29 a V-32); la especificación pasa a la versión 4. | D-01 a D-06 |
| 13 | 3. Alcance | Modificado | El proyecto incluye la metodología de calibración como módulo de analítica de la plataforma. | D-01 |
| 14 | 3.1 Incluye | Modificado | Documentación de la metodología de calibración; contrato MQTT; estaciones LoRa; simulador por MQTT; consulta pública; descargas sin historial; autenticación solo de administradores; catálogo y asignación de sensores; formato MQTT y credenciales; correo de contacto; nuevo bloque *Analítica de datos*; validación de modelos. Se quitan registro, Google/Microsoft y PQRS. | D-01 a D-08 |
| 15 | 3.2 No incluye | Modificado | Se quita la exclusión de la calibración (solo se excluye la calibración física); el firmware incluye la retransmisión LoRa; conectividad WiFi y LoRa; equipos de referencia; análisis científico limitado; se excluyen cuentas para visitantes y PQRS; integración con redes oficiales más allá de usarlas como referencia; objetivos distintos de 2 y 3. | D-01, D-03, D-06, D-08 |
| 16 | 3.3 Supuestos `S-01`, `S-02`, `S-04`, `S-05` | Modificado | MQTT, retransmisión LoRa, sensores iniciales con modelo y sensores futuros desde el catálogo, validación de la calibración por Ambiental. | D-02, D-03, D-04, D-01 |
| 17 | 3.3 Supuestos `S-11`, `S-12` | Agregado | Disponibilidad de datos de referencia y recursos de cómputo para entrenar. | D-01 |
| 18 | 3.4 Restricciones `R-01`, `R-05`, `R-06`, `R-07`, `R-08` | Modificado | Costos del bróker y del cómputo; alcance en objetivos 2 y 3; dependencia de datos de referencia; datos indicativos aun calibrados; se quita la Ley 1755 de 2015 (PQRS) y se agregan las condiciones de uso de fuentes externas. | D-01, D-02, D-08 |
| 19 | 3.4 Restricciones `R-10` | Agregado | La recepción de datos se hace por MQTT. | D-02 |
| 20 | 4. Interesados | Modificado | Ambiental valida la calibración; Electrónica envía por MQTT (WiFi y LoRa); los estudiantes desarrollan también la metodología de calibración; los administradores ya no atienden PQRS; la ciudadanía consulta sin registro. | D-01 a D-08 |
| 21 | 4. Interesados | Agregado | *Proveedores de datos de referencia*. | D-01 |
| 22 | 5. Ciclo de vida | Modificado | Contrato MQTT y diseño de la calibración en diseño; módulo de analítica en desarrollo; validación de modelos en pruebas; bróker en despliegue; calibración con datos reales en cierre. Nueva priorización en 7 grupos, sin PQRS y con la analítica. | D-01, D-02, D-08 |
| 23 | 6. Criterios de éxito | Modificado / Agregado | Criterio 3 por MQTT y con regeneración de credenciales; criterio 4 sin inicio de sesión; nuevo criterio 6 (calibración RLM frente a ANN). | D-01, D-02, D-05, D-06 |
| 24 | 7.1 Costos | Modificado / Agregado | Se agrega el bróker MQTT y el entrenamiento de modelos; archivos temporales y modelos; correo solo para recuperación y contacto; se quitan el captcha y Google/Microsoft y se agregan las APIs de referencia. | D-01, D-02, D-06, D-07, D-08 |
| 25 | 7.2 Volumen de datos | Modificado | El volumen crece con los sensores nuevos y con los datos de referencia almacenados. | D-01, D-04 |
| 26 | 7.3 Criterios del stack | Agregado | Soporte de bróker MQTT y bibliotecas de aprendizaje automático. | D-01, D-02 |
| 27 | 8. Riesgos `RG-03`, `RG-06`, `RG-07`, `RG-09` | Modificado | Control de cambios en este documento; datos calibrados también indicativos; alcance con analítica y MQTT; seguridad del bróker y de las credenciales. | D-01, D-02, D-05 |
| 28 | 8. Riesgos `RG-10` a `RG-14` | Agregado | Requisitos de analítica sin precisar, falta de datos de referencia, abuso de funciones públicas, cambios en el mensaje del firmware y pérdida de datos LoRa. | D-01, D-03, D-05, D-06, D-09 |
| 29 | 9. Pendientes `PC-02` | Modificado | Protocolo MQTT y redes WiFi/LoRa definidos; se detalla lo que falta validar. | D-02, D-03, D-09 |
| 30 | 9. Pendientes `PC-04` | Modificado | Se pregunta si el ICA se calcula con datos calibrados. | D-01 |
| 31 | 9. Pendientes `PC-05` | Modificado | **Resuelto:** consulta y descarga públicas; sesión solo para administradores; correo de contacto. | D-06, D-08 |
| 32 | 9. Pendientes `PC-09` | Modificado | La calibración queda incluida; se mantiene lo pendiente de la validación técnica. | D-01 |
| 33 | 9. Pendientes `PC-10` | Modificado | Las redes externas se usan como fuentes de referencia. | D-01 |
| 34 | 9. Pendientes `PC-14` | Modificado | Se relaciona con la descarga pública. | D-06 |
| 35 | 9. Pendientes `PC-17` a `PC-20` | Agregado | Requisitos de analítica, uso de los modelos, co-ubicación y sensores futuros. | D-01, D-04, D-09, I-01, I-05 |
| 36 | 9.1 Puntos abiertos prioritarios | Modificado | Se quita `PA-15` (resuelto) y se agregan `PA-48` a `PA-54`. | D-02 a D-06 |
| 37 | 10. Sugerencias | Modificado / Agregado | Versión mínima con 27 vistas; contrato MQTT y modelo del mensaje; resolver `PC-04` y `PC-17`; nueva sugerencia de fuentes de referencia; costos del bróker y del entrenamiento; nueva sugerencia sobre la respuesta a "¿por qué construir una interfaz?". | D-01, D-02, D-09, D-10 |
| 38 | 11. Glosario | Modificado / Agregado / Retirado | Sensor y contrato de integración actualizados; se agregan estación de enlace, MQTT, bróker, tópico, credenciales de estación, LoRa, calibración, co-ubicación, fuente de referencia, estimación de variables, RLM, ANN, costo computacional y visitante; se retira PQRS. | D-01 a D-08 |
| 39 | 12. Control de cambios | Modificado | Nota que remite a este documento; se conserva el historial de las versiones 1 y 2. | Organización documental |

---

### Identificadores retirados en GC-001

No se deben reutilizar.

| Tipo | Identificadores |
|------|-----------------|
| Vistas | V-09, V-20, V-22, V-23, V-24 |
| Requisitos funcionales | `RF-01`, `RF-08`, `RF-42`, `RF-47`, `RF-80` a `RF-86`, `RF-96`, `RF-123`, `RF-124`, `RF-182`, `RF-184`, `RF-190` a `RF-192`, `RF-201`, `RF-210` a `RF-213`, `RF-220`, `RF-221`, `RF-230` a `RF-233` |
| Reglas de negocio | `RN-05`, `RN-08`, `RN-17`, `RN-18`, `RN-21`, `RN-23`, `RN-25` |
| Catálogos | Sección 2.8 *PQRS* (reemplazada por 2.8 *Analítica de datos*); estado de cuenta *Pending* |

### Temas para llevar a la próxima reunión

- Confirmar las interpretaciones I-01 a I-08.
- `PC-17` / `PA-52`: qué se espera conseguir con la analítica (métricas y valores de aceptación).
- `PC-02` / `PA-50` / `PA-51`: validar con Ingeniería Electrónica el modelo del mensaje MQTT y el caso LoRa.
- `PC-19`: fuentes de referencia y periodo de co-ubicación disponibles.
- `PA-48`: creación del primer administrador y comunicación segura de la contraseña.

---

## GC-002 — 2026-10-08 — Solicitud de los estudiantes

**Origen:** solicitud de los estudiantes.
**Versiones:** contextualizacion.md 3 → 4.

### Decisiones

| # | Decisión |
|---|----------|
| D-11 | **Trabajo de grado conjunto:** los dos estudiantes presentan la plataforma como un único entregable de trabajo de grado. |
| D-12 | **Distribución del trabajo:** las actividades y los entregables se reparten en Azure DevOps según la disponibilidad semanal de cada integrante. |

### Cambios en contextualizacion.md (versión 3 → 4)

| # | Sección | Tipo | Cambio | Motivo |
|---|---------|------|--------|--------|
| 1 | 3.4 Restricciones `R-04` | Modificado | Se indica que los estudiantes presentan la plataforma como un trabajo de grado conjunto. | D-11 |
| 2 | 9. Pendientes `PC-16` | Modificado | **Resuelto:** trabajo de grado conjunto; las actividades y los entregables se reparten en Azure DevOps según la disponibilidad semanal de cada integrante. | D-11, D-12 |

---

## GC-003 — 2026-10-08 — Respuestas preliminares para la reunión del 13/10/2026

**Origen:** [Reunión 131026.md](reuniones/Reuni%C3%B3n%20131026.md) (respuestas ya registradas antes de la reunión).
**Versiones:** contextualizacion.md 4 → 5.

### Decisiones

| # | Decisión |
|---|----------|
| D-13 | **Datos simulados durante el desarrollo:** mientras se desarrolla la plataforma se simularán datos de prueba para probar su funcionamiento. |
| D-14 | **Sensores futuros:** aún no se sabe qué sensores se agregarán; la plataforma debe permitir agregar sensores y los entregables finales deben estimar cómo varían los costos según la información adicional que se agregue. |
| D-15 | **Calibración en espera de la analítica:** la calibración de las estaciones con datos reales se define cuando se resuelva el tema de la analítica (objetivo, requerimientos, riesgos, modelos y forma de desarrollo). |

### Interpretaciones y decisiones derivadas

| # | Interpretación | Dónde queda pendiente |
|---|----------------|-----------------------|
| I-09 | La respuesta sobre `PC-19` cubre la etapa de desarrollo, pero no la calibración con datos reales: sigue sin definirse la referencia co-ubicada, el lugar y la duración. Por eso `PC-19` queda parcialmente resuelto, y lo pendiente queda en espera de `PC-17` (D-15). | `PC-17`, `PC-19`, `S-11`, `RG-11` |
| I-10 | "Información adicional que se agregue" se interpretó como sensores, variables o estaciones que aumentan el volumen de datos, y la estimación se ubicó en el plan de gestión de recursos y costos. | `PC-03` |

### Cambios en contextualizacion.md (versión 4 → 5)

| # | Sección | Tipo | Cambio | Motivo |
|---|---------|------|--------|--------|
| 1 | 3.1 Incluye | Modificado | El plan de gestión de recursos y costos incluye la estimación de cómo varían los costos al agregar sensores o variables. | D-14, I-10 |
| 2 | 6. Criterios de éxito, objetivo 5 | Modificado | El plan de costos también estima la variación de costos al agregar sensores o variables. | D-14 |
| 3 | 7.2 Volumen de datos | Modificado | Se agrega la estimación de la variación de costos según lo que se agregue. | D-14, I-10 |
| 4 | 9. Pendientes `PC-19` | Modificado | **Parcialmente resuelto:** datos simulados durante el desarrollo; la referencia para la calibración con datos reales queda en espera de `PC-17`. | D-13, D-15, I-09 |
| 5 | 9. Pendientes `PC-20` | Modificado | **Resuelto:** sensores futuros desconocidos; catálogo ampliable y estimación de costos en los entregables. | D-14 |

---

## GC-004 — 2026-10-19 — Reunión del 06/10/2026

**Origen:** [Reunión 061026.md](reuniones/Reuni%C3%B3n%20061026.md) y aclaración de los estudiantes del 19/10/2026.
**Versiones:** contextualizacion.md 5 → 6.

### Decisiones de la reunión

| # | Decisión |
|---|----------|
| D-16 | **Se eliminan los objetivos específicos 2 (requerimientos) y 4 (visualización y exportación).** El contenido del objetivo 4 se integra al objetivo 5 (despliegue). |
| D-17 | **Contrato de integración no único:** el administrador puede modificar el contrato MQTT de cualquier estación; el objetivo 3 debe reflejarlo. |
| D-18 | **Objetivo 6 en espera** de un documento de los investigadores con lo que se espera realizar, para refinarlo. |
| D-19 | **Frecuencia de envío:** cada estación enviará datos en segundos (invalida `S-07`). |
| D-20 | **Criterio de éxito del objetivo 5:** los estudiantes no estarán presentes los 2 años; se entrega un documento de costos, estrategia, riesgos, mitigación de riesgos y demás información para que la plataforma se pueda usar, mantener y conservar en funcionamiento durante 2 años. |
| D-21 | **Atributos de calidad de la página web:** disponibilidad, rendimiento, usabilidad, seguridad, accesibilidad, compatibilidad, mantenibilidad y escalabilidad. |
| D-22 | **Elementos de seguridad:** seguridad del software (firewall, escaneo y eliminación de malware, actualizaciones periódicas, análisis de tráfico anti-DDoS), certificado SSL, copias de seguridad y restauraciones, protección DDoS, supervisión de redes, soporte CDN y validación de formularios contra la inyección de consultas. Los servicios están por definir: no se deben suponer servicios. |
| D-23 | **Azure DevOps:** validar los entregables y la estructura, que puede ser enredada para desarrollar los work items; desarrollar Product Backlog Items para priorizar. |
| D-24 | **Supuesto `S-11`:** por validar; los estudiantes lo validarán y actualizarán más adelante (no se modifica en esta versión). |

### Interpretaciones y decisiones derivadas

| # | Interpretación | Dónde queda pendiente |
|---|----------------|-----------------------|
| I-11 | Al eliminar los objetivos 2 y 4, los objetivos se renumeran: 1 → 1, 3 → 2, 5 → 3, 6 → 4. Las referencias de este registro anteriores a GC-004 usan la numeración antigua. | — |
| I-12 | El levantamiento de requerimientos deja de ser objetivo, pero sigue en el alcance (3.1) y en la etapa 2 del ciclo de vida. | — |
| I-13 | El ICA se conserva en el nuevo objetivo 3 mientras se confirma con el investigador principal si sigue en el alcance. | `PC-04` |
| I-14 | La redacción del objetivo de despliegue se mantiene; el cambio de D-20 se aplica en el criterio de éxito, en `R-02`, `RG-04` y `PC-06`. | `PC-06` |
| I-15 | Con frecuencias de segundos, la agregación y la retención de datos pasan a ser necesarias y la probabilidad de `RG-02` sube a *Alta*. Los ejemplos de 7.2 usan 1 lectura cada 10 segundos y 1 por segundo. | `PC-03`, `PC-15` |
| I-16 | Por D-22, se retira el ejemplo "Let's Encrypt" de la tabla de costos. | — |
| I-17 | El contrato por estación responde la pregunta de `PA-50` ("¿el formato es único o puede variar por estación?"), pero la especificación (V-27, `RN-29`) aún habla de un formato vigente único y debe actualizarse. | `PA-50` |

### Cambios en contextualizacion.md (versión 5 → 6)

| # | Sección | Tipo | Cambio | Motivo |
|---|---------|------|--------|--------|
| 1 | 1.4 Funcionamiento de las estaciones | Agregado | El contrato MQTT es configurable por estación; la frecuencia de envío es del orden de segundos. | D-17, D-19 |
| 2 | 2.2 Objetivos específicos 2 y 4 | Retirado | Se eliminan los objetivos de requerimientos y de visualización y exportación. | D-16 |
| 3 | 2.2 Objetivo específico 3 → 2 | Modificado | Contrato de integración configurable por estación, en lugar de único. | D-17, I-11 |
| 4 | 2.2 Objetivo específico 5 → 3 | Modificado | Incluye la visualización y exportación de las series históricas y del ICA con acceso libre. | D-16, I-11, I-13 |
| 5 | 2.2 Objetivo específico 6 → 4 | Modificado | Solo cambia la numeración; queda en espera del documento de analítica. | D-18, I-11 |
| 6 | 2.2 Sugerencias | Modificado | Se reemplaza la sugerencia sobre "operar 24 meses" por el documento de operación; se agregan el ICA por confirmar y el objetivo 4 en espera. | D-18, D-20, I-13 |
| 7 | 2.3 Trazabilidad | Modificado | Nueva numeración; se quita la fila de requerimientos; el contrato por estación queda pendiente en la especificación; nota sobre el levantamiento de requerimientos. | D-16, D-17, I-12, I-17 |
| 8 | 3.1 Incluye | Modificado / Agregado | Formato del mensaje configurable por estación; atributos de calidad; elementos de seguridad sin suponer servicios. | D-17, D-21, D-22 |
| 9 | 3.3 Supuestos `S-04`, `S-07` | Modificado | Formato configurado por estación; frecuencia de envío del orden de segundos. | D-17, D-19 |
| 10 | 3.4 Restricciones `R-02` | Modificado | Los estudiantes no operan los 24 meses; se entrega el plan de operación. | D-20, I-14 |
| 11 | 5. Ciclo de vida, etapa 2 | Modificado | Artefacto: Product Backlog Items priorizados en Azure DevOps. | D-23 |
| 12 | 6. Criterios de éxito | Retirado / Modificado | Se retiran los criterios 2 y 4; el criterio 3 (antes 5) integra la consulta pública y exige el documento de operación a 24 meses; el criterio 2 (antes 3) usa el contrato por estación; el criterio 4 (antes 6) se renumera. | D-16, D-17, D-20 |
| 13 | 7.1 Costos | Modificado / Agregado | Certificado SSL/TLS sin proveedor de ejemplo; nueva fila de seguridad. | D-22, I-16 |
| 14 | 7.2 Volumen de datos | Modificado | Estimación con frecuencias de segundos; agregación y retención necesarias. | D-19, I-15 |
| 15 | 8. Riesgos `RG-02`, `RG-04`, `RG-09`, `RG-13` | Modificado | `RG-02` sube a probabilidad *Alta*; `RG-04` con el documento de operación; `RG-09` con los elementos de seguridad; `RG-13` por estación. | D-17, D-19, D-20, D-22, I-15 |
| 16 | 9. Pendientes `PC-03`, `PC-06` | Modificado | **Parcialmente resueltos:** frecuencia en segundos; los estudiantes no operan los 24 meses. | D-19, D-20 |
| 17 | 9. Pendientes `PC-04` | Modificado | Confirmar si el ICA sigue en el alcance. | D-16, I-13 |
| 18 | 9. Pendientes `PC-09`, `PC-17` | Modificado | Nueva numeración del objetivo; `PC-17` espera el documento de analítica. | D-18, I-11 |
| 19 | 9. Pendientes `PC-21` | Agregado | Validar los entregables y la estructura de Azure DevOps. | D-23 |
| 20 | 9.1 y 10. Sugerencias | Modificado | Referencias a los objetivos con la nueva numeración. | I-11 |
| 21 | 11. Glosario, *Contrato de integración* | Modificado | El contrato no es único; se configura por estación. | D-17 |

### Temas para llevar a la próxima reunión

- `PC-04`: confirmar con el investigador principal si el ICA sigue en el alcance.
- `S-11`: validación a cargo de los estudiantes.
- `PC-17`: documento de los investigadores sobre la analítica.
- `PC-21`: estructura de Azure DevOps.
- Especificación: actualizar V-27, `RN-29` y `PA-50` al contrato por estación, y agregar los `RNF` de disponibilidad, compatibilidad, mantenibilidad y escalabilidad.

---

## GC-005 — 2026-10-26 — Reunión del 19/10/2026

**Origen:** [Reunión 191026.md](reuniones/Reuni%C3%B3n%20191026.md).
**Versiones:** contextualizacion.md 6 → 7. La especificación funcional no se modifica en este conjunto; los cambios que requiere quedan en *Temas para llevar a la próxima reunión*.

### Decisiones de la reunión

| # | Decisión |
|---|----------|
| D-25 | **Primera fase:** 5 estaciones conectadas por WiFi, con los sensores ya definidos. Las estaciones con LoRa quedan para fases futuras. |
| D-26 | **Contenido del mensaje JSON:** código de la estación, marca de tiempo, identificador de cada sensor y, por cada sensor, las variables que mide. Todos los sensores en el mismo mensaje. |
| D-27 | **Marca de tiempo:** la pone la estación con un reloj de hardware (por definir). La plataforma valida el formato; si no lo cumple, lo corrige y genera una alerta. |
| D-28 | **Unidades:** las unidades de las variables actuales están confirmadas; al registrar un sensor nuevo se agregan sus variables y unidades. |
| D-29 | **Pérdida de datos aceptada:** sin conexión, la estación no envía, no guarda ni reenvía los datos. |
| D-30 | **Hueco y desconexión:** 2 envíos perdidos (más de 60 s) = periodo "sin datos", sin interpolar; si siguen sin llegar mensajes, la estación está desconectada. |
| D-31 | **Datos que genera la API:** se definirán al validar la estructura de datos de la plataforma. |
| D-32 | **Volumen:** 5 estaciones, un mensaje cada 30 segundos. |
| D-33 | **ICA fuera del alcance** de la plataforma, del proyecto y de las estaciones. |
| D-34 | **Infraestructura:** el dominio es de Juan Camilo (docente), con quien se trata todo lo del dominio; la Universidad no provee servidores; la plataforma se aloja en Oracle Cloud Always Free; el bróker MQTT será administrado (proveedor por definir); las cuentas están por definir; el equipo define el presupuesto y su aprobación y pago se acordarán con el investigador. |
| D-35 | **Sin microplásticos;** la estación envía todas las variables de sus sensores y la plataforma debe soportar sensores con otras variables. |
| D-36 | **Analítica en espera** del documento de los investigadores (`PC-09`, `PC-17`, `PC-18`). |
| D-37 | **Sin redes externas** (por ejemplo, SIATA): toda comparación se hace entre estaciones registradas, para calibrar (con una estación casera de un administrador) o para comparar zonas. |
| D-38 | **Entregables:** solo los que define Scrum, sin otros artefactos ni formatos. |
| D-39 | **Sin informes** de ningún tipo; bastan los dashboards y la descarga. Se mantienen las alertas *Informative*. |
| D-40 | **Titularidad de los datos:** la Universidad; el área legal define la licencia y la cita. Durante el desarrollo solo se incluye una cita del macroproyecto. |
| D-41 | **Datos simulados** durante el desarrollo (ratifica D-13). |
| D-42 | **Colores del mapa:** según el nivel de alerta de los umbrales de las variables, no el estado de funcionamiento; se muestran las variables en alerta; gris y opaco si no hay datos o la estación está desconectada. Umbrales, niveles y colores se definen con el investigador principal y el de Ingeniería Ambiental. |
| D-43 | **Nombre de la plataforma** (`PA-01`): lo elige el equipo de desarrollo. |

### Interpretaciones y decisiones derivadas

Respuestas propuestas a las dudas que el acta deja abiertas (*Por aclarar*). Se incorporan al documento marcadas como **(propuesto)** y deben confirmarse.

| # | Interpretación | Dónde queda pendiente |
|---|----------------|-----------------------|
| I-18 | Formato esperado de la marca de tiempo: ISO 8601 en UTC. Un formato legible distinto se convierte; una marca ilegible o con hora imposible se reemplaza por la hora de recepción y la lectura se marca como *corregida*. | `PC-02` |
| I-19 | Una sola alerta por episodio de marca de tiempo inválida, para no generar 2.880 alertas al día. | `PC-02`, `PA-34` |
| I-20 | La desconexión se fija en 5 minutos sin mensajes; pone la estación en gris y genera la alerta *Tiempo sin conexión*. Huecos y desconexión se miden con la hora de llegada al servidor. | `PC-02`, `PA-31`, `PC-22` |
| I-21 | Al quedar fuera el ICA, se retira del objetivo 3, de la trazabilidad, del alcance (pasa a *No incluye*), de `R-07`, `R-08`, `RG-06`, de los criterios de éxito y de la priorización. I-13 queda superada. | — |
| I-22 | Al no usar redes externas, el objetivo 4 compara "frente a otras estaciones registradas en la plataforma", se retira el interesado *Proveedores de datos de referencia* y las APIs de referencia de los costos, y `S-11` y `RG-11` se ajustan a una estación de referencia interna. Si esa estación usa sensores de bajo costo, el resultado es una intercalibración. | `PC-10`, `PC-19` |
| I-23 | `RG-14` (pérdida de datos LoRa) deja de aplicar en la primera fase y se redefine como la pérdida aceptada durante una desconexión. | — |
| I-24 | Las dudas sobre los colores del mapa se agrupan en un nuevo pendiente `PC-22`, porque cambian la visualización del objetivo 3 y contradicen V-02 / `RF-11` de la especificación. | `PC-22` |
| I-25 | El alcance a 5 estaciones WiFi y el alojamiento en Oracle se registran como restricciones (`R-05`, nueva `R-11`); se agregan los riesgos de los límites de Oracle (`RG-15`), del reloj de las estaciones (`RG-16`) y de las cuentas a nombre de un estudiante (`RG-17`). | `PC-07` |
| I-26 | La tabla de 7.2 se recalcula con 5 estaciones y 30 s (14.400 mensajes y unos 230.000 valores al día); 10 y 20 estaciones quedan como escenarios ilustrativos. | `PC-03`, `PC-15` |
| I-27 | Con Scrum confirmado, la sección 5 deja de ser propuesta; la tabla de artefactos sigue siendo la propuesta del equipo. | `PC-11` |

### Cambios en contextualizacion.md (versión 6 → 7)

| # | Sección | Tipo | Cambio | Motivo |
|---|---------|------|--------|--------|
| 1 | 1.1 Participación | Modificado | Electrónica: reloj de las estaciones, WiFi en la primera fase y LoRa en fases futuras. Ambiental: umbrales, niveles y colores con el investigador principal; sin ICA; los microplásticos no pasan por la plataforma. | D-25, D-27, D-33, D-35, D-42 |
| 2 | 1.2 Problema | Modificado / Corrección | "sensores caídos" → "estaciones desconectadas"; "s uperan" → "superan". | D-30; ortografía |
| 3 | 1.4 Funcionamiento de las estaciones | Modificado / Agregado | Alcance de la primera fase; bróker administrado; diagrama separado por fases; envío de todas las variables y unidades confirmadas; frecuencia de 30 s; contenido del mensaje; marca de tiempo con su validación (propuesta); pérdida de conexión, hueco y desconexión (propuesta de 5 minutos). | D-25 a D-30, D-32, D-34, I-18 a I-20 |
| 4 | 2.2 Objetivos 2, 3 y 4 | Modificado | Objetivo 2: 5 estaciones WiFi con un mensaje cada 30 s. Objetivo 3: sin ICA. Objetivo 4: frente a otras estaciones registradas. | D-25, D-32, D-33, D-37, I-21, I-22 |
| 5 | 2.2 Sugerencias | Modificado / Agregado | Se retira la sugerencia del ICA; se agrega la de intercalibración. | D-33, I-22 |
| 6 | 2.3 Trazabilidad | Modificado / Retirado | Objetivo 1 con Scrum; objetivo 2 con lo definido y lo pendiente; objetivo 3 con el color del mapa por actualizar; despliegue parcial con Oracle; analítica sin fuentes externas. Se retira la fila del ICA. | D-25 a D-42, I-21, I-24 |
| 7 | 3. Alcance y 3.1 Incluye | Modificado / Agregado | 5 estaciones de la primera fase; entregables de Scrum; validación de la marca de tiempo; detección de huecos y desconexiones; mapa por umbrales; cita en las descargas; catálogo con variables y unidades; alertas por marca de tiempo e *Informative*; analítica con estaciones registradas; despliegue en Oracle; entrega de cuentas. Se retira el cálculo del ICA. | D-25 a D-30, D-33, D-34, D-37 a D-40, D-42 |
| 8 | 3.2 No incluye | Modificado / Agregado | Reloj de las estaciones; LoRa en fases futuras; almacenamiento y reenvío durante desconexiones e interpolación; ICA; microplásticos (sin condición); informes; redes externas incluso como referencia. | D-25, D-27, D-29, D-30, D-33, D-35, D-37, D-39 |
| 9 | 3.3 Supuestos `S-01`, `S-02`, `S-04`, `S-05`, `S-07`, `S-09`, `S-10`, `S-11` | Modificado | 5 estaciones WiFi; reloj con sincronización; unidades en el catálogo; umbrales con el investigador principal; 5 estaciones a 30 s; Oracle Always Free y bróker administrado; Universidad titular y área legal; estación de referencia interna (sigue por validar, D-24). | D-25 a D-28, D-32, D-34, D-37, D-40, D-42, I-22 |
| 10 | 3.4 Restricciones `R-01`, `R-05` a `R-08`, `R-10` | Modificado | Presupuesto definido por el equipo; primera fase; dependencia del documento de analítica; sin ICA; licencia por el área legal y sin fuentes externas; bróker administrado. | D-25, D-33, D-34, D-36, D-37, D-40 |
| 11 | 3.4 Restricciones `R-11` | Agregado | Infraestructura: Oracle Cloud Always Free y dominio de Juan Camilo. | D-34, I-25 |
| 12 | 4. Interesados | Modificado / Agregado / Retirado | Ajustes de roles; nuevo *Propietario del dominio*; la UCO como titular de los datos; se retira *Proveedores de datos de referencia*. | D-34, D-37, D-40, D-42, D-43, I-22 |
| 13 | 5. Ciclo de vida | Modificado | Scrum confirmado y entregables de Scrum; contrato MQTT con mensaje de ejemplo, QoS y marca de tiempo; despliegue en Oracle; priorización sin ICA y con mapa por umbrales. | D-27, D-33, D-34, D-38, D-42, I-27 |
| 14 | 6. Criterios de éxito 2, 3 y 4 | Modificado | Mensaje cada 30 s, marcas de tiempo, huecos y desconexión; despliegue en Oracle y mapa por umbrales sin ICA; referencia como estación registrada. | D-27, D-30, D-32 a D-34, D-37, D-42 |
| 15 | 7.1 Costos | Modificado | Servidor en Oracle Always Free; bróker administrado con su costo probable; dominio de Juan Camilo; sin APIs de referencia; recomendación sobre las cuentas de los servicios. | D-34, D-37 |
| 16 | 7.2 Volumen de datos | Modificado | Tabla recalculada con la primera fase y escenarios de 10 y 20 estaciones. | D-32, I-26 |
| 17 | 7.3 Criterios del stack | Modificado | Límites de Oracle Always Free; credenciales del bróker; se retira la infraestructura de la Universidad; alternativas actualizadas. | D-34 |
| 18 | 8. Riesgos `RG-02`, `RG-04`, `RG-06`, `RG-08`, `RG-10`, `RG-11`, `RG-14` | Modificado | Costo del bróker; entrega de cuentas; sin ICA; planes gratuitos de Oracle y del bróker; estaciones de referencia; intercalibración; pérdida aceptada por desconexión. | D-29, D-33, D-34, D-37, I-22, I-23 |
| 19 | 8. Riesgos `RG-15` a `RG-17` | Agregado | Límites de Oracle Always Free, reloj desajustado y cuentas a nombre de un estudiante. | D-27, D-34, I-25 |
| 20 | 9. Pendientes `PC-02`, `PC-07`, `PC-10`, `PC-14` | Modificado | **Parcialmente resueltos**, con lo que falta. | D-25 a D-31, D-34, D-37, D-40 |
| 21 | 9. Pendientes `PC-03`, `PC-04`, `PC-08`, `PC-11`, `PC-13` | Modificado | **Resueltos** (`PC-04` fuera del alcance), con lo que queda por confirmar. | D-26, D-32, D-33, D-35, D-38, D-39 |
| 22 | 9. Pendientes `PC-06`, `PC-09`, `PC-15`, `PC-17` a `PC-20` | Modificado | Entrega de cuentas; en espera del documento de analítica; retención sin respuesta con el volumen real; quién entrega el documento y cuándo; co-ubicación depende de `PC-10`; sensores con variables y unidades. | D-28, D-34, D-36, D-37, D-41 |
| 23 | 9. Pendientes `PC-22` | Agregado | Colores del mapa. | D-42, I-24 |
| 24 | 9.1 Puntos abiertos prioritarios | Modificado | Se agregan `PA-04`, `PA-34` y `PA-55`; `PA-51` queda para fases futuras. | D-25, D-28, D-37 |
| 25 | 10. Sugerencias | Modificado / Agregado | Versión mínima sin ICA; contrato MQTT con sus pendientes concretos; documento de analítica y retención; estación de referencia; costos con Oracle y bróker; nuevas sugerencias sobre cuentas, dominio y consulta legal. | D-33, D-34, D-36, D-37, D-40 |
| 26 | 11. Glosario | Modificado / Agregado | Estación de enlace (fases futuras), lectura, ICA (fuera del alcance), credenciales, LoRa, contrato, umbral, calibración, co-ubicación; se agregan estación de referencia, marca de tiempo, hueco "sin datos", desconexión, QoS, intercalibración y Oracle Cloud Always Free; *Fuente de referencia* se reemplaza por *Estación de referencia*. | D-25, D-27, D-30, D-33, D-34, D-37, D-42 |

### Temas para llevar a la próxima reunión

- Confirmar las propuestas I-18 a I-20 (formato de la marca de tiempo, alerta por episodio, 5 minutos para la desconexión).
- `PC-02`: JSON de ejemplo, tópicos, QoS, dispositivo de reloj y reporte de un sensor con falla.
- `PC-07`: titular de las cuentas, reunión con Juan Camilo, límites de Oracle Always Free y comparación de brókeres.
- `PC-08`: tratamiento de los gases.
- `PC-10` / `PC-19`: precisión de la estación de referencia, co-ubicación y alineación de los datos.
- `PC-15`: retención de los datos.
- `PC-17`: quién entrega el documento de analítica y en qué fecha.
- `PC-22`: reglas de color del mapa.
- **Especificación (pendiente de actualizar):** V-02 y `RF-11` (color por umbrales, variables en alerta, gris y opaco); sección 2.3 (unidades confirmadas, `PA-04`); V-27 y `RN-29` (marca de tiempo y contrato por estación); sección 2.6 (alerta por marca de tiempo inválida); V-29 y `PA-55` (solo estaciones registradas); `RN-30` y `PA-51` (LoRa en fases futuras); `PA-01` (nombre a cargo del equipo).

---

## GC-006 — 2026-10-26 — Decisiones sobre Azure DevOps y el repositorio

**Origen:** confirmación de los investigadores (Product Owner único) y decisión del equipo (repositorio y estructura de work items).
**Versiones:** contextualizacion.md 7 → 8. La especificación funcional no se modifica. También se actualizan [contexto_azure_devops.md](contexto_azure_devops.md) y [documento_fundacional.md](documento_fundacional.md) (versión 2), que llevan su propio registro.

### Decisiones

| # | Decisión |
|---|----------|
| D-44 | **Product Owner único:** confirmado con los investigadores. El director del proyecto (Investigador 1) es el Product Owner; el Investigador 2 es stakeholder consultado. |
| D-45 | **Repositorio privado en Azure Repos**, en lugar de GitHub, para manejar en un solo proyecto los pipelines, el análisis de código y el enlace de commits y PR con las historias de usuario. |
| D-46 | **Estructura de work items:** una épica por objetivo específico (`OE1`–`OE4`) y Features que representan funcionalidades concretas y validables de la plataforma (por ejemplo, inicio de sesión o administración de estaciones), con escenarios de validación que aprueba el Product Owner. Las áreas pasan a organizarse por módulo de la plataforma. |

### Interpretaciones y decisiones derivadas

| # | Interpretación | Dónde queda pendiente |
|---|----------------|-----------------------|
| I-28 | La sección 5 ya decía "iteraciones de una semana", pero marcada como propuesta; como `ADR-001` está aceptada, se retira la marca y se agregan los Releases cada dos Sprints. | — |
| I-29 | Con el repositorio privado, CodeQL deja de ser una opción sin costo; se propone SonarQube Cloud en su plan gratuito, sujeto a verificar su límite de líneas de código (`ADR-015`). | `ADR-015` |
| I-30 | La nueva estructura responde la preocupación de `PC-21` sobre una estructura enredada, pero debe validarse con el director. | `PC-21` |

### Cambios en contextualizacion.md (versión 7 → 8)

| # | Sección | Tipo | Cambio | Motivo |
|---|---------|------|--------|--------|
| 1 | 5. Ciclo de vida | Modificado | "iteraciones de una semana (propuesto)" → "Sprints de una semana y Releases cada dos Sprints". | I-28 |
| 2 | 9. Pendientes `PC-21` | Modificado | **Parcialmente resuelto:** Product Owner único, épicas por objetivo, Features como funcionalidades validables y repositorio privado en Azure Repos; falta validar con el director. | D-44 a D-46, I-30 |
