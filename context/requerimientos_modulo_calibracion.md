# Módulo de calibración de sensores con aprendizaje automático

**Proyecto:** Plataforma web para la calibración de estaciones de monitoreo ambiental mediante modelos de aprendizaje automático (Machine Learning, ML)

**Documento:** Especificación detallada de lo que se espera y se requiere construir, basada en el documento fuente *"regresión"*

---

## 1. Propósito de este documento

Este documento explica, de forma detallada y sin código, qué pide el documento fuente para la plataforma web: el problema que se quiere resolver, la herramienta que se debe construir, qué debe poder hacer el usuario, qué debe producir el sistema y con qué criterios se considera cumplido.

A lo largo del texto se distinguen dos tipos de contenido:

- **Requerimiento del documento fuente:** lo que el documento pide de manera explícita. Es obligatorio.
- **Interpretación o recomendación:** aclaraciones, supuestos y decisiones de diseño que el documento no menciona pero que hacen falta para que lo pedido funcione bien. Se marcan siempre como tales para que el equipo y el docente puedan validarlas.

---

## 2. Contexto del problema

### 2.1 Qué es calibrar un sensor

Las estaciones de monitoreo ambiental usan sensores de bajo costo para medir variables como material particulado (PM2.5), temperatura, humedad, presión o concentración de CO₂. Estos sensores no siempre miden el valor real: pueden subestimar o sobrestimar la variable de forma sistemática, y su error puede depender de las condiciones del ambiente.

Para corregirlos se instala, junto al sensor de bajo costo, un **sensor de referencia**: un equipo ya bien calibrado cuyas lecturas se consideran el valor "real". Si se tienen lecturas simultáneas de ambos sensores, se puede entrenar un modelo que aprenda a transformar la lectura del sensor de bajo costo en el valor que habría dado el sensor de referencia. A ese proceso se le llama **calibración**.

### 2.2 El caso que ilustra el documento

El documento usa como caso de ejemplo un sensor de PM2.5 llamado **hibou_PM2.5**, cuyas lecturas se comparan contra un sensor de referencia de PM2.5. Ambas magnitudes se expresan en microgramos por metro cúbico (µg/m³).

La figura del documento contiene los siguientes elementos:

| Elemento de la figura | Qué representa |
|---|---|
| Puntos amarillos, etiquetados "RAW (hibou_PM2.5)" | Cada punto es un par de mediciones tomadas al mismo tiempo: la lectura cruda del sensor de bajo costo frente al valor de referencia. |
| Triángulos azules, etiquetados "Calibrated" | Cada triángulo es la lectura del sensor después de pasar por el modelo de ML, frente al valor de referencia. |
| Línea punteada diagonal | La recta identidad, g(x) = x. Es el lugar donde caería un sensor perfecto. |
| Línea continua naranja, "Raw fit" | Recta de ajuste de los datos crudos, con una franja sombreada de incertidumbre. |
| Línea discontinua azul, "Cali fit" | Recta de ajuste de los datos calibrados, con una franja sombreada de incertidumbre. |

La figura es un ejemplo ilustrativo para entender el tipo de resultado esperado; no es una especificación de valores, colores ni estilos. Lo que ilustra es la idea central: los datos crudos se alejan de la diagonal y, después de la calibración, los datos se acercan a ella.

### 2.3 Qué debe hacer el modelo

El documento define el modelo de ML como una función:

> f(x) = modelo(x)

El modelo recibe la lectura del sensor que se quiere calibrar y devuelve un valor corregido. La meta es que, cuando el sensor reporta un valor equivocado, el modelo sea capaz de identificar cuál debió ser el valor real según el sensor de referencia.

Como el modelo también recibe variables auxiliares (sección 2.4), la forma completa es **f(x, z₁, …, zₙ)**, donde x es la lectura del sensor a calibrar y z₁, …, zₙ son las variables auxiliares. Ver la sección 9.3.

