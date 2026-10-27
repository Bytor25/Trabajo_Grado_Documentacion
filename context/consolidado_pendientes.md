# Consolidado de preguntas, dudas e inconsistencias

**Fecha:** 2026-10-27.
**Documentos revisados:**

| Abreviatura | Documento | Versión revisada |
|-------------|-----------|------------------|
| **DF** | [documento_fundacional.md](documento_fundacional.md) | 2 (2026-10-26) |
| **AZ** | [contexto_azure_devops.md](contexto_azure_devops.md) | 2026-10-26 |
| **CX** | [contextualizacion.md](contextualizacion.md) | 10 |
| **ES** | [plataforma_web_descripcion.md](plataforma_web_descripcion.md) | 5 |

**Cómo leer este documento.** Reúne en un solo lugar lo que hay que decidir, preguntar o corregir entre los cuatro documentos. No reemplaza los registros de cada documento (`PC-xx`, `PA-xx`, `ADR-xxx`): los referencia. Tiene cuatro partes:

1. **Contradicciones** entre documentos que necesitan una decisión (`INC-xx`).
2. **Referencias desactualizadas**: correcciones que no requieren decisión, porque la respuesta ya está tomada en otro documento (`DES-xx`).
3. **Preguntas abiertas**, agrupadas por a quién hay que hacerlas (`Q-xx`).
4. **Respuestas disponibles que aún no se incorporan.**

La columna *Propuesta* es una recomendación del equipo para resolver el punto; no es una decisión tomada.

---

## 1. Contradicciones entre documentos

