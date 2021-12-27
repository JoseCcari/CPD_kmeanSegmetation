# CPD_kmeanSegmetation
Para correr el programa de manera distribuida se usa OpenMPI como herramienta y un servdor predeterminado, planteamos 2 formas.
Estas pruebas fueron realizadas en el Sistema Operativo Linux.

Pasos la conexión distribuida con nfs:
Para esto se debe crear una máquina con la configuración de principal o master y las demás como esclavos o secundarios.

1.Instalar dependencias:
1.1 Instalar tool-nets:
    Comando usado: sudo apt-get install nets-tools
    Para saber su ip: ifconfig
1.2 Instalar ssh:
    Comando usado: sudo apt-get install openssh-server
    Comando usado: sudo apt-get install openssh-client
1.2.1 Crear archivo ssh:
      Comando usado: mkdir ~/.ssh
      Comando usado: chmod 700 ~/.ssh
1.2.2 Generar claves rsa:
      En esta parte se escogera una clave de autenticación para establecer la conexión y permisos correspondientes.
      Comando usado: ssh-keygen -t rsa
      A continuación se le pedirá una frase cualquiera para convertirlo en rsa y registrarlo como clave de autenticación, ejemplo " 1la".
      IMPORTANTE: Recordar dicha frase ya que posteriormente se usará.
1.2.3 Permisos y claves de autenticación:
1.3 Instalar nfs:
1.4 Instalar OpenMpi

Pasos para la conexion distribuida con hamachi:
