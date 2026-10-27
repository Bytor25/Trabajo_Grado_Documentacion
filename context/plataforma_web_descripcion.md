# Plataforma web — Especificación funcional

Este documento describe, de forma secuencial, cómo debe verse y funcionar la plataforma web de monitoreo de estaciones. Sirve como insumo para:

- **Diseño del mockup:** cada vista detalla su estructura de arriba hacia abajo, sus componentes, sus interacciones y sus estados (carga, vacío, error).
- **Ingeniería de requerimientos:** cada requisito tiene un identificador único para poder trazarlo hacia el diseño, el desarrollo y las pruebas.

**Versión:** 6. El historial de cambios a partir de esta versión se registra en [gestion_cambios.md](gestion_cambios.md).

**Convenciones de identificadores**

| Prefijo | Significado |
|---------|-------------|
| `V-xx` | Vista o ventana emergente |
| `C-xx` | Componente reutilizable de interfaz |
| `RF-Gxx` | Requisito funcional global (aplica a todas las vistas) |
| `RF-xx` | Requisito funcional. La decena indica la vista: los requisitos de `V-n` se numeran desde `RF-(n-1)0` (por ejemplo, V-04 → `RF-30`…`RF-39`; V-11 → `RF-100`…`RF-109`). |
| `RN-xx` | Regla de negocio |
| `RNF-xx` | Requisito no funcional |
| `PA-xx` | Punto abierto (tema pendiente por validar) |

Los identificadores retirados no se reutilizan; por eso hay saltos en la numeración de vistas, requisitos y reglas.

Los elementos marcados como **(propuesto)** no vienen del stakeholder: son sugerencias para completar la especificación y deben validarse. Los marcados como **(preliminar)** corresponden al módulo de calibración, cuyos requisitos están en [requerimientos_modulo_calibracion.md](requerimientos_modulo_calibracion.md) y en la [Reunión 26/10](<reuniones/Reunión 261026.md>) (`PCAL-xx`); quedan detalles abiertos en `PA-52` y `PA-54`.

---

## 1. Información general

### 1.1 Nombre de la plataforma

Lo elige el equipo de desarrollo; el nombre aún no se ha definido (`PA-01`).

### 1.2 Roles

| Rol | Rol en el sistema | Descripción |
|-----|-------------------|-------------|
| Visitante | — (sin sesión) | Cualquier persona, sin registrarse ni iniciar sesión. Consulta el mapa, el listado y el detalle de las estaciones, los gráficos y las tablas de datos, y genera archivos de descarga. Se comunica con los responsables de la plataforma por el correo de contacto del pie de página (`C-10`). |
| Administrador | `admin` | Inicia sesión con correo y contraseña. Además de lo anterior, gestiona las estaciones (crear, editar, habilitar/deshabilitar, ocultar/mostrar y eliminar), el catálogo de sensores y los sensores de cada estación, las credenciales de conexión de las estaciones con el bróker MQTT y el formato del mensaje MQTT; revisa las alertas, define la configuración del sistema, usa el módulo de calibración (exclusivo de los administradores; la estimación de variables está por confirmar, `PA-52`) y administra las cuentas de los demás administradores: las registra, las lista, las edita y las elimina. |

En las tablas de requisitos, *Ambos* significa que el requisito aplica al visitante y al administrador.

### 1.3 Mapa de navegación

```
Acceso público, sin sesión (barra lateral C-09 y pie de página C-10)
  Mapa principal (V-02) ──────────┐
  Estaciones (V-03) ──────────────┴──► Detalle de estación (V-04)
  Generar archivos de descarga (V-05)        [también desde V-03 y V-04]
  Iniciar sesión — administradores (V-01) ──► Recuperación de contraseña (V-10)
  Activación de cuenta — administradores (V-36)   [desde el enlace enviado al correo]

Tras iniciar sesión como administrador (además de lo anterior)
  Menú de perfil
     ├── Perfil (V-11) ──► Editar perfil (V-12)
     │                └──► Eliminación de cuenta (V-19)
     ├── Restablecer contraseña (V-21) ──► Recuperación de contraseña (V-10)
     └── Cerrar sesión

  Estaciones (V-03)
       ├── Crear estación (V-14)
       └── Estaciones eliminadas (V-18)       [acceso propuesto, PA-44]

  Detalle de estación (V-04)
       ├── Edición de estación (V-06)                [también desde V-03]
       ├── Habilitar/Deshabilitar/Mostrar (V-07)
       ├── Eliminación de estación (V-08)            [también desde V-03]
       ├── Sensores de la estación (V-26)
       └── Regenerar credenciales (V-28)

  Alertas (V-16) ──► Detalle de alerta (V-17)
  Analítica de datos (preliminar)
       ├── Calibración de sensores (V-30)
       └── Estimación de variables (V-31)     [por confirmar, PA-52]
  Catálogo de sensores (V-25)
  Formatos del mensaje MQTT (V-27)
  Configuración (V-15)
  Administradores (V-33)
       ├── Registrar administrador (V-13)
       ├── Edición de administrador (V-34)
       └── Eliminación de administrador (V-35)
```

**Estructura común:** todas las vistas comparten la barra lateral (`C-09`) y el pie de página (`C-10`), excepto el inicio de sesión (V-01), la recuperación de contraseña (V-10) y la activación de cuenta (V-36), que muestran solo el pie de página.

---

## 2. Catálogos y definiciones

### 2.1 Estados de las cuentas de administrador

Solo los administradores tienen cuenta en la plataforma; los visitantes no se registran.

| Estado (sistema) | Etiqueta en pantalla | Significado |
|------------------|----------------------|-------------|
| `New` | Pendiente de activación | Otro administrador registró la cuenta (V-13) y se envió el enlace de activación al correo, pero el nuevo administrador aún no ha establecido su contraseña. No puede iniciar sesión. Este estado solo existe durante el registro. |
| `Active` | Activa | El administrador estableció su contraseña con el enlace de activación (V-36) y puede iniciar sesión. |
| `Deleted` | — (no se muestra) | El administrador eliminó su cuenta (V-19) u otro administrador la eliminó (V-35). La cuenta se desactiva y sus datos personales se anonimizan (`RN-20`); no se puede reactivar. |

**Transiciones:**

```
Registro por otro administrador (V-13) ──► New ──(establece su contraseña con el enlace de activación, V-36)──► Active
Active ──(elimina su cuenta, V-19, u otro administrador la elimina, V-35)──► Deleted (final)
New ──(otro administrador la elimina, V-35; propuesto)──► Deleted (final)
```

El primer administrador se crea durante el despliegue de la plataforma (propuesto, `PA-48`).

### 2.2 Estados de las estaciones

| Estado (sistema) | Etiqueta en pantalla | Color | Significado |
|------------------|----------------------|-------|-------------|
| `NoData` | Sin datos | <span style="display:inline-block;width:1em;height:1em;background:#9CA3AF;border:1px solid #888"></span> `#9CA3AF` (propuesto) | La plataforma no está recibiendo mensajes de la estación desde el bróker: la estación se acaba de crear o de habilitar y aún no envía su primer mensaje, o está desconectada (no llegan mensajes suyos durante el tiempo máximo sin mensajes, 10 minutos por defecto, medido con la hora de llegada al servidor, `RN-34`). A diferencia de *Inactive*, *Hidden* y *Deleted*, la plataforma sigue escuchando sus mensajes; con el primer mensaje que llega, el estado se recalcula. |
| `Active` | Activa | <span style="display:inline-block;width:1em;height:1em;background:#10B981;border:1px solid #888"></span> `#10B981` | La estación envía mensajes y todos sus sensores están en *Ok*. |
| `Warning` | Advertencia | <span style="display:inline-block;width:1em;height:1em;background:#D97706;border:1px solid #888"></span> `#D97706` | Al menos un sensor está en *Warning* y ninguno está en *Offline*. |
| `Error` | Error | <span style="display:inline-block;width:1em;height:1em;background:#DC2626;border:1px solid #888"></span> `#DC2626` | La estación envía mensajes y al menos un sensor está en *Offline*. |
| `Inactive` | Inactiva | <span style="display:inline-block;width:1em;height:1em;background:#6B7280;border:1px solid #888"></span> `#6B7280` | Un administrador deshabilitó la estación, pero sigue visible en el mapa. La plataforma deja de escuchar sus mensajes en el bróker (`RN-38`). |
| `Hidden` | Oculta | <span style="display:inline-block;width:1em;height:1em;background:#cccccc;border:1px solid #888"></span> `#cccccc` | Un administrador deshabilitó la estación y decidió ocultarla del mapa. Solo aparece en el listado del administrador. La plataforma deja de escuchar sus mensajes en el bróker (`RN-38`). |
| `Deleted` | — (no se muestra) | Sin color | Un administrador eliminó la estación. Deja de mostrarse en el mapa y en el listado, pero se conserva en la base de datos por trazabilidad y consistencia de los datos (eliminación lógica) y aparece en la vista de estaciones eliminadas (V-18). La plataforma deja de escuchar sus mensajes en el bróker (`RN-38`). |

**Cálculo automático de `NoData`, `Active`, `Warning` y `Error`** (`RN-15`), evaluado en este orden de prioridad:

1. **NoData:** la estación no ha enviado su primer mensaje desde que se creó o se habilitó, o está desconectada (tiempo máximo sin mensajes, 10 minutos por defecto).
2. **Error:** algún sensor en *Offline*.
3. **Warning:** algún sensor en *Warning*.
4. **Active:** todos los sensores en *Ok*.

**Transiciones:**

```
Crear estación ──► NoData ──(primer mensaje: cálculo automático)──► [NoData / Active / Warning / Error]
[NoData / Active / Warning / Error] ──Deshabilitar (sin ocultar)──► Inactive ──Habilitar──► NoData ──► …
[NoData / Active / Warning / Error] ──Deshabilitar (ocultar)──────► Hidden ───Mostrar────► Inactive ──Habilitar──► …
Cualquier estado ──Eliminar──► Deleted (final)
```

Al habilitar una estación, la plataforma vuelve a escuchar sus mensajes y la estación queda en *NoData* hasta que llega el primero.

Una estación *Hidden* no se puede habilitar directamente: primero se muestra (pasa a *Inactive* y vuelve al mapa) y después se habilita. Falta confirmar si una estación *Inactive* se puede ocultar sin habilitarla antes (`PA-37`).

**Visibilidad según el estado:**

| Estado | Mapa (V-02), ambos roles | Listado, visitante (V-03) | Listado, administrador (V-03) | Estaciones eliminadas (V-18) |
|--------|:------------------------:|:-------------------------:|:-----------------------------:|:----------------------------:|
| `NoData`, `Active`, `Warning`, `Error` | ✔ | ✔ | ✔ | |
| `Inactive` | ✔ | ✔ | ✔ | |
| `Hidden` | ✘ | ✘ | ✔ (indicador con fondo `#cccccc`) | |
| `Deleted` | ✘ | ✘ | ✘ | ✔ |

**Datos históricos de estaciones ocultas o eliminadas** (`RN-39`): desde el momento en que una estación pasa a *Hidden* o *Deleted*, sus datos históricos dejan de graficarse y de poder descargarse, para cualquier rol: la estación deja de aparecer en los filtros de los gráficos (`C-03`), de las tablas de histórico (`C-04`) y de la generación de archivos (V-05). Si una estación *Hidden* se vuelve a mostrar (pasa a *Inactive*), sus datos vuelven a estar disponibles (propuesto). Falta confirmar si el módulo de calibración puede usar esos datos (`PA-36`).

**Estado frente a color del mapa:** los estados de esta tabla describen el **funcionamiento** de la estación y se muestran en el listado, en el detalle y en el resumen (`C-02`). El color del marcador en el mapa no depende de ellos, sino del nivel de alerta de los umbrales de las variables (V-02, `RN-37`). La excepción es *NoData*: una estación sin datos se muestra en el mapa en gris y opaca.

### 2.3 Sensores y variables

Las estaciones inician con los tres sensores de la tabla siguiente. En futuras mejoras se agregarán otros sensores, por lo que el catálogo de sensores es ampliable desde la plataforma (V-25) y cada estación puede tener un conjunto distinto de sensores (V-26).

| Sensor (nombre corto) | Modelo | Variables que mide |
|-----------------------|--------|--------------------|
| Winsen | Winsen ZPHS01B (módulo multisensor) | Temperatura, Humedad, CO₂, CO, O₃, CH₂O, NO₂, TVOC, PM10, PM2.5, PM1 |
| Honeywell | Honeywell HPM Series HPMA115S0-XXX (material particulado) | PM10, PM2.5, PM1 |
| AM2305B | AM2305B | Temperatura, Humedad |

**Matriz de variables por sensor inicial** (útil para los filtros de los gráficos, la comparación entre sensores, los umbrales de alerta y la analítica de datos):

| Variable | Unidad (validada, `PA-04`) | Winsen | Honeywell | AM2305B |
|----------|----------------------------|:------:|:---------:|:-------:|
| Temperatura | °C | ✔ | | ✔ |
| Humedad | % HR | ✔ | | ✔ |
| CO₂ | ppm | ✔ | | |
| CO | ppm | ✔ | | |
| O₃ | ppm | ✔ | | |
| CH₂O (formaldehído) | mg/m³ | ✔ | | |
| NO₂ | ppm | ✔ | | |
| TVOC | mg/m³ | ✔ | | |
| PM10 | µg/m³ | ✔ | ✔ | |
| PM2.5 | µg/m³ | ✔ | ✔ | |
| PM1 | µg/m³ | ✔ | ✔ | |

Las variables que miden dos o más sensores de una misma estación (con los sensores iniciales: Temperatura, Humedad, PM10, PM2.5 y PM1) son las que se pueden comparar entre sensores en el detalle de la estación (`RF-34`). Cuando se registren sensores nuevos, la matriz se amplía con la información del catálogo (V-25).

### 2.4 Estados de los sensores

| Estado (sistema) | Etiqueta en pantalla | Color | Significado |
|------------------|----------------------|-------|-------------|
| `Ok` | Funcionando | <span style="display:inline-block;width:1em;height:1em;background:#10B981;border:1px solid #888"></span> `#10B981` | El sensor captura datos en un rango normal y sin anomalías en los datos. |
| `Warning` | Advertencia | <span style="display:inline-block;width:1em;height:1em;background:#D97706;border:1px solid #888"></span> `#D97706` | El sensor captura datos, pero son extremos o atípicos: presentan anomalías o valores distintos a los que se deberían recibir. |
| `Offline` | Sin conexión | <span style="display:inline-block;width:1em;height:1em;background:#4B5563;border:1px solid #888"></span> `#4B5563` | No se reciben datos del sensor, o los datos llegan con un código de error. Los códigos de error se definirán con una matriz de valores y umbrales por sensor (`PA-32`); mientras tanto se conserva el valor `-1` como referencia. |

Detalles pendientes del cálculo (prioridad cuando una variable está *Offline* y otra en *Warning*, cuánto tiempo sin datos se considera *Offline* un sensor, cuándo vuelve a *Ok*, y la matriz de códigos de error que reemplaza al valor `-1`): ver `PA-31` y `PA-32`.

### 2.5 Estados de generación de archivos

