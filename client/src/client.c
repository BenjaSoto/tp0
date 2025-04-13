#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char *ip;
	char *puerto;
	char *valor;

	t_log *logger;
	t_config *config;

	/* ---------------- LOGGING ---------------- */

	//logger = log_create("tp0.log", "TP0", true, LOG_LEVEL_INFO);
	// Usando el logger creado previamente

	logger = iniciar_logger();
	// Escribi: "Hola! Soy un log"
	log_info(logger, "Hola! Soy un log");

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();
	
	// Usando el config creado previamente, leemos los valores del config y los
	// dejamos en las variables 'ip', 'puerto' y 'valor'
	valor = config_get_string_value(config, "CLAVE");
	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");
	// Loggeamos el valor de config
	log_info(logger,valor);
	log_info(logger,ip);
	log_info(logger,puerto);
	/* ---------------- LEER DE CONSOLA ---------------- */

	// leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor,conexion);
	// Armamos y enviamos el paquete
	paquete(conexion);

	log_destroy(logger);
	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log *iniciar_logger(void)
{
	t_log *nuevo_logger;
	nuevo_logger = log_create("tp0.log", "TP0", true, LOG_LEVEL_INFO);
	if(nuevo_logger==NULL){
		printf("No se pudo crear el log\n");
		exit(1);
	}
	return nuevo_logger;
}

t_config *iniciar_config(void)
{
	t_config *nuevo_config;
	nuevo_config = config_create("/home/utnso/tp0/client/cliente.config");

	if(nuevo_config==NULL){
		printf("No se pudo iniciar el config\n");
		exit(1);
	}
	return nuevo_config;
}

void leer_consola(t_log *logger)
{
	char *leido;

	while(1){
	// La primera te la dejo de yapa
	leido = readline("> ");

	if(!leido){
		break;
	}
	if(!strncmp(leido,"exit",4)){
		break;
	}
	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	log_info(logger,leido);
	// ¡No te olvides de liberar las lineas antes de regresar!
	free(leido);
	}
    free(leido);
}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char *leido;
	t_paquete *paquete;
	//creo el paquete
	paquete = crear_paquete();
	while(1){
		// La primera te la dejo de yapa
		leido = readline("> ");

		if(!leido){
			break;
		}
		if(!strncmp(leido,"exit",4)){
			break;
		}
		if(leido){
		agregar_a_paquete(paquete,leido,strlen(leido)+1);
		enviar_paquete(paquete, conexion);
		free(leido);
		}
	}
	// Leemos y esta vez agregamos las lineas al paquete
	eliminar_paquete(paquete);
	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	free(leido);
}

void terminar_programa(int conexion, t_log *logger, t_config *config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config)
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	  if(logger !=NULL){
		log_destroy(logger);
	  }
	  if(config !=NULL){
		config_destroy(config);
	  }
	  if(conexion != NULL){
		liberar_conexion(conexion);
	  }
}
