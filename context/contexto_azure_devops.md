# Contexto: configuración de Azure DevOps — Plataforma web IDI-FR-05

**Propósito de este archivo:** dar contexto a Claude (u otro asistente) sobre cómo está configurado el proyecto de Azure DevOps del trabajo de grado, para que sus respuestas sean coherentes con las decisiones ya tomadas.
**Última actualización:** 2026-10-01
**Documentos relacionados:** `contextualizacion.md` (contexto y alcance), `plataforma_web_descripcion.md` (especificación funcional: `V-xx`, `RF-xx`, `RN-xx`, `RNF-xx`, `PA-xx`), ficha `IDI-FR-05` del macroproyecto.

---

## 1. Resumen del proyecto

- **Producto:** plataforma web que recibe, valida, almacena, visualiza y exporta datos de una red de estaciones IoT de calidad del aire con sensores de bajo costo (Rionegro, Antioquia).
- **Origen:** producto del objetivo específico 3 del macroproyecto IDI-FR-05 (Universidad Católica de Oriente). Subtipo del producto: "Reportes interactivos y plataforma web de reporte tipo dashboard".
- **Contexto académico:** trabajo de grado de dos estudiantes de Ingeniería de Sistemas. Toda decisión de configuración debe poder **justificarse teóricamente y citarse** en el documento de sustentación.
- **Restricciones relevantes:** presupuesto de operación de $2.000.000 COP para 24 meses (`R-01`); equipo de 2 desarrolladores con dedicación parcial (`R-04`); zona horaria UTC−5 (`R-09`).
- **Preferencia del equipo:** toda la configuración de Azure DevOps se hace **a mano por la interfaz gráfica**. No proponer scripts (Bash, PowerShell, CLI, REST) ni YAML salvo que se pida expresamente.

---

## 2. Metodología

| Decisión | Valor |
|---|---|
| Marco de trabajo | **Scrum adaptado** a un equipo de 2 developers |
| Plantilla de proceso en Azure DevOps | **Agile**, mediante el proceso heredado `Agile-IDI-FR-05` |
| Sprint | **1 semana**, de lunes a domingo |
| Release | **Cada 2 Sprints** (2 semanas) |
| Kanban dentro del Sprint | Límite de WIP 2 en desarrollo y en revisión |

**Justificación usada en la tesis (resumen):**
- Factores de Boehm y Turner (2003): equipo pequeño, criticidad baja (datos indicativos), dinamismo alto (muchos `PA`/`PC` abiertos) → enfoque ágil.
- La plantilla Agile se eligió por las horas estimadas frente a reales en las tareas (*Original Estimate* y *Completed Work*), por las User Stories con Story Points y por el estado *Resolved*, que permite distinguir "implementado" de "aceptado por el PO".
- CMMI se descartó por sobrecarga de proceso. "Agile" en Azure DevOps es una plantilla, no una metodología: con ella se ejecuta Scrum.

**Eventos (timeboxes adaptados a Sprints de 1 semana):**

| Evento | Duración |
|---|---|
| Sprint Planning (lunes) | ≤ 1,5 h |
| Daily Scrum (puede ser asíncrono) | 15 min |
| Sprint Review | 30–45 min con el PO; cada 2 Sprints, revisión de Release con ambos investigadores |
| Retrospective | 30 min |

---

## 3. Roles y permisos

| Persona | Responsabilidad ágil | Nivel de acceso (organización) | Grupo del proyecto |
|---|---|---|---|
| Estudiante 1 | Developer + Scrum Master | Basic | `Desarrollo` → Project Administrators |
| Estudiante 2 | Developer | Basic | `Desarrollo` → Contributors + Build Administrators |
| Investigador 1 | **Product Owner** y stakeholder principal; dueño del proyecto | Basic | `Investigación` → Project Administrators |
| Investigador 2 | Co-dueño institucional / stakeholder | Basic | `Investigación` → Contributors |
| Director de tesis (opcional) | Supervisión académica | Stakeholder o Basic | Readers |

- Todos tienen Basic porque la organización incluye 5 usuarios Basic gratuitos y el nivel Stakeholder no permite reordenar el backlog.
- Project Collection Administrators: Estudiante 1 e Investigador 1, para que no haya un punto único de falla.
- Se recomendó transferir la propiedad de la organización al Investigador 1 o a una cuenta institucional por continuidad (`RG-04`, `PC-06`).
- Criterio aplicado: mínimo privilegio (Saltzer y Schroeder, 1975).

---

## 4. Organización y proyecto