| Estado (sistema) | Etiqueta en pantalla | Color | Significado |
|------------------|----------------------|-------|-------------|
| `Validating` | Validando | <span style="display:inline-block;width:1em;height:1em;background:#7C3AED;border:1px solid #888"></span> `#7C3AED` | Se están validando los filtros de la solicitud. |
| `Generating` | Generando | <span style="display:inline-block;width:1em;height:1em;background:#2563EB;border:1px solid #888"></span> `#2563EB` | El archivo se está construyendo; se muestra el porcentaje de avance. |
| `Generated` | Generado | <span style="display:inline-block;width:1em;height:1em;background:#10B981;border:1px solid #888"></span> `#10B981` | El archivo está listo para descargar; se muestra su tamaño. |
| `Error` | Error | <span style="display:inline-block;width:1em;height:1em;background:#DC2626;border:1px solid #888"></span> `#DC2626` | La generación falló. |
| `Canceled` | Cancelado | <span style="display:inline-block;width:1em;height:1em;background:#4B5563;border:1px solid #888"></span> `#4B5563` | La persona canceló la generación. |

**Transiciones permitidas:**

```
Validating ──► Generating ──► Generated ──(vencen las 12 horas o la persona genera otro archivo)──► el archivo se elimina del servidor
    │               │
    ├──► Error      ├──► Error
    └──► Canceled   └──► Canceled
```

`Generated`, `Error` y `Canceled` son estados finales. El botón *Cancelar* solo aparece en *Validating* y *Generating*. No se guarda un historial de archivos generados: el estado de la generación en curso solo se muestra en V-05. Cada archivo se conserva en el servidor un máximo de **12 horas**, se descargue o no; de cada persona solo se conserva el último archivo generado, de modo que si genera uno nuevo antes de ese plazo, el anterior se elimina (`RN-40`).

### 2.6 Estados y tipos de alertas

| Estado (sistema) | Etiqueta en pantalla | Color | Significado |
|------------------|----------------------|-------|-------------|
| `Warning` | Advertencia | <span style="display:inline-block;width:1em;height:1em;background:#D97706;border:1px solid #888"></span> `#D97706` | Una variable superó su umbral de advertencia sin superar el umbral crítico. |
| `Critical` | Crítica | <span style="display:inline-block;width:1em;height:1em;background:#DC2626;border:1px solid #888"></span> `#DC2626` | Una variable superó su umbral crítico. |
| `Informative` | Informativa | <span style="display:inline-block;width:1em;height:1em;background:#2563EB;border:1px solid #888"></span> `#2563EB` | Evento operativo o administrativo sobre una estación. |

**Tipos de alerta:**

| Tipo de alerta | Estado | Origen | Muestra usuario involucrado | Evidencia en el detalle (V-17) |
|----------------|--------|--------|:---------------------------:|--------------------------------|
| Umbral de advertencia superado | `Warning` | Valor recibido mayor que el umbral de advertencia y menor o igual que el umbral crítico de la variable (V-15). | | Tabla: últimos 10 registros del sensor en la estación con valores entre el umbral de advertencia y el crítico. |
| Umbral crítico superado | `Critical` | Valor recibido mayor que el umbral crítico de la variable (V-15). | | Tabla: últimos 10 registros del sensor en la estación con valores sobre el umbral crítico. |
| Desconexión con el bróker | `Informative` | La plataforma detecta una desconexión o una desincronización entre su API y el bróker MQTT. No se asocia a una estación (`PA-49`). | | Explicación del hallazgo (sin tabla). |
| Tiempo sin conexión | `Informative` | No llegan mensajes de una estación durante el tiempo máximo sin mensajes (10 minutos por defecto, V-15), medido con la hora de llegada al servidor; la estación pasa a *NoData* (`RN-34`). | | Explicación del hallazgo (sin tabla). |
| Marca de tiempo inválida | `Informative` | Llega un mensaje cuya marca de tiempo no se puede interpretar o está desajustada; la lectura se guarda con la hora de recepción y se marca como corregida (`RN-33`). Se genera **una sola alerta por episodio**, que se mantiene hasta que la estación vuelva a enviar marcas válidas. | | Explicación del hallazgo y marca de tiempo recibida (sin tabla). |
| Anomalía en un sensor | `Informative` | Se detecta un comportamiento anómalo en un sensor (ver definición abajo). | | Explicación del hallazgo (sin tabla). |
| Mensaje inválido (propuesto) | `Informative` | Llega al bróker un mensaje de la estación que no cumple el formato configurado (V-27) o trae sensores o variables no asignados a la estación. | | Explicación del hallazgo y fragmento del mensaje rechazado (sin tabla). |
| Creación de estación | `Informative` | Un administrador creó una estación. | ✔ | Explicación del hallazgo (sin tabla). |
| Edición de estación | `Informative` | Un administrador editó la información de una estación. | ✔ | Explicación del hallazgo (sin tabla). |
| Habilitación de estación | `Informative` | Un administrador habilitó una estación. | ✔ | Explicación del hallazgo (sin tabla). |
| Deshabilitación de estación | `Informative` | Un administrador deshabilitó una estación (con o sin ocultarla). | ✔ | Explicación del hallazgo (sin tabla). |
| Visualización de estación | `Informative` | Un administrador mostró en el mapa una estación oculta. | ✔ | Explicación del hallazgo (sin tabla). |
| Eliminación de estación | `Informative` | Un administrador eliminó una estación. | ✔ | Explicación del hallazgo (sin tabla). |
| Asignación o retiro de sensor | `Informative` | Un administrador agregó un sensor a una estación o lo retiró (V-26). | ✔ | Explicación del hallazgo (sin tabla). |
| Regeneración de credenciales | `Informative` | Un administrador regeneró las credenciales de conexión de una estación con el bróker (V-28). | ✔ | Explicación del hallazgo (sin tabla). |

**Anomalía en un sensor:** el sensor recibe datos que no son consistentes con su comportamiento esperado, por ejemplo:

- valores muy altos o muy bajos;
- valores negativos o en 0;
- valores extremos alternados (muy altos y luego muy bajos);
- cambios abruptos entre lecturas consecutivas;
- en general, falta de consistencia en la forma en que llegan los datos.

Falta definir los criterios cuantitativos de cada caso (`PA-33`) y cómo se evita generar una alerta por cada lectura mientras dura el problema en las alertas por umbral y por anomalía (`PA-34`); para la marca de tiempo inválida ya se definió una alerta por episodio.

**Relación entre alertas y estado del sensor:** un valor sobre un umbral genera una alerta *Warning* o *Critical* y deja el sensor en *Warning*; una anomalía genera una alerta *Informative* y deja el sensor en *Warning*; la ausencia de datos o los códigos de error (`PA-32`) dejan el sensor en *Offline*.

**Notificaciones:** las alertas **no** se envían por correo ni por notificaciones *push*; solo se muestran en la vista de alertas (V-16) y en el contador del ícono *Alertas* de la barra lateral (`C-09`).

### 2.7 Valores comunes de filtros

- **Rangos de tiempo de los gráficos:** últimas 24 horas, última semana, último mes, último año, últimos 2 años.
- **Cantidad de registros en las tablas de histórico:** 10, 20, 50 o 100 (últimos registros recibidos).
- **Formatos de archivo de descarga:** CSV y JSON.

### 2.8 Módulo de calibración (preliminar)

El módulo de calibración atiende el objetivo específico número 2 del macroproyecto: comparar modelos de aprendizaje automático que corrijan las lecturas de los sensores de bajo costo. Sus requisitos están en [requerimientos_modulo_calibracion.md](requerimientos_modulo_calibracion.md) y se aclararon en la [Reunión 26/10](<reuniones/Reunión 261026.md>) (`PCAL-xx`). Solo lo usan los administradores (`PCAL-02`).

**Tipos de análisis:**

| Tipo | Propósito | Ejemplo |
|------|-----------|---------|
| Calibración | Entrenar y comparar modelos que transformen la lectura de un sensor en el valor de una variable de referencia, con ayuda de variables auxiliares. | Comparar modelos que ajusten el PM2.5 del Honeywell de una estación con el PM2.5 de otra estación registrada, usando la humedad como variable auxiliar. |
| Estimación de variables | Calcular una variable a partir de otras variables correlacionadas. **Por confirmar** si sigue en el alcance (`PA-52`). | Estimar la temperatura a partir de la humedad. |

**Datos de referencia:** solo se usan estaciones registradas en la plataforma, incluidas las estaciones caseras que construyan los administradores; no se usan redes ni APIs externas. No se marca ninguna estación como "de referencia": en cada calibración el administrador elige la estación de la que toma la variable de referencia. Las estaciones **no tienen que estar en el mismo lugar**, y se puede calibrar con 2 o más estaciones. Las lecturas se emparejan con **promedios por minuto** (`RN-36`).

**Modelos:** regresión lineal, redes neuronales artificiales (ANN), Random Forest, SVM, AdaBoost y regresión polinomial; ElasticNet está por revisar (`PCAL-08`). Naive Bayes y regresión logística no se incluyen porque son modelos de clasificación. Los modelos mínimos de la primera versión están por definir (`PA-52`).

**Resultados:** una gráfica por modelo con los datos crudos y los calibrados frente a la referencia, la línea g(x) = x y el **R² de los datos calibrados** (`PCAL-10`, `PCAL-12`). No se fija un valor de R² de aceptación (`PCAL-11`). Está por definir cómo reportar el efecto de cada variable (valor p) y si el R² se calcula sobre un conjunto de prueba (`PA-52`).

**Uso de los modelos:** los modelos **no se aplican** a los datos de la plataforma: el público ve solo los datos crudos, y los calibrados solo sirven para comparar y elegir modelos (`PCAL-03`, `RN-35`). No se guardan los modelos ni el historial de calibraciones (`PCAL-13`). El administrador **descarga** el modelo que elija (`PCAL-01`), en formato `.keras` para las redes neuronales; el formato de los demás modelos está por definir (`PA-54`).

> **Nota de riesgo:** la plataforma no valida la calidad de los datos que se usen como referencia; con qué datos y cómo se calibra lo decide el investigador. Si la referencia usa sensores de bajo costo, el resultado es una intercalibración, y si las estaciones están en lugares distintos, las diferencias pueden deberse a la ubicación (sección 1.5 de [contextualizacion.md](contextualizacion.md)).

**Estados de un entrenamiento (propuesto):**

| Estado (sistema) | Etiqueta en pantalla | Color | Significado |
|------------------|----------------------|-------|-------------|
| `Queued` | En cola | <span style="display:inline-block;width:1em;height:1em;background:#7C3AED;border:1px solid #888"></span> `#7C3AED` | El entrenamiento espera turno para ejecutarse. |
| `Training` | Entrenando | <span style="display:inline-block;width:1em;height:1em;background:#2563EB;border:1px solid #888"></span> `#2563EB` | Los modelos se están entrenando; se muestra el avance. |
| `Completed` | Completado | <span style="display:inline-block;width:1em;height:1em;background:#10B981;border:1px solid #888"></span> `#10B981` | Los modelos terminaron de entrenarse y se muestran sus resultados. |
| `Error` | Error | <span style="display:inline-block;width:1em;height:1em;background:#DC2626;border:1px solid #888"></span> `#DC2626` | El entrenamiento falló (por ejemplo, por datos insuficientes). |
| `Canceled` | Cancelado | <span style="display:inline-block;width:1em;height:1em;background:#4B5563;border:1px solid #888"></span> `#4B5563` | El administrador canceló el entrenamiento. |

---

## 3. Componentes reutilizables

Estos componentes aparecen en varias vistas; conviene diseñarlos una sola vez en el mockup.

| ID | Componente | Contenido y comportamiento | Usado en |
|----|------------|----------------------------|----------|
| `C-01` | Indicador de estado | Etiqueta (*badge*) con el color y el texto del estado según las tablas de la sección 2. | Todas las vistas |
| `C-02` | Resumen de estación | Nombre, ID, estado de la estación (`C-01`), dirección y estado de cada sensor instalado (nombre del sensor + `C-01`). En la tarjeta del listado, el administrador ve además los botones *Editar* (lápiz) y *Eliminar* (basurero). | V-02 (cuadro emergente), V-03 (tarjeta) |
| `C-03` | Gráfico histórico | Gráfico de líneas de una variable en el tiempo. Tiene filtros de estación(es), sensor, variable y rango de tiempo (sección 2.7). Las opciones de *variable* dependen del *sensor* elegido. | V-03, V-04 |
| `C-04` | Tabla de histórico de datos | Muestra los últimos N registros recibidos (N según sección 2.7). En la parte superior tiene los filtros y el botón *Generar archivo de descarga*, que abre V-05 con los filtros actuales precargados. | V-03, V-04 |
| `C-05` | Ventana de confirmación | Ventana emergente con título, mensaje, botón *Cancelar* y botón de confirmación. | V-07, V-08, V-19, V-28, V-35 |
| `C-06` | Verificación por contraseña | Campo para escribir la contraseña actual del administrador que ejecuta la acción; el botón de confirmación se habilita solo cuando el campo está completo. | V-08, V-19, V-21, V-28, V-35 |
| `C-07` | Formulario de contraseña nueva | Campos *Contraseña* y *Confirmar contraseña*, con validación de coincidencia y de la política de contraseñas (`RN-14`). Muestra la lista de requisitos y marca los que ya se cumplen. | V-10, V-21, V-36 |
| `C-08` | Tarjeta de alerta | Estado (`C-01`), tipo de alerta, estación (si aplica), fecha y hora, usuario involucrado (si aplica), indicador de vista/no vista y botón *Marcar como vista* / *Marcar como no vista*. | V-16 |
| `C-09` | Barra lateral de navegación | Presente en todas las vistas salvo V-01, V-10 y V-36. De arriba hacia abajo: <br>1. *(Solo administrador)* **Ícono de perfil** con un submenú desplegable: *Perfil* (V-11), *Restablecer contraseña* (V-21) y *Cerrar sesión*. <br>2. *Mapa* (V-02). <br>3. *Estaciones* (V-03). <br>4. *Generar archivos de descarga* (V-05). <br>5. *(Solo administrador)* *Alertas* (V-16) con contador de alertas no vistas, *Analítica de datos* (submenú: *Calibración* V-30 y *Estimación de variables* V-31, esta última por confirmar), *Catálogo de sensores* (V-25), *Formatos del mensaje MQTT* (V-27), *Configuración* (V-15) y *Administradores* (V-33). <br>6. *(Solo visitante)* Botón *Iniciar sesión* (V-01), con la aclaración de que es solo para administradores. <br>Resalta la opción de la vista actual. En celulares se contrae en un botón de menú (propuesto). | Todas las vistas salvo V-01, V-10 y V-36 |
| `C-10` | Pie de página legal | Presente en la parte inferior de todas las páginas. Contiene: aviso de derechos de autor (© año, titular), **correo de contacto** (canal para que cualquier persona envíe peticiones, consultas o sugerencias), teléfono y dirección (por definir), y enlaces a *Términos y condiciones* y *Política de tratamiento de datos personales* (propuesto). Contenido exacto por definir (`PA-43`). | Todas las vistas |
| `C-11` | Panel de resultados de entrenamiento (preliminar) | Una gráfica por modelo entrenado, todas con el mismo formato y escala: referencia en el eje x y lecturas en el eje y, con los datos crudos y los calibrados (marcadores distintos), la línea g(x) = x, el nombre del modelo, la leyenda, las unidades y el **R² de los datos calibrados** (sección 2.8). Cada gráfica tiene un botón *Descargar modelo*. Si un modelo falla, se informa cuál y por qué sin ocultar los demás. | V-30, V-31 |
| `C-12` | Visualización de credenciales | Muestra **una sola vez** las credenciales de conexión de una estación con el bróker MQTT: dirección y puerto del bróker, usuario, contraseña o token y tópico de publicación. Tiene un botón *Copiar* por campo y el aviso de que las credenciales no se volverán a mostrar. | V-14, V-28 |

