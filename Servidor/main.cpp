
#include <QCoreApplication>
#include <QSettings>
#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <QSslSocket>
#include <fstream>
#include <QList>
#include <cstring>
#include <QSslCertificate>
#include <QSslKey>
#include <QFile>
#include <QDebug>
#include <unistd.h>
#include <errno.h>
#include <pwd.h>
#include <fcntl.h>
#include <signal.h>
#include <grp.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>

#include "protomessage.pb.h"
#include "server.h"


void signal_handler(int sig){
    switch(sig){
    case SIGTERM:
        syslog(LOG_ERR, "Interceptada señal SIGTERM");
        closelog();
        exit(0);
        break;
    }
}

int main(int argc, char *argv[])
{

    QCoreApplication a(argc, argv);
    QCoreApplication::setOrganizationName("ChatOsO");
    QCoreApplication::setOrganizationDomain("ChatOsO");
    QCoreApplication::setApplicationName("ServerChatOsO");

    QSettings settings;

    bool demonio = false;


    int op;
    std::string ip_option;
    std::string port_option;

    if(argc == 1){

        ip_option = settings.value("Direccion", "127.0.0.1").toString().toStdString();
        port_option = settings.value("Puerto", 3000).toString().toStdString();




    }else{

        while( (op = getopt(argc, argv, "ds:p:01")) != -1){
            switch (op){
                case '0':
                case '1': break;
                case 's': ip_option = std::string(optarg); break;
                case 'p': port_option = std::string(optarg); break;
                case 'd': demonio = true; break;
                case '?': break;
                default: std::fprintf(stderr, "?? getopt devolvio codigo de error 0%o ??\n", op);

            }
        }
    }

    if(!demonio){

        quint16 port;
        port = QString::fromStdString(port_option).toUInt();

        Server server(QString::fromStdString(ip_option), port);
        server.start();

        openlog(argv[0], LOG_NOWAIT | LOG_PID, LOG_USER);
        syslog(LOG_NOTICE, "Servidor iniciado con éxito\n");

        return a.exec();
    }
    else{
        //SERVIDOR EJECUTANDO EN MODO DEMONIO

        pid_t pid;
        pid_t sid;

        pid = fork();
        //fork() falló
        if(pid < 0){
            std::cerr << std::strerror(errno) << '\n';
            exit(10);
        }

        //PROCESO PADRE
        if(pid > 0){
            exit(0);
        }

        //A partir de este punto, estamos en el proceso hijo

        umask(0);

        //Abrir conexión demonio syslog
        openlog(argv[0], LOG_NOWAIT | LOG_PID, LOG_USER);

        sid = setsid();
        if(sid < 0){
            syslog(LOG_ERR, "No fue posible crear una nueva sesión\n");
            exit(11);
        }
        if((chdir("/")) < 0){
            syslog(LOG_ERR, "No fue posible cambiar el directorio de trabajo\n");
            exit(12);
        }

        //Cerrar descriptores estandar
        close(STDIN_FILENO);    //fd 0
        close(STDOUT_FILENO);   //fd 1
        close(STDERR_FILENO);   //fd 2

        int fd0 = open("/dev/null", O_RDONLY);  //fd0 == 0
        int fd1 = open("/dev/null", O_WRONLY);  //fd1 == 1
        int fd2 = open("/dev/null", O_WRONLY);  //fd2 == 2

    /*
        passwd* user = getpwnam("ServidorChatOsO");
        seteuid(user->pw_uid);

        group* group_ = getgrnam("ServidorChatOsO");
        setegid(group_->gr_gid);*/

        //Enviar paquete al demonio syslog
        syslog(LOG_NOTICE, "Demonio iniciado con éxito\n");

        // Archivo que contiene identificador de proceso del demonio
        QFile file("/var/run/ServidorChatOsO.pid");
        QTextStream out(&file);
        out << pid;
        file.close();


        quint16 port;
        port = QString::fromStdString(port_option).toUInt();

        Server server(QString::fromStdString(ip_option), port);
        server.start();
        signal(SIGTERM, signal_handler);


        return a.exec();


        closelog();
    }


}

