# CPD_kmeanSegmetation
Para correr el programa de manera distribuida se usa OpenMPI como herramienta y un servdor predeterminado, planteamos 2 formas.
Estas pruebas fueron realizadas en el Sistema Operativo Linux.

Pasos para la conexión distribuida con nfs:
Para esto se debe crear una máquina con la configuración de principal o master y las demás como esclavos o secundarios.

### 1.Instalar dependencias:
### 1.1 Instalar tool-nets:
    sudo apt-get install nets-tools
### 1.2 Dirección IP:
    ifconfig
### 1.3 Instalar servidor ssh:
    sudo apt-get install openssh-server
### 1.4 Instalar cliente ssh:
    sudo apt-get install openssh-client
### 1.5 Crear archivo ssh:
    mkdir ~/.ssh
### 1.6 Editar permisos archivo ssh:     
    chmod 700 ~/.ssh
### 1.7 Generar claves rsa:
#####     En esta parte se escogera una clave de autenticación para establecer la conexión y permisos correspondientes.
    ssh-keygen -t rsa   
#####      A continuación se le pedirá una frase cualquiera para convertirlo en rsa y registrarlo como clave de autenticación, ejemplo " 1la".
#####      IMPORTANTE: Recordar dicha frase ya que posteriormente se usará.
### 1.8 Copiar archivo id_rsa_xxx.pub 
### 1.8.1 Copiar archivos de esclavos 1,2,3...n a la computadora master
#####     Copiar los archivos generados por el paso anterior al directorio del master: /home/nombredelacomputadora/.ssh          
### 1.8.2 Copiar archivos del master hacia los esclavos:
#####     Copiar el archivo generado del master hacia los directorios de los esclavos: /home/nombreesclavox/.ssh
### 1.9 Autorización de conexión (solo master):
##### Se reemplaza "userslavex" con el nombre de la computadora esclavo, "usermaster" con el nombre de la computadora del master.
##### Se reemplaza "id_slavex.pub" por el nombre del archivo con extensión ".pub" generado.
    cat /home/userslavex/.ssh/id_slavex.pub >> /home/usermaster/.ssh/authorized.keys
##### El comando anterior se repetirá segun la cantidad de esclavos que presente la red.
### 1.10 Autorización de conexión (solo esclavos):
##### Se reemplaza "userslavex" con el nombre de la computadora esclavo, "usermaster" con el nombre de la computadora del master.
    cat /home/usermaster/.ssh/id_rsa_master.pub >> /home/userslavex/.ssh/authorized.keys
### 1.11 Instalar gedit:
    sudo apt-get install gedit
### 1.12 Editar el archivo sshd_config:
    sudo gedit /etc/ssh/sshd_config
##### Debajo de la linea "#PubkeyAuthentication" descomentarlo o quitarle el numeral y escribir debajo "RSAAuthentication  yes"
### 1.13 Reiniciar el servicio ssh
    sudo service ssh restart
### 1.14 Verificar la conexión
##### Reemplazar "ipslave" por la ip de la máquina esclavo a la que el master quiere ingresar y "slavename" por el nombre de la máquina del esclavo
    ssh slavename@ipslave
##### Reemplazar "ipmaster" por la ip de la máquina master y "mastername" por el nombre de la máquina del master
    ssh mastername@ipmaster
##### Presionar "yes" en caso lo requiera para ambos casos e ingresar las claves generadas en el paso 1.7 según el tipo de máquina
##### Para salir de la conexión establecida:
    exit
### 1.15 Instalar nfs
    sudo apt-get install nfs-kernel-server
### 1.16 Crear archivo compartido
##### Se reemplaza "mpiuser" por el nombre de la maquina
    sudo mkdir -p /home/mpiuser/Desktop/sharedfolder
### 1.17 Dar permiso para montar la carpeta compartida
##### Se reemplaza "mpiuser" por el nombre de la maquina
    sudo chown nobody:nogroup /home/mpiuser/Desktop/sharedfolder
    sudo chmod 777 /home/mpiuser/Desktop/sharedfolder
### 1.18 Editar el archivo exports (solo master)
    sudo gedit /etc/exports
##### Agregar este comando segun la cantidad de esclavos donde "mpiuser" es el nombre de la máquina master y "ipslave" es la dirección IP del esclavo
    /home/mpiuser/Desktop/sharedfolder ipslave(rw,sync,no_subtree_check)
### 1.19 Exportar el archivo compartido
    sudo exportfs -a
### 1.20 Reiniciar servidor nfs
    sudo systemcl restart nfs-kernel-server
### 1.21 Verificar si el firewall esta inactivo
    sudo ufw status
### 1.22 Instalar nfs client (solo esclavos)
    sudo apt-get install nfs-common
##### Crear archivo en máquinas esclavos (paso 1.16)
### 1.23 Montar carpeta de esclavo a la master
##### Se reemplaza "mastername" por el nombre de la máquina master y "slavename" por el nombre de la máquina esclavo
    sudo mount ipmaster:/home/mastername/Desktop/sharedfolder /home/slavename/Desktop/sharedfolder
##### Agregar cualquier archivo a la carpeta sharedfolder para verificar la conexión nfs
Pasos para la conexion distribuida con hamachi:
### 2.1 Instalación Hamachi 
### 2.1.1 Windows
    https://hamachi.uptodown.com/windows
### 2.1.2 Linux
    https://www.vpn.net/linux
### 2.2 Crear Servidor en Hamachi
##### El master crea un sevidor hamachi el cual generará una ip y password para su ingreso y conexión trabajando con las claves de autenticación de los pasos 1.7 a 1.10
### 2.3 Encender servidor Hamachi
### 2.4 Ingresar con id y password
##### Ingresar con el id y password del paso 2.2 el cual les asignará automáticamente una ip el cual se deberá cambiar el archivo de autenticación