---

## 4. Vistas

### Requisitos globales

| ID | Requisito | Rol |
|----|-----------|-----|
| `RF-G01` | Todas las vistas, salvo el inicio de sesión (V-01), la recuperación de contraseña (V-10) y la activación de cuenta (V-36), deben mostrar la barra lateral de navegación (`C-09`). | Ambos |
| `RF-G02` | La barra lateral debe mostrar al visitante las opciones *Mapa*, *Estaciones*, *Generar archivos de descarga* e *Iniciar sesión*. | Visitante |
| `RF-G03` | La barra lateral debe mostrar al administrador, además de *Mapa*, *Estaciones* y *Generar archivos de descarga*, el menú de perfil y las opciones *Alertas*, *Analítica de datos*, *Catálogo de sensores*, *Formatos del mensaje MQTT*, *Configuración* y *Administradores*; no muestra *Iniciar sesión*. | Administrador |
| `RF-G04` | El menú de perfil debe desplegar las opciones *Perfil*, *Restablecer contraseña* y *Cerrar sesión*. | Administrador |
| `RF-G05` | La opción *Alertas* debe mostrar un contador con la cantidad de alertas no vistas por el administrador actual. | Administrador |
| `RF-G06` | Todas las páginas deben mostrar el pie de página legal (`C-10`) con el correo de contacto. | Ambos |
| `RF-G07` | El mapa (V-02), el listado de estaciones (V-03), el detalle de estación (V-04) y la generación de archivos de descarga (V-05) deben estar disponibles sin registro ni inicio de sesión. | Ambos |

---

### V-01 Inicio de sesión — administradores

**Objetivo:** autenticar a los administradores. Los visitantes no necesitan iniciar sesión para consultar los datos.

**Estructura (de arriba hacia abajo):**
1. Nombre/logo de la plataforma.
2. Mensaje: "Acceso exclusivo para administradores".
3. Formulario con correo electrónico, contraseña y botón *Iniciar sesión*.
4. Enlace *¿Olvidó su contraseña?* (abre V-10).
5. Enlace *Volver al mapa* (abre V-02).
6. Pie de página (`C-10`).

**Estados de la vista:** mensaje de error por credenciales inválidas; cuenta pendiente de activación (indica revisar el correo con el enlace de activación); indicador de carga mientras se autentica.

| ID | Requisito | Rol |
|----|-----------|-----|
| `RF-02` | El sistema debe permitir a los administradores iniciar sesión con correo electrónico y contraseña. | Administrador |
| `RF-03` | Tras autenticarse, el sistema debe habilitar las funciones de administrador. | Administrador |
| `RF-04` | Tras autenticarse, el sistema debe dirigir al administrador al mapa principal (V-02). | Administrador |
| `RF-05` | El administrador debe poder cerrar sesión desde cualquier vista (menú de perfil de `C-09`); al hacerlo, sigue navegando como visitante. | Administrador |
| `RF-06` | El inicio de sesión debe tener un enlace a la recuperación de contraseña (V-10) y otro para volver al mapa (V-02). | Administrador |
| `RF-07` | El sistema no debe permitir iniciar sesión con una cuenta en estado *New* o *Deleted*. | Administrador |
| `RF-09` | El inicio de sesión debe ser accesible desde el botón *Iniciar sesión* de la barra lateral. | Visitante |

---

### V-02 Mapa principal

**Objetivo:** ver de un vistazo la ubicación de todas las estaciones y si sus variables ambientales superan algún umbral.

**Estructura:**
1. Barra lateral (`C-09`).
2. Mapa interactivo con un marcador por estación. El color de cada marcador corresponde al **nivel de alerta de los umbrales de sus variables ambientales** (V-15), no al estado de funcionamiento de la estación (`RN-37`). Una estación en estado *NoData* (recién creada o habilitada sin su primer mensaje, o desconectada, `RN-34`) aparece en **gris y opaca**. Los umbrales, niveles y colores se definirán con el investigador principal y el investigador de Ingeniería Ambiental; los detalles del cálculo están en `PA-56`.
3. Leyenda de los niveles de alerta y del gris de "sin datos" (propuesto).
4. Pie de página (`C-10`).

**Interacciones:**
- **Pasar el cursor sobre un marcador:** muestra un cuadro emergente con el resumen de la estación (`C-02`) y las variables que superan algún umbral.
- **Hacer clic en un marcador:** abre el detalle de la estación (V-04).
- Acercar, alejar y desplazar el mapa.

**Estados de la vista:** carga del mapa; mensaje si no hay estaciones registradas.

| ID | Requisito | Rol |
|----|-----------|-----|
| `RF-10` | El sistema debe mostrar un mapa interactivo con la ubicación de cada estación. | Ambos |
| `RF-11` | El marcador de cada estación debe tener el color del nivel de alerta de los umbrales de sus variables, y mostrarse en gris y opaco si la estación está en estado *NoData* (`RN-37`). | Ambos |
| `RF-12` | Al pasar el cursor sobre un marcador, el sistema debe mostrar el nombre, ID, estado, dirección y estado de cada sensor de la estación, y las variables que superan algún umbral. | Ambos |
| `RF-13` | Al hacer clic en un marcador, el sistema debe abrir el detalle de la estación. | Ambos |
| `RF-14` | Las estaciones en estado *Hidden* o *Deleted* no deben aparecer en el mapa ni para el visitante ni para el administrador (ver `RN-03` y `RN-04`). | Ambos |
| `RF-15` | El mapa debe actualizar los colores y los resúmenes de las estaciones según el intervalo de actualización configurado (V-15). | Ambos |

---

### V-03 Listado de estaciones

**Objetivo:** consultar todas las estaciones, comparar su comportamiento en el tiempo y revisar los últimos datos recibidos.

**Estructura:**
1. Barra lateral (`C-09`).
2. *(Solo administrador)* Botones *Crear estación* (abre V-14) y *Estaciones eliminadas* (abre V-18, propuesto) sobre las tarjetas.
3. **Tarjetas de estaciones:** una tarjeta por estación con el resumen `C-02`. Al hacer clic en una tarjeta se abre su detalle (V-04, propuesto). *(Solo administrador)* cada tarjeta tiene un botón *Editar* (lápiz, abre V-06) y un botón *Eliminar* (basurero, abre V-08). El administrador ve también las estaciones ocultas, con el indicador *Oculta* sobre fondo `#cccccc`.
4. **Gráfico histórico (`C-03`)** con los filtros:
   - Estación(es): una, varias o todas.
   - Sensor.
   - Variable (solo las del sensor elegido).
   - Rango de tiempo.
5. **Tabla de histórico de datos (`C-04`)** de todas las estaciones, con:
   - Selector de cantidad de registros (10, 20, 50, 100).
   - Filtros por estación, sensor y variable.
   - Botón *Generar archivo de descarga* (abre V-05 con los filtros precargados).
6. Pie de página (`C-10`).

Los filtros de estaciones del gráfico y de la tabla no incluyen las estaciones *Hidden* ni *Deleted*, tampoco para el administrador (`RN-39`).

**Estados de la vista:** carga; sin estaciones; sin datos para los filtros elegidos (en el gráfico y en la tabla).

| ID | Requisito | Rol |
|----|-----------|-----|
| `RF-20` | El sistema debe listar las estaciones en tarjetas con nombre, ID, estado, dirección y estado de cada sensor. | Ambos |
| `RF-21` | El sistema debe mostrar un gráfico de la evolución de una variable de un sensor en el tiempo para una, varias o todas las estaciones. | Ambos |
| `RF-22` | El filtro de variables del gráfico debe listar solo las variables del sensor elegido. | Ambos |
| `RF-23` | El usuario debe poder elegir el rango de tiempo del gráfico entre las opciones de la sección 2.7. | Ambos |
| `RF-24` | El sistema debe mostrar una tabla con los últimos 10, 20, 50 o 100 registros enviados por las estaciones. | Ambos |
| `RF-25` | La tabla de histórico debe poder filtrarse por estación, sensor y variable. | Ambos |
| `RF-26` | La tabla de histórico debe tener un botón que abra la generación de archivos de descarga (V-05) con los filtros actuales precargados. | Ambos |
| `RF-27` | El sistema debe mostrar al administrador un botón *Crear estación* sobre las tarjetas. | Administrador |
| `RF-28` | Cada tarjeta debe mostrar al administrador los botones *Editar* y *Eliminar* de la estación. | Administrador |
| `RF-29` | Las estaciones en estado *Hidden* solo deben aparecer en el listado del administrador, con el indicador de estado *Oculta* sobre fondo `#cccccc`. | Administrador |

---

### V-04 Detalle de estación

**Objetivo:** consultar toda la información de una estación y, en el caso del administrador, gestionarla.

**Estructura:**
1. Barra lateral (`C-09`).
2. **Encabezado de la estación** con:
   - ID.
   - Nombre.
   - Estado (`C-01`).
   - Dirección y mapa pequeño con la ubicación de la estación.
   - Descripción.
   - Fecha y hora del último envío de datos.
   - *(Solo administrador)* Botones *Editar*, *Habilitar/Deshabilitar/Mostrar* (la etiqueta depende del estado, ver V-07), *Eliminar*, *Sensores* (abre V-26) y *Regenerar credenciales* (abre V-28).
3. **Tarjetas de sensores:** una tarjeta por sensor asignado a la estación (V-26). Cada tarjeta muestra el nombre del sensor, su estado (`C-01`) y el último valor capturado de cada una de sus variables, con su unidad.
4. **Gráfico histórico (`C-03`)** con filtros flexibles:
   - **Modo una variable:** se elige un sensor y una de sus variables.
   - **Modo comparación:** se elige una variable compartida (ver sección 2.3) y se muestran en el mismo gráfico las series de los sensores que la miden.
   - Rango de tiempo.
5. **Tabla de histórico de datos (`C-04`)** de la estación, con:
   - Selector de cantidad de registros (10, 20, 50, 100).
   - Filtros por sensor y variable.
   - Botón *Generar archivo de descarga* (abre V-05 con la estación ya seleccionada).
6. Pie de página (`C-10`).

Los gráficos y las tablas muestran siempre los datos crudos: los modelos de calibración no se aplican a los datos de la plataforma (`RN-35`). Los periodos sin datos se muestran como huecos, sin interpolar (`RN-34`), y las lecturas con marca de tiempo corregida se distinguen con un indicador (propuesto, `RN-33`).

**Estados de la vista:** carga; estación sin datos recibidos; sin datos para los filtros elegidos.

| ID | Requisito | Rol |
|----|-----------|-----|
| `RF-30` | El sistema debe mostrar el ID, nombre, estado, dirección, descripción, ubicación en un mapa y fecha y hora del último envío de datos de la estación. | Ambos |
| `RF-31` | El sistema debe mostrar una tarjeta por cada sensor instalado, con su nombre, su estado y el último valor de cada variable. | Ambos |
| `RF-32` | El sistema debe mostrar un gráfico de la evolución de una variable de un sensor en el tiempo. | Ambos |
| `RF-33` | El usuario debe poder elegir el rango de tiempo del gráfico entre las opciones de la sección 2.7. | Ambos |
| `RF-34` | El gráfico debe permitir comparar una misma variable medida por distintos sensores de la estación. | Ambos |
| `RF-35` | El sistema debe mostrar una tabla con los últimos 10, 20, 50 o 100 registros enviados por la estación, filtrable por sensor y variable. | Ambos |
| `RF-36` | La tabla de histórico debe tener un botón que abra la generación de archivos de descarga (V-05) con la estación ya seleccionada. | Ambos |
| `RF-37` | El sistema debe mostrar los botones *Editar*, *Habilitar/Deshabilitar/Mostrar*, *Eliminar*, *Sensores* y *Regenerar credenciales* solo a los administradores. | Administrador |

---

### V-05 Generación de archivos de descarga

**Tipo:** vista pública, accesible desde la opción *Generar archivos de descarga* de la barra lateral. También se abre desde el botón *Generar archivo de descarga* de las tablas de V-03 y V-04, con los filtros precargados.

**Estructura:**
1. Barra lateral (`C-09`).
2. **Formulario de filtros:**
   - Tipo de archivo: CSV o JSON.
   - Estaciones: todas o estaciones específicas (selección múltiple). No se listan las estaciones *Hidden* ni *Deleted* (`RN-39`).
   - Datos a obtener: sensores y variables (selección múltiple).
   - Rango de fechas: fecha inicial y fecha final.
   - Verificación *captcha*.
   - Botón *Generar archivo*.
3. **Panel de la generación en curso** (aparece al pulsar *Generar archivo*):
   - Estado de generación (`C-01`, sección 2.5).
   - Porcentaje de avance.
   - Botón *Cancelar*: solo visible en estado *Validando* o *Generando*.
   - Tamaño del archivo y botón *Descargar*: solo en estado *Generado*.
4. Pie de página (`C-10`).

No se guarda un historial de archivos generados. Cada archivo se conserva en el servidor un máximo de 12 horas, se descargue o no, y de cada persona solo se conserva el último archivo generado; si genera uno nuevo antes de las 12 horas, el anterior se elimina. Hay un límite de solicitudes de generación y de descargas por dirección IP, tanto para visitantes como para administradores (`RN-40`; valores por definir, `PA-46`). Falta definir cómo se identifica a la persona para conservar su último archivo y si puede volver a descargarlo después de salir de la vista (`PA-46`).

**Estados de la vista:** validación de campos del formulario; *captcha* sin resolver o inválido; error al solicitar la generación; límite de solicitudes o de descargas alcanzado (`RN-40`).

| ID | Requisito | Rol |
|----|-----------|-----|
| `RF-40` | El usuario debe poder solicitar la generación de un archivo CSV o JSON eligiendo las estaciones, los datos y el rango de fechas. | Ambos |
| `RF-41` | Antes de iniciar la generación, el sistema debe validar que los filtros estén completos, que la fecha inicial no sea posterior a la final, que se haya resuelto el *captcha* y que la dirección IP no haya superado el límite de solicitudes (`RN-40`). | Ambos |
| `RF-43` | El botón *Cancelar* solo debe mostrarse, y la cancelación solo debe permitirse, cuando la solicitud esté en estado *Validando* o *Generando*. | Ambos |
| `RF-44` | El usuario debe poder descargar un archivo solo cuando esté en estado *Generado*. | Ambos |
| `RF-45` | El sistema debe mostrar el tamaño del archivo cuando esté en estado *Generado*. | Ambos |
| `RF-46` | El sistema debe mostrar el estado y el porcentaje de avance de la generación en curso. | Ambos |
| `RF-48` | El sistema no debe conservar un historial de archivos generados; debe eliminar del servidor cada archivo a las 12 horas de generado, se haya descargado o no, y conservar solo el último archivo generado por cada persona (`RN-40`). | Ambos |
| `RF-49` | La vista debe ser accesible desde la barra lateral y desde las tablas de histórico; en el segundo caso, con los filtros de la tabla precargados. | Ambos |

---

### V-06 Ventana de edición de estación *(solo Administrador)*

**Tipo:** ventana emergente. Se abre con el botón *Editar* de V-04 o con el lápiz de la tarjeta en V-03, y carga los datos actuales de la estación.

**Estructura:** formulario con los campos *Nombre*, *Ubicación* y *Descripción*; botones *Cancelar* y *Guardar*. La ubicación se define de cualquiera de estas tres formas, y las demás se completan automáticamente:

- escribiendo la **dirección** (el sistema obtiene las coordenadas);
- escribiendo las **coordenadas** (latitud y longitud; el sistema obtiene la dirección);
- **seleccionando el punto en el mapa** (el sistema obtiene coordenadas y dirección).

La dirección obtenida automáticamente se puede corregir a mano (propuesto).

| ID | Requisito | Rol |
|----|-----------|-----|
| `RF-50` | El administrador debe poder editar el nombre, la ubicación y la descripción de una estación. | Administrador |
| `RF-51` | Al guardar la edición, el sistema debe generar una alerta informativa de edición de estación con el administrador involucrado. | Administrador |
| `RF-52` | El administrador debe poder definir la ubicación ingresando la dirección, las coordenadas o seleccionando el punto en el mapa; el sistema debe mantener sincronizados los tres datos. | Administrador |

---

### V-07 Ventana de habilitar/deshabilitar estación *(solo Administrador)*

**Tipo:** ventana de confirmación (`C-05`). Se abre con el botón de estado de V-04, cuya etiqueta depende del estado actual:

| Estado actual | Botón | Resultado |
|---------------|-------|-----------|
| `NoData`, `Active`, `Warning`, `Error` | *Deshabilitar* | `Inactive`, o `Hidden` si se marca *Ocultar del mapa*. La plataforma deja de escuchar sus mensajes (`RN-38`). |
| `Inactive` | *Habilitar* | La plataforma vuelve a escuchar sus mensajes; queda en `NoData` hasta recibir el primero y luego en el estado automático (`Active`, `Warning` o `Error`). |
| `Hidden` | *Mostrar* | `Inactive`: vuelve a aparecer en el mapa y en el listado con el estado *Inactiva*, sin habilitarse. A partir de ahí el botón dice *Habilitar*. |

**Estructura al deshabilitar:** mensaje de confirmación, casilla *Ocultar la estación del mapa*, botones *Cancelar* y *Deshabilitar*.

**Estructura al habilitar:** mensaje de confirmación, botones *Cancelar* y *Habilitar*.

**Estructura al mostrar:** mensaje que aclara que la estación volverá al mapa pero seguirá deshabilitada, botones *Cancelar* y *Mostrar*.

| ID | Requisito | Rol |
|----|-----------|-----|
| `RF-60` | El administrador debe poder deshabilitar una estación, lo que cambia su estado a *Inactive* y hace que la plataforma deje de escuchar sus mensajes en el bróker (`RN-38`). | Administrador |
| `RF-61` | Al deshabilitar una estación, el sistema debe preguntar si se ocultará del mapa; si se oculta, el estado pasa a *Hidden*. | Administrador |
| `RF-62` | El administrador debe poder volver a habilitar una estación en estado *Inactive*; la plataforma vuelve a escuchar sus mensajes y la estación queda en *NoData* hasta recibir el primero. | Administrador |
| `RF-63` | El administrador debe poder mostrar una estación *Hidden*: pasa a *Inactive*, vuelve a aparecer en el mapa y cambia su estado en el listado, sin habilitarse. Una estación *Hidden* no se puede habilitar directamente. | Administrador |
| `RF-64` | La etiqueta del botón de estado debe cambiar según el estado actual de la estación (*Deshabilitar*, *Habilitar* o *Mostrar*). | Administrador |
| `RF-65` | Al habilitar, deshabilitar (con o sin ocultar) o mostrar una estación, el sistema debe generar una alerta informativa con el administrador involucrado. | Administrador |

---

### V-08 Ventana de eliminación de estación *(solo Administrador)*

**Tipo:** ventana de confirmación (`C-05`) con verificación de seguridad. Se abre con el botón *Eliminar* de V-04 o con el basurero de la tarjeta en V-03.

**Estructura:**
1. Advertencia de que la estación dejará de mostrarse en el mapa y en el listado.
2. Campo *Motivo de eliminación* (obligatorio).
3. Verificación por contraseña del administrador (`C-06`).
4. Botones *Cancelar* y *Eliminar* (este último deshabilitado hasta completar el motivo y la contraseña).

**Estados de la vista:** contraseña incorrecta; motivo vacío.

| ID | Requisito | Rol |
|----|-----------|-----|
| `RF-70` | El administrador debe poder eliminar una estación tras confirmar la acción, indicar un motivo obligatorio y verificar su contraseña. | Administrador |
| `RF-71` | Al eliminar una estación, el sistema debe marcarla como *Deleted*, conservar sus datos en la base de datos (ver `RN-04`) y dejar de escuchar sus mensajes en el bróker (`RN-38`). | Administrador |
| `RF-72` | El sistema debe registrar la fecha de eliminación, el administrador que la realizó y el motivo. | Administrador |
| `RF-73` | Al eliminar una estación, el sistema debe generar una alerta informativa de eliminación con el administrador involucrado. | Administrador |

---

### V-10 Recuperación de contraseña *(Administrador, sin sesión)*

**Objetivo:** permitir que un administrador restablezca su contraseña cuando la olvidó. Se abre desde *¿Olvidó su contraseña?* (V-01) u *Olvidé mi contraseña* (V-21).

**Estructura (dos pasos):**
1. **Solicitud:** campo *Correo electrónico* y botón *Enviar enlace*. Tras enviar se muestra siempre el mismo mensaje neutro: "Si el correo está registrado, recibirá un enlace para restablecer su contraseña", y un botón *Reenviar enlace* con cuenta regresiva.
2. **Restablecimiento** (se abre desde el enlace enviado al correo): nueva contraseña y confirmación (`C-07`), botón *Restablecer contraseña*. Al terminar, muestra "Contraseña restablecida exitosamente" y redirige al inicio de sesión (V-01).
3. Pie de página (`C-10`).

**Valores de reenvío (propuestos según prácticas comunes de la industria, p. ej. la guía *Forgot Password* de OWASP; por confirmar, `PA-29`):**

| Parámetro | Valor propuesto |
|-----------|-----------------|
| Espera entre reenvíos | 60 segundos (el botón *Reenviar enlace* muestra la cuenta regresiva) |
| Máximo de solicitudes | 5 por hora por correo (y límite por dirección IP) |
| Vigencia del enlace | 60 minutos |
| Uso | Un solo uso; al generar un enlace nuevo, los anteriores quedan inválidos |

**Estados de la vista:** enlace inválido, vencido o ya usado; límite de reenvíos alcanzado; la nueva contraseña es igual a la anterior.

| ID | Requisito | Rol |
|----|-----------|-----|
| `RF-90` | El sistema debe solicitar el correo y mostrar siempre el mismo mensaje neutro, exista o no una cuenta asociada, para no revelar qué correos están registrados. | Administrador |
| `RF-91` | Si el correo corresponde a una cuenta de administrador activa, el sistema debe enviar un enlace que abra el formulario de restablecimiento de contraseña. | Administrador |
| `RF-92` | El sistema debe validar que la nueva contraseña cumpla la política (`RN-14`) y no sea igual a la anterior (`RN-07`). | Administrador |
| `RF-93` | Tras restablecer la contraseña, el sistema debe mostrar un mensaje de éxito y dirigir al administrador al inicio de sesión. | Administrador |
| `RF-94` | El sistema debe ofrecer un botón *Reenviar enlace* con un tiempo de espera entre reenvíos y un número máximo de solicitudes por hora (valores en la tabla anterior). | Administrador |
| `RF-95` | Los enlaces de restablecimiento deben ser de un solo uso, vencer según la vigencia definida e invalidarse cuando se genera uno nuevo. | Administrador |

---

### V-11 Perfil *(Administrador)*

**Objetivo:** consultar la información registrada del administrador. Se abre con *Perfil* en el menú de perfil de la barra lateral.

**Estructura:**
1. Barra lateral (`C-09`).
2. Datos del administrador: nombre, correo y rol.
3. Botones *Editar perfil* (abre V-12) y *Eliminar cuenta* (abre V-19).
4. Pie de página (`C-10`).

| ID | Requisito | Rol |
|----|-----------|-----|
| `RF-100` | El sistema debe mostrar el nombre, el correo y el rol del administrador. | Administrador |
| `RF-101` | El perfil debe tener botones para editar el perfil y eliminar la cuenta. El cambio de contraseña y el cierre de sesión están en el menú de perfil (`C-09`). | Administrador |

---

### V-12 Ventana de edición de perfil *(Administrador)*

**Tipo:** ventana emergente. Se abre con *Editar perfil* de V-11.

**Estructura:**
1. Campo *Nombre*.
2. Botones *Cancelar* y *Guardar*.

El correo no es editable (propuesto). El cambio de contraseña está en V-21.

**Estados de la vista:** errores de validación.

| ID | Requisito | Rol |
|----|-----------|-----|
| `RF-110` | El administrador debe poder editar su nombre. | Administrador |
| `RF-111` | *(Trasladado a `RF-200`, V-21.)* | — |
| `RF-112` | *(Trasladado a `RF-202`, V-21.)* | — |

---

### V-13 Ventana de registro de administrador *(solo Administrador)*

**Objetivo:** que un administrador registre a un nuevo administrador.

**Tipo:** ventana emergente o vista (por definir). Se abre con el botón *Registrar administrador* de V-33.

**Estructura:**
1. Formulario:
   - Nombre.
   - Correo electrónico.
2. Botones *Cancelar* y *Registrar administrador*.

**Flujo:** el administrador registra al nuevo administrador con nombre y correo → la cuenta queda en estado *New* y el sistema envía al correo un enlace de activación → el nuevo administrador abre el enlace (V-36), establece su propia contraseña y la cuenta pasa a *Active*. Mientras la cuenta esté en *New*, no puede iniciar sesión. La vigencia del enlace y su reenvío se describen en V-33 y V-36.

**Estados de la vista:** errores de validación; correo ya registrado; registro exitoso (indica que se envió el enlace de activación).

| ID | Requisito | Rol |
|----|-----------|-----|
| `RF-120` | El administrador debe poder registrar un nuevo administrador validando su nombre y su correo. Quien registra no define la contraseña. | Administrador |
| `RF-121` | Tras el registro, la cuenta debe quedar en estado *New* y el sistema debe enviar al correo registrado un enlace de activación para que el nuevo administrador establezca su contraseña (V-36). | Administrador |
| `RF-122` | El sistema debe asignar el rol `admin` a toda cuenta creada desde esta vista. | Administrador |
| `RF-125` | El sistema no debe permitir registrar un correo que ya tenga una cuenta de administrador en estado *New* o *Active*. | Administrador |

---

### V-14 Crear estación *(solo Administrador)*

**Tipo:** ventana emergente o vista (por definir). Se abre con el botón *Crear estación* de V-03.

**Estructura:**
1. Formulario:
   - *Nombre*.
   - *Ubicación* (dirección, coordenadas o selección en el mapa, igual que en V-06).
   - *Descripción*.
   - *Sensores instalados* (selección múltiple del catálogo, V-25).
   - *Formato del mensaje*: formato con el que se interpretan sus mensajes MQTT (V-27). Cada estación puede tener su propio formato, incluidas las estaciones caseras.
   - *Red de comunicación* (fases futuras, `PA-51`): en la primera fase todas las estaciones usan WiFi. Cuando se incorporen estaciones LoRa, se indicará la *Estación de enlace* (la estación WiFi que retransmite sus datos).
2. Botones *Cancelar* y *Crear*.
3. Al crear la estación, el sistema muestra sus credenciales de conexión con el bróker MQTT (`C-12`).

| ID | Requisito | Rol |
|----|-----------|-----|
| `RF-130` | El administrador debe poder registrar una estación nueva con nombre, ubicación, descripción y sensores instalados. | Administrador |
| `RF-131` | Al crear una estación, el sistema debe generar una alerta informativa de creación con el administrador involucrado. | Administrador |
| `RF-132` | El administrador debe poder definir la ubicación ingresando la dirección, las coordenadas o seleccionando el punto en el mapa. | Administrador |
| `RF-133` | Una estación recién creada debe quedar en estado *NoData* hasta que la plataforma reciba su primer mensaje. | Administrador |
| `RF-134` | Al crear una estación, el sistema debe generar sus credenciales de conexión con el bróker MQTT y su tópico de publicación, y mostrarlos una sola vez (`C-12`). | Administrador |
| `RF-135` | El administrador debe poder indicar si la estación usa WiFi o LoRa y, en el caso de LoRa, la estación de enlace (fases futuras, `PA-51`). | Administrador |
| `RF-136` | El administrador debe asignar a la estación un formato del mensaje MQTT (V-27) al crearla. | Administrador |

---

### V-15 Configuración *(solo Administrador)*

**Objetivo:** definir los parámetros de funcionamiento del sistema.

**Estructura:**
1. Barra lateral (`C-09`).
2. **Umbrales de alerta:** tabla con una fila por variable del catálogo (sección 2.3 y V-25) y las columnas *Variable*, *Unidad*, *Umbral de advertencia* y *Umbral crítico* (alcance por definir, `PA-24`).
3. **Actualización de datos:** intervalo de actualización de la página (5 s, 10 s, 30 s, etc.; valores por definir, `PA-25`).
4. **Conexión con las estaciones:** *tiempo máximo sin mensajes*, que se evalúa por cada estación: al superarlo, la estación se considera desconectada, pasa a *NoData* y se genera la alerta *Tiempo sin conexión* (por defecto, 10 minutos, `RN-34`). Falta definir si es un valor único para todas las estaciones o se puede definir por estación (`PA-25`). La desconexión o desincronización entre la API y el bróker se detecta de forma automática y genera la alerta *Desconexión con el bróker*; no tiene parámetros en esta vista.
5. **Detección de anomalías** (propuesto, si se decide que sea configurable, `PA-33`): parámetros como el cambio máximo permitido entre lecturas consecutivas.
6. Botones *Cancelar* y *Guardar cambios*.
7. Pie de página (`C-10`).

**Estados de la vista:** errores de validación (por ejemplo, umbral de advertencia mayor o igual que el crítico); cambios guardados.

| ID | Requisito | Rol |
|----|-----------|-----|
| `RF-140` | El administrador debe poder definir, para cada variable, un umbral de advertencia y un umbral crítico. | Administrador |
| `RF-141` | El administrador debe poder definir el intervalo de actualización de los datos mostrados en la plataforma. | Administrador |
| `RF-142` | El administrador debe poder definir el tiempo máximo sin mensajes después del cual una estación se considera desconectada y pasa a *NoData* (por defecto, 10 minutos). | Administrador |
| `RF-143` | *(Retirado: los reintentos de conexión no aplican con MQTT; se reemplazan por el tiempo máximo sin mensajes, `RF-142`, y por la alerta Desconexión con el bróker, `PA-49`.)* | — |
| `RF-144` | El sistema debe validar que el umbral de advertencia sea menor que el umbral crítico (propuesto). | Administrador |

---

### V-16 Alertas *(solo Administrador)*

**Objetivo:** revisar las anomalías y eventos detectados en las estaciones. Se abre con *Alertas* en la barra lateral, que muestra el contador de alertas no vistas.