El criterio de éxito visual es que **los puntos calibrados queden muy cerca de la diagonal g(x) = x**. Esa diagonal significa: "cuando el sensor calibrado dice que la concentración es x, la concentración realmente era x". En este documento se usa **g** para la diagonal y **f** para el modelo, para no confundirlos.

### 2.4 Por qué el modelo debe recibir más de una variable

El documento aclara que la lectura directa del sensor puede no ser suficiente para corregirlo. El error de un sensor suele estar relacionado con otras condiciones ambientales. Por ejemplo, una mala medición de temperatura puede estar relacionada con la humedad, el CO₂ o la presión.

Por eso, el modelo completo no debe recibir solo la lectura del sensor que se quiere calibrar, sino también **todas las demás variables que el usuario considere relevantes**. Estas variables adicionales se llamarán en este documento **variables auxiliares** (también se conocen como covariables).

---

## 3. Qué se debe construir

Se debe desarrollar, dentro de la plataforma web, una **herramienta de calibración** que permita al usuario:

1. Elegir qué variable quiere calibrar.
2. Elegir qué variable contiene el valor de referencia.
3. Elegir qué otras mediciones se usarán como entradas adicionales del modelo.
4. Elegir uno o varios modelos de ML.
5. Obtener, por cada modelo elegido, una gráfica comparativa con su coeficiente de determinación R².

Las variables entre las que el usuario elige son las mediciones que la plataforma ya está guardando. La herramienta no captura datos nuevos: trabaja sobre los datos almacenados de las estaciones (ver la sección 9.7 sobre el origen de los datos de referencia).

---

## 4. Requerimientos funcionales

### RF-01. Selección de la variable a calibrar

- **Requerimiento del documento fuente:** el usuario debe poder seleccionar la variable que desea calibrar, entre las mediciones que se están guardando en la plataforma.
- Es una sola variable por cada ejecución de la calibración.
- Es la variable cuyo error se quiere corregir.
- **Interpretación:** conviene que el selector muestre, para cada variable, la estación a la que pertenece, su unidad de medida y el rango de fechas con datos disponibles, para que el usuario elija con información suficiente.

### RF-02. Selección de la variable de referencia

- **Requerimiento del documento fuente:** el usuario debe poder seleccionar la variable que contiene el valor de referencia, también entre las mediciones que se están guardando.
- Es una sola variable por ejecución.
- Representa el valor considerado verdadero y es el objetivo que el modelo debe aprender a reproducir.
- **Interpretación:**
  - La variable de referencia no puede ser la misma variable que se va a calibrar. El sistema debe impedir esa selección.
  - La variable de referencia debe medir la misma magnitud física que la variable a calibrar y estar en la misma unidad; de lo contrario la comparación con la diagonal g(x) = x no tiene sentido. Lo recomendable es que el sistema advierta al usuario si las unidades no coinciden.

### RF-03. Selección de variables auxiliares

- **Requerimiento del documento fuente:** el usuario debe poder seleccionar todas las demás mediciones que considere relevantes, y el modelo debe recibirlas como entradas adicionales junto a la lectura del sensor a calibrar.
- La selección es múltiple y libre: el usuario decide cuántas y cuáles.
- **Interpretación:**
  - Debe permitirse no seleccionar ninguna variable auxiliar. En ese caso el modelo se entrena solo con la lectura del sensor a calibrar.
  - Ni la variable a calibrar ni la variable de referencia deben aparecer disponibles como auxiliares. En particular, si la variable de referencia se usara como entrada del modelo, el modelo "haría trampa" y el resultado sería inválido.
  - Lo mismo aplica, aunque de forma menos evidente, a otras variables medidas por el equipo de referencia. Ver la sección 9.9.

### RF-04. Selección de modelos de ML

- **Requerimiento del documento fuente:** el usuario debe poder seleccionar todos los modelos de ML que desee usar. La selección es múltiple.
- El documento menciona como posibles modelos:
  - Regresión lineal
  - Naive Bayes
  - Regresión logística
  - Random Forest
  - Perceptrón multicapa (red neuronal)
  - XGBoost
