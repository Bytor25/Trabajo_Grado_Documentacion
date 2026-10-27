# Contexto: configuración de Azure DevOps — Plataforma web IDI-FR-05

**Propósito de este archivo:** dar contexto a Claude (u otro asistente) sobre cómo está configurado el proyecto de Azure DevOps del trabajo de grado, para que sus respuestas sean coherentes con las decisiones ya tomadas.
**Última actualización:** 2026-10-26
**Documentos relacionados:** `contextualizacion.md` (contexto, alcance y objetivos `OE1`–`OE4`), `plataforma_web_descripcion.md` (especificación funcional: `V-xx`, `RF-xx`, `RN-xx`, `RNF-xx`, `PA-xx`), `documento_fundacional.md` (decisiones `ADR-xxx` y plan de trabajo), `gestion_cambios.md` (`GC-xxx`), ficha `IDI-FR-05` del macroproyecto.

---

## 1. Resumen del proyecto

- **Producto:** plataforma web que recibe, valida, almacena, visualiza y exporta datos de una red de estaciones IoT de calidad del aire con sensores de bajo costo (Rionegro, Antioquia), e incluye un módulo de calibración.
- **Origen:** producto de los objetivos específicos 2 y 3 del macroproyecto IDI-FR-05 (Universidad Católica de Oriente). Subtipo del producto: "Reportes interactivos y plataforma web de reporte tipo dashboard".
- **Contexto académico:** trabajo de grado de dos estudiantes de Ingeniería de Sistemas. Toda decisión de configuración debe poder **justificarse teóricamente y citarse** en el documento de sustentación.
- **Restricciones relevantes:** presupuesto de operación de $2.000.000 COP para 24 meses (`R-01`); equipo de 2 desarrolladores con dedicación parcial (`R-04`); zona horaria UTC−5 (`R-09`).
- **Preferencia del equipo:** toda la configuración de Azure DevOps se hace **a mano por la interfaz gráfica**. No proponer scripts (Bash, PowerShell, CLI, REST) ni YAML salvo que se pida expresamente.

---

## 2. Metodología

| Decisión | Valor |
|---|---|
| Marco de trabajo | **Scrum adaptado** a un equipo de 2 developers (`ADR-001`, Aceptada) |
| Plantilla de proceso en Azure DevOps | **Agile**, mediante el proceso heredado `Agile-IDI-FR-05` |
| Sprint | **1 semana**, de lunes a domingo (se trabaja de lunes a viernes) |
| Release | **Cada 2 Sprints** (2 semanas) |
| Kanban dentro del Sprint | Límite de WIP 2 en desarrollo y en revisión |

**Justificación usada en la tesis (resumen):**
- Factores de Boehm y Turner (2003): equipo pequeño, criticidad baja (datos indicativos), dinamismo alto (muchos `PA`/`PC` abiertos) → enfoque ágil.
- La plantilla Agile se eligió por las horas estimadas frente a reales en las tareas (*Original Estimate* y *Completed Work*), por las User Stories con Story Points y por el estado *Resolved*, que permite distinguir "implementado" de "aceptado por el PO".
- CMMI se descartó por sobrecarga de proceso. "Agile" en Azure DevOps es una plantilla, no una metodología: con ella se ejecuta Scrum.

**Eventos (timeboxes adaptados a Sprints de 1 semana):**

| Evento | Duración |
|---|---|
| Sprint Planning (lunes; martes si el lunes es festivo) | ≤ 1,5 h |
| Daily Scrum (puede ser asíncrono) | 15 min |
| Sprint Review | 30–45 min con el PO; cada 2 Sprints es la revisión del Release, donde se **validan las funcionalidades** (sección 8.3) |
| Retrospective | 30 min |

---

## 3. Roles y permisos

| Persona | Responsabilidad ágil | Nivel de acceso (organización) | Grupo del proyecto |
|---|---|---|---|
| Estudiante 1 | Developer + Scrum Master | Basic | `Desarrollo` → Project Administrators |
| Estudiante 2 | Developer | Basic | `Desarrollo` → Contributors + Build Administrators |
| Investigador 1 (director del proyecto) | **Product Owner único** y stakeholder principal; dueño del proyecto | Basic | `Investigación` → Project Administrators |
| Investigador 2 | Stakeholder (consultado); sin rol de PO | Basic | `Investigación` → Contributors |
| Director de tesis (opcional) | Supervisión académica | Stakeholder o Basic | Readers |

- **Product Owner único (confirmado con los investigadores):** el Investigador 1 prioriza el backlog y acepta las historias y las funcionalidades. El Investigador 2 aporta requisitos por Issues, pero no acepta trabajo. Cumple la Guía de Scrum (un solo PO).
- Para no mezclar la aceptación del producto con la evaluación académica, cada Review registra por separado la **aceptación** (como PO) y las **observaciones académicas** (`RG-19` del documento fundacional).
- Todos tienen Basic porque la organización incluye 5 usuarios Basic gratuitos y el nivel Stakeholder no permite reordenar el backlog.
- Project Collection Administrators: Estudiante 1 e Investigador 1, para que no haya un punto único de falla.
- Criterio aplicado: mínimo privilegio (Saltzer y Schroeder, 1975).

---

## 4. Organización y proyecto

**Organización:**
- Zona horaria: (UTC−05:00) Bogotá.
- *Allow public projects*: Off.
- *Allow team and project administrators to invite new users*: Off.

**Proyecto:**
- Visibilidad: **Private** (el repositorio hereda esta visibilidad).

| Servicio | Estado | Motivo |
|---|---|---|
| Boards | On | Gestión del trabajo |
| Repos | **On** | Código en Azure Repos, repositorio privado (sección 10) |
| Pipelines | On | CI/CD |
| Test Plans | Off | Licencia de pago |
| Artifacts | Off | No se publican paquetes por ahora |

---

## 5. Proceso heredado `Agile-IDI-FR-05`

Se hereda de Agile, porque los procesos de sistema no se pueden modificar. Personalizaciones:

| Elemento | Personalización |
|---|---|
| Epic | Campo `Objetivo específico` (picklist `OE1`–`OE4`, numeración vigente desde `GC-004`). |
| Feature | Campo `Requisitos asociados` (vistas y requisitos, por ejemplo `V-14, V-06, RN-28`). El campo *Acceptance Criteria* se usa para los **escenarios de validación** de la funcionalidad; si el formulario no lo muestra, se agrega en el proceso heredado. |
| User Story | Campo `Requisitos asociados` (texto de una línea, grupo "Trazabilidad"). Ejemplo: `RF-40, RF-41`. |
| Nuevo tipo `Riesgo` | Campos `Probabilidad` y `Impacto` (Alta/Media/Baja), `Respuesta` y `Disparador`. Estados: Identificado → Monitoreado → Materializado → Cerrado. No pertenece a ningún backlog; se gestiona por consulta. Registra los `RG-xx` de `contextualizacion.md` §8 y los del documento fundacional. |

- **No se modificaron los estados** de Feature ni de User Story (New → Active → Resolved → Closed). La granularidad visual se maneja con las columnas del tablero.
- La trazabilidad buscada es: objetivo específico → Epic → Feature (funcionalidad) → User Story → commit/PR → prueba.

---

## 6. Equipo y áreas

**Equipo único:** `Plataforma Web` (el equipo por defecto, renombrado).
- Miembros: los dos estudiantes y el Investigador 1.
- Administradores: Estudiante 1 e Investigador 1.

**Configuración del equipo:**

| Opción | Valor |
|---|---|
| Niveles de backlog | Epics, Features, Stories |
| Días laborables | Lunes a viernes |
| Bugs | Gestionados con los requisitos (en el backlog) |
| Área del equipo | Raíz `IDI-FR-05`, con *Include sub areas* |
| Área por defecto | Raíz |

**Áreas, por módulo de la plataforma.** El área indica *de qué módulo* es un work item, sin importar la épica; permite filtrar el tablero y las consultas por módulo (por ejemplo, todo lo de "Estaciones y sensores"). Reemplaza la organización anterior por componentes, que tenía el área `PQRS` (retirada en `GC-001`) y el ICA (fuera del alcance desde `GC-005`).

```
IDI-FR-05
├── Gestión y documentación      planeación, actas, informe de tesis, planes de riesgo y costos
├── Requisitos y diseño          especificación, mockups, arquitectura, modelo de datos
├── Integración MQTT             contrato, simulador, recepción y validación de lecturas
├── Estaciones y sensores        V-06 a V-08, V-14, V-18, V-25 a V-28
├── Consulta pública             V-02 a V-04
├── Descargas                    V-05
├── Acceso y cuentas             V-01, V-10 a V-13, V-19, V-21
├── Alertas y configuración      V-15 a V-17, estados automáticos
├── Analítica y calibración      V-29 a V-32
└── Infraestructura y DevOps     despliegue, respaldos, seguridad, monitoreo, CI/CD
```

