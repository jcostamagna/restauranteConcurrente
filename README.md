# restauranteConcurrente
Trabajo practico de Tecnicas de programacion Concurrente.

[Ver en pdf](2016-2C-PrimerProyecto.pdf)

Objetivo

El objetivo de este proyecto consiste en implementar la simulaci´on parcial del funcionamiento de un
restaurant.

Requerimientos Funcionales

Los requerimientos funcionales son los siguientes:

1. Al restaurant ingresan grupos de comensales que son recibidos en la puerta por los recepcionistas.
2. Cada recepcionista atiende a un grupo de personas a la vez y los ubica en una mesa disponible.
Si no hay m´as mesas disponibles, los hace pasar al living del restaurant, donde esperar´an a que
se desocupe una mesa.
3. Si todos los recepcionistas est´an ocupados, las personas esperan en la puerta del restaurant.
4. Cuando las personas est´an en la mesa, le ordenan al mozo un pedido de comida. El mozo toma
nota de lo solicitado y le solicita la comida al cocinero. Hay un s´olo cocinero que atiende a los
mozos de a uno por vez. El cocinero le entrega el pedido al mozo y ´este a los clientes de la mesa,
quienes luego comer´an.
5. Los clientes pueden repetir el ciclo de ordenar comida y comer, cuantas veces lo deseen.
6. Una vez que terminaron de comer, los clientes le solicitan al mozo “la cuenta”. Le pagan el
importe al mozo, quien lo deposita en la caja del restaurant. Una vez que pagaron, los clientes
se retiran del restaurant.
7. De forma inesperada y sorpresiva, se corta el suministro de energ´ıa el´ectrica en el restaurant,
quedando totalmente a oscuras. Los clientes, furiosos, se van del restaurant sin pagar lo que hayan
consumido hasta el momento. Tambi´en se van los que estaban esperando en el living. Cuando se
resuelve el problema el´ectrico, el restaurant vuelve a funcionar normalmente, recibiendo nuevos
clientes.
8. La simulaci´on finaliza cuando todas las personas que iban a comer ese d´ıa en el restuarant se
retiraron.
9. Peri´odicamente, el Gerente del restaurant consulta:
a) La cantidad de dinero en la caja.
b) La cantidad de dinero que el restaurant perdi´o de facturar debido a los cortes el´ectricos.
c) La cantidad de grupos de personas esperando en el living.
10. Los siguientes par´ametros deben ser configurables sin necesidad de recompilar el c´odigo:
a) La cantidad de recepcionistas
1
b) La cantidad de mesas
c) La cantidad de mozos
d) El men´u del restaurant: el listado de platos y el precio de cada uno
11. Durante la ejecuci´on de la simulaci´on se deber´a poder lanzar manualmente el corte de energ´ıa
el´ectrica.

Requerimientos no Funcionales

Los siguientes son los requerimientos no funcionales de la aplicaci´on:

1. El proyecto deber´a ser desarrollado en lenguaje C o C++, siendo este ´ultimo el lenguaje de
preferencia.
2. La simulaci´on puede no tener interfaz gr´afica y ejecutarse en una o varias consolas de l´ınea de
comandos.
3. El proyecto deber´a funcionar en ambiente Unix / Linux.
4. La aplicaci´on deber´a funcionar en una ´unica computadora.
5. El programa deber´a poder ejecutarse en “modo debug”, lo cual dejar´a registro de la actividad que
realiza en un ´unico archivo de texto para su revisi´on posterior. Se deber´a poder seguir el recorrido
de cada uno de los grupos de personas. Se deber´a tambi´en poder observar el momento en que
ingresan y se retiran del restaurante.
6. Las facilidades de IPC que se podr´an utilizar para la realizaci´on de este proyecto son las que
abarcan la primera parte de la materia, es decir, hasta el primer parcial. Dichas facilidades son:
a) Memoria compartida
b) Se˜nales
c) Pipes y fifos
d) Locks
e) Sem´aforos
Cualquier otra facilidad queda expresamente excluida para este proyecto.
Tareas a Realizar
A continuaci´on se listan las tareas a realizar para completar el desarrollo del proyecto:
1. Dividir el proyecto en procesos. El objetivo es lograr que la simulaci´on est´e conformada por un
conjunto de procesos que sean lo m´as sencillos posible.
2. Una vez obtenida la divisi´on en procesos, establecer un esquema de comunicaci´on entre ellos
teniendo en cuenta los requerimientos de la aplicaci´on. ¿Qu´e procesos se comunican entre s´ı?
¿Qu´e datos necesitan compartir para poder trabajar?
3. Tratar de mapear la comunicaci´on entre los procesos a los problemas conocidos de concurrencia.
4. Determinar los mecanismos de concurrencia a utilizar para cada una de las comunicaciones entre
procesos que fueron detectadas en el ´ıtem 2. No se requiere la utilizaci´on de alg´un mecanismo
espec´ıfico, la elecci´on en cada caso queda a cargo del grupo y debe estar debidamente justificada.
5. Realizar la codificaci´on de la aplicaci´on. El c´odigo fuente debe estar documentado.


Entrega

La entrega del proyecto comprende lo siguiente:

1. Informe, se deber´a presentar impreso en una carpeta o folio y en forma digital (PDF) a trav´es del
campus
2. El c´odigo fuente de la aplicaci´on, que se entregar´a ´unicamente mediante el campus
La entrega en el campus estar´a habilitada hasta las 19 hs de la fecha indicada oportunamente.
El informe a entregar debe contener los siguientes items:
1. Breve an´alisis del problema, incluyendo una especificaci´on de los casos de uso de la aplicaci´on.
2. Detalle de resoluci´on de la lista de tareas anterior.
3. Diagrama que refleje los procesos, el flujo de comunicaci´on entre ellos y los datos que intercambian.
4. Diagramas de clases realizados.
5. Diagrama de transici´on de estados de un mozo