- La lista termina en "etc.", por lo que es abierta: el sistema debe estar preparado para incorporar más modelos.
- Todos los modelos seleccionados se entrenan con las mismas variables (la variable a calibrar más las auxiliares) y el mismo objetivo (la variable de referencia), para que sus resultados sean comparables entre sí.
- **Interpretación:** ver la observación sobre Naive Bayes y regresión logística en la sección 9.1, y sobre la configuración de los modelos en la sección 9.11.

### RF-05. Entrenamiento y aplicación de los modelos

- Por cada modelo seleccionado, el sistema debe:
  1. Entrenarlo para que aprenda la relación entre las variables de entrada y la variable de referencia.
  2. Aplicarlo a las lecturas del sensor para obtener los **valores calibrados**.
- **Interpretación:** el documento no indica sobre qué datos se debe evaluar el modelo, ni qué se hace con el modelo después de evaluarlo. Ver la sección 6.4 y la sección 9.8.

### RF-06. Generación de una gráfica por modelo

- **Requerimiento del documento fuente:** la herramienta debe generar **tantas gráficas como modelos haya seleccionado el usuario**. Si eligió cinco modelos, deben aparecer cinco gráficas, una por modelo.
- Cada gráfica debe contener, como mínimo, los cuatro elementos que exige el documento:

| # | Elemento obligatorio | Descripción |
|---|---|---|
| 1 | Datos medidos vs. referencia | Los valores crudos del sensor frente a los valores de referencia. |
| 2 | Datos calibrados vs. referencia | Los valores que produce el modelo frente a los valores de referencia. |
| 3 | Línea de referencia identidad | La recta g(x) = x, que representa la calibración perfecta. |
| 4 | Valor de R² | El coeficiente de determinación correspondiente a ese modelo. |

- Las dos series de puntos deben distinguirse con marcadores diferentes (el documento habla de "dos tipos de marcadores distintos").
- **Interpretación:**
  - Cada gráfica debe indicar claramente a qué modelo corresponde, con un título o etiqueta.
  - Cada gráfica debe tener leyenda, nombres de los ejes y unidades.
  - Se adopta como convención la **referencia en el eje x** y las lecturas del sensor (crudas y calibradas) en el **eje y**, aplicada igual en todas las gráficas.
  - Ambos ejes deben usar la misma escala y el mismo rango, para que la diagonal g(x) = x se vea realmente a 45 grados y la comparación visual sea correcta.
  - Es opcional, pero deseable, incluir una recta de ajuste para los datos crudos y otra para los calibrados. Su pendiente ayuda a interpretar el resultado (ver la sección 9.4).

### RF-07. Cálculo y presentación de R²

- **Requerimiento del documento fuente:** cada gráfica debe mostrar "su respectivo valor de R²".
- R² mide qué tan bien los valores calibrados reproducen los valores de referencia. Un valor cercano a 1 indica que los puntos calibrados están muy cerca de la diagonal.
- **Interpretación:**
  - El valor de R² más útil para el usuario es el que compara los **valores calibrados** contra la **referencia**, porque es el que indica qué tan buena es la calibración de ese modelo.
  - El R² debe calcularse **contra la diagonal g(x) = x** (coeficiente de determinación), y no como el cuadrado de la correlación ni como el R² de una recta de ajuste. La diferencia es importante y se explica en la sección 9.2.
  - Es recomendable mostrar también el R² de los **datos crudos** contra la referencia, calculado de la misma forma, para que el usuario vea cuánto mejoró el sensor con cada modelo. Si se muestran ambos, deben estar claramente rotulados para no confundirlos.
  - Debe quedar documentado en la plataforma cómo se calcula el R² que se muestra y sobre qué datos (entrenamiento o prueba).

---

## 5. Flujo de uso esperado

Este es el recorrido que un usuario haría con la herramienta, descrito paso a paso:

1. El usuario entra al módulo de calibración de la plataforma.
2. Elige la variable que quiere calibrar.
3. Elige la variable de referencia.
4. Marca las variables auxiliares que considere relevantes, o no marca ninguna.
5. Marca uno o varios modelos de ML.
6. (Recomendado) Define el periodo de tiempo de los datos que se usarán.
7. Ejecuta la calibración.
8. El sistema valida que la selección sea coherente y que haya datos suficientes.
9. El sistema entrena cada modelo y calcula los valores calibrados.
10. El sistema muestra una gráfica por modelo, cada una con los datos crudos, los datos calibrados, la línea g(x) = x y el valor de R².
11. El usuario compara las gráficas y los valores de R² para decidir qué modelo calibra mejor el sensor.

---

## 6. Requerimientos sobre los datos

El documento fuente no detalla cómo deben prepararse los datos, pero lo que pide solo funciona si los datos cumplen ciertas condiciones. Todo este apartado es **interpretación** necesaria para la implementación.

### 6.1 Coincidencia en el tiempo

Para que un modelo aprenda a calibrar, cada lectura del sensor debe compararse con la lectura de referencia **tomada en el mismo momento**. Los sensores rara vez registran en el mismo segundo exacto, así que la plataforma debe:

- Emparejar las mediciones de la variable a calibrar, la variable de referencia y las variables auxiliares por fecha y hora.
- Definir un criterio de emparejamiento, por ejemplo agrupar todas las mediciones en intervalos de tiempo iguales y usar el promedio de cada intervalo. El intervalo no puede ser más fino que la resolución del equipo de referencia, que con frecuencia es horaria.
- Unificar la zona horaria y la convención de marca de tiempo (inicio o fin del intervalo) de todas las fuentes antes de emparejar.
- Usar solo los instantes en los que existen datos para todas las variables seleccionadas.

### 6.2 Ubicación

El sensor a calibrar y el sensor de referencia deben estar instalados en el mismo lugar, o lo bastante cerca como para medir el mismo ambiente. La herramienta debería dejar claro de qué estación proviene cada variable.

### 6.3 Calidad de los datos

- Manejo de datos faltantes: los registros con valores vacíos en alguna de las variables seleccionadas deben excluirse o tratarse de forma explícita.
- Manejo de valores atípicos: lecturas fuera de rango físico deben identificarse, pero con criterios configurables y no por eliminación automática (ver la sección 9.10).
- Cantidad mínima de datos: si después de emparejar y limpiar quedan muy pocos registros, el sistema debe avisar al usuario en lugar de entrenar un modelo poco confiable.

### 6.4 Separación entre entrenamiento y evaluación

Si un modelo se evalúa con los mismos datos con los que se entrenó, su R² puede verse mucho mejor de lo que realmente es, sobre todo en modelos flexibles como Random Forest, XGBoost o el perceptrón multicapa. Para que la comparación entre modelos sea justa, se recomienda:

- Dividir los datos emparejados en un conjunto de entrenamiento y otro de prueba **por bloques de tiempo** (por ejemplo, un periodo continuo para entrenar y el periodo siguiente para probar), y no al azar registro por registro. La razón se explica en la sección 9.6.
- Entrenar cada modelo solo con el conjunto de entrenamiento.
- Calcular el R² y graficar los puntos calibrados usando el conjunto de prueba, o indicar claramente en la gráfica qué conjunto se está mostrando.
- Usar la misma división de datos para todos los modelos seleccionados en una misma ejecución.

---

## 7. Requerimientos no funcionales

Todos los puntos de este apartado son **recomendaciones** derivadas de que la herramienta forma parte de una plataforma web.

| Aspecto | Qué se espera |
|---|---|
| Usabilidad | Los selectores deben ser claros, mostrar nombres comprensibles de las variables y sus unidades, e impedir combinaciones inválidas antes de ejecutar. |
| Retroalimentación | Mientras los modelos se entrenan, el usuario debe ver que el proceso está en curso. Si un modelo falla, el sistema debe informar cuál falló y por qué, sin impedir que se muestren los demás. |
| Rendimiento | Entrenar varios modelos puede tardar. El sistema no debe bloquear la interfaz mientras tanto. |
| Comparabilidad | Todas las gráficas de una misma ejecución deben compartir formato, escala y estilo, para poder compararlas lado a lado. |
| Extensibilidad | Agregar un nuevo modelo de ML a la lista debe ser posible sin rediseñar la herramienta, ya que el documento deja la lista abierta. |
| Trazabilidad | Es conveniente registrar qué variables, modelos, configuración, semilla aleatoria, periodo de datos y resultados se usaron en cada ejecución, para poder repetirla o consultarla después. |
| Visualización | Las gráficas deben ser legibles en pantalla, con leyenda, ejes rotulados y unidades. Es deseable poder descargarlas. |

