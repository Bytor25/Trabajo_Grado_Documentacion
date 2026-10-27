# Documento fundacional: decisiones y plan de trabajo

**Proyecto:** plataforma web para la visualización informativa de una iniciativa IoT de calidad del aire (macroproyecto **IDI-FR-05**, UCO).
**Tipo de documento:** Project Charter con registro de decisiones de arquitectura (ADR) y plan de trabajo.
**Versión:** 1 (preliminar, sujeta a validación con el director del proyecto).
**Fecha:** 2026-10-01.
**Estado:** borrador para revisión en la Sprint Review del Sprint 01.
**Documentos relacionados:**

- [contextualizacion.md](contextualizacion.md): contexto, objetivos (`OE1`–`OE6`), alcance, supuestos (`S-xx`), restricciones (`R-xx`), riesgos (`RG-xx`) y pendientes de contexto (`PC-xx`).
- [plataforma_web_descripcion.md](plataforma_web_descripcion.md): especificación funcional v4 (`V-xx`, `C-xx`, `RF-xx`, `RN-xx`, `RNF-xx`, `PA-xx`).
- [contexto_azure_devops.md](contexto_azure_devops.md): configuración vigente de Azure DevOps.
- [gestion_cambios.md](gestion_cambios.md): registro de cambios de los documentos controlados (`GC-xxx`).

**Cómo leer este documento.** Es la fuente de la verdad sobre *cómo* se trabaja en el proyecto y *qué* se ha decidido técnicamente. Lo que ya está en los documentos relacionados no se repite: se referencia por su identificador. Cada decisión tiene un estado:

| Estado | Significado |
|--------|-------------|
| **Aceptada** | Decidida y en uso. Solo cambia mediante un ADR nuevo que la reemplace. |
| **Propuesta** | Recomendación del equipo con buen sustento; pendiente del aval del director. |
| **Abierta** | Requiere validación técnica (spike) o información de terceros antes de decidir. Tiene fecha límite. |

Identificadores nuevos que introduce este documento: `ADR-xxx` (decisión de arquitectura o de proceso), `SPK-xx` (spike de validación técnica), `HT-xx` (hito), `CP-xxx` (caso de prueba) y `RG-15` en adelante (riesgos nuevos).

---

## 0. Resumen ejecutivo

### 0.1 Decisiones principales

| Tema | Decisión | Estado |
|------|----------|--------|
| Metodología | Scrum adaptado a 2 developers, Sprints de 1 semana, Releases cada 2 Sprints, plantilla Agile de Azure DevOps (`ADR-001`). | Aceptada |
| Gestión del trabajo y código | Azure Boards para el trabajo; GitHub para el código, en un **monorepo** (`ADR-002`, `ADR-014`). | Aceptada / Propuesta |
| Ramas | **GitFlow simplificado:** `main` + `develop` + `feature/*` + `hotfix/*`, sin ramas `release/*` salvo que se necesite estabilizar (`ADR-003`). | Propuesta |
| Arquitectura | **Monolito modular** en contenedores, con procesos separados para API, ingesta MQTT y tareas en segundo plano (`ADR-016`). | Propuesta |
| Backend | Python 3.12 + FastAPI + SQLAlchemy 2 + Alembic (`ADR-004`). | Propuesta |
| Frontend | React + Vite + **TypeScript** (`ADR-005`). | Propuesta |
| Base de datos | PostgreSQL + TimescaleDB **autoalojado**, con modelo de lecturas en formato largo (`ADR-006`). | Propuesta (validar en `SPK-03`) |
| Gráficas | **Ni Matplotlib ni Grafana para las vistas públicas:** se recomienda una librería de gráficas en el frontend (Apache ECharts como candidata). Matplotlib queda para figuras de la tesis y reportes de modelos; Grafana, como opción de monitoreo interno (`ADR-008`). | Abierta (decidir en `SPK-04`, 11-oct) |
| Despliegue | Oracle Cloud *Always Free* (VM Ampere A1, ARM) como opción principal; VPS de bajo costo como respaldo (`ADR-009`). | Abierta (decidir en `SPK-01`, 11-oct) |
| Bróker MQTT | Falta en el stack preliminar. Candidato: Mosquitto 2 con *dynamic security* (`ADR-007`). | Abierta (decidir en `SPK-02`, 11-oct) |

### 0.2 Alertas para el director

1. **La Fase 1 termina el 4 de octubre y quedan 4 días.** Los entregables de diseño no caben completos en ese plazo. Se propone cerrar la Fase 1 con una **línea base de planeación** (sección 7.2) y terminar las decisiones de stack como spikes con fecha límite en el Sprint 02 (diseño continuo, propio del enfoque ágil).
2. **El alcance completo (27 vistas, MQTT, analítica y despliegue) no cabe en 8 Sprints de dedicación parcial** (`RG-07`). Hay que acordar un **producto mínimo (MVP)** con el director antes del 11 de octubre (sección 3.5).
3. **Falta el objetivo de analítica en Azure DevOps.** La contextualización define 6 objetivos específicos, pero el campo `Objetivo específico` solo llega hasta `OE5`, no hay épica para la calibración (`OE6`) y sigue existiendo el área `PQRS`, que fue retirada (sección 6.1).
4. **Dependencias externas sin dueño:** el contrato MQTT depende de Ingeniería Electrónica y el ICA, los umbrales y la calibración dependen de Ingeniería Ambiental, cuyo investigador no participa directamente. Se proponen fechas límite y planes alternativos para cada una (sección 10).

---

## 1. Equipo, roles y gobierno

### 1.1 Roles

| Persona | Rol en el proyecto | Rol en Scrum | Rol en Azure DevOps |
|---------|--------------------|--------------|---------------------|
| Estudiante 1 | Developer / Tester | Developer + Scrum Master | Investigador 1 y Estudiante 1 son *Project Collection Administrators* |
| Estudiante 2 | Developer / Tester | Developer | Contributors + Build Administrators |
| Director del proyecto (investigador de Sistemas) | Stakeholder principal, tutor del trabajo de grado, revisor de cada etapa y responsable del aval ante el comité de currículo | **Product Owner** | "Investigador 1" en [contexto_azure_devops.md](contexto_azure_devops.md) |
| Investigador de Ingeniería Ambiental | Parte del macroproyecto; sin participación directa en el desarrollo | Stakeholder (consultado) | "Investigador 2" |
| Ingeniería Electrónica | Contraparte del contrato de integración MQTT (`PC-02`) | Stakeholder externo | Sin acceso (se gestiona por Issues) |

**Decisión sobre el Product Owner (Propuesta):** con la información del equipo, el director asume el rol de Product Owner. Esto cierra el pendiente "Product Owner único" de [contexto_azure_devops.md](contexto_azure_devops.md) §11.

**Riesgo de la doble función del director.** El director es a la vez PO (acepta el producto) y evaluador académico (avala el trabajo de grado). Para no mezclar los dos papeles, cada Sprint Review separa explícitamente dos decisiones: la **aceptación de las historias** (como PO) y las **observaciones académicas** (como director), registradas en el acta del Release.

### 1.2 Matriz de responsabilidades (RACI resumida)

R = responsable, A = aprueba, C = consultado, I = informado.

| Actividad | Estudiantes | Director (PO) | Ambiental | Electrónica |
|-----------|:-----------:|:-------------:|:---------:|:-----------:|
| Priorizar el backlog y definir el MVP | R | A | I | I |
| Especificación funcional y mockups | R | A | C | I |
| Contrato de integración MQTT | R | A | I | C |
| ICA, umbrales, criterios de anomalía (`PC-04`, `PA-24`, `PA-33`) | R (propone) | A | C | — |
| Metodología de calibración (`PC-17`, `PC-19`) | R | A | C | C |
| Decisiones de arquitectura (ADR) | R | A | — | — |
| Aceptación de historias | R | A | — | — |
| Documento de trabajo de grado | R | A | — | — |

**Regla para el investigador ambiental.** Como no participa en el día a día, se le consulta solo por Issues concretos (`PC-04`, `PA-24`, `PA-33`, `PC-17`, `PC-19`) con fecha límite. Si no responde a tiempo, el equipo aplica una **propuesta por defecto documentada** (por ejemplo, el ICA de la Resolución 2254 de 2017) que el director avala, y la marca como pendiente de validación.

---

## 2. Principios técnicos y de gestión

Estos principios guían las decisiones de este documento y las futuras:

1. **Primero lo que cumple los objetivos del macroproyecto.** El orden de valor es: ingesta → consulta pública → exportación → administración → analítica → alertas → complementos (contextualización §5).
2. **Simplicidad operativa sobre sofisticación.** Dos developers con dedicación parcial deben poder operar todo, y alguien más debe poder heredarlo (`RG-04`). Se evita agregar servicios que no sean imprescindibles.
3. **Costo predecible.** Toda decisión de infraestructura se evalúa con su costo proyectado a 24 meses frente a $2.000.000 COP, con una reserva del 15 % (`R-01`).
4. **Portabilidad.** Todo corre en contenedores, para cambiar de proveedor sin reescribir (`RG-08`).
5. **Decidir en el último momento responsable.** Lo que depende de información pendiente (analítica, ICA, LoRa) se deja abierto con una fecha límite, en lugar de decidirlo a ciegas.
6. **Trazabilidad demostrable para la sustentación:** objetivo → épica → feature → historia (`RF`) → commit/PR → caso de prueba → resultado.

---

## 3. Metodología y artefactos

### 3.1 Marco de trabajo (`ADR-001`, Aceptada)

Scrum adaptado, ejecutado con la plantilla **Agile** de Azure DevOps (proceso heredado `Agile-IDI-FR-05`). La justificación completa está en [contexto_azure_devops.md](contexto_azure_devops.md) §2: equipo pequeño, criticidad baja y requisitos con alto dinamismo (Boehm y Turner, 2003).

| Parámetro | Valor |
|-----------|-------|
| Duración del Sprint | 1 semana (lunes a domingo; se trabaja de lunes a viernes) |
| Release | Cada 2 Sprints, con tag de versión en GitHub y acta en la Wiki |
| Excepción | El Release 05 tiene un solo Sprint (Sprint 09, 23 al 29 de noviembre) porque la Fase 2 termina el 29 de noviembre |
| Límite de trabajo en curso | 2 en *En desarrollo* y 2 en *En revisión de código* |

**Ajuste pendiente:** la sección 5 de [contextualizacion.md](contextualizacion.md) todavía dice "iteraciones de dos semanas". Debe actualizarse con un `GC-xxx` para cerrar `PC-11`.

### 3.2 Cadencia semanal

