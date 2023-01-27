#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <wait.h>
#ifdef EC_NOKILL
int input_size = 512;
pid_t pid;
int status = 0;
void alarm_handler(int signo){
    if (signo == SIGALRM){
        write(STDOUT_FILENO, CATCHPHRASE, sizeof(CATCHPHRASE));
     }
}    

void sigint_handler(int signo){
    if (signo == SIGINT){
        if (status != 0){
            write(1, "\n", sizeof("\n"));
        }
        else {
            write(1, "\n", sizeof("\n"));
            write(STDOUT_FILENO, PROMPT, sizeof(PROMPT)); 
        }
    }
}

void sig_handler(){
    if (signal(SIGALRM, alarm_handler) == SIG_ERR){
        perror("Unable to catch SIGALRM\n");
        exit(EXIT_FAILURE);
    }
    if (signal(SIGINT, sigint_handler) == SIG_ERR){
        perror("Unable to catch SIGINT\n");
        exit(EXIT_FAILURE);
    }       
}

//trim the white space at the front and in between the characters
char *trimWhiteSpace(char *str){
    int index = 0;
    int j = 0;
    int k = 0;
    char *str1 = (char *)malloc(input_size);
    while (str[index] == ' '){
        index ++;
    }
    for (j = index; j < input_size; j++){
        str1[k] = str[j];
        k++;
        if (str[j] == '\0'){
            return str1;
        }
        else if (str[j] == ' '){
            break;
        }
    }
    index = j;
    while (str[index] == ' ' || str[index] == '\0'){
        if (str[index] == '\0'){
            return str1;
        }
        index ++;
    }
    for (j = index; j < input_size; j++){
        if (str[j] == '\n'){
            break;
        }
        str1[k] = str[j];
        k++;
    }
    return str1;
}
int main(int argc, char **argv){
    int timeout = 0;
    char user_command[input_size];
    char user_args[input_size];
    if (argc > 2){
        perror("Please enter only one positive integer \n");
        exit(EXIT_FAILURE);
    }
    if (argc == 2){
        int temp_time = atoi(argv[1]);
        if (temp_time < 0){
            perror("Please enter a positive integer\n");
            exit(EXIT_FAILURE);
        }
        timeout = temp_time;
    }
    while(1){
        write(STDOUT_FILENO, PROMPT, sizeof(PROMPT)); 
        char *inputBuffer = (char *)malloc(input_size);
        int n = read(STDIN_FILENO, inputBuffer, input_size);
        char cmd[input_size];
        inputBuffer[n] = '\0';
        int a = 0;
        while (a < input_size){
            cmd[a] = '\0';
            a++;
        }
        if (n == 0 && inputBuffer[0] != '\n'){
            free(inputBuffer);
            exit(0);
        }
        else if (n > 1 && inputBuffer[n - 1] != '\n'){
            free(inputBuffer);
            write(1, "\n", sizeof("\n"));
            continue;
        }
        int m = 0;
        for (m = 0; m < input_size; m++){
            cmd[m] = inputBuffer[m];
            if (cmd[m] == '\n' || cmd[m] == '\0'){
                break;
            }
        }
        char *command = trimWhiteSpace(cmd);
        free(inputBuffer);
        if (command[0] == '\n'){
            continue;
        }
        int i = 0;
        for (i = 0; i < input_size; i++){
            if (command[i] == ' ' || command[i] == '\n'){
                user_command[i] = '\0';
                break;
            }
            user_command[i] = command[i];
        }
        int j;
        int k = 0;
        for (j = i + 1; j < input_size; j++){
            user_args[k] = command[j];
            k++;
            if (command[j] == '\0'){
                break;
            }
        }
        free(command);
        char *args[] = {user_command, user_args, NULL}; 
        if (user_args[0] == '\0'){
            args[1] = NULL;
        }
        pid = 0;
        pid = fork();
        status = 10;
        if (pid == -1){
            perror("an error occurs at fork\n");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0){ 
            signal(SIGALRM, SIG_DFL);
            alarm(timeout);
            if (execve(user_command, args, NULL) < 0){
                perror(user_command);
                exit(EXIT_FAILURE);
            }
        }
        else {
            sig_handler();
            alarm(timeout);
            pid_t cpid = wait(&status);
            if (cpid < 0){
                perror("wait error");
                exit(EXIT_FAILURE);
            } 
        }
        alarm(0);
        status = 0;
    }
    return 0;
}
#else
// regular credit code
int input_size = 512;
pid_t pid;
int status = 0;
void terminate(){
    if (kill(pid, SIGKILL) == -1){
        perror("Error occurs when terminating the function");
        exit(EXIT_FAILURE);
    }
}
void alarm_handler(int signo){
    if (signo == SIGALRM){
        terminate();
        write(STDOUT_FILENO, CATCHPHRASE, sizeof(CATCHPHRASE));
     }
}    