---

## 8. Criterios de aceptación

La herramienta cumple con lo que pide el documento fuente cuando:

1. El usuario puede elegir exactamente una variable a calibrar entre las mediciones almacenadas.
2. El usuario puede elegir exactamente una variable de referencia entre las mediciones almacenadas.
3. El usuario puede elegir cero, una o varias variables auxiliares entre las mediciones almacenadas.
4. El usuario puede elegir uno o varios modelos de ML de una lista que incluye, como mínimo, los modelos mencionados en el documento (o sus equivalentes de regresión, según se defina con el docente; ver sección 9.1).
5. Los modelos se entrenan usando como entradas la variable a calibrar y las variables auxiliares, y como objetivo la variable de referencia.
6. El número de gráficas generadas es igual al número de modelos seleccionados.
7. Cada gráfica muestra los datos medidos frente a la referencia.
8. Cada gráfica muestra los datos calibrados frente a la referencia, con un marcador distinto al de los datos medidos.
9. Cada gráfica muestra la línea de referencia g(x) = x.
10. Cada gráfica muestra su valor de R².
11. Cada gráfica identifica a qué modelo corresponde.

Criterios adicionales recomendados:

12. El sistema impide que la variable de referencia se use también como variable a calibrar o como variable auxiliar.
13. El sistema avisa cuando no hay datos suficientes o cuando las unidades de la variable a calibrar y la de referencia no coinciden.
14. Los valores de R² se calculan de forma consistente para todos los modelos, contra la diagonal g(x) = x, y se indica sobre qué datos se calcularon.

---

## 9. Inconsistencias, conflictos y puntos para aclarar con el docente

Esta sección reúne los puntos en los que lo que pide el documento fuente entra en conflicto consigo mismo, con la teoría de los modelos o con cómo funcionan en la práctica los sensores y los datos. Cada punto incluye una recomendación para resolverlo.

### 9.1 Naive Bayes y regresión logística son modelos de clasificación

La calibración es un problema de **regresión**: el modelo debe devolver un número continuo (una concentración, una temperatura). Sin embargo, Naive Bayes y la regresión logística, en su forma habitual, son modelos de **clasificación**: devuelven una categoría, no un valor continuo. A pesar de su nombre, la regresión logística no sirve para predecir un valor numérico.

Para incluirlos habría que decidir una de estas opciones:

- Reemplazarlos por modelos de regresión equivalentes o cercanos.
- Adaptarlos convirtiendo el problema en clasificación por rangos de valores, lo que haría que los resultados no fueran directamente comparables con los demás modelos y que el valor calibrado solo pudiera tomar unos pocos valores fijos.
- Excluirlos de la lista.

**Recomendación:** consultar con el docente cuál de estas opciones espera. El resto de modelos mencionados (regresión lineal, Random Forest, perceptrón multicapa y XGBoost) tienen versiones de regresión y se ajustan directamente a lo que pide el documento.

### 9.2 "R²" puede significar dos cosas distintas, y solo una mide la calibración

El documento pide "su respectivo valor de R²" en cada gráfica, sin precisar si se refiere al R² de los datos calibrados, al de los datos crudos o a ambos, ni si debe calcularse sobre los datos de entrenamiento o sobre datos no vistos por el modelo.

Además, el término "R²" se usa en la práctica para dos cálculos distintos que dan resultados muy diferentes:

- **Coeficiente de determinación contra la diagonal:** compara cada valor (crudo o calibrado) directamente con su valor de referencia. Vale 1 solo si los puntos caen exactamente sobre g(x) = x, y puede ser **negativo** si los valores están muy lejos de la diagonal, como suele pasar con los datos crudos de un sensor que subestima.
- **Cuadrado de la correlación, o R² de una recta de ajuste:** solo mide si los puntos forman una línea recta, sin importar cuál. Un sensor que siempre mide exactamente la tercera parte del valor real tendría un R² de 1 con este cálculo, aunque esté muy lejos de la diagonal.

El criterio de éxito del documento es la cercanía a la diagonal, así que solo el primer cálculo es coherente con lo que se pide. Usar el segundo haría que los datos crudos parecieran buenos aunque no lo sean.

Por otra parte, R² por sí solo no muestra si el sensor tiene un error sistemático (sesgo) ni de qué tamaño es el error en µg/m³, y depende del rango de los datos: el mismo modelo puede dar un R² alto en un periodo con mucha variación y uno bajo en un periodo estable. También conviene recordar que el equipo de referencia tiene su propia incertidumbre, por lo que un R² igual a 1 no es alcanzable en la práctica.

**Recomendación:** calcular el R² contra la diagonal, sobre el conjunto de prueba, para los datos crudos y los calibrados. Como complemento, mostrar el error cuadrático medio (RMSE), el error absoluto medio (MAE) y el sesgo medio, que se expresan en las unidades de la variable. Confirmar con el docente si estas métricas adicionales son deseables.

### 9.3 El modelo no es una función solo de la lectura del sensor

El documento define el modelo como f(x) = modelo(x), con una sola entrada, pero luego pide que reciba también las variables auxiliares. Las dos definiciones no son compatibles: el modelo real es f(x, z₁, …, zₙ).

Esto tiene consecuencias prácticas:

- Una misma lectura cruda puede producir valores calibrados distintos según las condiciones del ambiente. Por eso no existe una única "curva de calibración" que se pueda dibujar en función de la lectura cruda.
- La gráfica de datos calibrados frente a la referencia sigue siendo válida, porque compara resultados y no depende del número de entradas.
- Para aplicar el modelo en el futuro se necesitan **todas** las variables con las que se entrenó, no solo la lectura del sensor (ver las secciones 9.8 y 9.9).

**Recomendación:** documentar el modelo como f(x, z₁, …, zₙ) y no proponer en la interfaz una "curva de calibración" en función de x cuando haya variables auxiliares.

### 9.4 Los puntos calibrados no quedan exactamente sobre la diagonal, aunque el modelo sea bueno

El documento plantea como meta que los puntos calibrados queden "muy cerca de la diagonal". En la práctica hay dos efectos que lo impiden, incluso con un buen modelo:

- **Encogimiento hacia la media:** los modelos de regresión aprenden el valor promedio esperado de la referencia para cada combinación de entradas. Cuando las entradas tienen ruido, el modelo tiende a sobrestimar los valores bajos y a subestimar los picos. Al graficar con la referencia en el eje x, la nube de puntos calibrados queda con una pendiente menor que la de la diagonal. Esto no es un error de la implementación, sino una propiedad de los modelos.
- **Incapacidad de extrapolar:** Random Forest y XGBoost no pueden producir valores fuera del rango que vieron durante el entrenamiento. Si en el entrenamiento la referencia nunca superó, por ejemplo, 40 µg/m³, el modelo nunca va a predecir más que eso, aunque ocurra un episodio de contaminación mucho mayor. La regresión lineal y el perceptrón multicapa sí pueden extrapolar, aunque sin garantía de acierto.

**Recomendación:** interpretar la cercanía a la diagonal junto con la pendiente de la recta de ajuste de los datos calibrados, advertir al usuario cuando los datos de prueba salgan del rango de los de entrenamiento, y procurar que el periodo de entrenamiento incluya valores altos y bajos.

### 9.5 Un R² calibrado muy alto puede ser sospechoso