| Día | Evento | Duración | Participantes |
|-----|--------|----------|---------------|
| Lunes | Sprint Planning | ≤ 1,5 h | Estudiantes (el PO aprueba el objetivo del Sprint de forma asíncrona si no asiste) |
| Lunes a viernes | Daily Scrum (puede ser asíncrono en un canal) | 15 min | Estudiantes |
| Miércoles | Refinamiento del backlog | 45 min | Estudiantes; el PO cuando haya historias por aclarar |
| Viernes | Sprint Review | 30–45 min | Estudiantes y PO. Cada 2 Sprints es la revisión del Release |
| Viernes | Retrospectiva | 30 min | Estudiantes |

Cuando el lunes es festivo (12-oct, 2-nov y 16-nov), el Sprint Planning se hace el martes.

### 3.3 Artefactos del ciclo de vida

| Artefacto | Propósito | Dónde vive | Responsable |
|-----------|-----------|------------|-------------|
| Product Backlog | Lista priorizada de todo el trabajo | Azure Boards (Epics → Features → User Stories) | Estudiante 1 lo mantiene; el PO prioriza |
| Sprint Backlog y objetivo del Sprint | Compromiso de la semana | Azure Boards (iteración del Sprint) | Estudiantes |
| Incremento | Software funcionando y desplegado en el entorno de pruebas | GitHub + entorno `pruebas` | Estudiantes |
| Definition of Ready / Definition of Done | Criterios de calidad de entrada y salida | Wiki `Acuerdos de trabajo` y sección 3.4 | Estudiantes; avala el PO |
| Roadmap | Planeación de Releases e hitos | Delivery Plans "Roadmap IDI-FR-05" | Estudiante 1 |
| Registro de riesgos | Riesgos `RG-xx` con probabilidad, impacto y respuesta | Work item `Riesgo` en Azure Boards | Estudiante 1 |
| Registro de puntos abiertos | `PA-xx` y `PC-xx` | Work item `Issue` en Azure Boards | Quien gestiona cada Issue |
| Registro de decisiones | `ADR-xxx` | `/docs/adr` del repositorio (sección 4.4) | Estudiantes; avala el director |
| Registro de cambios de requisitos | `GC-xxx` | [gestion_cambios.md](gestion_cambios.md) | Estudiantes |
| Actas | Acuerdos de cada Review de Release y de cada reunión con investigadores | Wiki `Actas/Release-NN` | Scrum Master |
| Tablero de seguimiento | Burndown, velocidad, flujo acumulado, *cycle time* | Dashboard "Seguimiento del proyecto" | Estudiante 1 |
| Matriz de trazabilidad | `OE` → `RF` → historia → `CP-xxx` → resultado | `/docs/pruebas/matriz_trazabilidad.md` | Estudiantes |
| CHANGELOG y tags | Qué cambió en cada versión del software | `CHANGELOG.md` + tags SemVer en GitHub | Estudiantes |

### 3.4 Definition of Ready y Definition of Done

La DoR y la DoD de [contexto_azure_devops.md](contexto_azure_devops.md) §8 se mantienen y se precisan así (Propuesta):

**Definition of Ready (historia lista para entrar a un Sprint):**

- Tiene la forma "Como `<rol>`, quiero… para…" y criterios de aceptación *Dado / Cuando / Entonces*.
- Tiene el campo `Requisitos asociados` (`RF-xx`, `RN-xx`, `RNF-xx`) diligenciado.
- Está estimada en Story Points (≤ 8) y cabe en un Sprint.
- No tiene un `PA`/`PC` abierto que la bloquee, o el bloqueo tiene una propuesta por defecto avalada.
- Si tiene interfaz, el mockup de la vista existe o la historia indica que se construye sin mockup.

**Definition of Done (historia terminada):**

- El código pasa el *linter*, el formateador y la verificación de tipos en el pipeline (sección 5.7 y 5.8).
- Hay pruebas automatizadas de la funcionalidad nueva y todas las pruebas pasan.
- El PR fue revisado y aprobado por el otro developer y fusionado en `develop`.
- Si cambia la base de datos, incluye su migración de Alembic.
- Si cambia la API, el esquema OpenAPI está actualizado.
- No hay secretos en el repositorio.
- La documentación afectada está actualizada (README, `/docs`, manual).
- Está desplegada en el entorno `pruebas` y los criterios de aceptación se verificaron allí.
- El PO la aceptó (columna *Validación PO* → *Hecho*).

### 3.5 Gestión del alcance: MVP propuesto (MoSCoW)