**Columnas del tablero (nivel Stories):**

| Columna | Estado | WIP |
|---|---|---|
| Nuevo | New | — |
| Listo | New | — |
| En desarrollo (*split*: Doing/Done) | Active | 2 |
| En revisión de código | Active | 2 |
| Validación PO | Resolved | — |
| Hecho | Closed | — |

**Columnas del tablero (nivel Features):** Nuevo (New) → En construcción (Active) → **Por validar** (Resolved: todas sus historias cerradas) → **Validada** (Closed: el PO aprobó los escenarios de validación en la Review del Release).

---

## 7. Iteraciones: Sprints y Releases

Las iteraciones son **jerárquicas**: un nodo `Release NN` contiene `Sprint NN` y `Sprint NN+1`.

- El equipo **solo está suscrito a los Sprints** (nodos hoja), nunca a los nodos Release.
- *Backlog iteration*: raíz `IDI-FR-05`.
- *Default iteration*: `@CurrentIteration`.
- El alcance de un Release se consulta con `Iteration Path Under IDI-FR-05\Release NN`.
- El "Release" es un concepto de planificación; no es lo mismo que *Pipelines → Releases*.

| Nodo | Inicio | Fin | Festivos (*Team days off*) |
|---|---|---|---|
| Release 01 | 2026-09-28 | 2026-10-11 | |
| └ Sprint 01 | 2026-09-28 | 2026-10-04 | |
| └ Sprint 02 | 2026-10-05 | 2026-10-11 | |
| Release 02 | 2026-10-12 | 2026-10-25 | |
| └ Sprint 03 | 2026-10-12 | 2026-10-18 | 12 oct |
| └ Sprint 04 | 2026-10-19 | 2026-10-25 | |
| Release 03 | 2026-10-26 | 2026-11-08 | |
| └ Sprint 05 | 2026-10-26 | 2026-11-01 | |
| └ Sprint 06 | 2026-11-02 | 2026-11-08 | 2 nov |
| Release 04 | 2026-11-09 | 2026-11-22 | |
| └ Sprint 07 | 2026-11-09 | 2026-11-15 | |
| └ Sprint 08 | 2026-11-16 | 2026-11-22 | 16 nov |
| Release 05 | 2026-11-23 | 2026-11-29 | |
| └ Sprint 09 | 2026-11-23 | 2026-11-29 | |

- **Excepción:** el Release 05 tiene un solo Sprint porque la Fase 2 termina el 29 de noviembre; es solo de estabilización (congelamiento de funcionalidades desde el 22 de noviembre).
- Si el calendario se extiende (`PC-12`), se sigue la regla: cada Release dura 2 semanas y contiene 2 Sprints de lunes a domingo.
- La capacidad se registra por persona según su dedicación real, con *Activity* (Development, Documentation, Testing…).
- Al cierre de cada Release se crea en Azure Repos un tag con versionado semántico (Release 01 → `v0.1.0`; la entrega final es `v1.0.0`) y un acta en la Wiki.

---

## 8. Work items

### 8.1 Estructura: objetivos y funcionalidades

La estructura tiene dos ejes:

- **Épica = objetivo específico** (`OE1`–`OE4`). Responde *para qué* se hace el trabajo y da la trazabilidad con los objetivos del trabajo de grado.
- **Feature = funcionalidad validable** de la plataforma (o entregable del proyecto, en `E1`). Responde *qué se puede demostrar*: es concreta (por ejemplo, "Inicio de sesión y recuperación de contraseña" o "Administración de estaciones"), agrupa las vistas `V-xx` que la componen y tiene **escenarios de validación** que el PO aprueba. Es la unidad con la que se valida la plataforma.
- **Área = módulo** (sección 6), para filtrar por módulo a través de las épicas.

| Tipo | Horizonte / tamaño | Uso |
|---|---|---|
| Epic | Todo el proyecto; una por objetivo específico | Objetivos del trabajo de grado; campo `Objetivo específico` |
| Feature | Cabe en 1 Release | Funcionalidad validable; *Start Date* y *Target Date*; `Requisitos asociados`; escenarios de validación en *Acceptance Criteria* |
| User Story | Cabe en 1 Sprint; ≤ 8 Story Points (Fibonacci) | "Como `<rol>`, quiero… para…"; criterios *Dado/Cuando/Entonces*; campo `Requisitos asociados` |
| Task | ≤ 1 día | Horas: *Original Estimate*, *Remaining Work*, *Completed Work* |
| Bug | Dentro de un Sprint | Defectos de lo ya entregado |
| Issue | Variable | Cada `PA-xx`/`PC-xx` pendiente, asignado a quien lo gestiona, con *Due Date* y vinculado (*Predecessor/Successor*) con las historias que bloquea |
| Riesgo | Todo el proyecto | Registro `RG-xx` |