void sigint_handler(int signo){
    if (signo == SIGINT){
        if (status != 0){
            write(1, "\n", sizeof("\n"));
            terminate();
        }
        else {
            write(1, "\n", sizeof("\n"));
            write(STDOUT_FILENO, PROMPT, sizeof(PROMPT)); 
        }
    }
}

void sig_handler(){
    if (signal(SIGALRM, alarm_handler) == SIG_ERR){
        perror("Unable to catch SIGALRM\n");
        exit(EXIT_FAILURE);
    }
    if (signal(SIGINT, sigint_handler) == SIG_ERR){
        perror("Unable to catch SIGINT\n");
        exit(EXIT_FAILURE);
    }       
}

//trim the white space at the front and in between the characters
char *trimWhiteSpace(char *str){
    int index = 0;
    int j = 0;
    int k = 0;
    char *str1 = (char *)malloc(input_size);
    while (str[index] == ' '){
        index ++;
    }
    for (j = index; j < input_size; j++){
        str1[k] = str[j];
        k++;
        if (str[j] == '\0'){
            return str1;
        }
        else if (str[j] == ' '){
            break;
        }
    }
    index = j;
    while (str[index] == ' ' || str[index] == '\0'){
        if (str[index] == '\0'){
            return str1;
        }
        index ++;
    }
    for (j = index; j < input_size; j++){
        if (str[j] == '\n'){
            break;
        }
        str1[k] = str[j];
        k++;
    }
    return str1;
}
int main(int argc, char **argv){
    int timeout = 0;
    char user_command[input_size];
    char user_args[input_size];
    if (argc > 2){
        perror("Please enter only one positive integer \n");
        exit(EXIT_FAILURE);
    }
    if (argc == 2){
        int temp_time = atoi(argv[1]);
        if (temp_time < 0){
            perror("Please enter a positive integer\n");
            exit(EXIT_FAILURE);
        }
        timeout = temp_time;
    }
    while(1){
        write(STDOUT_FILENO, PROMPT, sizeof(PROMPT)); 
        char *inputBuffer = (char *)malloc(input_size);
        int n = read(STDIN_FILENO, inputBuffer, input_size);
        char cmd[input_size];
        inputBuffer[n] = '\0';
        int a = 0;
        sig_handler();
        while (a < input_size){
            cmd[a] = '\0';
            a++;
        }
        if (n == 0 && inputBuffer[0] != '\n'){
            free(inputBuffer);
            exit(0);
        }
        else if (n > 1 && inputBuffer[n - 1] != '\n'){
            free(inputBuffer);
            write(1, "\n", sizeof("\n"));
            continue;
        }
        int m = 0;
        for (m = 0; m < input_size; m++){
            cmd[m] = inputBuffer[m];
            if (cmd[m] == '\n' || cmd[m] == '\0'){
                break;
            }
        }
        char *command = trimWhiteSpace(cmd);
        free(inputBuffer);
        if (command[0] == '\n'){
            continue;
        }
        int i = 0;
        for (i = 0; i < input_size; i++){
            if (command[i] == ' ' || command[i] == '\n'){
                user_command[i] = '\0';
                break;
            }
            user_command[i] = command[i];
        }
        int j;
        int k = 0;
        for (j = i + 1; j < input_size; j++){
            user_args[k] = command[j];
            k++;
            if (command[j] == '\0'){
                break;
            }
        }
        free(command);
        char *args[] = {user_command, user_args, NULL}; 
        if (user_args[0] == '\0'){
            args[1] = NULL;
        }
        pid = 0;
        pid = fork();
        status = 10;
        if (pid == -1){
            perror("an error occurs at fork\n");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0){ 
            if (execve(user_command, args, NULL) < 0){
                perror(user_command);
                exit(EXIT_FAILURE);
            }
        }
        else {
            alarm(timeout);
            pid_t cpid = wait(&status);
            if (cpid < 0){
                perror("wait error");
                exit(EXIT_FAILURE);
            } 
        }
        alarm(0);
        status = 0;
    }
    return 0;
}
#endif