| ID | Tema | Qué dice cada documento | Por qué importa | Propuesta |
|----|------|-------------------------|-----------------|-----------|
| `INC-01` | **Quién es el Product Owner y quién es quién** | **DF §1.1** y **AZ §3**: el PO es el "director del proyecto (investigador de Sistemas)", que en Azure es el *Investigador 1*; AZ agrega además un "Director de tesis (opcional)". **CX §4** llama "dueño del producto" al *investigador principal del macroproyecto* y deja como "por definir" al *director del trabajo de grado*. **CX `PC-21`** dice que el PO es "el director del proyecto". | Hay hasta cuatro nombres (director del proyecto, director del trabajo de grado, director de tesis, investigador principal) sin una tabla que diga cuáles son la misma persona. Eso afecta quién aprueba, quién acepta las historias y quién firma el aval. | Confirmar los nombres y unificar en los tres documentos una sola tabla de personas, con su equivalencia en Azure (*Investigador 1* / *Investigador 2*). |
| `INC-02` | **Bróker MQTT: administrado o propio** | **CX `R-10`, §1.4, `PC-07`** y la reunión del 19/10: un **servicio administrado** (por ejemplo, de Amazon). **DF `ADR-007`, §8.4, §8.5** y `SPK-02`: **Mosquitto autoalojado** en la VM, con *dynamic security*, y el diagrama de arquitectura lo ubica dentro de la VM. | Cambia la arquitectura, el costo (`R-01`), cómo se crean y regeneran las credenciales (V-14, V-28, `C-12`) y las pruebas de integración. | Decidirlo en `ADR-007` con la comparación de costos de `PC-07`. Si se mantiene el bróker administrado, actualizar el diagrama de DF §8.5, `SPK-02` y el entorno local (DF §5.10). |
| `INC-03` | **Despliegue** | **CX `R-11`**: la Universidad no provee servidores y la plataforma va en **Oracle Cloud Always Free** (decidido). **DF `ADR-009`** lo deja *Abierta*, con un VPS como respaldo, y DF §8.3 recomienda consultar a TI de la UCO (opción C). | La opción C ya está descartada y la opción A ya es una decisión de los investigadores. | Pasar `ADR-009` a *Aceptada* (Oracle Always Free, VPS como plan de contingencia) y retirar la opción C. |
| `INC-04` | **Titular de las cuentas de los servicios** | **CX `PC-07`, §7.1**: se usa la cuenta del proyecto `monitoringiotsystemuco@gmail.com`, que se entrega con la plataforma. **DF §10.1 #6**: "cuenta institucional o del investigador principal". **AZ §11**: transferir la organización de Azure DevOps al Investigador 1 o a una cuenta institucional. | No está claro si la organización de Azure DevOps también pasa a la cuenta del proyecto, ni a nombre de quién va la tarjeta de crédito que Oracle exige al registrarse (DF §8.3). | Confirmar que todas las cuentas (Oracle, bróker, Azure DevOps, correo) usan la cuenta del proyecto, y definir quién aporta la tarjeta. |
| `INC-05` | **Formato de exportación de los modelos y librerías de aprendizaje automático** | **Reunión 26/10 (`PCAL-14`), CX §1.5, ES §2.8**: formato `.keras`. **DF §8.4 y `SPK-06`**: scikit-learn (`LinearRegression`, `MLPRegressor`), coeficientes en JSON para RLM, ONNX para la ANN y `joblib` como respaldo. | `.keras` implica usar Keras/TensorFlow para la ANN, una dependencia más pesada que scikit-learn y que hay que probar en la VM ARM de Oracle. Además, `.keras` no sirve para Random Forest, SVM, AdaBoost ni las regresiones. | Cerrar `ADR-011` / `SPK-06` con: Keras para las redes neuronales (`.keras`), scikit-learn para los demás modelos y un formato definido para estos (`PA-54`). |
| `INC-06` | **Modelos y métricas del módulo de calibración** | **CX §1.5, ES §2.8**: seis modelos (regresión lineal, ANN, Random Forest, SVM, AdaBoost, polinomial), solo **R² de los datos calibrados** y sin valor de aceptación. **DF §3.5, §5.11, `SPK-06`**: RLM y ANN con R², RMSE, MAE y costo computacional. | Cambia lo que se prueba, lo que se muestra y el criterio de éxito del objetivo 4. | Actualizar DF y dejar como pregunta si se quieren RMSE, MAE o costo computacional como métricas internas de la metodología (`Q-15`). |
| `INC-07` | **Formato del mensaje: único o por estación** | **CX §1.4, ES V-27 y `RN-29`, AZ F2.5**: cada estación puede tener su propio formato, configurable desde la plataforma (Must en AZ). **DF §5.9 y §5.1**: un esquema JSON único (`contracts/mqtt/reading.schema.json`, "fuente única") con la versión en el tópico; **DF §3.5** pone V-27 como *Should* y dice que en el MVP el formato vive en el código. | Hay que decidir si el MVP incluye la configuración por estación (necesaria para registrar estaciones caseras con otro formato) o si arranca con un formato base en el código. | Mantener un esquema base versionado en el código para las 5 estaciones de la primera fase y dejar la configuración por estación (V-27) como Should, salvo que se necesite antes para una estación casera. Alinear la prioridad en DF y AZ. |
| `INC-08` | **Plantilla del tópico MQTT** | **DF §5.9**: `idi-fr-05/v1/stations/{station_id}/readings`. **ES V-27**: ejemplo `estaciones/{id_estacion}/lecturas`. | Son dos convenciones distintas para lo mismo. | Unificar en la del DF (inglés, con versión), que sigue la convención de idioma del código (DF §5.1), y corregir el ejemplo de ES. |
| `INC-09` | **Numeración de riesgos `RG-15` a `RG-19`** | **DF §9** define `RG-15` (cierre de Fase 1), `RG-16` (VM de Oracle), `RG-17` (insumos de Ambiental), `RG-18` (contrato con Electrónica) y `RG-19` (doble función del director). **CX §8** usa `RG-15` (límites de Oracle), `RG-16` (reloj de las estaciones) y `RG-17` (cuentas de los servicios). | El mismo identificador nombra riesgos distintos; AZ registra los riesgos de ambos documentos como work items. | Renumerar los riesgos de CX como `RG-20` a `RG-22` (o los de DF) y fusionar los que se repiten (`RG-16` de DF con `RG-15` de CX). |
| `INC-10` | **Quién define los umbrales** | **CX `S-05`, §1.1, `PC-22`**: el investigador principal y el investigador de Ingeniería Ambiental. **DF §1.2 (RACI)**: el equipo propone, el director aprueba y Ambiental solo es consultado; si Ambiental no responde, se aplica una propuesta por defecto. | Es la regla que decide el color del mapa público (`RN-37`). | Confirmar el responsable; si los investigadores los definen, ajustar la RACI de DF y mantener la propuesta por defecto solo como plan de contingencia. |
| `INC-11` | **Cronograma: fechas definidas o por definir** | **DF §7 y AZ §7**: Fase 1 del 1-sep al 4-oct, Fase 2 hasta el 29-nov, con Sprints, Releases e hitos fechados. **CX `R-03` y `PC-12`**: las fechas del trabajo de grado están "por definir". | Uno de los dos está desactualizado. | Si las fechas de DF y AZ son las oficiales, cerrar la parte correspondiente de `PC-12` y dejar abiertos solo la primera estación real (`HT-07`) y el comité (`HT-10`). |
| `INC-12` | **Prioridad de las alertas** | **CX §6, criterio de éxito 2**: la plataforma debe generar alertas por marca de tiempo inválida y detectar estaciones desconectadas (alerta *Tiempo sin conexión*). **DF §3.5 y AZ F2.6**: las alertas (V-16, V-17) son *Should*. | Sin la vista de alertas no se puede demostrar el criterio de éxito del objetivo 2. | Separar en AZ: alertas de integración (marca de tiempo, desconexión, mensaje inválido) como Must, y alertas por umbral y anomalía como Should. |
| `INC-13` | **Prioridad de V-21 (cambio de contraseña)** | **AZ F3.4**: V-21 va con el inicio de sesión (Must). **DF §3.5**: V-21 es *Should*. | Diferencia menor de priorización. | Alinear con AZ, que es más reciente. |
| `INC-14` | **Reserva para imprevistos** | **DF §2 y §8.3**: 15 %. **CX §7.1**: entre el 10 % y el 15 %. | Cambia el techo operativo mensual. | Fijar un solo valor (se propone 15 %) en el plan de costos. |
| `INC-15` | **Frecuencia de envío en las pruebas de carga** | **CX `PC-03`, §7.2**: un mensaje cada 30 s (2 por minuto). **DF `SPK-03`**: 1 lectura por minuto. | La prueba subestimaría a la mitad el volumen real. | Corregir `SPK-03` a 2 mensajes por minuto, o volver a ejecutarlo si ya se hizo. |
| `INC-16` | **Roles de usuario** | **ES §1.2**: solo hay *Visitante* y *Administrador*. **DF §6.4**: las historias pueden escribirse como "investigador". | Un rol "investigador" no existe en la plataforma. | Usar solo *visitante*, *administrador* y *estación* (historias técnicas). |