### 8.2 Épicas y funcionalidades

Convención del título: `F<épica>.<n> <funcionalidad>`. Prioridad MoSCoW según el MVP del documento fundacional (§3.5), ajustada a las decisiones posteriores (sin ICA, contrato configurable por estación).

| Feature | Área | Vistas o requisitos | Prioridad |
|---|---|---|---|
| **E1. Ciclo de vida y gestión del proyecto** (`OE1`) | | | |
| F1.1 Planeación y metodología (documento fundacional, ADR de proceso, planes de riesgos y de costos) | Gestión y documentación | `OE1`, `R-01`, `RG-xx` | Must |
| F1.2 Requisitos y diseño (especificación, mockups, arquitectura, modelo de datos) | Requisitos y diseño | DOC-03, DOC-07, DOC-09, DOC-12 | Must |
| F1.3 Validación con usuarios (aceptación y usabilidad) | Requisitos y diseño | `RNF-12`, DOC-21 | Must |
| F1.4 Seguimiento, actas y documento del trabajo de grado (tag `tesis`) | Gestión y documentación | DOC-22, DOC-23 | Must |
| F1.5 Cierre y transferencia de conocimiento | Gestión y documentación | `RG-04`, `PC-06` | Must |
| **E2. Integración y gestión de datos de las estaciones** (`OE2`) | | | |
| F2.1 Contrato de integración MQTT y simulador de estaciones | Integración MQTT | DOC-10, `S-03`, `PC-02` | Must |
| F2.2 Recepción y validación de lecturas (marca de tiempo, huecos y desconexión) | Integración MQTT | `RN-29`, `PC-02`, `PC-09` | Must |
| F2.3 Administración de estaciones (crear, editar, habilitar/deshabilitar, eliminar) | Estaciones y sensores | V-14, V-06, V-07, V-08 | Must |
| F2.4 Catálogo de sensores y asignación a estaciones | Estaciones y sensores | V-25, V-26 | Must |
| F2.5 Credenciales y formato del mensaje por estación | Estaciones y sensores | V-27, V-28, `C-12` | Must |
| F2.6 Estados automáticos y alertas | Alertas y configuración | `RN-15`, `RN-16`, V-16, V-17 | Must (estados) / Should (alertas por umbral) |
| F2.7 Estaciones eliminadas | Estaciones y sensores | V-18 | Could |
| **E3. Consulta pública, acceso y operación de la plataforma** (`OE3`) | | | |
| F3.1 Mapa de estaciones por umbrales | Consulta pública | V-02, `PC-22` | Must |
| F3.2 Listado y detalle de estaciones con gráficos y tablas | Consulta pública | V-03, V-04 | Must |
| F3.3 Descarga de datos (CSV/JSON con límites) | Descargas | V-05, `RNF-10` | Must |
| F3.4 Inicio de sesión y recuperación de contraseña | Acceso y cuentas | V-01, V-10, V-21, `RNF-07` | Must |
| F3.5 Gestión de administradores y perfil | Acceso y cuentas | V-13, V-11, V-12, V-19 | Should / Could |
| F3.6 Configuración de umbrales y del sistema | Alertas y configuración | V-15 | Should |
| F3.7 Entornos, CI/CD y despliegue en producción | Infraestructura y DevOps | `OE3`, sección 10 | Must |
| F3.8 Seguridad, respaldos y monitoreo | Infraestructura y DevOps | `RNF-03`, `RG-05` | Must |
| F3.9 Manuales y plan de operación a 24 meses | Infraestructura y DevOps | DOC-18, DOC-19, DOC-20 | Must |
| **E4. Calibración y analítica de datos** (`OE4`) | | | |
| F4.1 Diseño de la metodología de calibración | Analítica y calibración | DOC-17, `PC-17`, `PC-19` | Must |
| F4.2 Estaciones de referencia | Analítica y calibración | V-29, `PC-10` | Should |
| F4.3 Entrenamiento y comparación de modelos | Analítica y calibración | V-30, `C-11`, `RNF-11` | Must |
| F4.4 Exportación de modelos | Analítica y calibración | V-32 | Must |
| F4.5 Estimación de variables | Analítica y calibración | V-31 | Could |

Las Features de `E2` y `E3` que dependen del inicio de sesión (F2.3 a F2.7, F3.5, F3.6) tienen a F3.4 como *Predecessor*.

### 8.3 Cómo se valida una funcionalidad

1. **Al crear la Feature** se escriben de 2 a 5 **escenarios de validación** de extremo a extremo en *Acceptance Criteria*, en lenguaje del usuario. Cada escenario se convierte en una prueba de aceptación (`CP-xxx`).
2. **Las User Stories** son los pasos concretos de la funcionalidad, normalmente una por vista o por regla de negocio.
3. **Cuando todas sus historias están cerradas**, la Feature pasa a *Por validar* (Resolved).
4. **En la Review del Release** se demuestran los escenarios en el entorno `pruebas`. Si el PO los aprueba, la Feature pasa a *Validada* (Closed) y queda registrada en el acta; si no, se crean Bugs o historias nuevas y la Feature sigue en *Por validar*.
5. **La consulta "Validación de funcionalidades"** (Features por área y estado) es la lista de control para la sustentación: muestra qué funcionalidades de la plataforma están validadas.

**Ejemplo:**

> **F2.3 Administración de estaciones** · Área: Estaciones y sensores · `Requisitos asociados`: V-14, V-06, V-07, V-08
>
> *Escenarios de validación:*
> 1. Un administrador crea una estación con su ubicación y sensores, y aparece en el mapa público.
> 2. Un administrador edita los datos de una estación y el cambio se ve en el detalle público.
> 3. Un administrador deshabilita una estación, deja de aparecer en la consulta pública y sus datos históricos se conservan.
> 4. Un administrador elimina una estación tras confirmar la acción.
> 5. Una persona sin sesión no puede acceder a ninguna de estas acciones.
>
> *User Stories:* Crear una estación (V-14) · Editar una estación (V-06) · Habilitar o deshabilitar una estación (V-07) · Eliminar una estación (V-08).

### 8.4 Definiciones y convenciones

**Definition of Ready (historia):** criterios de aceptación definidos; historia estimada; sin `PA`/`PC` abierto que la bloquee; requisitos asociados registrados; el PO la entiende y la acepta. El detalle está en el documento fundacional §3.4.

**Definition of Done (historia):** PR revisado por el otro developer y fusionado; pruebas automatizadas pasando; criterios de aceptación verificados; documentación afectada actualizada; desplegado en el entorno de pruebas; aceptado por el PO.

**Definition of Done (funcionalidad):** todas sus historias cumplen la DoD y el PO aprobó sus escenarios de validación en la Review del Release.

**Documentación:**

| Tipo | Cómo se gestiona |
|---|---|
| Inseparable de un incremento (API, README, sección del manual) | Parte de la DoD o Task con *Activity* = Documentation |
| Entregable con valor propio (especificación, planes, arquitectura, manuales) | User Story bajo la Feature de `E1` o `E3` correspondiente |
| Redacción de la tesis | User Story bajo F1.4 con tag `tesis` |
| Actas y acuerdos | Wiki del proyecto |
| Documentación técnica versionada y ADR | Carpeta `/docs` del repositorio en Azure Repos (ADR en `/docs/adr`) |
| Eventos de Scrum | **No** se registran como work items; se descuentan de la capacidad |

**Tags:** `documentación`, `tesis`, `deuda-técnica`, `bloqueado`, `simulador`, `spike`, `mvp`, `dependencia-externa`.

**Wiki:** `Acuerdos de trabajo`, `Actas/Release-NN`, `Decisiones de arquitectura` (índice con enlaces a `/docs/adr`).

---

## 9. Cronogramas y seguimiento

- **Delivery Plans:** "Roadmap IDI-FR-05", uno a nivel Features y otro a nivel Epics. Incluye marcadores de hitos (fin de cada Release, entregas académicas, primera estación real, sustentación) y dependencias *Predecessor/Successor*.
- **Forecast** del backlog con la velocidad promedio.
- **Dashboard "Seguimiento del proyecto":**
  - Sprint Burndown, Velocity y Cumulative Flow
  - Cycle Time y Lead Time
  - Burnup por Release (filtro `Iteration Path Under Release NN`, rango de fechas del Release)
  - Query tiles de Issues abiertos, de Riesgos y de **funcionalidades validadas / por validar**