Si los datos crudos casi no tienen relación con la referencia (el sensor prácticamente no responde a los cambios reales), la información necesaria para recuperar el valor real no está en la lectura del sensor. En ese caso, un R² calibrado muy alto solo puede venir de:

- Las variables auxiliares: el modelo estaría prediciendo la referencia a partir de otras condiciones (por ejemplo, la humedad o la hora del día), no corrigiendo el sensor.
- Sobreajuste: el modelo memorizó los datos y se está evaluando sobre los mismos datos de entrenamiento.
- Fuga de información: alguna variable auxiliar contiene, de forma directa o indirecta, el valor de referencia (sección 9.9).

**Recomendación:** mostrar siempre el R² crudo junto al calibrado y, de forma opcional, entrenar un modelo solo con las variables auxiliares como punto de comparación. Si ese modelo da un resultado parecido al del modelo completo, el sensor está aportando poco.

### 9.6 La división aleatoria de los datos infla el R² en series de tiempo

Las mediciones ambientales consecutivas son casi iguales entre sí: la concentración de las 10:00 se parece mucho a la de las 10:05. Si los datos se dividen al azar entre entrenamiento y prueba, cada punto de prueba tiene "vecinos" casi idénticos en el entrenamiento, y el modelo parece predecir bien aunque solo esté recordando. Esto contradice el propósito de separar los datos y afecta sobre todo a Random Forest, XGBoost y el perceptrón multicapa.

A esto se suma que la relación entre el sensor y la referencia cambia con el tiempo: los sensores de bajo costo se degradan con el uso, y las condiciones cambian entre temporadas (por ejemplo, entre temporada seca y de lluvias). Un modelo entrenado en un periodo puede no servir en otro.

**Recomendación:** dividir los datos por bloques de tiempo, como se indica en la sección 6.4, y registrar el periodo de entrenamiento de cada modelo para saber hasta cuándo su calibración es válida.

### 9.7 Los datos de referencia pueden no estar en la plataforma

El documento pide que la variable de referencia se elija "entre las mediciones que se están guardando". En la práctica, los equipos de referencia suelen pertenecer a otra entidad (por ejemplo, una red oficial de monitoreo de calidad del aire) y sus datos no llegan automáticamente a la plataforma. Además, esos equipos suelen reportar promedios horarios, mientras que los sensores de bajo costo registran cada pocos minutos, y pueden usar otra zona horaria u otra convención de marca de tiempo.

**Recomendación:** confirmar con el docente de dónde vendrán los datos de referencia. Si son externos, la plataforma necesita un mecanismo para cargarlos (por ejemplo, importación de archivos) y registrarlos como una estación más, con su ubicación y resolución temporal. El emparejamiento debe hacerse a la resolución del equipo de referencia (sección 6.1).

### 9.8 No está definido qué se hace con el modelo después de entrenarlo

El propósito del documento es que el modelo "identifique cuál debió ser el valor real" cuando el sensor reporta un valor equivocado. Eso implica aplicar el modelo a lecturas **nuevas**, en las que no hay referencia disponible. Sin embargo, lo único que el documento pide producir son gráficas comparativas. Si la plataforma no guarda el modelo elegido ni lo aplica a los datos posteriores, los datos almacenados nunca quedan corregidos.

**Recomendación:** aclarar con el docente si el alcance es solo comparar modelos o también usar el mejor para calibrar los datos. En el segundo caso, la plataforma debería guardar el modelo y su configuración, aplicarlo a las nuevas lecturas, guardar los valores calibrados separados de los crudos e indicar con qué modelo se obtuvo cada valor.

### 9.9 Variables auxiliares que filtran la referencia o que no estarán disponibles

RF-03 impide usar la variable de referencia como auxiliar, pero el mismo problema aparece con otras variables medidas por el equipo de referencia. Por ejemplo, el PM10 o la temperatura reportados por la estación de referencia están muy relacionados con su PM2.5 y le darían al modelo información que el sensor de bajo costo no tiene.

Además, si el modelo se va a usar después para calibrar datos nuevos (sección 9.8), solo sirven las variables auxiliares que seguirán existiendo cuando el sensor ya no esté junto a la referencia, es decir, las que mide la propia estación de bajo costo.