**Organización:**
- Zona horaria: (UTC−05:00) Bogotá.
- *Allow public projects*: Off.
- *Allow team and project administrators to invite new users*: Off.

**Proyecto:**
- Visibilidad: Private.

| Servicio | Estado | Motivo |
|---|---|---|
| Boards | On | Gestión del trabajo |
| Repos | Off | El código está en GitHub |
| Pipelines | On | CI/CD futuro |
| Test Plans | Off | Licencia de pago |
| Artifacts | Off | No se publican paquetes por ahora |

---

## 5. Proceso heredado `Agile-IDI-FR-05`

Se hereda de Agile, porque los procesos de sistema no se pueden modificar. Personalizaciones:

| Elemento | Personalización |
|---|---|
| User Story | Campo `Requisitos asociados` (texto de una línea, grupo "Trazabilidad"). Ejemplo: `RF-40, RF-41`. |
| Epic | Campo `Objetivo específico` (picklist `OE1`–`OE5`). |
| Nuevo tipo `Riesgo` | Campos `Probabilidad` y `Impacto` (Alta/Media/Baja), `Respuesta` y `Disparador`. Estados: Identificado → Monitoreado → Materializado → Cerrado. No pertenece a ningún backlog; se gestiona por consulta. Registra `RG-01` a `RG-09`. |

- **No se modificaron los estados de User Story** (New → Active → Resolved → Closed). La granularidad visual se maneja con las columnas del tablero.
- La trazabilidad buscada es: objetivo específico → Epic → Feature → User Story → commit/PR → prueba.

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

**Áreas, por componente funcional:**

```
IDI-FR-05
├── Gestión y documentación      planeación, actas, informe de tesis, planes de riesgo y costos
├── Requisitos y diseño          especificación, mockups, arquitectura, modelo de datos
├── Ingesta de datos             contrato de integración, simulador, validación, almacenamiento
├── Visualización                V-02, V-03, V-04, gráficos, tablas, ICA
├── Exportación                  V-05, CSV/JSON
├── Identidad y acceso           V-01, V-09 a V-13, V-19 a V-21, roles
├── Administración               V-06 a V-08, V-14 a V-18, estados, alertas
├── PQRS                         V-22 a V-24
└── Infraestructura y DevOps     despliegue, respaldos, monitoreo, CI/CD
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

- La regla para continuar el calendario: cada Release dura 2 semanas y contiene 2 Sprints de lunes a domingo. Se crean 2 o 3 Releases por adelantado.
- La capacidad se registra por persona según su dedicación real, con *Activity* (Development, Documentation, Testing…).
- Al cierre de cada Release se crea en GitHub un tag con versionado semántico (Release 01 → `v0.1.0`) y un acta en la Wiki.

---

## 8. Work items

**Jerarquía y tamaño:**

| Tipo | Horizonte / tamaño | Uso |
|---|---|---|
| Epic | Varios Releases; una por objetivo específico | Objetivos del trabajo de grado |
| Feature | Cabe en 1 Release | Capacidad demostrable; usar *Start Date* y *Target Date* |
| User Story | Cabe en 1 Sprint; ≤ 8 Story Points (Fibonacci) | "Como `<rol>`, quiero… para…"; criterios *Dado/Cuando/Entonces*; campo `Requisitos asociados` |
| Task | ≤ 1 día | Horas: *Original Estimate*, *Remaining Work*, *Completed Work* |
| Bug | Dentro de un Sprint | Defectos de lo ya entregado |
| Issue | Variable | Cada `PA-xx`/`PC-xx` pendiente, asignado al investigador que decide y vinculado (*Predecessor/Successor*) con las historias que bloquea |
| Riesgo | Todo el proyecto | Registro `RG-xx` |

**Épicas:**

| Épica | Objetivo específico |
|---|---|
| E1. Ciclo de vida y gestión del proyecto | OE1 |
| E2. Levantamiento y validación de requerimientos | OE2 |
| E3. Contrato de integración y gestión de datos | OE3 |
| E4. Visualización, ICA y exportación | OE4 |
| E5. Despliegue, operación y respaldo | OE5 |
| E6. Seguridad, administración y soporte (transversal) | OE4 / OE5 |

**Definition of Ready:** criterios de aceptación definidos; historia estimada; sin `PA`/`PC` abierto que la bloquee; requisitos asociados registrados; el PO la entiende y la acepta.

**Definition of Done:** PR revisado por el otro developer y fusionado; pruebas automatizadas pasando; criterios de aceptación verificados; documentación afectada actualizada; desplegado en el entorno de pruebas; aceptado por el PO.

**Documentación:**

| Tipo | Cómo se gestiona |
|---|---|
| Inseparable de un incremento (API, README, sección del manual) | Parte de la DoD o Task con *Activity* = Documentation |
| Entregable con valor propio (especificación, planes, arquitectura, manuales) | User Story bajo E1 o E2 |
| Redacción de la tesis | User Story bajo E1 con tag `tesis` |
| Actas y acuerdos | Wiki del proyecto |
| Documentación técnica versionada | Carpeta `/docs` del repositorio de GitHub |
| Eventos de Scrum | **No** se registran como work items; se descuentan de la capacidad |

**Tags:** `documentación`, `tesis`, `deuda-técnica`, `bloqueado`, `simulador`.

**Wiki:** `Acuerdos de trabajo`, `Actas/Release-NN`, `Decisiones de arquitectura` (formato ADR).

---

## 9. Cronogramas y seguimiento

- **Delivery Plans:** "Roadmap IDI-FR-05", uno a nivel Features y otro a nivel Epics. Incluye marcadores de hitos (fin de cada Release, entregas académicas, primera estación real, sustentación) y dependencias *Predecessor/Successor*.
- **Forecast** del backlog con la velocidad promedio.
- **Dashboard "Seguimiento del proyecto":**
  - Sprint Burndown, Velocity y Cumulative Flow
  - Cycle Time y Lead Time
  - Burnup por Release (filtro `Iteration Path Under Release NN`, rango de fechas del Release)
  - Query tiles de Issues abiertos y de Riesgos
- **Cronograma para la tesis:** consulta en árbol Epics → Features → Stories exportada con *Export to CSV* y graficada como Gantt.
- **Extensiones** (Microsoft DevLabs, gratuitas): Retrospectives y Estimate. Se descartaron las extensiones de Gantt de pago por `R-01`.

---

## 10. Preparación para CI/CD (repositorio en GitHub)

- **GitHub connection de Azure Boards**, limitada al repositorio de la plataforma. Los commits y PR usan `AB#<id>`; `Fixes AB#<id>` cierra el work item al fusionar.
- **Service connection** `github-plataforma-idi-fr-05`:
  - Método: Azure Pipelines GitHub App.
  - *Grant access permission to all pipelines* desactivado.
