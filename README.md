# Capturador de paquetes

Dll injectable que captura y modifica los paquetes que son enviados desde una aplicacion. Diseñado para aplicaciones de 32 bits.

El trafico capturado se imprime en consola.


## Detalles.

Este Dll inicializa un hook a las librerias wsock32.dll para interceptar los paquetes utilizados por la aplicacion

por defecto wsock32.dll es la libreria principal utilizada por Windows para la transmicion de paquete.
Toda aplicacion que utilize esta puerta de comunicacion y llame a esta libreria sera interceptada.

Este capturador de paquetes puede trabajar con la libreria winsock32.dll que es utilizada por  aplicaciones mas antiguas.

Se probo el interceptar paquetes que utilicen RECV los cuales son los que recibe la aplicacion y los paquetes SEND que envia la aplicacion

Permite capturar y modificar los paquetes. Tiene algunos filtros para capturar paquetes especificos definidos por el usuario y transforma los datos de data a hexadecimal para un trabajo mas simple.