**Estructura:**
1. Barra lateral (`C-09`).
2. **Filtros:** estación, estado de la alerta (*Warning*, *Critical*, *Informative*), vista/no vista y fecha (rango).
3. **Lista de alertas:** una tarjeta por alerta (`C-08`), ordenadas de la más reciente a la más antigua (propuesto). Las alertas no vistas por el administrador se distinguen visualmente.
4. Pie de página (`C-10`).

**Interacciones:**
- **Botón *Marcar como vista* / *Marcar como no vista*:** cambia la marca solo para el administrador actual.
- **Clic en una tarjeta:** abre el detalle de la alerta (V-17) y la marca como vista.

**Estados de la vista:** carga; sin alertas; sin resultados para los filtros elegidos.

| ID | Requisito | Rol |
|----|-----------|-----|
| `RF-150` | El sistema debe generar alertas *Warning* cuando un valor recibido supere el umbral de advertencia de la variable sin superar el umbral crítico. | Administrador |
| `RF-151` | El sistema debe generar alertas *Critical* cuando un valor recibido supere el umbral crítico de la variable. | Administrador |
| `RF-152` | El sistema debe generar alertas *Informative* por desconexión con el bróker, tiempo sin conexión, marcas de tiempo inválidas (una por episodio, `RN-33`), anomalías en sensores, mensajes inválidos (propuesto), y por la creación, edición, habilitación, deshabilitación, visualización (mostrar) y eliminación de estaciones, la asignación o el retiro de sensores y la regeneración de credenciales. | Administrador |
| `RF-153` | El sistema debe listar las alertas en tarjetas con estado, tipo, fecha y hora y, cuando aplique, la estación y el usuario involucrado. | Administrador |
| `RF-154` | La lista de alertas debe poder filtrarse por estación, estado, vista/no vista y fecha. | Administrador |
| `RF-155` | El administrador debe poder marcar una alerta como vista y volver a marcarla como no vista; la marca solo aplica para ese administrador (`RN-09`). | Administrador |
| `RF-156` | Al hacer clic en una tarjeta, el sistema debe abrir el detalle de la alerta. | Administrador |
| `RF-157` | El ícono *Alertas* de la barra lateral debe mostrar la cantidad de alertas no vistas por el administrador actual. | Administrador |
| `RF-158` | Las alertas solo deben mostrarse dentro de la plataforma; no se envían por correo ni por notificaciones *push* (`RN-22`). | Administrador |

---

### V-17 Detalle de alerta *(solo Administrador)*

**Objetivo:** consultar toda la información de una alerta.

**Estructura:**
1. Barra lateral (`C-09`).
2. Estado de la alerta (`C-01`), tipo de alerta, estación de origen (si aplica; con enlace a V-04, propuesto; si la estación fue eliminada, enlace a V-18), sensor y variable (si aplica), usuario involucrado (si aplica) y fecha y hora de generación.
3. Descripción del hallazgo.
4. Evidencia según el estado de la alerta:
   - **Warning:** tabla con los últimos 10 registros del sensor en la estación cuyos valores estén entre el umbral de advertencia y el crítico.
   - **Critical:** tabla con los últimos 10 registros del sensor en la estación cuyos valores estén sobre el umbral crítico.
   - **Informative:** sin tabla; texto que explica qué se encontró y por qué se generó la alerta.
5. Pie de página (`C-10`).

| ID | Requisito | Rol |
|----|-----------|-----|
| `RF-160` | El sistema debe mostrar el estado, tipo, estación, fecha y hora y descripción del hallazgo de la alerta. | Administrador |
| `RF-161` | Al abrir el detalle, la alerta debe quedar marcada como vista para el administrador actual. | Administrador |
| `RF-162` | Para alertas *Warning*, el sistema debe mostrar una tabla con los últimos 10 registros del sensor en la estación con valores entre el umbral de advertencia y el crítico. | Administrador |
| `RF-163` | Para alertas *Critical*, el sistema debe mostrar una tabla con los últimos 10 registros del sensor en la estación con valores sobre el umbral crítico. | Administrador |
| `RF-164` | Para alertas *Informative*, el sistema no debe mostrar tabla, sino una explicación de lo que se encontró y generó la alerta. | Administrador |

---

### V-18 Estaciones eliminadas *(solo Administrador)*

**Objetivo:** consultar las estaciones eliminadas y la trazabilidad de su eliminación.

**Estructura:**
1. Barra lateral (`C-09`).
2. Tarjetas de estaciones eliminadas con:
   - Nombre.
   - ID.
   - Dirección.
   - Fecha de eliminación.
   - Administrador que eliminó la estación.
   - Motivo de la eliminación.
3. Pie de página (`C-10`).

**Estados de la vista:** carga; sin estaciones eliminadas.

| ID | Requisito | Rol |
|----|-----------|-----|
| `RF-170` | El sistema debe listar las estaciones en estado *Deleted* con nombre, ID, dirección, fecha de eliminación, administrador que la eliminó y motivo. | Administrador |
| `RF-171` | El administrador debe poder acceder a esta vista desde un botón *Estaciones eliminadas* en el listado de estaciones (V-03) (propuesto, `PA-44`). | Administrador |

---

### V-19 Ventana de eliminación de cuenta *(Administrador)*

**Tipo:** ventana de confirmación (`C-05`). Se abre con *Eliminar cuenta* de V-11.

**Estructura:**
1. Advertencia sobre las consecuencias: la acción es irreversible, la cuenta no se podrá reactivar y los datos personales se reemplazarán.
2. Verificación por contraseña (`C-06`).
3. Botones *Cancelar* y *Eliminar cuenta*.

**Datos al eliminar la cuenta** (desactivación con anonimización, no borrado total):

| Dato | Valor tras la eliminación |
|------|---------------------------|
| ID | Se mantiene (trazabilidad) |
| Nombre | "Usuario no disponible" |
| Correo | Correo ficticio construido con un UUID (por ejemplo, `<uuid>@eliminado.invalid`, propuesto) |
| Contraseña | `NULL` |

La misma anonimización se aplica cuando otro administrador elimina la cuenta (V-35).

**Estados de la vista:** contraseña incorrecta; es el último administrador activo (propuesto, `PA-40`).

| ID | Requisito | Rol |
|----|-----------|-----|
| `RF-180` | El administrador debe poder eliminar su cuenta tras confirmar la acción y verificar su contraseña; la cuenta pasa a estado *Deleted* y se cierra la sesión. | Administrador |
| `RF-181` | Al eliminar la cuenta, el sistema debe conservar el ID y reemplazar el nombre, el correo y la contraseña según la tabla anterior. | Administrador |
| `RF-183` | Una cuenta eliminada no se puede reactivar. | Administrador |

---

### V-21 Ventana de restablecer contraseña (con sesión iniciada) *(Administrador)*

**Tipo:** ventana emergente. Se abre con *Restablecer contraseña* en el menú de perfil de la barra lateral.

**Estructura:**
1. Contraseña actual (`C-06`).
2. Nueva contraseña y confirmación (`C-07`).
3. Enlace *Olvidé mi contraseña* (abre V-10).
4. Botones *Cancelar* y *Guardar*.

**Estados de la vista:** contraseña actual incorrecta; nueva contraseña igual a la anterior; la nueva contraseña no cumple la política.

| ID | Requisito | Rol |
|----|-----------|-----|
| `RF-200` | El administrador debe poder cambiar su contraseña verificando la contraseña actual; la nueva debe cumplir la política (`RN-14`) y no puede ser igual a la anterior. | Administrador |
| `RF-202` | La ventana debe ofrecer la opción *Olvidé mi contraseña*, que abre la recuperación de contraseña (V-10). | Administrador |

---

### V-25 Catálogo de sensores *(solo Administrador)*

**Objetivo:** registrar los modelos de sensores que se pueden instalar en las estaciones y las variables que mide cada uno, para que agregar un sensor nuevo en futuras mejoras no exija cambios en el código de la plataforma.

**Estructura:**
1. Barra lateral (`C-09`).
2. Botón *Agregar sensor*.
3. **Tabla de sensores:** nombre corto, fabricante, modelo, variables que mide (con unidad), cantidad de estaciones que lo tienen asignado y botones *Editar* y *Eliminar*.
4. **Formulario de sensor** (ventana emergente al agregar o editar):
   - Nombre corto (por ejemplo, *Honeywell*).
   - Fabricante y modelo (por ejemplo, *Honeywell HPMA115S0-XXX*).
   - Enlace a la hoja técnica (propuesto).
   - Variables que mide: una fila por variable con *Variable* (de la lista de variables existentes o nueva), *Unidad*, *Rango físico mínimo* y *Rango físico máximo* (propuesto, útil para la validación de datos y `PA-33`).
   - Botones *Cancelar* y *Guardar*.
5. Pie de página (`C-10`).

Los tres sensores iniciales (sección 2.3) vienen precargados.

**Estados de la vista:** errores de validación; nombre de sensor duplicado; intento de eliminar un sensor asignado a alguna estación.

| ID | Requisito | Rol |
|----|-----------|-----|
| `RF-240` | El administrador debe poder registrar un sensor con su nombre corto, fabricante, modelo y las variables que mide, cada una con su unidad. | Administrador |
| `RF-241` | El administrador debe poder editar la información de un sensor del catálogo. | Administrador |
| `RF-242` | Una variable nueva registrada en el catálogo debe quedar disponible en los filtros de gráficos y tablas, en la generación de archivos, en los umbrales de alerta (V-15) y en la analítica de datos. | Administrador |
| `RF-243` | El sistema no debe permitir eliminar un sensor que esté asignado a alguna estación (propuesto). | Administrador |
| `RF-244` | El catálogo debe incluir precargados los sensores Winsen ZPHS01B, Honeywell HPMA115S0-XXX y AM2305B con sus variables. | Administrador |

---

### V-26 Ventana de sensores de la estación *(solo Administrador)*

**Tipo:** ventana emergente. Se abre con el botón *Sensores* de V-04.

**Estructura:**
1. Lista de sensores asignados a la estación: nombre, modelo, identificador del sensor dentro del mensaje MQTT (según V-27) y fecha de instalación (propuesto); botón *Retirar* por sensor.
2. Botón *Agregar sensor*: selector de sensores del catálogo (V-25).
3. Botones *Cancelar* y *Guardar*.

**Estados de la vista:** sin sensores asignados; confirmación al retirar un sensor.

| ID | Requisito | Rol |
|----|-----------|-----|
| `RF-250` | El administrador debe poder agregar a una estación uno o varios sensores del catálogo. | Administrador |
| `RF-251` | El administrador debe poder retirar un sensor de una estación; los datos históricos de ese sensor se conservan. | Administrador |
| `RF-252` | Al agregar o retirar un sensor, el sistema debe generar una alerta informativa con el administrador involucrado. | Administrador |
| `RF-253` | Los sensores asignados determinan las tarjetas de sensores de V-04 y los sensores y variables que el sistema espera en los mensajes MQTT de la estación. | Administrador |

---

### V-27 Formatos del mensaje MQTT *(solo Administrador)*

**Objetivo:** definir cómo se interpreta el mensaje JSON que publica cada estación en el bróker MQTT, para adaptar la plataforma a cambios del firmware sin modificar el código. **Cada estación puede tener su propio formato**, incluidas las estaciones caseras de los administradores. La vista administra formatos con nombre que se asignan a las estaciones; un mismo formato puede servir a varias estaciones, por ejemplo un formato base para las 5 estaciones de la primera fase (propuesto). La estructura concreta del JSON, los tópicos y el QoS están por definir con Ingeniería Electrónica (`PA-50`).

**Estructura:**
1. Barra lateral (`C-09`).
2. **Lista de formatos:** nombre, estaciones que lo usan, fecha de la última modificación y administrador; botones *Agregar formato* y *Editar*.
3. **Formulario de formato:**
   - Nombre del formato.
   - **Estructura del tópico:** plantilla del tópico en el que publican las estaciones que usan el formato (por ejemplo, `estaciones/{id_estacion}/lecturas`, propuesto).
   - **Correspondencia de campos:** tabla que relaciona cada dato que necesita la plataforma con su ubicación dentro del JSON:
     - Identificador de la estación de origen.
     - Marca de tiempo de la lectura y su formato. Los formatos admitidos dependen del reloj de las estaciones (`RN-33`); referencia propuesta: ISO 8601 en UTC.
     - Por cada sensor y variable: ruta del valor dentro del JSON (por ejemplo, `sensores.honeywell.pm25`).
     - Código de estado o de error del sensor, según la matriz de valores y umbrales que se defina (`PA-32`).
   - **Estaciones asignadas:** estaciones que usan este formato.
4. **Prueba del formato:** área de texto para pegar un mensaje JSON de ejemplo y botón *Probar*, que muestra cómo se interpretaría: estación, marca de tiempo (y si se corregiría), valores reconocidos, campos faltantes y errores.
5. **Versiones del formato** (propuesto): lista de versiones guardadas con fecha y administrador.
6. Botones *Cancelar* y *Guardar*.
7. Pie de página (`C-10`).

**Estados de la vista:** formato inválido; el mensaje de prueba no cumple el formato; formato sin estaciones asignadas; cambios guardados.

| ID | Requisito | Rol |
|----|-----------|-----|
| `RF-260` | El administrador debe poder definir, para cada formato, la plantilla del tópico MQTT en el que publican las estaciones que lo usan. | Administrador |
| `RF-261` | El administrador debe poder definir la correspondencia entre los campos del mensaje JSON y la estación, la marca de tiempo (con su formato), los sensores, las variables, sus valores y los códigos de error. | Administrador |
| `RF-262` | El administrador debe poder probar el formato con un mensaje de ejemplo antes de guardarlo. | Administrador |
| `RF-263` | Al guardar el formato, el sistema debe interpretar con él los mensajes que reciba a partir de ese momento de las estaciones que lo usan; los mensajes que no lo cumplan se rechazan y se registran (propuesto, `PA-50`). | Administrador |
| `RF-264` | El sistema debe conservar las versiones anteriores del formato con su fecha y el administrador que las guardó (propuesto). | Administrador |
| `RF-265` | Cada estación debe tener asignado un formato; el administrador lo asigna al crear la estación (V-14) y lo puede cambiar desde esta vista. | Administrador |
| `RF-266` | El sistema debe interpretar los mensajes de cada estación con el formato asignado a ella. | Administrador |

---

### V-28 Ventana de regenerar credenciales *(solo Administrador)*

**Tipo:** ventana de confirmación (`C-05`) con verificación por contraseña (`C-06`). Se abre con el botón *Regenerar credenciales* de V-04.

**Estructura:**
1. Advertencia: las credenciales actuales dejarán de funcionar de inmediato y la estación no podrá enviar datos hasta que se configure con las nuevas.
2. Verificación por contraseña del administrador (`C-06`).
3. Botones *Cancelar* y *Regenerar*.
4. Tras confirmar: visualización de las nuevas credenciales (`C-12`).

**Estados de la vista:** contraseña incorrecta.

| ID | Requisito | Rol |
|----|-----------|-----|
| `RF-270` | El administrador debe poder regenerar las credenciales de conexión de una estación con el bróker tras confirmar la acción y verificar su contraseña. | Administrador |
| `RF-271` | Al regenerar las credenciales, el sistema debe invalidar de inmediato las anteriores. | Administrador |
| `RF-272` | El sistema debe mostrar las nuevas credenciales una sola vez (`C-12`) y almacenarlas de forma que no se puedan volver a consultar en texto plano. | Administrador |
| `RF-273` | Al regenerar las credenciales, el sistema debe generar una alerta informativa con el administrador involucrado. | Administrador |