---

## 2. Referencias desactualizadas (no requieren decisión)

La respuesta ya está tomada en CX o ES; falta reflejarla en el documento indicado.

| ID | Documento | Dónde | Qué dice | Debe decir |
|----|-----------|-------|----------|------------|
| `DES-01` | DF | Encabezado (documentos relacionados), §3.5, §8.4 | Objetivos `OE1`–`OE6`; criterios de `OE3`, `OE4`, `OE5`, `OE6`. | Objetivos `OE1`–`OE4` (numeración de `GC-004`). |
| `DES-02` | DF | Encabezado, §4.2 (`DOC-02`, `DOC-03`) | Especificación v4; contextualización v3. | Especificación v5; contextualización v10. |
| `DES-03` | DF | §0.2 alerta 4, §1.2, regla del investigador ambiental, §2 principio 5, §3.5 (*Must condicionado*), §5.11, §7.3 (Sprint 06), `HT-05`, `RG-17` | ICA (Resolución 2254 de 2017) como entregable o propuesta por defecto. | El ICA está fuera del alcance (`PC-04`). Se retira de todas esas secciones. |
| `DES-04` | DF | §0.2 alerta 2 | 27 vistas. | 25 vistas vigentes (se retiraron V-29 y V-32). |
| `DES-05` | DF | §3.5 (MVP), §7.3 (Sprint 07) | V-29 *Fuentes de referencia* (internas y una externa); V-32 *Modelos y exportación*; *Won't*: aplicar modelos (`PA-53`, `RF-314`). | V-29 y V-32 están retiradas; la descarga del modelo está en V-30; `PA-53` está resuelto (los modelos no se aplican, `RN-35`). |
| `DES-06` | DF | §8.6, párrafo final | "Servicios externos de referencia para la calibración": identificar una fuente externa y probar su API. | No se usan fuentes externas (`PC-10`, `PA-55` descartado). Se retira el párrafo. |
| `DES-07` | DF | §3.5, §7.3, §8.4 | Calibración "por co-ubicación" frente a una estación de referencia. | Datos de 2 o más estaciones registradas, sin exigir que estén en el mismo lugar (`RN-36`). |
| `DES-08` | DF | §9 | "Se suman a `RG-01`–`RG-14`". | Ver `INC-09`. |
| `DES-09` | DF | §8.5 (modelo de lecturas) | "Los valores calibrados nunca reemplazan a los originales." | Los valores calibrados no se guardan (`RN-35`). El campo `quality_flag` puede usarse para marcar las lecturas con marca de tiempo corregida (`RN-33`). |
| `DES-10` | AZ | §6 (áreas) | *Analítica y calibración*: V-29 a V-32. | V-30 y V-31. |
| `DES-11` | AZ | §8.2 | F4.2 *Estaciones de referencia* (V-29); F4.4 *Exportación de modelos* (V-32). | Retirar F4.2 (la selección de estaciones va en V-30) y pasar la descarga del modelo a F4.3. |
| `DES-12` | AZ | §8.2, F2.2 | Requisitos `RN-29`, `PC-02`, `PC-09`. | Agregar `RN-33` (marca de tiempo) y `RN-34` (huecos y desconexión). |
| `DES-13` | AZ | §8.2, F3.1 | V-02, `PC-22`. | Agregar `RN-37` y `PA-56`. |
| `DES-14` | DF | §10.2 #9 | Registrar `GC-002`. | Verificar si ya se registró; si es así, marcar la acción como hecha. |
| `DES-15` | DF | §4.1 | Los documentos pasan de `context/` a `/docs/requisitos/` al crear el repositorio (Sprint 01). | Siguen en `context/`. Confirmar si el repositorio ya existe y cuándo se hace el traslado (`Q-05`). |

