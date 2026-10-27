# Decisiones

1. Anonimización de datos

La anonimización de datos a la hora de eliminar administradores se va a llevar de la siguiente manera:

| Campo | Decisión | Ejemplo |
|-------|----------|---------|
|   ID  | Este no cambia ni se elimina para mantener la consistencia de los datos, historial y trazabilidad | No aplica |
| Nombre| Se reemplaza por un identificador único aleatorio predecedido por el identificador *'user-'* para evitar que sea reconocible | user-AF5739B |
|Apellidos| Se elimina y se deja vacio el campo | No aplica |
|Correo| Se elimina y se deja vacio el campo | No aplica |
|Contraseña| Se elimina y se deja vacio el campo | No aplica |

Al eliminar el estado del usuario pasa a 'Deleted'.

Esta decision se basa en base al documento presentado por el gobierno 'Guia_de_Anonimizacion-min.pdf'.

Normas principales que utiliza la guía

|Norma |¿Qué establece y por qué es relevante?|
|------|--------------------------------------|
|Constitución Política de Colombia de 1991, artículo 15| Protege la intimidad, el buen nombre y el derecho al habeas data. |
|Ley Estatutaria 1581 de 2012| Es la norma central sobre protección de datos personales: establece principios, derechos de los titulares y obligaciones para quienes tratan esos datos.|
|Decreto 1377 de 2013 |Reglamentó parcialmente la Ley 1581 de 2012.|
|Decreto 1074 de 2015 |Decreto Único Reglamentario del sector Comercio, Industria y Turismo; incorpora la reglamentación relacionada con la protección de datos personales.|
|Ley 1266 de 2008 |Regula el habeas data en el ámbito financiero, crediticio, comercial, de servicios y otros supuestos contemplados en su ámbito de aplicación.|
|Ley 1273 de 2009 |Introduce disposiciones penales relacionadas con la protección de la información y los datos.|

La guía presenta estas normas en su apartado de marco normativo colombiano. Puedes consultar también el texto oficial de la Ley 1581 de 2012 en Función Pública