---

### V-29 Fuentes de referencia — *Retirada*

Se retira porque no se usan fuentes externas y no se marca ninguna estación como referencia: en cada calibración el administrador elige directamente, en V-30, cualquier estación registrada (`RN-36`). Las estaciones caseras se registran como cualquier otra estación (V-14). Se retiran `RF-280` a `RF-284`.

---

### V-30 Calibración de sensores *(solo Administrador; preliminar)*

**Objetivo:** comparar modelos de aprendizaje automático que calibren una variable de un sensor con los datos de 2 o más estaciones registradas, para que el administrador elija y descargue el modelo que prefiera (sección 2.8).

**Estructura:**
1. Barra lateral (`C-09`).
2. **Configuración:**
   - **Variable a calibrar:** estación, sensor y variable. Se muestran su unidad y el rango de fechas con datos (propuesto).
   - **Variable de referencia:** estación, sensor y variable, de cualquier estación registrada. No puede ser la variable a calibrar; si las unidades no coinciden, se advierte.
   - **Variables auxiliares:** cero, una o varias, de cualquier estación registrada, con su estación de origen visible. No se pueden elegir la variable a calibrar ni la de referencia; se advierte si una auxiliar proviene de la misma estación que la referencia (propuesto).
   - **Periodo de datos:** fecha inicial y fecha final.
   - **Modelos:** selección múltiple de la lista de la sección 2.8.
   - **División de los datos para entrenamiento y prueba** (propuesto: por bloques de tiempo, 70 % / 30 %; por confirmar, `PA-52`).
   - Botón *Ejecutar calibración*.
3. **Estado del entrenamiento** (`C-01`, sección 2.8), con avance y botón *Cancelar*.
4. **Resultados** (`C-11`): una gráfica por modelo con su R² y el botón *Descargar modelo*.
5. Aviso de que los resultados no se guardan: al salir de la vista se pierden (`PCAL-13`).
6. Pie de página (`C-10`).

**Estados de la vista:** datos insuficientes después de emparejar; sin coincidencia temporal entre las variables elegidas; unidades distintas entre la variable a calibrar y la de referencia (advertencia); error en uno de los modelos.

| ID | Requisito | Rol |
|----|-----------|-----|
| `RF-290` | El sistema debe permitir configurar una calibración eligiendo la variable a calibrar, la variable de referencia, las variables auxiliares, el periodo y los modelos, entre los datos de cualquier estación registrada. | Administrador |
| `RF-291` | El sistema debe emparejar en el tiempo las variables elegidas con promedios por minuto y usar solo los minutos con datos de todas ellas (`RN-36`). | Administrador |
| `RF-292` | El sistema debe entrenar cada modelo seleccionado con las mismas variables de entrada, el mismo objetivo y la misma división de datos. | Administrador |
| `RF-293` | El sistema debe mostrar una gráfica por modelo con los datos crudos y calibrados frente a la referencia, la línea g(x) = x y el R² de los datos calibrados (`C-11`). | Administrador |
| `RF-294` | El sistema debe impedir que la variable de referencia se use también como variable a calibrar o como variable auxiliar. | Administrador |
| `RF-295` | El administrador debe poder descargar el modelo que elija: `.keras` para las redes neuronales; el formato de los demás modelos está por definir (`PA-54`). | Administrador |
| `RF-296` | El sistema debe validar que, después de emparejar los datos, queden datos suficientes antes de entrenar. | Administrador |
| `RF-297` | El sistema no debe guardar los resultados de la calibración ni aplicar los modelos a los datos de la plataforma (`RN-35`). | Administrador |

---

### V-31 Estimación de variables *(solo Administrador; por confirmar si sigue en el alcance, `PA-52`; preliminar)*

**Objetivo:** calcular una variable a partir de otras variables correlacionadas (por ejemplo, estimar la temperatura a partir de la humedad), eligiendo las variables de entrada, la variable a estimar y el modelo.

**Estructura:**
1. Barra lateral (`C-09`).
2. **Configuración:**
   - Origen de los datos: una o varias estaciones registradas.
   - Variables de entrada (selección múltiple).
   - Variable a estimar (distinta de las de entrada).
   - Matriz de correlación entre las variables elegidas, como ayuda para seleccionarlas (propuesto).
   - Modelo(s) a entrenar.
   - Periodo de datos.
   - Botón *Entrenar*.
3. **Estado del entrenamiento** (`C-01`, sección 2.8).
4. **Resultados** (`C-11`), con el botón *Descargar modelo*. Los resultados no se guardan (`RN-35`).
5. Pie de página (`C-10`).

**Estados de la vista:** datos insuficientes; variable a estimar incluida entre las de entrada; error en el entrenamiento.

| ID | Requisito | Rol |
|----|-----------|-----|
| `RF-300` | El sistema debe permitir elegir las variables de entrada y una variable a estimar distinta de ellas. | Administrador |
| `RF-301` | El sistema debe mostrar la correlación entre las variables elegidas (propuesto). | Administrador |
| `RF-302` | El sistema debe permitir elegir el modelo o los modelos con los que se hará la estimación. | Administrador |
| `RF-303` | El sistema debe mostrar los resultados del entrenamiento con una gráfica por modelo y su R² (`C-11`). | Administrador |
| `RF-304` | El administrador debe poder descargar el modelo elegido (`PA-54`). | Administrador |

---

### V-32 Modelos — *Retirada*

Se retira porque los modelos no se guardan en la plataforma ni se aplican a los datos: el administrador descarga el modelo elegido desde V-30 (`PCAL-01`, `PCAL-03`, `PCAL-13`). Se retiran `RF-310` a `RF-314`.

---

### V-33 Administradores *(solo Administrador)*

**Objetivo:** listar y administrar las cuentas de los administradores de la plataforma. Se abre con *Administradores* en la barra lateral.

**Estructura:**
1. Barra lateral (`C-09`).
2. Botón *Registrar administrador* (abre V-13).
3. **Filtros** (propuesto): estado (*Pendiente de activación*, *Activa*) y búsqueda por nombre o correo.
4. **Tabla de administradores:** nombre, correo, estado (`C-01`), fecha de registro y administrador que lo registró (propuesto). Acciones por fila:
   - *Editar* (abre V-34).
   - *Eliminar* (abre V-35). No aparece en la fila del administrador actual, que elimina su propia cuenta desde el perfil (V-19) (propuesto).
   - *Reenviar enlace de activación*: solo en cuentas *New* (propuesto).
5. Pie de página (`C-10`).

Las cuentas *Deleted* no se listan (propuesto).

**Estados de la vista:** carga; sin resultados para los filtros elegidos; enlace de activación reenviado.

| ID | Requisito | Rol |
|----|-----------|-----|
| `RF-320` | El sistema debe listar las cuentas de administrador en estado *New* y *Active* con su nombre, correo, estado y fecha de registro. | Administrador |
| `RF-321` | La vista debe dar acceso al registro (V-13), a la edición (V-34) y a la eliminación (V-35) de administradores. | Administrador |
| `RF-322` | El administrador debe poder reenviar el enlace de activación de una cuenta en estado *New*; al reenviarlo, el enlace anterior queda inválido (propuesto). | Administrador |
| `RF-323` | La lista de administradores debe poder filtrarse por estado y buscarse por nombre o correo (propuesto). | Administrador |

---

### V-34 Ventana de edición de administrador *(solo Administrador)*

**Tipo:** ventana emergente. Se abre con el botón *Editar* de V-33 y carga los datos actuales del administrador.

**Estructura:**
1. Campos *Nombre* y *Correo electrónico* (campos editables propuestos; por confirmar, `PA-48`).
2. Botones *Cancelar* y *Guardar*.

Un administrador no puede definir ni ver la contraseña de otro: si la olvidó, la recupera con V-10 (propuesto).

**Estados de la vista:** errores de validación; correo ya registrado; cambios guardados.

| ID | Requisito | Rol |
|----|-----------|-----|
| `RF-330` | El administrador debe poder editar el nombre y el correo de otro administrador (propuesto, `PA-48`). | Administrador |
| `RF-331` | El sistema no debe permitir cambiar el correo por uno que ya tenga una cuenta en estado *New* o *Active*. | Administrador |
| `RF-332` | Si se cambia el correo de una cuenta en estado *New*, el sistema debe invalidar el enlace de activación anterior y enviar uno nuevo al correo actualizado (propuesto). | Administrador |

---

### V-35 Ventana de eliminación de administrador *(solo Administrador)*

**Tipo:** ventana de confirmación (`C-05`) con verificación por contraseña (`C-06`). Se abre con el botón *Eliminar* de V-33.

**Estructura:**
1. Advertencia: la acción es irreversible, la cuenta no se podrá reactivar y sus datos personales se reemplazarán como en V-19 (`RN-20`).
2. Verificación por contraseña del administrador que ejecuta la acción (`C-06`).
3. Botones *Cancelar* y *Eliminar administrador*.

**Estados de la vista:** contraseña incorrecta.

| ID | Requisito | Rol |
|----|-----------|-----|
| `RF-340` | El administrador debe poder eliminar la cuenta de otro administrador tras confirmar la acción y verificar su propia contraseña; la cuenta pasa a *Deleted* y sus datos se anonimizan como en V-19 (`RN-20`). | Administrador |
| `RF-341` | Al eliminar una cuenta, el sistema debe cerrar sus sesiones abiertas e invalidar sus enlaces de activación o de recuperación pendientes (propuesto). | Administrador |
| `RF-342` | Un administrador no debe poder eliminar su propia cuenta desde esta vista; para hacerlo usa V-19 (propuesto). | Administrador |

---

### V-36 Activación de cuenta *(Administrador, sin sesión)*

**Objetivo:** que el nuevo administrador establezca su contraseña y active su cuenta. Se abre desde el enlace de activación que recibe por correo al ser registrado (V-13).

**Estructura:**
1. Nombre/logo de la plataforma.
2. Mensaje de bienvenida con el nombre y el correo de la cuenta.
3. Nueva contraseña y confirmación (`C-07`).
4. Botón *Activar cuenta*. Al terminar, muestra "Cuenta activada exitosamente" y redirige al inicio de sesión (V-01).
5. Pie de página (`C-10`).

**Valores del enlace (propuestos; por confirmar, `PA-48`):** un solo uso; vigencia de 24 horas; al reenviarlo (V-33), el anterior queda inválido.

**Estados de la vista:** enlace inválido, vencido o ya usado (indica pedir a un administrador que reenvíe el enlace); la contraseña no cumple la política.

| ID | Requisito | Rol |
|----|-----------|-----|
| `RF-350` | El sistema debe mostrar el formulario de activación solo si el enlace es válido y la cuenta está en estado *New*. | Administrador |
| `RF-351` | El sistema debe validar que la contraseña cumpla la política (`RN-14`) y coincida con su confirmación. | Administrador |
| `RF-352` | Al establecer la contraseña, la cuenta debe pasar a estado *Active* y el sistema debe dirigir al administrador al inicio de sesión (V-01). | Administrador |
| `RF-353` | Los enlaces de activación deben ser de un solo uso, vencer según la vigencia definida e invalidarse cuando se genera uno nuevo. | Administrador |

---

## 5. Reglas de negocio

| ID | Regla |
|----|-------|
| `RN-01` | Una estación puede tener instalados uno o varios sensores del catálogo (V-25). Las estaciones inician con Winsen ZPHS01B, Honeywell HPMA115S0-XXX y AM2305B, pero no todas tienen que tener los mismos sensores y se pueden agregar sensores nuevos. |
| `RN-02` | En el listado de estaciones (V-03), el gráfico muestra una sola variable de un solo sensor a la vez. |
| `RN-03` | Una estación deshabilitada solo se oculta del mapa (estado *Hidden*) si el administrador lo indicó al deshabilitarla. Ni el visitante ni el administrador ven las estaciones *Hidden* en el mapa; solo el administrador las ve en el listado. Una estación *Hidden* debe mostrarse (pasar a *Inactive*) antes de poder habilitarse. |
| `RN-04` | Una estación eliminada no se muestra en el mapa ni en el listado; sus datos se conservan por trazabilidad (eliminación lógica) y solo es visible en la vista de estaciones eliminadas (V-18). |
| `RN-06` | Crear, editar, habilitar/deshabilitar, ocultar/mostrar y eliminar estaciones; gestionar el catálogo de sensores y los sensores de cada estación; regenerar credenciales; configurar el formato del mensaje MQTT; ver alertas; modificar la configuración; usar el módulo de calibración y registrar, listar, editar y eliminar administradores son acciones exclusivas del rol Administrador. |
| `RN-07` | La nueva contraseña no puede ser igual a la contraseña inmediatamente anterior. |
| `RN-09` | La marca de "vista" de una alerta es individual: solo afecta al administrador que la marcó; para los demás administradores la alerta sigue sin ver. La marca se puede quitar. |
| `RN-10` | El estado de una alerta por umbral se determina así: *Warning* si el valor supera el umbral de advertencia sin superar el crítico; *Critical* si supera el umbral crítico. |
| `RN-11` | No existe registro público: solo un administrador puede crear cuentas, y todas tienen rol `admin` (V-13). Los administradores pueden listar, editar y eliminar las cuentas de los demás administradores (V-33 a V-35). |
| `RN-12` | Las cuentas de administrador quedan en estado *New* al crearse y pasan a *Active* cuando el nuevo administrador establece su contraseña con el enlace de activación (V-36). Una cuenta *New* no puede iniciar sesión. |
| `RN-13` | Para eliminar una estación son obligatorios el motivo y la contraseña del administrador. |
| `RN-14` | Política de contraseñas: mínimo 8 caracteres, con al menos una minúscula, una mayúscula, un número y un carácter especial. |
| `RN-15` | El estado automático de una estación se calcula con esta prioridad: *NoData* si no ha enviado su primer mensaje desde que se creó o se habilitó, o si está desconectada (`RN-34`); si no, *Error* si algún sensor está *Offline*; si no, *Warning* si algún sensor está en *Warning*; si no, *Active*. |
| `RN-16` | El estado de un sensor es *Offline* si no envía datos o envía un código de error (definido con la matriz de valores y umbrales, `PA-32`); *Warning* si sus datos superan un umbral o presentan una anomalía; *Ok* en cualquier otro caso. |
| `RN-19` | Al registrar a un administrador solo se le solicitan nombre y correo; cada administrador define su propia contraseña al activar la cuenta (V-36). A los visitantes no se les solicita ningún dato personal. |
| `RN-20` | Eliminar una cuenta de administrador, sea su titular (V-19) u otro administrador (V-35), la desactiva y anonimiza sus datos personales (se conserva el ID) y es irreversible. |
| `RN-22` | Las alertas solo se notifican dentro de la plataforma (vista de alertas y contador de la barra lateral); no se envían por correo ni por notificaciones *push*. |
| `RN-24` | La recuperación de contraseña muestra siempre un mensaje neutro y limita los reenvíos (valores propuestos en V-10). |
| `RN-26` | Crear, editar, habilitar, deshabilitar, ocultar, mostrar y eliminar una estación, agregarle o retirarle sensores y regenerar sus credenciales genera una alerta informativa con el administrador involucrado. |
| `RN-27` | La consulta de datos (mapa, listado, detalle, gráficos y tablas) y la generación de archivos de descarga no requieren registro ni inicio de sesión. |
| `RN-28` | Cada estación tiene credenciales propias para conectarse al bróker MQTT. Las credenciales se muestran una sola vez al crearlas o regenerarlas, y regenerarlas invalida las anteriores. |
| `RN-29` | Las estaciones envían sus datos por MQTT. Cada estación puede tener su propio formato de mensaje, y sus mensajes se interpretan con el formato que tiene asignado (V-27); los que no lo cumplen se rechazan (propuesto). |
| `RN-30` | *(Fases futuras.)* Las estaciones con LoRa envían sus datos a una estación con WiFi, que los publica en el bróker. Cada lectura se asocia a la estación de origen, no a la estación de enlace (propuesto, `PA-51`). En la primera fase todas las estaciones usan WiFi. |
| `RN-31` | Todos los modelos de una misma calibración se entrenan con los mismos datos. Los modelos mínimos que debe ofrecer el módulo están por definir; como base, regresión lineal múltiple y red neuronal artificial (`PA-52`). |
| `RN-32` | Los datos originales de los sensores se conservan siempre; los valores calibrados o estimados nunca reemplazan a los originales. |
| `RN-33` | La plataforma valida la marca de tiempo de cada mensaje. Un formato distinto pero legible se convierte; si la marca no se puede interpretar o está desajustada, la lectura se guarda con la hora de recepción del servidor, se marca como *corregida* y se genera una sola alerta por episodio, que se mantiene hasta que la estación vuelva a enviar marcas válidas. Los formatos admitidos y la diferencia tolerada están por definir (`PC-02`). |
| `RN-34` | Los huecos y la desconexión los mide el servidor con la hora de llegada de los mensajes: más de 60 s sin mensajes es un periodo "sin datos", que se muestra sin interpolar; superar el tiempo máximo sin mensajes (10 minutos por defecto, configurable en V-15) es una desconexión, y la estación pasa a *NoData*. Los datos de una estación sin conexión no se guardan ni se reenvían (pérdida aceptada). |
| `RN-35` | Los modelos de calibración no se aplican a los datos de la plataforma: el público ve solo los datos crudos, y los valores calibrados solo se usan para comparar modelos. No se guardan los modelos ni el historial de calibraciones; el administrador descarga el modelo que elija. |
| `RN-36` | En una calibración se pueden usar datos de cualquier estación registrada, sin marcarla como referencia y sin exigir que las estaciones estén en el mismo lugar. Las lecturas se emparejan con promedios por minuto. La calidad de la referencia la decide el investigador. |
| `RN-37` | El color de una estación en el mapa corresponde al nivel de alerta de los umbrales de sus variables ambientales, no a su estado de funcionamiento. Una estación en estado *NoData* se muestra en gris y opaca. |
| `RN-38` | La plataforma solo escucha en el bróker los mensajes de las estaciones en estado *NoData*, *Active*, *Warning* o *Error*. Al deshabilitar, ocultar o eliminar una estación, deja de escuchar sus mensajes: no se reciben ni se almacenan sus datos, no se calcula el estado de sus sensores y no se generan alertas por umbrales, anomalías o conexión. Al habilitarla, vuelve a escucharlos. |
| `RN-39` | Desde el momento en que una estación pasa a *Hidden* o *Deleted*, sus datos históricos no se grafican ni se pueden descargar, para ningún rol. Si una estación *Hidden* se vuelve a mostrar, sus datos vuelven a estar disponibles (propuesto). |
| `RN-40` | La generación de archivos de descarga exige resolver un *captcha*. Cada archivo se conserva en el servidor un máximo de 12 horas, se descargue o no, y de cada persona solo se conserva el último archivo generado. Las solicitudes de generación y las descargas tienen un límite por dirección IP, tanto para visitantes como para administradores (valores por definir, `PA-46`). |