**Recomendación:** mostrar la estación de origen de cada variable auxiliar y advertir al usuario cuando seleccione una que provenga de la misma estación que la variable de referencia.

### 9.10 Algunos valores que parecen inválidos son lecturas reales

La sección 6.3 propone identificar lecturas fuera de rango físico, como concentraciones negativas. En la práctica, algunos equipos de referencia reportan valores levemente negativos cuando la concentración es muy baja, como parte del ruido normal del instrumento; eliminarlos sesga el promedio hacia arriba. Del mismo modo, los sensores de bajo costo tienen errores grandes con humedad alta, y descartar esos casos eliminaría justamente las situaciones que el modelo debe aprender a corregir.

**Recomendación:** que los criterios de limpieza sean configurables y visibles para el usuario, que se informe cuántos registros se excluyeron y por qué, y que no se eliminen datos de forma automática sin dejar constancia.

### 9.11 La configuración de los modelos no está especificada

El documento no dice cómo configurar los modelos. Varios de ellos tienen parámetros que cambian mucho el resultado (número de árboles, profundidad, tamaño de la red neuronal, tasa de aprendizaje), y el perceptrón multicapa necesita que las variables se escalen antes de entrenar. Además, Random Forest, XGBoost y el perceptrón multicapa tienen un componente aleatorio, por lo que dos ejecuciones iguales pueden dar resultados distintos.

**Recomendación:** usar una configuración fija y documentada para cada modelo, escalar las variables cuando el modelo lo requiera, fijar una semilla aleatoria y registrarla en la trazabilidad. Si se decide ajustar los parámetros automáticamente, ese ajuste debe hacerse solo con los datos de entrenamiento, nunca con los de prueba.

### 9.12 Error de escritura en el documento

En una ocasión el documento escribe "hubou_PM2.5" en lugar de "hibou_PM2.5". Se trata del mismo sensor.

---

## 10. Glosario

| Término | Significado en este proyecto |
|---|---|
| Calibración | Proceso de corregir las lecturas de un sensor para que coincidan con las de un sensor de referencia. |
| Sensor de bajo costo | Sensor cuyas lecturas se quieren corregir. En el ejemplo del documento, hibou_PM2.5. |
| Sensor de referencia | Equipo ya bien calibrado cuyas lecturas se consideran el valor real. |
| Variable a calibrar | La medición almacenada que el usuario quiere corregir. |
| Variable de referencia | La medición almacenada que el usuario designa como valor real. |
| Variables auxiliares | Otras mediciones almacenadas que el usuario elige como entradas adicionales del modelo, como temperatura, humedad, presión o CO₂. |
| Datos crudos (RAW) | Lecturas del sensor sin ninguna corrección. |
| Datos calibrados | Lecturas del sensor después de pasar por el modelo de ML. |
| Modelo, f(x, z₁, …, zₙ) | Función que recibe la lectura del sensor y las variables auxiliares y devuelve el valor calibrado. |
| Línea identidad, g(x) = x | Diagonal de la gráfica donde la lectura coincide exactamente con la referencia. Representa la calibración perfecta. |
| R² | Coeficiente de determinación. Calculado contra la diagonal, indica qué tan bien un conjunto de valores reproduce los valores de referencia; cuanto más cerca de 1, mejor. Puede ser negativo. |
| RMSE, MAE, sesgo medio | Medidas del tamaño del error en las unidades de la variable: error cuadrático medio, error absoluto medio y diferencia promedio con signo. |
| Encogimiento hacia la media | Tendencia de los modelos de regresión a subestimar los valores altos y sobrestimar los bajos cuando las entradas tienen ruido. |
| PM2.5 | Material particulado con diámetro menor o igual a 2.5 micrómetros, medido en µg/m³. |
| Regresión | Tipo de problema de ML en el que el modelo predice un valor numérico continuo. |
| Clasificación | Tipo de problema de ML en el que el modelo predice una categoría. |