---

## 3. Preguntas abiertas por interlocutor

Cada pregunta indica su origen. Las marcadas con ★ bloquean trabajo del Release actual o del MVP.

### 3.1 Director del proyecto / Product Owner

| ID | Pregunta | Origen |
|----|----------|--------|
| `Q-01` ★ | ¿Quiénes son, con nombre, el director del proyecto, el director del trabajo de grado y el investigador principal? ¿Cuáles son la misma persona? | `INC-01`, CX §4, `PC-01` |
| `Q-02` ★ | ¿Se aprueba el MVP de DF §3.5 (con las correcciones de `DES-03` a `DES-07`)? La fecha límite era el 11 de octubre (`HT-03`). | DF §3.5, §10.1 #3 |
| `Q-03` | ¿Se aprueba el documento fundacional como línea base y el ajuste de la Fase 1? | DF §10.1 #1 y #4 |
| `Q-04` | ¿Se aprueba la estructura de Azure DevOps (épicas por objetivo, Features validables)? | `PC-21`, AZ §11 |
| `Q-05` | ¿Ya se creó el repositorio en Azure Repos? ¿Cuándo se trasladan los documentos de `context/` a `/docs`? | DF §4.1, §10.2 #2 |
| `Q-06` | ¿Qué estado tienen los ADR propuestos (`ADR-003` a `ADR-006`, `ADR-013`, `ADR-014`, `ADR-016`) y los spikes `SPK-01` a `SPK-04`, cuya fecha límite era el 11 de octubre? | DF §4.4, §8.6 |
| `Q-07` | ¿Cuál es la dedicación semanal esperada de cada estudiante? DF asume 15 horas (por confirmar). | DF §7.5, §10.1 #8 |
| `Q-08` | ¿Cuáles son las fechas del comité de currículo, el formato del entregable académico y la fecha oficial de cierre del trabajo de grado? | `PC-12`, `INC-11`, `HT-10` |
| `Q-09` | ¿Quién opera la plataforma, paga los servicios y atiende fallas después del trabajo de grado, y desde cuándo cuentan los 24 meses? | `PC-06`, `PC-07`, `R-02` |

### 3.2 Investigador principal