---

## 6. Requisitos no funcionales (propuestos)

Estos requisitos no estaban en la descripción original; se proponen para completar la especificación y deben validarse.

| ID | Requisito |
|----|-----------|
| `RNF-01` | **Usabilidad:** la interfaz debe estar en español y adaptarse a pantallas de computador, tableta y celular; en celulares, la barra lateral se contrae en un botón de menú. |
| `RNF-02` | **Accesibilidad:** el estado no debe comunicarse solo con color; cada indicador debe incluir también el texto del estado. |
| `RNF-03` | **Seguridad:** las vistas de consulta (V-02 a V-05) son públicas; las demás, salvo el inicio de sesión (V-01), la recuperación de contraseña (V-10) y la activación de cuenta (V-36), requieren una sesión de administrador. Los permisos deben validarse también en el servidor. |
| `RNF-04` | **Rendimiento:** los gráficos deben cargar en un tiempo razonable incluso con el rango de 2 años (valor objetivo por definir). |
| `RNF-05` | **Consistencia de datos:** las fechas y horas se almacenan en UTC y se muestran en la zona horaria de Colombia (UTC−5), con un formato uniforme (`R-09` de la contextualización). |
| `RNF-06` | **Trazabilidad:** las acciones de los administradores sobre las estaciones (crear, editar, habilitar/deshabilitar, ocultar/mostrar, eliminar, asignar o retirar sensores y regenerar credenciales) quedan registradas con usuario, fecha y hora mediante las alertas informativas (`RN-26`). |
| `RNF-07` | **Seguridad de credenciales:** las contraseñas de los administradores y las credenciales de las estaciones deben almacenarse cifradas con un algoritmo de *hash*; los enlaces de recuperación y de activación deben ser de un solo uso y tener fecha de vencimiento; las solicitudes de recuperación y reenvío deben tener límite de frecuencia. |
| `RNF-08` | **Protección de datos personales:** la plataforma solo trata datos personales de los administradores (nombre y correo); debe publicar su política de tratamiento de datos conforme a la normativa aplicable (en Colombia, Ley 1581 de 2012). |
| `RNF-09` | **Seguridad del bróker MQTT:** el bróker debe exigir autenticación por estación, usar cifrado TLS y restringir a cada estación a publicar solo en su propio tópico. |
| `RNF-10` | **Uso público responsable:** las funciones públicas (consultas y generación de archivos) deben tener límites de frecuencia por dirección IP y límites de tamaño para evitar abusos que afecten el rendimiento o el costo. La generación de archivos exige un *captcha* y tiene límites de solicitudes y de descargas por dirección IP también para los administradores (`RN-40`, `PA-46`). |
| `RNF-11` | **Costo computacional de la analítica:** los entrenamientos solo se ejecutan cuando un administrador lo decide (`PCAL-05`), en segundo plano, sin afectar la consulta de datos y con un límite de entrenamientos simultáneos. |
| `RNF-12` | **Lenguaje claro:** las vistas públicas deben usar un lenguaje no técnico, comprensible por cualquier persona, para favorecer la apropiación social del conocimiento. |

---

## 7. Puntos abiertos