- **Parallel jobs:** se solicitó (o debe solicitarse) el grant gratuito de agentes hospedados por Microsoft.
- **Configuración de Pipelines:**
  - *Limit job authorization scope to current project*: On
  - *Protect access to repositories in YAML pipelines*: On
  - *Disable creation of classic build/release pipelines*: On
- **Environments:** `pruebas` y `produccion`. Producción tiene aprobación manual (Estudiante 1 o Investigador 1).
- **Library:** variable group `plataforma-comun`. Los secretos se marcan como *secret* y nunca van en el repositorio.
- **GitHub:** protección de `main` con PR obligatorio y 1 revisión. Los *status checks* de Azure Pipelines se activarán cuando exista el primer pipeline.
- **Análisis estático:** pendiente decidir entre SonarQube Cloud y CodeQL. Ambos son gratuitos solo en repositorios públicos; si el repositorio es privado hay costo (`R-01`).

---

## 11. Pendientes y decisiones abiertas

- **Product Owner único:** la Guía de Scrum exige un solo PO. Se propuso al Investigador 1 como PO y al Investigador 2 como co-dueño y stakeholder. Falta confirmarlo con los investigadores.
- **Calendario completo** de Sprints y Releases hasta la sustentación: depende de `PC-12`.
- **Actualizar `contextualizacion.md` §5**, que todavía menciona iteraciones de 2 semanas. Con esto se cierra `PC-11`.
- **Visibilidad del repositorio** (público o privado): afecta la herramienta de análisis de código y su costo.

---

## 12. Referencias base de la justificación

Boehm y Turner (2003); Boehm (2002); Schwaber y Sutherland (2020), *The Scrum Guide*; Cohn (2004, 2005); Leffingwell (2011); Wake (2003), INVEST; Dybå y Dingsøyr (2008); Dingsøyr et al. (2012); Serrador y Pinto (2015); Rising y Janoff (2000); CMMI Product Team (2010); Anderson (2010); Kniberg y Skarin (2010); Ambler (2002); Rüping (2003); Humble y Farley (2010); ISO/IEC/IEEE 29148:2018; Saltzer y Schroeder (1975); Nygard (2011); Preston-Werner, SemVer; documentación de Azure DevOps en Microsoft Learn (procesos, proceso heredado, acceso Stakeholder, iteraciones, Delivery Plans, conexión con GitHub).