| ID | Pregunta | Origen |
|----|----------|--------|
| `Q-10` ★ | ¿Se acepta la nueva redacción del objetivo 4, que ya no incluye la co-ubicación que menciona el objetivo específico 2 del macroproyecto? ¿Se acepta por escrito la nota de riesgo sobre la calidad de la referencia? | CX §2.2, §1.5, `PC-10` |
| `Q-11` ★ | ¿Cuáles son los modelos mínimos de la primera versión? ¿Se incluye ElasticNet? ¿XGBoost, que estaba en el documento fuente, sale de la lista? | `PCAL-09`, `PA-52`, `PC-17` |
| `Q-12` | ¿La estimación de variables sigue en el alcance? Si se descarta, se retira V-31. | `PC-18`, `PA-52` |
| `Q-13` | ¿Qué formato se usa para exportar los modelos que no son redes neuronales? | `PA-54`, `INC-05` |
| `Q-14` | ¿Qué significa "subir el modelo y calibrar las estaciones"? ¿Se carga en el microcontrolador de la estación? En ese caso, ¿qué formato acepta? | `PA-54`, `PC-18` |
| `Q-15` | ¿Se quiere ver el efecto de cada variable (valor p en los modelos lineales, importancia de variables en los demás)? ¿Se documentan RMSE, MAE o costo computacional en la metodología, aunque no se muestren en la plataforma? | `PA-52`, `INC-06` |
| `Q-16` | ¿El R² se calcula sobre los datos de entrenamiento o sobre un conjunto de prueba separado por bloques de tiempo? | `PA-52`, requerimientos de calibración §6.4 |
| `Q-17` | En el emparejamiento por minuto, ¿se usan los minutos con una sola lectura? ¿Entran las lecturas con marca de tiempo corregida? | `PC-10` |
| `Q-18` | ¿Se guardan todos los datos capturados, o hay datos que no se deben guardar (fuera de rango, con código de error)? | `PC-09` |
| `Q-19` ★ | ¿Los datos se conservan indefinidamente o solo durante los 24 meses? ¿Qué pasa con la plataforma al terminar? | `PC-15` |
| `Q-20` | ¿Cuántas estaciones se esperan en fases posteriores, aunque sea un estimado? | `PC-03` |
| `Q-21` | ¿Los gases reciben el mismo tratamiento que el material particulado (público, umbrales, descarga), o solo se almacenan? | `PC-08` |
| `Q-22` | ¿Cuál es el texto de la cita del macroproyecto durante el desarrollo? ¿Cuándo se hace la consulta al área legal? | `PC-14`, `R-08` |
| `Q-23` | ¿Quién define los umbrales: los investigadores o el equipo con aval del director? | `INC-10`, `PA-24` |
| `Q-24` | Pie de página y documentos legales: titular de los derechos, correo de contacto (¿la cuenta del proyecto?), teléfono, dirección, y quién redacta los términos y la política de datos. | `PA-43` |

### 3.3 Investigador principal e Ingeniería Ambiental: mapa y alertas

| ID | Pregunta | Origen |
|----|----------|--------|
| `Q-25` ★ | Umbrales, niveles y colores del mapa por variable. ¿Son globales o por estación o sensor? | `PC-22`, `PA-24` |
| `Q-26` | ¿El color se calcula con la última lectura o con un promedio (horario, 24 h)? ¿La estación toma el color del nivel más alto entre sus variables? ¿Qué color tiene sin alertas y mientras no haya umbrales? | `PA-56` (a)–(c) |
| `Q-27` | ¿Una estación con un hueco corto (entre 60 s y 10 minutos) mantiene su color? ¿Cómo se muestra una estación con un sensor sin datos? | `PA-56` (d)–(e) |
| `Q-28` | ¿Qué color tiene en el mapa una estación *Inactive*, para no confundirla con el gris de "sin datos"? ¿El visitante sigue viendo el estado de funcionamiento en el cuadro emergente? | `PA-56` (f)–(g), `PA-35` |
| `Q-29` | Criterios cuantitativos de anomalía y si son configurables. | `PA-33` |
| `Q-30` | ¿Se genera una alerta por cada lectura sobre el umbral o una por episodio? | `PA-34` |

### 3.4 Ingeniería Electrónica

| ID | Pregunta | Origen |
|----|----------|--------|
| `Q-31` ★ | Estructura concreta del JSON, con un mensaje de ejemplo, estructura de tópicos y nivel de QoS. ¿Se acepta el borrador v0.1 del contrato (`HT-02`)? | `PC-02`, `PA-50`, `INC-08` |
| `Q-32` | ¿Qué reloj se usará, qué formatos de marca de tiempo produce y cómo se pone en hora (por ejemplo, NTP)? | `PC-02`, `S-02` |
| `Q-33` | ¿Cuánta diferencia con la hora de recepción se tolera antes de considerar una marca desajustada? | `PC-02` |
| `Q-34` ★ | Matriz de valores y umbrales por sensor para los códigos de error. | `PC-02`, `PA-32` |
| `Q-35` | ¿Todos los sensores van en el mismo mensaje como regla del contrato? | `PC-03` |
| `Q-36` | Si Electrónica no tiene acceso a Azure DevOps (DF §1.1), ¿por qué canal se le hacen llegar los Issues? | DF §1.1, §6.5 |

### 3.5 Propietario del dominio (Juan Camilo)

| ID | Pregunta | Origen |
|----|----------|--------|
| `Q-37` | ¿Fecha de la reunión? ¿Será un subdominio? ¿Quién configura los registros DNS hacia Oracle? ¿Hay costo o requisitos de seguridad (HTTPS)? | `PC-07` |