- **Cronograma para la tesis:** consulta en árbol Epics → Features → Stories exportada con *Export to CSV* y graficada como Gantt.
- **Extensiones** (Microsoft DevLabs, gratuitas): Retrospectives y Estimate. Se descartaron las extensiones de Gantt de pago por `R-01`.

---

## 10. Repositorio y CI/CD (Azure Repos)

**Decisión (`ADR-002`, Aceptada):** el código vive en **Azure Repos**, en un repositorio **privado** (`plataforma-idi-fr-05`, monorepo según `ADR-014`). Se eligió sobre GitHub porque:
- Boards, Repos y Pipelines quedan en un mismo proyecto: los commits, ramas y PR se enlazan con los work items sin conexiones externas, lo que facilita la trazabilidad historia → PR → build y la validación de las historias.
- Las políticas de rama funcionan sin costo en repositorios privados (en GitHub requieren un plan de pago).
- Los pipelines usan el repositorio sin *service connections* a terceros.

**Enlace con work items:**
- Las ramas se crean desde el work item (*Create branch*) o se enlazan al crear el PR.
- Los commits mencionan el work item con `#<id>`.
- En los PR hacia `develop` se deja desmarcado *Complete associated work items after merging*: la historia la cierra el PO (columna *Validación PO*).

**Políticas de rama (`main` y `develop`):**

| Política | Valor |
|---|---|
| *Require a minimum number of reviewers* | 1; el autor no puede aprobar sus propios cambios |
| *Check for linked work items* | Required |
| *Check for comment resolution* | Required |
| *Limit merge types* | `develop`: squash merge; `main`: merge (no fast-forward) |
| *Build validation* | Pipeline de verificación; se activa cuando exista el primer pipeline |
| *Automatically included reviewers* | El otro developer (reemplaza a CODEOWNERS) |

- En la seguridad del repositorio, *Force push* y *Bypass policies* quedan en **Deny** para todos los grupos.
- Plantilla de PR en `.azuredevops/pull_request_template.md`.

**Pipelines:**
- **Parallel jobs:** se solicitó (o debe solicitarse) el grant gratuito de agentes hospedados por Microsoft para proyectos privados.
- *Limit job authorization scope to current project*: On
- *Protect access to repositories in YAML pipelines*: On
- *Disable creation of classic build/release pipelines*: On
- **Environments:** `pruebas` y `produccion`. Producción tiene aprobación manual (Estudiante 1 o Investigador 1).
- **Library:** variable group `plataforma-comun`. Los secretos se marcan como *secret* y nunca van en el repositorio.

**Análisis estático (`ADR-015`, Propuesta):** con el repositorio privado, CodeQL en Azure DevOps exige GitHub Advanced Security for Azure DevOps, que es de pago, y se descarta por `R-01`. Se propone:
- En el pipeline, sin costo: *linters* y verificación de tipos (sección 5.7 y 5.8 del documento fundacional) y auditoría de dependencias.
- **SonarQube Cloud, plan gratuito**, que admite proyectos privados con un límite de líneas de código. Antes de adoptarlo hay que verificar el límite vigente frente al tamaño esperado del monorepo.

---

## 11. Pendientes

- **Análisis estático:** confirmar las condiciones vigentes del plan gratuito de SonarQube Cloud y cerrar `ADR-015`.
- **Validar con el director la estructura de work items** (sección 8, `PC-21`).
- **Fechas fuera del calendario:** primera estación real (`HT-07`) y presentación al comité (`HT-10`) dependen de `PC-12`.
- **Propiedad de la organización:** transferirla al Investigador 1 o a una cuenta institucional por continuidad (`RG-04`, `PC-06`, `PC-07`).

---

## 12. Referencias base de la justificación

Boehm y Turner (2003); Boehm (2002); Schwaber y Sutherland (2020), *The Scrum Guide*; Cohn (2004, 2005); Leffingwell (2011); Wake (2003), INVEST; Dybå y Dingsøyr (2008); Dingsøyr et al. (2012); Serrador y Pinto (2015); Rising y Janoff (2000); CMMI Product Team (2010); Anderson (2010); Kniberg y Skarin (2010); Ambler (2002); Rüping (2003); Humble y Farley (2010); ISO/IEC/IEEE 29148:2018; Saltzer y Schroeder (1975); Nygard (2011); Preston-Werner, SemVer; documentación de Azure DevOps en Microsoft Learn (procesos, proceso heredado, acceso Stakeholder, iteraciones, Delivery Plans, políticas de rama de Azure Repos, enlace de work items con commits y PR, plantillas de PR).