La priorización MoSCoW (*Must, Should, Could, Won't*) se usa para proteger el cronograma. Lo que está en **Must** es lo mínimo para cumplir los objetivos `OE3`, `OE4`, `OE5` y `OE6`; el resto entra según la velocidad real. **Este MVP debe acordarse con el director antes del 11 de octubre (`HT-03`).**

| Prioridad | Funcionalidad | Vistas o referencias |
|-----------|---------------|----------------------|
| **Must** | Contrato MQTT v1, bróker con credenciales por estación, ingesta, validación y almacenamiento | `RN-28`, `RN-29`, `RNF-09` |
| **Must** | Simulador de estaciones | `S-03` |
| **Must** | Catálogo de sensores precargado y asignación de sensores a estaciones | V-25 (versión básica), V-26 |
| **Must** | Consulta pública: mapa, listado, detalle, gráficos y tablas | V-02, V-03, V-04 |
| **Must** | Exportación CSV/JSON con límites de uso | V-05, `RNF-10` |
| **Must** | Inicio de sesión de administradores y recuperación de contraseña | V-01, V-10 |
| **Must** | Crear, editar, deshabilitar/ocultar y eliminar estaciones; regenerar credenciales | V-06, V-07, V-08, V-14, V-28 |
| **Must** | Estados automáticos de estaciones y sensores | `RN-15`, `RN-16` |
| **Must** | Calibración: comparación de RLM y ANN para al menos una variable (PM2.5) y exportación del modelo | V-30, V-32 (básicas), criterio de éxito del `OE6` |
| **Must** | Despliegue en producción con respaldos probados | `OE5` |
| **Must (condicionado)** | ICA: si `PC-04` no se resuelve antes del 25-oct, se implementa el ICA de la Resolución 2254 de 2017 propuesto por el equipo, con aviso de datos indicativos | `OE4`, `PC-04` |
| **Should** | Alertas por umbral y su detalle | V-16, V-17 |
| **Should** | Configuración de umbrales e intervalo de actualización | V-15 |
| **Should** | Registro de administradores y cambio de contraseña | V-13, V-21 |
| **Should** | Fuentes de referencia (internas y una externa) | V-29 |
| **Should** | Formato del mensaje MQTT configurable desde la interfaz | V-27 (en el MVP, el formato vive en un esquema versionado en el código) |
| **Could** | Estimación de variables | V-31 |
| **Could** | Estaciones eliminadas, perfil, edición de perfil, eliminación de cuenta | V-18, V-11, V-12, V-19 |
| **Could** | Alertas por anomalías, versiones del formato del mensaje, matriz de correlación | `PA-33`, `RF-264`, `RF-301` |
| **Won't (en este trabajo de grado)** | Aplicar modelos de calibración a las lecturas en vivo | `PA-53`, `RF-314` |
| **Won't** | Datos de microplásticos, notificaciones por correo, aplicación móvil | `PC-08`, `RN-22` |

### 3.6 Control de cambios del alcance

Todo cambio de alcance después de la línea base del 4 de octubre sigue este flujo:

1. Se registra como Issue en Azure Boards (origen: reunión, correo o hallazgo).
2. El equipo estima su impacto en Story Points y en fechas, y propone qué sale del Sprint o del MVP para compensarlo.
3. El PO decide en la siguiente Review (o de forma asíncrona si es urgente).
4. Si se aprueba, se registra un `GC-xxx` en [gestion_cambios.md](gestion_cambios.md), se actualizan los documentos afectados y el backlog.

---

## 4. Plan de documentación

### 4.1 Principios

- **Documentación como código (*docs-as-code*):** la documentación técnica vive en Markdown dentro del repositorio, se versiona con el código y se revisa en los mismos PR.
- **La Wiki de Azure DevOps** se usa para lo que no está atado a una versión del código: actas, acuerdos de trabajo e índices.
- **Documentación suficiente (Ambler, 2002; Rüping, 2003):** cada documento existe porque alguien lo necesita (el director, el comité, quien opere la plataforma o el propio equipo).
- **Un documento, un dueño.** Cada documento tiene un responsable y una fecha de la primera versión.

**Ajuste propuesto sobre los ADR:** [contexto_azure_devops.md](contexto_azure_devops.md) ubica los ADR en la Wiki. Se propone que los ADR vivan en `/docs/adr` del repositorio (se revisan por PR y quedan junto al código que justifican) y que la página `Decisiones de arquitectura` de la Wiki sea un índice con enlaces.

**Ubicación de los documentos de contexto:** mientras no exista el repositorio, los documentos de requisitos siguen en `context/`. Al crear el repositorio (Sprint 01), se mueven a `/docs/requisitos/` y desde entonces se modifican solo por PR.

### 4.2 Mapa documental

| ID | Documento | Contenido | Referente | Ubicación | Responsable | Primera versión |
|----|-----------|-----------|-----------|-----------|-------------|-----------------|
| DOC-01 | Documento fundacional (este) | Metodología, decisiones, plan de trabajo | PMBOK (Project Charter); Nygard (2011) | `/docs/gestion/` | Estudiante 1 | 2026-10-01 |
| DOC-02 | Contextualización | Contexto, objetivos, alcance, supuestos, restricciones | — | `/docs/requisitos/` | Ambos | Existe (v3) |
| DOC-03 | Especificación funcional | Vistas, `RF`, `RN`, `RNF`, `PA` | ISO/IEC/IEEE 29148:2018 | `/docs/requisitos/` | Ambos | Existe (v4) |
| DOC-04 | Gestión de cambios | `GC-xxx` | — | `/docs/requisitos/` | Ambos | Existe |
| DOC-05 | Plan de gestión de riesgos | Metodología, matriz y seguimiento de `RG-xx` | PMBOK | `/docs/gestion/` + work items `Riesgo` | Estudiante 1 | Sprint 01 |
| DOC-06 | Plan de recursos y costos | Proyección a 24 meses, consumo real, reserva | — | `/docs/gestion/` | Estudiante 2 | Sprint 02 (tras `SPK-01`) |
| DOC-07 | Documento de arquitectura | Contexto, contenedores, componentes, despliegue, decisiones transversales | Modelo C4 (Brown); plantilla arc42 | `/docs/arquitectura/` | Estudiante 1 | v0.9 en Sprint 01; v1.0 en Sprint 02 |
| DOC-08 | Registro de decisiones | `ADR-xxx` | Nygard (2011) | `/docs/adr/` | Ambos | Sprint 01 |
| DOC-09 | Modelo de datos | Diagrama entidad-relación, diccionario de datos, política de retención y agregados | — | `/docs/arquitectura/datos/` | Estudiante 2 | v0.9 en Sprint 01 |
| DOC-10 | Contrato de integración MQTT | Tópicos, esquema JSON del mensaje, QoS, credenciales, errores, LWT, ejemplos | AsyncAPI (opcional) | `/contracts/mqtt/` + `/docs/arquitectura/` | Estudiante 1 | v0.1 en Sprint 01 (borrador para Electrónica) |
| DOC-11 | Especificación de la API | Endpoints REST | OpenAPI 3 (generado por FastAPI) | Generado + `/docs/api/` | Ambos | Sprint 02 |
| DOC-12 | Mockups | Vistas públicas y de administración | — | Figma (enlace en `/docs/diseno/`) | Estudiante 2 | Públicas en Sprint 02; administración en Sprint 04 |
| DOC-13 | Convenciones de desarrollo | Normativas de la sección 5 | PEP 8, PEP 257, Conventional Commits | `CONTRIBUTING.md` | Ambos | Sprint 01 |
| DOC-14 | Plan de pruebas | Estrategia, niveles, herramientas, criterios de entrada y salida | ISO/IEC/IEEE 29119-3 | `/docs/pruebas/` | Estudiante 2 | v0.9 en Sprint 01 |
| DOC-15 | Casos de prueba y matriz de trazabilidad | `CP-xxx` ↔ `RF` ↔ historia | ISO/IEC/IEEE 29119-3 | `/docs/pruebas/` | Ambos | Incremental desde Sprint 02 |
| DOC-16 | Informe de pruebas | Resultados por Release, cobertura, defectos | ISO/IEC/IEEE 29119-3 | `/docs/pruebas/informes/` | Ambos | Cada Release |
| DOC-17 | Metodología de calibración | Diseño experimental, datos, modelos, métricas, resultados | — | `/docs/analitica/` + `/analytics/notebooks/` | Ambos | Diseño en Sprint 03; resultados en Sprint 08 |
| DOC-18 | Manual de despliegue y operación | Instalación, variables, respaldos y restauración, monitoreo, incidentes | — | `/docs/operacion/` | Estudiante 1 | Sprint 06; final en Sprint 09 |
| DOC-19 | Plan de operación y mantenimiento a 24 meses | Responsables, tareas periódicas, costos, actualizaciones, fin de vida | ISO/IEC 14764 (mantenimiento) | `/docs/operacion/` | Ambos | Sprint 08 |
| DOC-20 | Manual de usuario | Visitante y administrador, con capturas | ISO/IEC/IEEE 26514 | `/docs/manuales/` | Estudiante 2 | Sprint 08; final en Sprint 09 |
| DOC-21 | Plan de evaluación del producto | Criterios de éxito por `OE`, aceptación, usabilidad (cuestionario SUS) | Brooke (1996), SUS | `/docs/pruebas/` | Estudiante 2 | Sprint 03 |
| DOC-22 | Actas | Reuniones con investigadores y Reviews de Release | — | Wiki `Actas/` | Scrum Master | Continuo |
| DOC-23 | Documento del trabajo de grado | Informe académico | Formato del programa (`PC-11`) | Según el programa | Ambos | Continuo (tag `tesis`) |

### 4.3 Estructura de `/docs`

```
docs/
├── README.md                  índice de la documentación
├── gestion/                   documento fundacional, riesgos, costos
├── requisitos/                contextualización, especificación, gestión de cambios
├── adr/                       0001-metodologia.md, 0002-..., plantilla.md
├── arquitectura/              arquitectura.md, diagramas C4, datos/
├── diseno/                    enlaces a Figma, guía visual (colores de estados)
├── api/                       guía de uso de la API (el contrato lo genera FastAPI)
├── analitica/                 metodología de calibración y resultados
├── pruebas/                   plan, casos, matriz de trazabilidad, informes/
├── operacion/                 manual de despliegue, plan de operación, runbooks
└── manuales/                  manual de usuario
```

### 4.4 Bitácora de decisiones (ADR)

**Formato** (Nygard, 2011), en `/docs/adr/NNNN-titulo-corto.md`:

```markdown
# ADR-NNNN: <título>

- Estado: Propuesta | Aceptada | Rechazada | Reemplazada por ADR-XXXX
- Fecha: AAAA-MM-DD
- Decisores: <nombres>
- Relacionado con: <RF, RNF, PC, PA, RG, SPK>

## Contexto
<problema, fuerzas y restricciones>

## Opciones consideradas
<opción A, B, C con pros y contras>

## Decisión
<qué se decide y por qué>

## Consecuencias
<positivas, negativas y lo que queda por hacer>
```

**Registro inicial:**

| ID | Decisión | Estado | Se decide en |
|----|----------|--------|--------------|
| `ADR-001` | Scrum adaptado con la plantilla Agile, Sprints de 1 semana y Releases de 2 | Aceptada | — |
| `ADR-002` | Azure Boards para el trabajo y GitHub para el código | Aceptada | — |
| `ADR-003` | GitFlow simplificado | Propuesta | Sprint 01 |
| `ADR-004` | Backend en Python 3.12 con FastAPI | Propuesta | Sprint 01 |
| `ADR-005` | Frontend en React + Vite + TypeScript | Propuesta | Sprint 01 |
| `ADR-006` | PostgreSQL + TimescaleDB autoalojado; modelo de lecturas en formato largo | Propuesta | `SPK-03`, Sprint 02 |
| `ADR-007` | Bróker MQTT | Abierta | `SPK-02`, Sprint 02 |
| `ADR-008` | Librería de visualización de datos | Abierta | `SPK-04`, Sprint 02 |
| `ADR-009` | Infraestructura de despliegue | Abierta | `SPK-01`, Sprint 02 |
| `ADR-010` | Procesamiento en segundo plano (exportaciones, entrenamientos, estados) | Abierta | `SPK-05`, Sprint 04 |
| `ADR-011` | Librerías de aprendizaje automático y formato de exportación de modelos | Abierta | `SPK-06`, Sprint 04 |
| `ADR-012` | Mapas y geocodificación | Abierta | `SPK-07`, Sprint 03 |
| `ADR-013` | Autenticación y sesiones de administradores | Propuesta | Sprint 04 |
| `ADR-014` | Monorepo | Propuesta | Sprint 01 |
| `ADR-015` | Análisis estático de código y visibilidad del repositorio | Abierta | Sprint 01 (depende del director) |
| `ADR-016` | Monolito modular con procesos separados | Propuesta | Sprint 01 |
| `ADR-017` | Proveedor de correo para la recuperación de contraseña | Abierta | Sprint 04 |

### 4.5 Control de cambios

Hay tres registros, cada uno para un tipo de cambio:

| Qué cambia | Dónde se registra | Formato |
|------------|-------------------|---------|
| Requisitos, alcance y contexto | [gestion_cambios.md](gestion_cambios.md) | `GC-xxx` con origen, decisiones, interpretaciones y cambios por documento (convención vigente) |
| Decisiones técnicas o de proceso | `/docs/adr/` | Un ADR nuevo; los anteriores no se editan, se marcan como *Reemplazados* |
| Software | `CHANGELOG.md` + tags de GitHub | *Keep a Changelog* + SemVer, por Release |

### 4.6 Bitácora de trabajo

- **Horas:** cada Task registra *Original Estimate*, *Remaining Work* y *Completed Work*. Es la evidencia de esfuerzo para el informe final.
- **Avance:** el dashboard de Azure DevOps (burndown, velocidad, flujo acumulado).
- **Acuerdos:** las actas de la Wiki.
- **Exportación para la tesis:** la consulta en árbol Epics → Features → Stories exportada a CSV al cierre de cada Release ([contexto_azure_devops.md](contexto_azure_devops.md) §9).

---

## 5. Flujo de trabajo y normativas de desarrollo

### 5.1 Repositorio (`ADR-014`, Propuesta: monorepo)

Un solo repositorio en GitHub, porque la conexión de Azure Boards está limitada a un repositorio, el contrato MQTT se comparte entre el backend y el simulador, y un PR puede cambiar a la vez la API y la vista que la consume.

```
plataforma-idi-fr-05/
├── backend/                      FastAPI, ingesta MQTT, workers, analítica
│   ├── pyproject.toml
│   ├── uv.lock
│   ├── alembic/                  migraciones
│   ├── src/app/
│   │   ├── main.py               aplicación FastAPI
│   │   ├── core/                 configuración, seguridad, logging
│   │   ├── db/                   sesión, modelos SQLAlchemy
│   │   ├── modules/              un paquete por dominio:
│   │   │   ├── stations/         router.py, schemas.py, service.py, repository.py
│   │   │   ├── sensors/
│   │   │   ├── readings/
│   │   │   ├── exports/
│   │   │   ├── auth/
│   │   │   ├── alerts/
│   │   │   └── analytics/
│   │   ├── ingestion/            suscriptor MQTT (proceso independiente)
│   │   └── workers/              tareas en segundo plano (proceso independiente)
│   └── tests/                    unit/, integration/
├── frontend/                     React + Vite + TypeScript
│   ├── package.json
│   └── src/
│       ├── app/                  rutas, proveedores, layout (C-09, C-10)
│       ├── features/             map/, stations/, exports/, admin/, analytics/
│       ├── components/           componentes reutilizables (C-01 a C-12)
│       ├── api/                  cliente generado desde OpenAPI
│       └── lib/
├── simulator/                    simulador de estaciones (Python)
├── contracts/mqtt/               esquema JSON del mensaje y ejemplos (fuente única)
├── analytics/notebooks/          exploración y prototipos de calibración
├── infra/                        docker-compose, Mosquitto, proxy, scripts de respaldo
├── docs/                         sección 4.3
├── azure-pipelines.yml
├── .github/                      PULL_REQUEST_TEMPLATE.md, CODEOWNERS
├── .editorconfig
├── .gitattributes                * text=auto eol=lf (el equipo trabaja en Windows)
├── .pre-commit-config.yaml
├── CONTRIBUTING.md
├── CHANGELOG.md
└── README.md
```

**Idioma (Propuesta):** identificadores de código, nombres de tablas y valores de estado en **inglés** (coherente con los estados de la especificación: `Active`, `Warning`, `Offline`); comentarios, docstrings, documentación, mensajes de commit y textos de la interfaz en **español**.

### 5.2 Estrategia de ramas (`ADR-003`, Propuesta: GitFlow simplificado)

| Rama | Propósito | Se crea desde | Se fusiona en | Despliega en |
|------|-----------|---------------|---------------|--------------|
| `main` | Código en producción; cada fusión es un Release con tag | — | — | `produccion` (aprobación manual) |
| `develop` | Integración del Sprint | `main` (una vez) | `main` al cerrar el Release | `pruebas` (automático) |
| `feature/<id>-<descripcion>` | Una User Story o Bug | `develop` | `develop` | — |
| `hotfix/<id>-<descripcion>` | Corrección urgente en producción | `main` | `main` y `develop` | `produccion` |
| `docs/<id>-<descripcion>` | Cambios solo de documentación | `develop` | `develop` | — |

Ejemplo: `feature/123-ingesta-validacion-mensaje`, donde `123` es el ID del work item.

**Justificación.** GitFlow (Driessen, 2010) separa lo que está en producción (`main`) de lo que se integra (`develop`), lo que encaja con la regla "la historia se valida en `pruebas` antes de ir a producción" de la DoD y con los Releases cada dos semanas. El propio Driessen (2020) advierte que GitFlow completo es excesivo para software de entrega continua; por eso se omiten las ramas `release/*`: con dos developers, el cierre del Release es un PR `develop` → `main`. Si al final de un Release hace falta estabilizar mientras se sigue desarrollando, se crea `release/vX.Y.0` como excepción.

**Reglas:**

- Las ramas `feature/*` viven como máximo un Sprint; si una historia no cabe, se divide.
- Fusión `feature/*` → `develop`: **squash merge** (un commit por historia, historial limpio).
- Fusión `develop` → `main`: **merge commit** (conserva el historial del Release), seguido del tag.
- Se borra la rama después de fusionarla.

**Protección de ramas en GitHub** (`main` y `develop`): PR obligatorio, 1 aprobación, *status checks* del pipeline obligatorios, sin *force push*. **Atención:** GitHub solo aplica las reglas de protección en repositorios **privados** con un plan de pago (Pro o Team). Opciones: hacer público el repositorio, o alojarlo en una cuenta con GitHub Pro (incluido en el GitHub Student Developer Pack). Se decide junto con `ADR-015`.

### 5.3 Commits

Formato **Conventional Commits** con el ID del work item:

```
<tipo>(<alcance>): <descripción en español, en imperativo>

<cuerpo opcional: qué y por qué>

AB#123
```

- **Tipos:** `feat`, `fix`, `docs`, `test`, `refactor`, `perf`, `build`, `ci`, `chore`.
- **Alcances:** `ingesta`, `api`, `web`, `bd`, `simulador`, `analitica`, `infra`, `docs`.
- Ejemplo: `feat(ingesta): rechazar mensajes con variables no asignadas a la estación`.
- `Fixes AB#123` en la descripción del PR mueve el work item al fusionar. Como la DoD exige la validación del PO, se recomienda usar `AB#123` (solo enlace) en los PR hacia `develop` y que la historia la cierre el PO.

### 5.4 Pull requests y revisión de código

- Plantilla en `.github/PULL_REQUEST_TEMPLATE.md` con: work item (`AB#`), qué cambia, cómo probarlo, capturas (si hay interfaz), lista de la DoD.
- Tamaño recomendado: menos de 400 líneas cambiadas; si es más grande, dividir.
- El revisor responde en menos de 24 horas hábiles (con WIP 2 en revisión, una revisión lenta bloquea el flujo).
- Qué revisa: que cumpla los criterios de aceptación, la legibilidad, las pruebas, la seguridad (validación de entradas, permisos en el servidor, secretos) y el rendimiento de las consultas.

### 5.5 Versionado y Releases

- **SemVer** (Preston-Werner): durante el trabajo de grado, `0.Y.Z` (Release 01 → `v0.1.0`, Release 02 → `v0.2.0`…). La entrega final es `v1.0.0`.
- Al cerrar cada Release: PR `develop` → `main`, tag, entrada en `CHANGELOG.md`, despliegue a `produccion` (desde el Release 03) y acta en la Wiki.

### 5.6 Integración y despliegue continuos (Azure Pipelines)

| Etapa | Se ejecuta en | Qué hace |
|-------|---------------|----------|
| Verificación | Todo PR | Backend: `ruff check`, `ruff format --check`, `mypy`, `pytest` con cobertura. Frontend: `eslint`, `prettier --check`, `tsc --noEmit`, `vitest`. Validación del esquema del contrato MQTT. |
| Integración | Fusión en `develop` | Pruebas de integración con contenedores de TimescaleDB y Mosquitto; construcción de imágenes Docker. |
| Despliegue a pruebas | Fusión en `develop` | Publica las imágenes y actualiza el entorno `pruebas`. |
| Despliegue a producción | Tag en `main` | Requiere aprobación manual en el entorno `produccion`; aplica migraciones y respaldo previo. |

**Arquitectura ARM:** si se elige Oracle Cloud Ampere A1 (ARM), los agentes de Microsoft son x86_64. Las opciones son construir imágenes multiarquitectura con `docker buildx` (más lento) o instalar un **agente autoalojado** en la VM de pruebas (Azure DevOps da un *parallel job* autoalojado gratuito). Se decide en `SPK-01`.

### 5.7 Normativa para Python (obligatoria)

| Aspecto | Norma | Herramienta |
|---------|-------|-------------|
| Estilo | PEP 8 (líneas de 100 caracteres) | **Ruff** (linter y formateador; reemplaza flake8, black e isort) |
| Reglas de Ruff | `E`, `F`, `W`, `I` (imports), `N` (nombres), `UP`, `B`, `S` (seguridad), `SIM`, `ASYNC` | `pyproject.toml` |
| Tipado | Anotaciones de tipo obligatorias en funciones públicas | **mypy** (modo estricto en `modules/` e `ingestion/`) |
| Docstrings | PEP 257, estilo Google, en español | Ruff (`D`) en módulos públicos |
| Nombres | `snake_case` para funciones y variables; `PascalCase` para clases; `UPPER_CASE` para constantes | Ruff (`N`) |
| Dependencias | `pyproject.toml` + archivo de bloqueo | **uv** |
| Pruebas | `pytest`, `pytest-asyncio`; cobertura mínima de 70 % en `modules/` e `ingestion/` | `pytest-cov` |
| Configuración | Variables de entorno validadas | `pydantic-settings`; `.env.example` versionado, `.env` nunca |
| Validación de datos | Esquemas Pydantic para la API y el mensaje MQTT | Pydantic v2 |
| Base de datos | Toda modificación del esquema por migración | Alembic |
| Ganchos | Se ejecutan antes de cada commit | `pre-commit` |
| Capas | `router` (HTTP) → `service` (reglas de negocio) → `repository` (acceso a datos). El router no consulta la base de datos directamente. | Revisión de código |

### 5.8 Normativa para React (obligatoria)

| Aspecto | Norma | Herramienta |
|---------|-------|-------------|
| Lenguaje | **TypeScript** en modo `strict` (no JavaScript) | `tsc` |
| Estilo | Formato automático | **Prettier** |
| Calidad | Reglas recomendadas + `react-hooks` + `jsx-a11y` (accesibilidad, `RNF-02`) | **ESLint** (configuración *flat*) |
| Componentes | Funcionales con hooks; un componente por archivo; `PascalCase.tsx` | ESLint + revisión |
| Hooks | `useNombre`; la lógica de datos fuera de los componentes de presentación | Revisión |
| Organización | Por funcionalidad (`features/`), no por tipo de archivo | — |
| Estado del servidor | **TanStack Query** (caché, reintentos, actualización periódica de `RF-15`) | — |
| Rutas | React Router | — |
| Formularios y validación | React Hook Form + Zod | — |
| Cliente de la API | Tipos generados desde el OpenAPI del backend (`openapi-typescript`) | Script en `package.json` |
| Textos | En español, centralizados en un módulo de textos (lenguaje claro, `RNF-12`) | — |
| Pruebas | Vitest + Testing Library; Playwright para los flujos de extremo a extremo | — |
| Estilos | Los colores de estados de la especificación se definen una sola vez como tokens | — |

### 5.9 Convenciones de base de datos, API y MQTT

- **Base de datos:** `snake_case`, tablas en plural, llaves primarias `id`, llaves foráneas `<tabla_singular>_id`. Fechas en `timestamptz` guardadas en UTC y mostradas en `America/Bogota` (`R-09`, `RNF-05`). Eliminación lógica con `deleted_at` (`RN-04`).
- **API REST:** prefijo `/api/v1`, recursos en plural (`/stations/{id}/readings`), JSON en `snake_case`, errores con el formato *Problem Details* (RFC 9457), paginación por cursor en las series de tiempo.
- **MQTT** (borrador, a validar con Electrónica en `PC-02` y `PA-50`): tópico `idi-fr-05/v1/stations/{station_id}/readings`; mensaje JSON validado contra `contracts/mqtt/reading.schema.json`; la versión del contrato va en el tópico y en el mensaje.

### 5.10 Entorno de desarrollo

- **Herramientas:** Git, Docker Desktop con WSL 2, Python 3.12 con uv, Node.js LTS con npm, VS Code con las extensiones de Ruff, ESLint y Prettier.
- **Servicios locales:** `docker compose up` levanta TimescaleDB, Mosquitto y el simulador. El backend y el frontend se ejecutan localmente con recarga en caliente.
- **Datos de prueba:** un script carga el catálogo de sensores y estaciones de ejemplo, y el simulador publica lecturas.
- **Secretos:** solo en `.env` local (ignorado por Git) y en el *variable group* `plataforma-comun` de Azure Pipelines.

### 5.11 Plan de pruebas (resumen; el detalle va en DOC-14)

| Nivel | Qué se prueba | Herramienta | Cuándo |
|-------|---------------|-------------|--------|
| Unitarias | Reglas de negocio: validación de mensajes, cálculo de estados (`RN-15`, `RN-16`), umbrales, ICA, métricas de modelos | pytest, Vitest | Cada PR |
| Integración | API + base de datos; ingesta bróker → base de datos | pytest + contenedores (Testcontainers) | Fusión en `develop` |
| Contrato | Mensajes del simulador y ejemplos de Electrónica contra el esquema JSON | pytest + `jsonschema` | Cada PR |
| Extremo a extremo | Flujos públicos (mapa → detalle → exportación) e inicio de sesión | Playwright | Antes de cada Release |
| Carga básica | Ingesta de *N* estaciones y gráficos de 2 años (`RNF-04`) | Locust o k6 | Releases 02 y 04 |
| Seguridad básica | Permisos en el servidor (`RNF-03`), límites de frecuencia, ACL del bróker | Pruebas automatizadas + OWASP ZAP (escaneo base) | Release 04 |
| Validación de modelos | Partición temporal entrenamiento/prueba; R², RMSE, MAE y costo computacional | pytest + notebooks | Releases 03 y 04 |
| Aceptación | Criterios de aceptación de las historias con el PO | Manual, en `pruebas` | Cada Sprint Review |
| Usabilidad | Lenguaje claro con personas sin formación técnica; cuestionario SUS | Sesiones guiadas (5 personas) | Sprint 08 |

Como *Test Plans* está desactivado (licencia de pago), los casos de prueba (`CP-xxx`) se documentan en Markdown en `/docs/pruebas/`, enlazados a sus `RF`, y los resultados automatizados se publican en cada ejecución del pipeline.

---

## 6. Estrategia de work items en Azure DevOps

### 6.1 Ajustes necesarios a la configuración actual

| # | Hallazgo | Ajuste propuesto |
|---|----------|------------------|
| A-01 | El campo `Objetivo específico` de las épicas solo tiene `OE1`–`OE5`, pero la contextualización define el `OE6` (calibración). | Agregar `OE6` a la lista del campo. |
| A-02 | No hay épica para la analítica y la calibración, que es un objetivo completo del trabajo de grado. | Crear `E7. Analítica de datos y calibración` (`OE6`). |
| A-03 | Existe el área `PQRS`, retirada en `GC-001` (`D-08`), y no hay área de analítica. | Renombrar el área `PQRS` a `Analítica de datos` (V-29 a V-32). |
| A-04 | El tipo `Riesgo` registra `RG-01` a `RG-09`, pero la contextualización tiene `RG-01` a `RG-14`. | Registrar `RG-10` a `RG-14` y los nuevos `RG-15` a `RG-19` (sección 9). |
| A-05 | No hay un tag para los spikes. | Agregar el tag `spike`. |
| A-06 | El calendario llega hasta el Sprint 08 (22-nov); la Fase 2 termina el 29-nov. | Crear `Release 05` (23-nov al 29-nov) con un solo Sprint: `Sprint 09`. |
| A-07 | El área `Visualización` no menciona el ICA como Feature propia. | Sin cambio de área; el ICA es una Feature de `E4`. |

### 6.2 Épicas

| Épica | Objetivo | Área principal | Horizonte |
|-------|----------|----------------|-----------|
| E1. Ciclo de vida y gestión del proyecto | OE1 | Gestión y documentación | Todo el proyecto |
| E2. Levantamiento y validación de requerimientos | OE2 | Requisitos y diseño | Releases 01 a 05 (con mayor carga en 01–02) |
| E3. Contrato de integración y gestión de datos | OE3 | Ingesta de datos | Releases 01 a 03 |
| E4. Visualización, ICA y exportación | OE4 | Visualización, Exportación | Releases 02 a 04 |
| E5. Despliegue, operación y respaldo | OE5 | Infraestructura y DevOps | Releases 01 a 05 |
| E6. Seguridad, administración y soporte (transversal) | OE3 / OE4 / OE5 | Identidad y acceso, Administración | Releases 03 a 04 |
| **E7. Analítica de datos y calibración** (nueva) | **OE6** | **Analítica de datos** | Releases 02 a 05 |

### 6.3 Features por épica

Convención del título: `F<épica>.<n> <nombre>`. Cada Feature tiene *Start Date* y *Target Date* y cabe en un Release. La columna *Release* es la meta propuesta.

| Feature | Vistas o requisitos | Prioridad | Release |
|---------|---------------------|-----------|---------|
| **E1** | | | |
| F1.1 Plan del proyecto y metodología (este documento, ADR de proceso) | `OE1` | Must | R01 |
| F1.2 Planes de riesgos y de costos a 24 meses | `R-01`, `RG-xx` | Must | R01–R02 |
| F1.3 Seguimiento, actas y reportes de avance | — | Must | Continuo |
| F1.4 Documento del trabajo de grado (tag `tesis`) | — | Must | Continuo |
| F1.5 Cierre, transferencia de conocimiento y entrega final | `RG-04` | Must | R05 |
| **E2** | | | |
| F2.1 Línea base de requisitos v5 y MVP aprobado | DOC-03 | Must | R01 |
| F2.2 Resolución de los puntos abiertos que bloquean el MVP | `PA-30`–`PA-34`, `PA-46`, `PA-49`–`PA-51`, `PC-04` | Must | R01–R02 |
| F2.3 Mockups de las vistas públicas | V-02 a V-05 | Must | R01–R02 |
| F2.4 Mockups de las vistas de administración y analítica | V-01, V-06 a V-32 | Should | R02–R03 |
| F2.5 Validación con usuarios (aceptación y usabilidad) | `RNF-12`, DOC-21 | Must | R04–R05 |
| **E3** | | | |
| F3.1 Contrato de integración MQTT v1 acordado con Electrónica | `PC-02`, `PA-50` | Must | R01–R02 |
| F3.2 Bróker con autenticación, ACL por tópico y TLS | `RNF-09`, `RN-28` | Must | R01–R02 |
| F3.3 Simulador de estaciones | `S-03` | Must | R01 |
| F3.4 Ingesta y validación de lecturas | `RN-29`, `PC-09` | Must | R01–R02 |
| F3.5 Almacenamiento de series de tiempo, agregados y retención | `RNF-04`, `PC-15` | Must | R02 |
| F3.6 Catálogo de sensores y asignación a estaciones | V-25, V-26 | Must | R02–R03 |
| F3.7 Estados automáticos de estaciones y sensores | `RN-15`, `RN-16`, `PA-30`, `PA-31` | Must | R03 |
| F3.8 Formato del mensaje configurable desde la interfaz | V-27 | Should | R04 |
| **E4** | | | |
| F4.1 Estructura común: barra lateral, pie de página, diseño adaptable | `C-09`, `C-10`, `RNF-01` | Must | R02 |
| F4.2 Mapa de estaciones | V-02 | Must | R02 |
| F4.3 Listado de estaciones con gráficos y tablas | V-03, `C-03`, `C-04` | Must | R02 |
| F4.4 Detalle de estación con comparación entre sensores | V-04 | Must | R02–R03 |
| F4.5 Generación de archivos CSV/JSON con límites | V-05, `RNF-10` | Must | R03 |
| F4.6 Índice de Calidad del Aire | `PC-04` | Must (condicionado) | R03–R04 |
| **E5** | | | |
| F5.1 Entorno de desarrollo y contenedores | sección 5.10 | Must | R01 |
| F5.2 Pipeline de CI | sección 5.6 | Must | R01 |
| F5.3 Entorno de pruebas desplegado | DoD | Must | R01 |
| F5.4 Producción: dominio, TLS y despliegue con aprobación | `OE5` | Must | R03 |
| F5.5 Respaldos automáticos fuera del servidor y restauración probada | `RG-05`, criterio de éxito `OE5` | Must | R03–R04 |
| F5.6 Monitoreo básico y alertas de costos | `RG-02` | Should | R04 |
| F5.7 Manual de despliegue y plan de operación a 24 meses | DOC-18, DOC-19 | Must | R04–R05 |
| **E6** | | | |
| F6.1 Autenticación y recuperación de contraseña | V-01, V-10, `RNF-07` | Must | R03 |
| F6.2 Gestión de estaciones | V-06, V-07, V-08, V-14 | Must | R03 |
| F6.3 Credenciales de estaciones en el bróker | V-28, `C-12`, `RF-134` | Must | R03 |
| F6.4 Gestión de administradores y perfil | V-13, V-21, V-11, V-12, V-19 | Should / Could | R04 |
| F6.5 Alertas y su detalle | V-16, V-17 | Should | R04 |
| F6.6 Configuración del sistema | V-15 | Should | R04 |
| F6.7 Endurecimiento: límites de frecuencia, cabeceras de seguridad, auditoría | `RNF-03`, `RNF-10` | Must | R04 |
| F6.8 Estaciones eliminadas | V-18 | Could | R04 |
| **E7** | | | |
| F7.1 Diseño experimental de la calibración | DOC-17, `PC-17`, `PC-19` | Must | R02 |
| F7.2 Prototipo de calibración fuera de la plataforma (RLM vs ANN) | `RN-31` | Must | R02–R03 |
| F7.3 Fuentes de referencia | V-29 | Should | R03–R04 |
| F7.4 Entrenamiento y comparación de modelos en la plataforma | V-30, `C-11`, `RNF-11` | Must | R04 |
| F7.5 Gestión y exportación de modelos | V-32, `PA-54` | Must | R04 |
| F7.6 Estimación de variables | V-31 | Could | R05 (si hay capacidad) |

### 6.4 User Stories

- **Título:** verbo + resultado, en lenguaje del usuario. Ejemplo: *Consultar el gráfico histórico de una variable en el detalle de la estación*.
- **Descripción:** "Como `<visitante | administrador | investigador | estación>`, quiero… para…".
- **Criterios de aceptación:** *Dado / Cuando / Entonces*, verificables y numerados (se convierten en `CP-xxx`).
- **Campo `Requisitos asociados`:** `RF-32, RF-33, RNF-04`.
- **Tamaño:** ≤ 8 Story Points; si es más grande, se divide por flujo, por regla de negocio o por tipo de dato (INVEST; Wake, 2003).
- **Historias técnicas** (sin usuario final visible): se escriben desde el rol que se beneficia. Ejemplo: "Como estación, quiero autenticarme en el bróker con credenciales propias para que nadie publique datos en mi nombre".

**Ejemplo completo:**

> **Rechazar mensajes MQTT que no cumplan el contrato**
> Como administrador, quiero que la plataforma rechace y registre los mensajes que no cumplen el formato del contrato, para que no se almacenen datos inválidos.
> *Requisitos asociados:* `RN-29`, `RF-263`, `PC-09`.
> 1. **Dado** un mensaje con un JSON mal formado, **cuando** llega al bróker, **entonces** no se almacena ninguna lectura y queda registrado con la causa del rechazo.
> 2. **Dado** un mensaje con una variable que no está asignada a la estación, **cuando** se procesa, **entonces** se rechaza esa variable y se registra la causa.
> 3. **Dado** un mensaje válido, **cuando** se procesa, **entonces** sus lecturas se almacenan con la marca de tiempo en UTC.

### 6.5 Otros tipos de work item

| Tipo | Uso en este proyecto |
|------|----------------------|
| **Task** | Pasos de una historia (≤ 1 día) con *Activity*: Development, Testing, Documentation, Design, Deployment. Se registran horas. |
| **Bug** | Defecto de algo ya aceptado. Se prioriza en el backlog con las historias; un Bug crítico en producción va por `hotfix/*`. |
| **Issue** | Cada `PA-xx` y `PC-xx` que bloquea trabajo. Título `PA-50 Modelo del mensaje MQTT`; asignado a quien lo gestiona; con *Due Date*; enlazado como *Predecessor* de las historias que bloquea. |
| **Spike** | User Story con tag `spike`, con un tiempo máximo (*timebox*) en horas y un entregable concreto: un ADR o una prueba de concepto. No se le asignan Story Points; se descuenta de la capacidad. |
| **Riesgo** | `RG-xx`, revisado en cada Retrospectiva. Un riesgo materializado genera un Issue o un Bug. |

### 6.6 Tags y consultas

- **Tags:** `documentación`, `tesis`, `deuda-técnica`, `bloqueado`, `simulador`, `spike`, `mvp`, `dependencia-externa`.
- **Consultas compartidas:** "Issues abiertos por responsable y fecha", "Historias bloqueadas", "Riesgos activos por impacto", "Alcance del Release actual" (`Iteration Path Under Release NN`), "Historias sin Requisitos asociados" (control de trazabilidad).

---

## 7. Cronograma

### 7.1 Fase 1: ingeniería y planeación (1 de septiembre al 4 de octubre)

Las semanas 1 a 4 fueron anteriores al uso de Sprints en Azure DevOps; se registran como *Sprint 0 (inception)* con base en la evidencia disponible en el repositorio de documentos.

| Semana | Fechas | Objetivo | Evidencia / entregables | Estado |
|--------|--------|----------|-------------------------|--------|
| 1 | 1–6 sep | Vinculación y comprensión del macroproyecto | Ficha IDI-FR-05; propuesta técnica (7-sep) | Hecho |
| 2 | 7–13 sep | Formalización y primeros requisitos | Carta de vinculación (11-sep) | Hecho |
| 3 | 14–20 sep | Levantamiento de requisitos | Borradores de requisitos | Hecho |
| 4 | 21–27 sep | Especificación y contexto | `requerimientos.md` (24-sep), contextualización v2 (25-sep) | Hecho |
| 5 (Sprint 01) | 28 sep – 4 oct | Reunión de alcance; configuración de Azure DevOps; **línea base de planeación** | Acta del 28-sep, `GC-001`, especificación v4, contextualización v3, Azure DevOps configurado, este documento | En curso |

### 7.2 Cierre de la Fase 1: línea base de planeación (`HT-01`, 4 de octubre)

**Lo que se entrega el 4 de octubre** (realista para los días que quedan del Sprint 01):

| Entregable | Nivel esperado |
|------------|----------------|
| Documento fundacional (este) | v1, revisado por el director |
| Propuesta de MVP (sección 3.5) | Presentada al director |
| Arquitectura | v0.9: diagramas C4 de contexto y de contenedores, y la vista de despliegue candidata |
| ADR | `ADR-001` a `ADR-005`, `ADR-014` y `ADR-016` redactados |
| Modelo de datos | v0.9: entidades principales y modelo de lecturas |
| Contrato MQTT | Borrador v0.1 enviado a Ingeniería Electrónica |
| Repositorio | Creado, con estructura, `CONTRIBUTING.md`, `pre-commit` y protección de ramas |
| Plan de pruebas | v0.9 |
| Riesgos | `RG-01` a `RG-19` registrados en Azure Boards |
| Backlog | Épicas y Features cargadas; historias del Sprint 02 refinadas |

**Lo que pasa a la Fase 2 de forma explícita** (con fecha límite): decisiones de infraestructura, bróker y gráficas (`SPK-01` a `SPK-04`, 11-oct), mockups de las vistas de administración, diseño experimental de la calibración y plan de costos definitivo. **Esto debe comunicarse al director** como un ajuste del cronograma, no como un retraso oculto.

### 7.3 Fase 2: desarrollo, pruebas y entrega (5 de octubre al 29 de noviembre)

| Release | Sprint | Fechas | Objetivo del Sprint (propuesto) | Entregables principales |
|---------|--------|--------|--------------------------------|-------------------------|
| **R01** `v0.1.0` | 01 | 28-sep – 4-oct | Línea base de planeación | Sección 7.2 |
| | 02 | 5-oct – 11-oct | **Esqueleto funcional:** un dato viaja del simulador a la base de datos y se ve en la web, desplegado en `pruebas` | Spikes `SPK-01` a `SPK-04` y sus ADR; `docker-compose`; simulador v0; ingesta mínima; un gráfico en el frontend; pipeline de CI; MVP aprobado; arquitectura v1.0 |
| **R02** `v0.2.0` | 03 (festivo 12-oct) | 12-oct – 18-oct | Ingesta robusta y modelo de datos definitivo | Validación de mensajes; catálogo de sensores precargado; agregados continuos; mapa (V-02); diseño experimental de la calibración (F7.1) |
| | 04 | 19-oct – 25-oct | Consulta pública | Listado (V-03) y detalle (V-04) con gráficos y tablas; prueba de carga de gráficos de 2 años; prototipo de calibración con datos simulados o públicos (F7.2). **Demo a los investigadores** |
| **R03** `v0.3.0` | 05 | 26-oct – 1-nov | Administración de estaciones | Autenticación (V-01, V-10); crear y editar estaciones (V-14, V-06); credenciales en el bróker (V-28); estados automáticos |
| | 06 (festivo 2-nov) | 2-nov – 8-nov | Exportación y producción | Exportación CSV/JSON (V-05); deshabilitar y eliminar (V-07, V-08); ICA; **primer despliegue en producción**; respaldos |
| **R04** `v0.4.0` | 07 | 9-nov – 15-nov | Analítica en la plataforma | Entrenamiento y comparación RLM vs ANN (V-30); modelos y exportación (V-32); fuentes de referencia (V-29) |
| | 08 (festivo 16-nov) | 16-nov – 22-nov | Alertas, endurecimiento y **congelamiento de funcionalidades** | Alertas (V-16, V-17) y configuración (V-15) si hay capacidad; seguridad básica; pruebas de usabilidad; manual de usuario v0.9 |
| **R05** `v1.0.0` | 09 | 23-nov – 29-nov | Estabilización y entrega | Solo corrección de defectos; pruebas de aceptación finales; informe de pruebas; manuales y plan de operación finales; resultados de la calibración; **entrega del producto** |

**Regla de congelamiento:** desde el 22 de noviembre (`HT-08`) no entran funcionalidades nuevas; el Sprint 09 es solo de estabilización, pruebas y documentación.

### 7.4 Hitos

| ID | Fecha | Hito | Criterio de cumplimiento |
|----|-------|------|--------------------------|
| `HT-01` | 2026-10-04 | Fin de la Fase 1: línea base de planeación | Entregables de la sección 7.2 presentados al director |
| `HT-02` | 2026-10-09 | Contrato MQTT v0.1 revisado por Electrónica | Respuesta registrada en el Issue `PC-02` |
| `HT-03` | 2026-10-11 | Fin del Release 01: esqueleto funcional, stack decidido y MVP aprobado | `v0.1.0`; ADR `007`–`009` aceptados; acta |
| `HT-04` | 2026-10-25 | Fin del Release 02: consulta pública con datos simulados | `v0.2.0`; demo a los investigadores |
| `HT-05` | 2026-10-25 | Fecha límite de los insumos de Ingeniería Ambiental (`PC-04`, `PA-24`, `PC-17`, `PC-19`) | Respuesta o aplicación de la propuesta por defecto |
| `HT-06` | 2026-11-08 | Fin del Release 03: plataforma en producción | `v0.3.0` en producción; restauración de respaldo probada |
| `HT-07` | Por definir (`PC-12`) | Primera estación real publicando datos | Lecturas reales almacenadas con el mismo contrato |
| `HT-08` | 2026-11-22 | Fin del Release 04: congelamiento de funcionalidades | `v0.4.0`; todo el MVP implementado |
| `HT-09` | 2026-11-29 | Entrega final del producto y de la documentación técnica | `v1.0.0`; documentación completa |
| `HT-10` | Por definir | Aval del director y presentación al comité de currículo | Aval registrado |

### 7.5 Supuestos de capacidad

- **Capacidad:** se registra por persona en cada Sprint según su dedicación real. Si no hay otro dato, se asumen **15 horas semanales por estudiante** (por confirmar), de las que se descuentan las 3 horas de eventos de Scrum.
- **Distribución sugerida de la capacidad:** 60 % desarrollo, 15 % pruebas, 15 % documentación y tesis, 10 % gestión.
- **Velocidad:** se mide desde el Sprint 02. A partir del Sprint 04, el *forecast* del backlog con la velocidad promedio dice si el MVP cabe; si no cabe, se lleva al director la propuesta de recorte en la Review del Release 02.

---

## 8. Análisis del stack y plan de validación

### 8.1 Evaluación preliminar del stack propuesto

| Componente | Propuesta | Evaluación | Riesgos o condiciones | Confianza |
|------------|-----------|------------|------------------------|-----------|
| Backend | Python + FastAPI | **Adecuado.** Un solo lenguaje para la API, la ingesta MQTT, el simulador y el aprendizaje automático (scikit-learn, PyTorch). Pydantic valida los mensajes del contrato y FastAPI genera el OpenAPI. Soporte asíncrono para la ingesta. | FastAPI no trae tareas en segundo plano robustas: los entrenamientos y las exportaciones necesitan un proceso aparte (`ADR-010`). | Alta |
| Frontend | React + Vite | **Adecuado.** Ecosistema amplio para mapas (Leaflet) y gráficas; Vite produce archivos estáticos que se sirven sin servidor de Node. | Se recomienda TypeScript para reducir errores de integración con la API. Si el posicionamiento en buscadores de las vistas públicas fuera importante, habría que reconsiderarlo; no es un requisito. | Alta |
| Base de datos | PostgreSQL + TimescaleDB | **Adecuado.** *Hypertables* para las lecturas, compresión (reduce mucho el volumen de la sección 7.2 de la contextualización), agregados continuos para los gráficos de 2 años (`RNF-04`) y políticas de retención. Sigue siendo PostgreSQL, así que el resto del modelo es relacional normal. | La compresión y los agregados continuos son funciones con licencia *Timescale License*: están en la edición *Community* autoalojada, pero **no** en los PostgreSQL administrados de los grandes proveedores, que solo ofrecen la edición Apache. Elegir TimescaleDB implica autoalojarlo o pagar Timescale Cloud. | Alta (autoalojado) |
| Visualización | Matplotlib vs Grafana | **Ninguna de las dos es la opción adecuada para las vistas públicas** (sección 8.2). | — | — |
| Despliegue | VM en Oracle Cloud | **Prometedor:** la capa *Always Free* con Ampere A1 tiene recursos suficientes y costo cero. | Arquitectura ARM, disponibilidad de capacidad, política de recuperación de instancias inactivas y titularidad de la cuenta (sección 8.3). | Media |

### 8.2 Visualización de datos: Matplotlib vs Grafana vs librería en el frontend (`ADR-008`)

**El problema.** Las vistas V-03 y V-04 necesitan gráficos interactivos (filtros por estación, sensor y variable; comparación entre sensores, `RF-34`; rangos de hasta 2 años; información al pasar el cursor), integrados en una interfaz en español con lenguaje claro, adaptable a celulares y con acceso público sin sesión. La analítica (`C-11`) necesita gráficos de dispersión y de series de tiempo.

| Criterio | Matplotlib | Grafana | Librería JS en el frontend (p. ej., Apache ECharts) |
|----------|------------|---------|-----------------------------------------------------|
| Interactividad (zoom, cursor, leyendas) | Baja: genera imágenes estáticas en el servidor | Alta | Alta |
| Integración con la interfaz y los filtros de los mockups | Baja: cada cambio de filtro regenera una imagen | Baja–media: se incrusta con *iframes* y tiene su propia interfaz y estilo | Alta: es un componente React más |
| Lenguaje claro y diseño propio (`RNF-12`) | Medio | Bajo: la interfaz de Grafana es técnica | Alto |
| Acceso público sin sesión | Sí | Requiere habilitar el acceso anónimo y la incrustación, con implicaciones de seguridad | Sí, a través de la API pública |
| Adaptable a celulares (`RNF-01`) | Imágenes que se escalan mal | Limitado | Sí |
| Costo de infraestructura | Usa CPU del servidor por cada gráfico | Un servicio más (memoria adicional, actualizaciones, usuarios) | Ninguno: se ejecuta en el navegador |
| Rendimiento con 2 años de datos | Depende del servidor | Bueno | Bueno si el backend entrega datos agregados (`time_bucket`); ECharts además submuestrea con LTTB |
| Mantenimiento por el equipo | Simple | Otro sistema que configurar y respaldar | Simple (mismo código del frontend) |

**Recomendación (a validar en `SPK-04`):**

- **Vistas públicas y de analítica:** librería de gráficas en el frontend, con **Apache ECharts** (licencia Apache 2.0) como candidata principal, y uPlot o Recharts como alternativas. El backend entrega series ya agregadas según el rango (por ejemplo, datos crudos para 24 horas, promedios horarios para un mes, promedios diarios para 2 años).
- **Matplotlib:** para las figuras del documento de la tesis y de la metodología de calibración (notebooks), donde las figuras estáticas son lo adecuado.
- **Grafana (opcional, *Could*):** como tablero interno de monitoreo técnico (ingesta, errores, recursos del servidor) para los administradores, si sobra capacidad. No reemplaza las vistas de la especificación.

### 8.3 Despliegue: criterios y opciones (`ADR-009`)

**Presupuesto de referencia:** $2.000.000 COP / 24 meses ≈ $83.000 COP al mes (≈ USD 20 al mes a una tasa de referencia de 4.000 COP/USD, por verificar). Con la reserva del 15 %, el techo operativo es ≈ $70.000 COP al mes.

| Opción | Costo mensual estimado | Ventajas | Riesgos |
|--------|------------------------|----------|---------|
| **A. Oracle Cloud *Always Free*** (Ampere A1, ARM) | $0 | Recursos generosos (hasta 4 OCPU y 24 GB de RAM repartibles en varias VM): permite separar `pruebas` y `produccion` sin costo; almacenamiento de bloques y transferencia de salida suficientes | Arquitectura ARM (imágenes `arm64` necesarias); errores de "sin capacidad" al crear instancias A1 en algunas regiones; las instancias *Always Free* inactivas pueden ser recuperadas por Oracle (pasar la cuenta a *Pay As You Go* sin consumir pagos lo evita); los recursos gratuitos solo existen en la región principal elegida al registrarse; requiere tarjeta de crédito y definir quién es el titular de la cuenta (`PC-07`). *Verificar las condiciones vigentes en la documentación de Oracle.* |
| **B. VPS de bajo costo** (x86, 2 vCPU / 4 GB) | ≈ USD 5–8 | Simple, predecible, x86 (sin problemas de arquitectura) | Una sola VM para todo (o el doble de costo para separar entornos); costo en dólares |
| **C. Infraestructura de la UCO** | ≈ $0 | Continuidad institucional (`RG-04`), posible subdominio de la UCO | Tiempos de gestión con TI, que suelen ser largos; restricciones de red (puerto MQTT expuesto a internet) |
| D. Servicios administrados (PaaS + base de datos administrada) | > USD 30 | Menos operación | Excede el presupuesto y no ofrece las funciones de TimescaleDB que se necesitan |

**Recomendación:** **A como principal y B como respaldo inmediato**, con todo en contenedores para pasar de una a otra en horas. Incluso la opción B cuesta en 24 meses del orden de $500.000 a $800.000 COP, dentro del presupuesto, así que el riesgo económico de la decisión es bajo. Se debe iniciar en paralelo la consulta a TI de la UCO (opción C) para la continuidad después del trabajo de grado.

**Respaldos fuera del servidor principal:** almacenamiento de objetos (el *Object Storage* gratuito de Oracle, o un proveedor distinto con capa gratuita para tener los respaldos fuera del mismo proveedor). Se decide en `SPK-01`.

### 8.4 Componentes que faltan en el stack preliminar

| Componente | Por qué hace falta | Candidatos | Decisión |
|------------|--------------------|------------|----------|
| **Bróker MQTT** | Es el punto de entrada de los datos (`R-10`). La plataforma debe **crear, regenerar e invalidar credenciales** de estaciones desde la interfaz (`RF-134`, `RF-270`–`RF-272`) y restringir cada estación a su tópico (`RNF-09`). | **Mosquitto 2** con el plugin *dynamic security* (credenciales y ACL administrables por mensajes de control; ligero). EMQX (autenticación y ACL contra PostgreSQL; tablero propio; más consumo de memoria). Bróker administrado con capa gratuita (menos operación, pero límites de conexiones y dependencia del proveedor para gestionar credenciales). | `SPK-02` |
| **Tareas en segundo plano** | Exportaciones con avance (V-05), entrenamientos (`RNF-11`), cálculo periódico de estados y alertas. | Cola sobre PostgreSQL (p. ej., *procrastinate*, sin servicios extra); Celery o Dramatiq con Redis (más maduro, un servicio más). | `SPK-05` |
| **Proxy inverso y TLS** | HTTPS para la web y la API; TLS para MQTT. | Caddy (certificados de Let's Encrypt automáticos) o Nginx + certbot. | Con `SPK-01` |
| **Mapas y geocodificación** | V-02, V-04, V-06, V-14 (`PA-45`). | Leaflet + teselas de OpenStreetMap + Nominatim (gratuitos con políticas de uso: atribución y como máximo 1 solicitud por segundo en Nominatim, suficiente para el uso administrativo). | `SPK-07` |
| **Correo transaccional** | Recuperación de contraseña (V-10). | Correo institucional de la UCO por SMTP, o un proveedor con capa gratuita. | `ADR-017` |
| **Aprendizaje automático** | RLM y ANN con bajo costo computacional y exportables (`OE6`, `PA-54`). | scikit-learn (`LinearRegression`, `MLPRegressor`) como base; PyTorch solo si la ANN requerida lo exige. Exportación: coeficientes en JSON para RLM (portables incluso a un microcontrolador) y ONNX para la ANN. | `SPK-06` |

### 8.5 Arquitectura preliminar (`ADR-016`, Propuesta)

**Monolito modular**: un solo código de backend organizado por módulos de dominio, desplegado como varios procesos. Se descartan los microservicios: dos developers, un servidor y un presupuesto fijo no justifican su complejidad operativa.

```
                    Estaciones (WiFi / enlace LoRa)        Simulador
                                   │  MQTT + TLS               │
                                   ▼                           ▼
┌───────────────────────────── VM (docker compose) ─────────────────────────────┐
│                                                                               │
│   ┌──────────────┐   suscripción   ┌──────────────────┐                       │
│   │ Bróker MQTT  │ ───────────────►│ Ingesta (Python) │── valida y escribe ──┐│
│   │ (Mosquitto)  │◄── credenciales │  mismo código    │                      ││
│   └──────────────┘    y ACL        └──────────────────┘                      ▼│
│          ▲                                                  ┌──────────────┐ │
│          │ gestión de credenciales                          │ PostgreSQL + │ │
│   ┌──────┴───────┐       lee / escribe                      │ TimescaleDB  │ │
│   │ API FastAPI  │─────────────────────────────────────────►│              │ │
│   └──────▲───────┘                                          └──────▲───────┘ │
│          │                      ┌──────────────────────┐           │         │
│          │ encola tareas        │ Worker: exportaciones│───────────┘         │
│          └─────────────────────►│ entrenamientos,      │                     │
│                                 │ estados y alertas    │                     │
│   ┌──────────────────────────┐  └──────────────────────┘                     │
│   │ Proxy inverso (Caddy)    │  sirve el frontend estático y enruta /api     │
│   └────────────▲─────────────┘                                               │
└────────────────┼──────────────────────────────────────────────────────────────┘
                 │ HTTPS
          Navegador (React): visitantes y administradores
                                                  Respaldos ──► almacenamiento de objetos externo
```

**Modelo de lecturas (`ADR-006`, Propuesta):** formato **largo** (`time`, `station_id`, `sensor_id`, `variable_id`, `value`, `quality_flag`) en una *hypertable*, en lugar de una columna por variable. Razón: el catálogo de sensores debe poder ampliarse sin cambiar el código ni el esquema (`S-04`, `RF-242`). La compresión de TimescaleDB, segmentada por estación, sensor y variable, compensa el mayor número de filas. Los valores calibrados nunca reemplazan a los originales (`RN-32`). Se valida con datos sintéticos en `SPK-03`.

### 8.6 Plan de validación (spikes)

Cada spike tiene un tiempo máximo, criterios de decisión medibles y un ADR como entregable. Si un spike no logra una conclusión en su tiempo máximo, se aplica la opción de respaldo.

| ID | Pregunta | Tiempo máximo | Criterios de decisión | Respaldo | Fecha límite |
|----|----------|:-------------:|-----------------------|----------|--------------|
| `SPK-01` | ¿Dónde se despliega? | 8 h | Costo a 24 meses ≤ techo operativo; todas las imágenes funcionan en la arquitectura elegida; VM creada en menos de 2 días de intentos; entrenamiento de una ANN pequeña sin agotar la memoria; respaldo y restauración funcionando; titularidad de la cuenta definida | VPS x86 de bajo costo (opción B) | 11-oct (Sprint 02) |
| `SPK-02` | ¿Qué bróker MQTT? | 6 h | Crear, regenerar e invalidar credenciales por estación desde Python; ACL de un tópico por estación; TLS; mensaje *Last Will* (`PA-49`); consumo de memoria < 100 MB en reposo | Mosquitto con archivo de contraseñas y recarga por señal | 11-oct (Sprint 02) |
| `SPK-03` | ¿TimescaleDB rinde con el volumen esperado? | 6 h | Cargar 2 años sintéticos (10 estaciones × 1 lectura/min × 16 valores); consulta del gráfico de 2 años con agregados continuos en < 2 s (p95); tamaño comprimido medido y proyectado a 24 meses | Tablas de agregados propias, mantenidas por el worker | 11-oct (Sprint 02) |
| `SPK-04` | ¿Qué librería de gráficas? | 6 h | Prototipo de V-04 con 2 series comparadas, zoom y cursor; fluido en un celular de gama media; accesible (estado con texto, `RNF-02`); tamaño del paquete razonable; licencia libre | Recharts | 11-oct (Sprint 02) |
| `SPK-05` | ¿Cómo se ejecutan las tareas en segundo plano? | 4 h | Exportación con avance y cancelación (V-05); entrenamiento aislado de la API (`RNF-11`); límite de tareas simultáneas; sin servicios adicionales si es posible | Celery con Redis | 25-oct (Sprint 04) |
| `SPK-06` | ¿Qué librerías de aprendizaje automático y qué formato de exportación? | 8 h | RLM y ANN entrenadas con los mismos datos (`RN-31`); métricas de precisión y de costo (sección 2.8 de la especificación); exportación con metadatos (`PA-54`); entrenamiento en la VM en minutos, no horas | scikit-learn para ambos modelos + exportación con `joblib` y JSON | 25-oct (Sprint 04) |
| `SPK-07` | ¿Mapas y geocodificación sin costo? | 3 h | Mapa con marcadores por estado; dirección ↔ coordenadas en V-14; cumplimiento de las políticas de uso | Ingreso manual de coordenadas, sin geocodificación | 18-oct (Sprint 03) |

**Servicios externos de referencia para la calibración** (`PC-10`, `PA-55`): antes del 25-oct (Sprint 04) se identifica al menos una fuente candidata (por ejemplo, la red oficial de la región o estaciones de otros investigadores), se revisan sus condiciones de uso y licencia, y se prueba su API. Si no hay fuente externa viable, la calibración usa una estación interna como referencia y se documenta la limitación (`RG-11`).

### 8.7 Dependencias preliminares

| Ámbito | Dependencias principales |
|--------|--------------------------|
| Backend | `fastapi`, `uvicorn`, `pydantic`, `pydantic-settings`, `sqlalchemy` 2, `asyncpg`, `alembic`, `aiomqtt` (o `paho-mqtt`), librería de *hash* Argon2, `scikit-learn`, `pandas`, `numpy` |
| Backend (desarrollo) | `ruff`, `mypy`, `pytest`, `pytest-asyncio`, `pytest-cov`, `testcontainers`, `httpx`, `pre-commit` |
| Frontend | `react`, `react-router`, `@tanstack/react-query`, `react-hook-form`, `zod`, `leaflet` + `react-leaflet`, librería de gráficas (`SPK-04`) |
| Frontend (desarrollo) | `typescript`, `vite`, `eslint`, `prettier`, `vitest`, `@testing-library/react`, `@playwright/test`, `openapi-typescript` |
| Infraestructura | Docker, Docker Compose, TimescaleDB (imagen oficial), bróker (`SPK-02`), Caddy |

Las versiones exactas se fijan en los archivos de bloqueo (`uv.lock`, `package-lock.json`) al crear el repositorio.

---

## 9. Riesgos nuevos

Se suman a `RG-01`–`RG-14` de [contextualizacion.md](contextualizacion.md) §8.

| ID | Riesgo | Probabilidad | Impacto | Respuesta |
|----|--------|:------------:|:-------:|-----------|
| `RG-15` | La Fase 1 cierra sin todos los entregables de diseño, y el director lo percibe como incumplimiento. | Alta | Medio | Línea base de planeación (sección 7.2) con lo pendiente declarado y fechado; comunicarlo en la Review del 2 al 4 de octubre. |
| `RG-16` | No se puede crear o se pierde la VM de Oracle Cloud (capacidad, recuperación de instancias inactivas). | Media | Medio | Contenedores portables; respaldo fuera del servidor; opción B lista; cuenta *Pay As You Go* sin consumo. |
| `RG-17` | El investigador ambiental no entrega a tiempo los insumos de dominio (ICA, umbrales, anomalías, calibración). | Alta | Alto | Issues con fecha límite (`HT-05`); propuesta por defecto basada en la normativa, avalada por el director. |
| `RG-18` | Ingeniería Electrónica no valida el contrato MQTT a tiempo o lo cambia tarde. | Media | Alto | Borrador temprano (`HT-02`); contrato versionado; validación por esquema; formato configurable (V-27) como mitigación posterior. |
| `RG-19` | La doble función del director (PO y evaluador) mezcla la aceptación del producto con la evaluación académica. | Baja | Medio | Separar las dos decisiones en cada Review y en las actas (sección 1.1). |

---

## 10. Decisiones pendientes y próximos pasos

### 10.1 Para el director (antes del 11 de octubre)

| # | Decisión | Recomendación del equipo |
|---|----------|--------------------------|
| 1 | Aprobar este documento como línea base | — |
| 2 | Confirmar el rol de Product Owner | El director, con la separación de papeles de la sección 1.1 |
| 3 | Aprobar el MVP (sección 3.5) | Como está propuesto |
| 4 | Aceptar el ajuste de la Fase 1 (sección 7.2) | Spikes de stack en el Sprint 02 |
| 5 | Visibilidad del repositorio (`ADR-015`) | Público, si la UCO lo permite: habilita la protección de ramas y el análisis estático sin costo. Requiere que nunca haya secretos en el repositorio y definir la licencia (`PC-14`). |
| 6 | Titular de la cuenta de nube y de los servicios (`PC-07`) | Cuenta institucional o del investigador principal, con los estudiantes como administradores |
| 7 | Fechas del comité de currículo y formato del entregable (`PC-11`, `PC-12`) | — |
| 8 | Dedicación semanal esperada de cada estudiante | Para fijar la capacidad (sección 7.5) |

### 10.2 Acciones inmediatas (Sprint 01, hasta el 4 de octubre)

| # | Acción | Responsable sugerido |
|---|--------|----------------------|
| 1 | Aplicar los ajustes A-01 a A-06 en Azure DevOps (sección 6.1) | Estudiante 1 |
| 2 | Crear el repositorio con la estructura de la sección 5.1, `CONTRIBUTING.md`, `pre-commit` y protección de ramas | Estudiante 2 |
| 3 | Cargar en Azure Boards las épicas, Features (sección 6.3), Issues (`PA`/`PC` del MVP) y riesgos | Estudiante 1 |
| 4 | Redactar el borrador v0.1 del contrato MQTT y enviarlo a Ingeniería Electrónica | Estudiante 1 |
| 5 | Redactar la arquitectura v0.9 (C4 de contexto y contenedores) y el modelo de datos v0.9 | Ambos |
| 6 | Redactar los ADR `001`–`005`, `014` y `016` | Ambos |
| 7 | Plan de pruebas v0.9 | Estudiante 2 |
| 8 | Enviar al investigador ambiental los Issues `PC-04`, `PA-24`, `PA-33`, `PC-17` y `PC-19` con fecha límite 25-oct | Estudiante 1 |
| 9 | Registrar un `GC-002` en [gestion_cambios.md](gestion_cambios.md): Sprints de 1 semana (contextualización §5) y este documento como documento controlado | Estudiante 2 |
| 10 | Refinar las historias del Sprint 02 (esqueleto funcional y spikes) | Ambos |

---

## 11. Referencias

- Ambler, S. (2002). *Agile Modeling*. Wiley.
- Boehm, B. y Turner, R. (2003). *Balancing Agility and Discipline*. Addison-Wesley.
- Brooke, J. (1996). SUS: A "quick and dirty" usability scale. En *Usability Evaluation in Industry*. Taylor & Francis.
- Brown, S. *The C4 model for visualising software architecture*. https://c4model.com
- Conventional Commits 1.0.0. https://www.conventionalcommits.org
- Driessen, V. (2010, nota de reflexión de 2020). *A successful Git branching model*. https://nvie.com/posts/a-successful-git-branching-model/
- ISO/IEC/IEEE 29148:2018. *Requirements engineering*.
- ISO/IEC/IEEE 29119-3:2021. *Software testing — Test documentation*.
- ISO/IEC/IEEE 26514:2022. *Design and development of information for users*.
- ISO/IEC/IEEE 14764:2022. *Software engineering — Maintenance*.
- Keep a Changelog. https://keepachangelog.com
- Ministerio de Ambiente y Desarrollo Sostenible (2017). *Resolución 2254 de 2017*, norma de calidad del aire ambiente.
- Nygard, M. (2011). *Documenting Architecture Decisions*.
- Preston-Werner, T. *Semantic Versioning 2.0.0*. https://semver.org
- Python Software Foundation. *PEP 8 — Style Guide for Python Code* y *PEP 257 — Docstring Conventions*.
- RFC 9457 (2023). *Problem Details for HTTP APIs*.
- Rüping, A. (2003). *Agile Documentation*. Wiley.
- Schwaber, K. y Sutherland, J. (2020). *The Scrum Guide*.
- Wake, B. (2003). *INVEST in Good Stories, and SMART Tasks*.
- Documentación oficial consultada para las decisiones abiertas (verificar las condiciones vigentes al decidir): Oracle Cloud *Always Free Resources*; TimescaleDB *Editions* (Apache 2 vs Community); Eclipse Mosquitto *Dynamic Security Plugin*; Apache ECharts; políticas de uso de las teselas de OpenStreetMap y de Nominatim; Azure DevOps (procesos heredados, iteraciones, conexión con GitHub, *parallel jobs*); GitHub (ramas protegidas por plan).

---

## 12. Control de cambios

| Versión | Fecha | Cambios |
|---------|-------|---------|
| 1 | 2026-10-01 | Versión inicial: roles y gobierno, metodología y artefactos, MVP, plan de documentación, flujo de trabajo y normativas, estrategia de work items, cronograma de las Fases 1 y 2, análisis del stack con plan de validación, riesgos `RG-15` a `RG-19` y decisiones pendientes. |