### 3.6 Decisiones del equipo (técnicas y de especificación)

| ID | Pregunta | Origen |
|----|----------|--------|
| `Q-38` ★ | Bróker administrado o propio; comparación de costos con unos 432.000 mensajes al mes; mecanismo de credenciales (afecta V-28). | `INC-02`, `ADR-007`, `PC-07` |
| `Q-39` | ¿Alcanza el plan Always Free para la base de datos y la aplicación? ¿Funcionan Keras y TimescaleDB en ARM? | `INC-03`, `INC-05`, `RG-15` (CX) |
| `Q-40` | Quién guarda las credenciales de la cuenta del proyecto; verificación en dos pasos; quién aporta la tarjeta para Oracle. | `INC-04`, `PC-07` |
| `Q-41` | ¿El correo transaccional sale de la cuenta del proyecto (Gmail) o de otro proveedor? | `ADR-017` |
| `Q-42` | Nombre de la plataforma. | `PA-01` |
| `Q-43` | Estación recién creada: ¿estado *Sin datos* o tiempo de gracia? | `PA-30` |
| `Q-44` | Detalles del estado de los sensores: umbral crítico, variable con error, tiempo para *Offline*, ventana de cálculo. | `PA-31` |
| `Q-45` | Significado del timeout y los reintentos con MQTT. | `PA-49` |
| `Q-46` | ¿Las estaciones *Inactive* y *Hidden* siguen recibiendo datos y generando alertas? ¿Sus datos aparecen en gráficos y descargas? | `PA-35`, `PA-36` |
| `Q-47` | Transiciones faltantes (ocultar una *Inactive*) y restauración de estaciones eliminadas. | `PA-37`, `PA-28` |
| `Q-48` | Gestión de administradores: comunicación de la contraseña, listado y desactivación, primer administrador, ingreso con Microsoft. ¿Se impide que el último administrador elimine su cuenta? | `PA-48`, `PA-40` |
| `Q-49` | Límites y retención de las descargas públicas. | `PA-46` |
| `Q-50` | Evidencias y retención de las alertas. | `PA-47` |
| `Q-51` | Intervalo de actualización y valores de la recuperación de contraseña. | `PA-25`, `PA-29` |
| `Q-52` | Servicio de mapas y geocodificación. | `PA-45`, `SPK-07` |
| `Q-53` | Acceso a la vista de estaciones eliminadas. | `PA-44` |

---

## 4. Respuestas disponibles que aún no se incorporan

El archivo [cambios_correciones_plataforma.md](../cambios_correciones_plataforma.md) ya trae respuestas a varios de estos puntos, pero todavía no se reflejan en los cuatro documentos:

| Punto | Respuesta registrada | Conflicto que hay que revisar al incorporarla |
|-------|----------------------|----------------------------------------------|
| Rol administrador (ES §1.2) | Los administradores registran, actualizan, listan y eliminan a otros administradores; nacen vistas nuevas. | Amplía F3.5 de AZ y responde parte de `PA-48` (`Q-48`). |
| Estados de las cuentas (ES §2.1) | Nuevo estado *New* hasta que el administrador define su contraseña con un enlace de activación. | Reintroduce un flujo parecido al estado *Pending*, que se había descartado (`PA-20`, `PA-41`), y cambia `RF-121` y `RN-12`. |
| `PA-04` | Unidades revisadas contra las hojas técnicas. | Ya incorporado (ES v5). |
| `PA-30` | Estado nuevo *Sin datos* para estaciones recién creadas o sin datos del bróker; en *Inactive*, *Hidden* y *Deleted* se deja de escuchar o consultar el bróker. | Responde `Q-43` y parte de `Q-46`. El estado *Sin datos* se cruza con el gris de "sin datos" del mapa (`PA-56`). |
| `PA-36` | El histórico de las estaciones *Hidden* y *Deleted* no se grafica ni se descarga desde el cambio de estado. | Responde parte de `Q-46`. |
| `PA-46` | Captcha en la descarga, retención de 12 horas, solo se conserva el último archivo y límite de descargas por IP. | Responde `Q-49`. |
| `PA-49` | Reemplazar el timeout y los reintentos por un tiempo máximo sin mensajes por estación y por la detección de desconexión entre la API y el bróker. | Responde `Q-45` y es coherente con los 10 minutos de `RN-34`. |