| ID | Tema pendiente | Estado |
|----|----------------|--------|
| `PA-01` | Definir el nombre de la plataforma. | **Resuelto en cuanto al responsable:** lo elige el equipo de desarrollo; el nombre aún no se ha definido. |
| `PA-02` | Definir las condiciones de los estados de estaciones y sensores. | **Resuelto** (secciones 2.2 y 2.4, `RN-15`, `RN-16`). Quedan detalles en `PA-31` y `PA-32` (`PA-30` quedó resuelto con el estado *NoData*). |
| `PA-03` | Validar el manejo del estado *Deleted* (eliminación lógica). | **Resuelto:** ya no se valida. Quedan temas relacionados en `PA-28` y `PA-36`. |
| `PA-04` | Confirmar las unidades de cada variable según la hoja técnica de cada sensor. | **Resuelto:** se revisaron las unidades y todas corresponden con la hoja técnica de cada sensor (sección 2.3; reunión 19/10, `PC-02`). |
| `PA-05` | Definir si habrá registro de nuevos usuarios y recuperación de contraseña, y cómo se asigna el rol a cada usuario. | **Resuelto:** no hay registro público; la consulta es libre y solo los administradores tienen cuenta, creada por otro administrador (V-13). Los administradores pueden recuperar su contraseña (V-10). |
| `PA-06` | Definir los formatos de archivo de descarga. | **Resuelto:** CSV y JSON. |
| `PA-07` | Definir los campos editables de una estación. | **Resuelto:** nombre, ubicación y descripción. |
| `PA-08` | Definir el mecanismo de verificación de seguridad para eliminar una estación. | **Resuelto:** confirmación, motivo obligatorio y contraseña del administrador. |
| `PA-09` | Definir cómo se crean nuevas estaciones: campos del formulario y procedimiento para que la estación empiece a enviar datos. | Parcial: ya existe la vista (V-14), la forma de definir la ubicación (`PA-27`), la asignación de sensores, la asignación del formato del mensaje (V-27) y la generación de credenciales MQTT; falta la estructura concreta del mensaje (`PA-50`). Las estaciones LoRa quedan para fases futuras (`PA-51`). |
| `PA-10` | Definir si el administrador puede ver las estaciones ocultas o eliminadas desde alguna vista. | **Resuelto:** ocultas en el listado (V-03), eliminadas en V-18. |
| `PA-11` | Definir si el historial de archivos generados tiene un tiempo de expiración. | **Descartado:** no se guarda historial de archivos generados. |
| `PA-12` | Confirmar el color del estado *Warning* de los sensores. | **Resuelto:** `#D97706`. |
| `PA-13` | Confirmar en qué estados aparece el botón *Cancelar* de una descarga. | **Resuelto:** solo en *Validating* y *Generating*. |
| `PA-14` | Definir si el administrador ve en el mapa las estaciones *Hidden* y dónde se usa el color `#cccccc`. | **Resuelto:** no las ve en el mapa; solo en el listado, con el indicador *Oculta* sobre fondo `#cccccc`. |
| `PA-15` | Confirmar si todas las estaciones tienen los mismos sensores o si pueden tener distintos. | **Resuelto:** inician con AM2305B, Winsen ZPHS01B y Honeywell HPMA115S0-XXX, pueden tener sensores distintos y se agregarán otros en el futuro (V-25, V-26). |
| `PA-16` | Definir si una estación *Hidden* se puede habilitar directamente. | **Resuelto:** no; primero se muestra (pasa a *Inactive* y vuelve al mapa) y luego el botón cambia a *Habilitar*. |
| `PA-17` | Aclarar qué significa "validar el peso del archivo" en estado *Generated*. | **Resuelto:** mostrar su tamaño. |
| `PA-18` | Definir qué ocurre con un archivo al vencer su mes de disponibilidad. | **Descartado:** no se guarda historial; ver `PA-46` para la retención temporal. |
| `PA-19` | Registro con proveedores de identidad y número de teléfono. | **Descartado:** no hay registro de usuarios; los administradores usan correo y contraseña (ver `PA-48`). |
| `PA-20` | Inicio de sesión con cuenta *Pending* y vigencia del enlace de verificación. | **Descartado:** no existe el estado *Pending*. La activación por enlace se retomó en la versión 6 con el estado *New* (`RF-07`, V-36). |
| `PA-21` | Registro de administradores: quién define la contraseña. | **Resuelto (modificado en la versión 6):** quien registra define el nombre y el correo; el nuevo administrador define su propia contraseña con el enlace de activación (V-13, V-36). Pendientes en `PA-48`. |
| `PA-22` | Eliminación de cuenta: verificación, datos y reactivación. | **Resuelto:** contraseña, anonimización y sin reactivación (V-19). Casos especiales en `PA-40`. |
| `PA-23` | Política de contraseñas. | **Resuelto:** mínimo 8 caracteres con minúsculas, mayúsculas, números y caracteres especiales; solo se compara con la contraseña anterior. |
| `PA-24` | Alcance de los umbrales: definir si son globales por variable o por estación o sensor. | Abierto |
| `PA-25` | Definir los valores permitidos del intervalo de actualización y el rango del tiempo máximo sin mensajes, y si este último es un valor único o se define por estación. | Abierto (el *timeout* y los reintentos se retiraron, `PA-49`). |
| `PA-26` | Alertas: anomalías, evidencias, notificaciones, marca de vista y alertas por cambios de estado. | **Resuelto** (sección 2.6, V-16, V-17). Criterios cuantitativos y frecuencia en `PA-33` y `PA-34`. |
| `PA-27` | Definir cómo se obtiene la dirección y ubicación de la estación. | **Resuelto:** por dirección, coordenadas o selección en el mapa. Servicio de geocodificación en `PA-45`. |
| `PA-28` | Definir si una estación eliminada se puede restaurar desde la vista de estaciones eliminadas. | Abierto |
| `PA-29` | Mensaje de la recuperación de contraseña y reenvío del enlace. | **Resuelto:** mensaje neutro y botón *Reenviar enlace*. Por confirmar los valores propuestos: 60 s entre reenvíos, 5 solicitudes por hora, enlace válido 60 minutos y de un solo uso. |
| `PA-30` | **Estación recién creada:** se define que al crearla queda *Active*, pero *Active* exige que todos los sensores estén *Ok*; una estación sin datos todavía tendría sensores *Offline* y pasaría de inmediato a *Error*. ¿Cómo se trata ese periodo? *Recomendación:* un estado *Sin datos* / *Pendiente de conexión* (gris) hasta recibir la primera lectura, o conservar *Active* durante un tiempo de gracia. | **Resuelto:** nuevo estado *NoData* (*Sin datos*) para las estaciones recién creadas o que no reciben datos desde el bróker (sección 2.2, `RN-15`). Se diferencia de *Inactive*, *Hidden* y *Deleted* en que, en estos tres, la plataforma deja de escuchar los mensajes de la estación en el bróker (`RN-38`). |
| `PA-31` | **Detalles del cálculo de estados:** (a) si un sensor supera el umbral crítico, ¿queda en *Warning* (no existe un estado *Critical* de sensor)?; (b) si solo una variable del sensor llega con código de error y las demás llegan bien, ¿el sensor queda *Offline* o *Warning*?; (c) ¿cuánto tiempo sin datos convierte un **sensor** en *Offline*?; (d) ¿el estado se calcula con la última lectura o con una ventana de lecturas, y cuándo vuelve a *Ok*? *Recomendación:* usar la última lectura. | Parcial: la desconexión de la **estación** quedó en el tiempo máximo sin mensajes (10 minutos por defecto), medido con la hora de llegada, y deja la estación en *NoData* (`RN-34`); siguen abiertos (a) a (d). |
| `PA-32` | **Códigos de error de los sensores:** `-1` indica *Offline*, pero −1 °C es una temperatura válida, y 0 puede ser un valor real de algunos gases. A su vez, "valores negativos o en 0" se consideran anomalía. | Parcial: los códigos de error se definirán con una **matriz de valores y umbrales** por sensor que permita identificar y marcar los sensores con fallas (`PC-02`); falta construir esa matriz. *Recomendación:* excluir la temperatura de la regla de valores negativos. |
| `PA-33` | **Criterios de anomalía:** definir en números qué es un valor "muy alto", "muy bajo" o un "cambio abrupto" (por ejemplo, fuera del rango físico registrado en el catálogo de sensores, o una variación mayor a X entre lecturas consecutivas), y si esos parámetros serán configurables en V-15. | Abierto |
| `PA-34` | **Frecuencia de alertas:** si una variable permanece sobre el umbral durante horas, ¿se genera una alerta por cada lectura? *Recomendación:* una alerta por episodio (se abre al cruzar el umbral y no se repite hasta que el valor vuelva a la normalidad) o un tiempo mínimo entre alertas iguales. | Parcial: para la marca de tiempo inválida se definió una alerta por episodio (`RN-33`); falta decidir para las alertas por umbral y por anomalía. |
| `PA-35` | **Estaciones deshabilitadas:** ¿las estaciones *Inactive* y *Hidden* siguen recibiendo datos? Si es así, ¿se calculan los estados de sus sensores y se generan alertas por umbrales o anomalías? | **Resuelto:** la plataforma deja de escuchar sus mensajes en el bróker, por lo que no recibe datos, no calcula estados de sensores ni genera alertas (`RN-38`). |
| `PA-36` | **Datos de estaciones ocultas o eliminadas:** ¿aparecen en los filtros de gráficos, tablas y archivos de descarga? En particular, ¿el visitante puede ver o descargar datos de una estación *Hidden*? | **Resuelto en lo principal:** desde el cambio a *Hidden* o *Deleted*, sus datos históricos no se grafican ni se pueden descargar (`RN-39`). **Falta:** confirmar si la regla también aplica a las tablas de histórico y al detalle de una estación *Hidden* que abre el administrador, si el módulo de calibración puede usar esos datos y si los datos vuelven a estar disponibles al mostrar la estación. |
| `PA-37` | **Transiciones faltantes:** ¿se puede ocultar una estación que ya está *Inactive* sin habilitarla antes? Confirmar además que la etiqueta del botón tras *Mostrar* es *Habilitar* (el stakeholder mencionó "Habilitar o Activar"). | Abierto |
| `PA-38` | **Cuentas de Google/Microsoft:** eliminación de cuenta sin contraseña, vinculación de cuentas e ingreso de administradores con Microsoft. | **Descartado:** no hay inicio de sesión con Google ni Microsoft (el ingreso con Microsoft institucional queda como pregunta en `PA-48`). |
| `PA-39` | **Registro de administradores:** contraseña temporal por correo, reenvío de la invitación, captcha y catálogo de facultades. | **Descartado:** el registro pide solo nombre y correo, sin captcha ni facultad; desde la versión 6 la contraseña la define el nuevo administrador con un enlace de activación, que se puede reenviar (V-33, V-36). Lo pendiente pasa a `PA-48`. |
| `PA-40` | **Eliminación de cuentas de administradores:** (a) al anonimizar, las alertas y las estaciones eliminadas mostrarán "Usuario no disponible" como responsable, y se pierde la trazabilidad de quién hizo qué; ¿es aceptable?; (b) ¿se debe impedir que el último administrador elimine su cuenta? *Recomendación:* sí. | Abierto |
| `PA-41` | **Cuentas *Pending* vencidas.** | **Descartado:** no existe el estado *Pending*. |
| `PA-42` | **PQRS.** | **Descartado:** no hay PQRS; la comunicación se hace por el correo de contacto del pie de página (`C-10`). |
| `PA-43` | **Pie de página y documentos legales:** definir el titular de los derechos de autor, el correo de contacto (y quién lo atiende), el teléfono y la dirección, y quién redacta los *Términos y condiciones* y la *Política de tratamiento de datos personales*. | Abierto |
| `PA-44` | **Acceso a estaciones eliminadas:** la barra lateral definida no incluye V-18. *Recomendación:* un botón en el listado de estaciones (V-03) solo para administradores, o una opción adicional en la barra lateral. | Abierto |
| `PA-45` | **Geocodificación:** convertir dirección ↔ coordenadas requiere un servicio externo (por ejemplo, Google Maps, que es de pago, u OpenStreetMap/Nominatim, que es gratuito con límites de uso). Definir cuál se usará. | Abierto |
| `PA-46` | **Límites y retención de las descargas públicas:** como cualquier persona puede generar archivos sin iniciar sesión, ¿hay un rango máximo de fechas, un tamaño máximo de archivo, un número máximo de generaciones por dirección IP o un captcha? ¿Cuánto tiempo se conserva en el servidor un archivo generado que no se descarga? *Recomendación:* límite por IP, rango máximo de fechas y retención de pocas horas. | **Resuelto en lo principal** (`RN-40`): *captcha* para generar el archivo; retención máxima de 12 horas, se descargue o no; solo se conserva el último archivo de cada persona; límite de solicitudes y de descargas por IP para visitantes y administradores. **Falta:** los valores de los límites; si hay rango máximo de fechas o tamaño máximo de archivo; cómo se identifica a la persona para conservar su último archivo (sesión o IP) y si puede volver a descargarlo después de salir de V-05; el servicio de *captcha* (costo). |
| `PA-47` | **Evidencias y retención de alertas:** la tabla de evidencias, ¿muestra los registros del momento en que se generó la alerta (fotografía fija) o los últimos registros al abrir el detalle? *Recomendación:* fotografía fija, para que la evidencia no cambie. ¿Por cuánto tiempo se conservan las alertas? | Abierto |
| `PA-48` | **Gestión de administradores:** (a) si quien registra define la contraseña, ¿cómo se comunica de forma segura y debe cambiarse en el primer ingreso? *Recomendación:* exigir el cambio en el primer ingreso o enviar un enlace de invitación para que el nuevo administrador defina su propia contraseña; (b) ¿se necesita una vista para listar a los administradores y desactivarlos?; (c) ¿cómo se crea el primer administrador? *Recomendación:* durante el despliegue; (d) ¿se permite el ingreso con la cuenta institucional de Microsoft? | Parcial: (a) **resuelto**: quien registra define nombre y correo, la cuenta queda en *New* y el nuevo administrador define su contraseña con un enlace de activación (V-13, V-36); (b) **resuelto**: los administradores registran, listan, editan y eliminan a los demás administradores (V-33 a V-35). **Falta:** (c) y (d); la vigencia del enlace de activación (propuesto: 24 horas); qué datos de otro administrador se pueden editar (propuesto: nombre y correo); si un administrador puede eliminar a otro sin restricciones. |
| `PA-49` | **Conexión con MQTT:** con MQTT las estaciones se conectan al bróker y la plataforma se suscribe a los tópicos; la plataforma no se conecta directamente a las estaciones. ¿Qué significan entonces el *timeout* y los *reintentos de conexión* de V-15 y la alerta *Reintentos de conexión*? *Recomendación:* reemplazarlos por un *tiempo máximo sin mensajes* por estación y por el aviso de desconexión del bróker (mensaje *Last Will*). | **Resuelto:** se acepta la recomendación. El *timeout* y los reintentos se reemplazan por el tiempo máximo sin mensajes de cada estación (V-15, `RF-142`; se retira `RF-143`), y la alerta *Reintentos de conexión* por *Desconexión con el bróker*, que se genera cuando se detecta una desconexión o desincronización entre la API y el bróker (sección 2.6). |
| `PA-50` | **Modelo del mensaje MQTT:** validar con Ingeniería Electrónica el modelo de envío de variables: estructura del JSON y de los tópicos, nivel de QoS, quién pone la marca de tiempo, códigos de error por sensor (`PA-32`) y reenvío de datos guardados durante una desconexión. ¿El formato es único para todas las estaciones o puede variar por estación? ¿Los mensajes rechazados se descartan o se guardan para revisión? | Parcial: la marca de tiempo la pone la estación (`RN-33`); no hay reenvío (pérdida aceptada, `RN-34`); cada estación puede tener su propio formato (`RN-29`, V-27). Falta la estructura concreta del JSON con un mensaje de ejemplo, los tópicos, el QoS, la matriz de códigos de error y qué se hace con los mensajes rechazados. |
| `PA-51` | **Estaciones LoRa:** ¿cómo se identifica la estación de origen en los mensajes que retransmite la estación WiFi? ¿Con qué credenciales publica la estación de enlace los datos de las estaciones LoRa? Si LoRa se usa como respaldo, ¿una misma estación puede alternar entre WiFi y LoRa? ¿Se debe mostrar qué red usó cada lectura? | **Aplazado:** las estaciones LoRa quedan para fases futuras; la primera fase son 5 estaciones WiFi. |
| `PA-52` | **Requisitos de analítica.** | Parcial: requisitos recibidos y aclarados (sección 2.8; `PCAL-xx`): solo administradores, modelos de la lista de 2.8, gráfica por modelo con R², sin valor de aceptación, entrenamiento en la plataforma. **Falta:** (a) los modelos mínimos de la primera versión (`PCAL-09`), si se incluye ElasticNet y si XGBoost sale de la lista; (b) si la estimación de variables sigue en el alcance (V-31); (c) cómo reportar el efecto de cada variable (valor p, que solo aplica a los modelos lineales); (d) si el R² se calcula sobre un conjunto de prueba separado por bloques de tiempo. |
| `PA-53` | **Aplicación de los modelos.** | **Resuelto:** los modelos no se aplican a los datos de la plataforma; el público ve solo los datos crudos y los calibrados solo sirven para comparar modelos (`RN-35`). |
| `PA-54` | **Exportación de modelos:** formato del modelo exportado y metadatos que debe incluir. ¿Para qué se usará el modelo exportado? | Parcial: el administrador descarga el modelo elegido para luego calibrar las estaciones, en formato `.keras` (`PCAL-01`, `PCAL-14`). **Falta:** el formato de los modelos que no son redes neuronales (`.keras` solo sirve para modelos de Keras; por ejemplo, `.joblib` u ONNX); qué significa "subirlo y calibrar las estaciones" y si exige un formato compatible con el microcontrolador; qué metadatos acompañan al modelo (variables de entrada y unidades, periodo de datos). |
| `PA-55` | **Fuentes externas de referencia.** | **Descartado:** no se usan fuentes externas ni se marca ninguna estación como referencia; se usa cualquier estación registrada, sin exigir que esté en el mismo lugar (`RN-36`). Se retira V-29. |
| `PA-56` | **Colores del mapa por umbrales** (`RN-37`, `PC-22`): (a) ¿se evalúa la última lectura o un promedio (por ejemplo, horario o de 24 h)?; (b) ¿la estación toma el color del nivel más alto entre sus variables?; (c) ¿qué color tiene una estación sin variables en alerta, y mientras no haya umbrales definidos?; (d) ¿una estación con un hueco corto (entre 60 s y 10 minutos) mantiene su color?; (e) ¿cómo se muestra una estación con un sensor sin datos y los demás funcionando?; (f) ¿qué color tiene en el mapa una estación *Inactive*, que antes se mostraba con el gris `#6B7280` de su estado y ahora podría confundirse con el gris de "sin datos" (relacionado con `PA-35`)?; (g) ¿el cuadro emergente del mapa sigue mostrando al visitante el estado de funcionamiento de la estación y de sus sensores? | Abierto |

---

## 8. Control de cambios

Desde la versión 4, el control de cambios se registra en [gestion_cambios.md](gestion_cambios.md). Este apartado conserva el historial de las versiones anteriores.

**Versión 3** — respuestas del stakeholder a los puntos abiertos:

- **Resueltos:** `PA-02`, `PA-03`, `PA-12`, `PA-13`, `PA-14`, `PA-16`, `PA-17`, `PA-18`, `PA-19`, `PA-20`, `PA-21`, `PA-22`, `PA-23`, `PA-26`, `PA-27` y `PA-29`. Siguen abiertos `PA-01`, `PA-04`, `PA-15`, `PA-24`, `PA-25` y `PA-28`; `PA-09` sigue parcial.
- **Catálogos:** se definieron los estados de estaciones y sensores con su prioridad de cálculo; los estados de usuario incluyen la activación de administradores y la anonimización; se agregaron cuatro tipos de alerta informativa (habilitación, deshabilitación, visualización y la definición de anomalía) y la evidencia por tipo; se agregó el catálogo de PQRS (sección 2.8, propuesto).
- **Componentes nuevos:** `C-09` Barra lateral de navegación (reemplaza la barra superior) y `C-10` Pie de página legal. Se agregaron los requisitos globales `RF-G01` a `RF-G06`.
- **Vistas nuevas:** V-20 Establecer contraseña (primer ingreso), V-21 Restablecer contraseña con sesión iniciada, V-22 PQRS, V-23 Creación de PQRS y V-24 Detalle de PQRS.
- **Vistas modificadas:** V-01 (bloqueo de cuentas *Pending*, redirección del primer ingreso), V-02 (el administrador tampoco ve las ocultas en el mapa), V-03 (acceso a estaciones eliminadas, indicador *Oculta*), V-05 (pasa de ventana emergente a vista; se elimina del historial al vencer), V-06 y V-14 (ubicación por dirección, coordenadas o mapa), V-07 (flujo *Mostrar* → *Habilitar*, alertas), V-09 (sin teléfono, aceptación de política), V-10 (mensaje neutro, reenvío, cuentas de Google/Microsoft), V-11 y V-12 (sin teléfono para usuarios generales; el cambio de contraseña pasa a V-21), V-13 (sin contraseña, correos de activación y credenciales), V-16 y V-17 (marca de no vista, filtro de vistas, contador, evidencias por tipo), V-19 (anonimización).
- **Reglas de negocio:** se actualizaron `RN-01`, `RN-03`, `RN-06`, `RN-07`, `RN-09` y `RN-12`, y se agregaron `RN-14` a `RN-26`.
- **Requisitos no funcionales:** se actualizaron `RNF-01`, `RNF-06` y `RNF-07`, y se agregó `RNF-08`.
- **Puntos abiertos nuevos:** `PA-30` a `PA-47`, derivados de inconsistencias o vacíos detectados en las respuestas.

**Versión 2** — actualización con base en `requerimientos.md`:

- **Catálogos:** se agregaron los estados de usuario, el estado de estación *Hidden* (con transiciones y tabla de visibilidad) y los estados y tipos de alerta. Los estados de sensor cambiaron de *Working/Warning/Error* a *Ok/Warning/Offline*.
- **Vistas modificadas:** V-01 (enlaces a registro y recuperación), V-03 (botón *Crear estación*, editar y eliminar en las tarjetas, visibilidad de estaciones ocultas), V-04 (descripción en el encabezado), V-05 (formatos CSV/JSON, vigencia de un mes), V-06 (campos editables), V-07 (flujo *Hidden* / *Mostrar*), V-08 (motivo obligatorio y contraseña).
- **Vistas nuevas:** V-09 Registro, V-10 Recuperación de contraseña, V-11 Perfil, V-12 Edición de perfil, V-13 Registrar administrador, V-14 Crear estación, V-15 Configuración, V-16 Alertas, V-17 Detalle de alerta, V-18 Estaciones eliminadas, V-19 Eliminación de cuenta.
- **Componentes nuevos:** `C-06` Verificación por contraseña, `C-07` Formulario de contraseña nueva, `C-08` Tarjeta de alerta.
- **Reglas de negocio:** se actualizaron `RN-01`, `RN-03`, `RN-04` y `RN-06`, y se agregaron `RN-07` a `RN-13`.
- **Requisitos no funcionales:** se actualizaron `RNF-03` y `RNF-06`, y se agregó `RNF-07`.
- **Puntos abiertos:** se resolvieron `PA-05`, `PA-06`, `PA-07`, `PA-08`, `PA-10` y `PA-11`; `PA-09` quedó parcial; se agregaron `PA-12` a `PA-29`.
