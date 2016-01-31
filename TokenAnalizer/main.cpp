/******************************************************************
 Name:                          Alexis Matuk
 Date:                          30 de enero de 2016
 Description:                   Analizador de tokens aritméticos basados en un DFA
 ***********************************************************/

#include <iostream>
#include <vector>
#include <fstream>

/******************************************************************
 Función que sirve para mapear un caracter a la posición correspondiente en la tabla de transición
 ***********************************************************/
int getInputFromChar(char c)
{
    int result = 0;
    if(c == '+')
        result = 0;
    if(c == '-')
        result = 1;
    if(c == '*')
        result = 2;
    if(c == '/')
        result = 3;
    if(c == '^')
        result = 4;
    if(c == '=')
        result = 5;
    if(c == '(')
        result = 6;
    if(c == ')')
        result = 7;
    if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
        result = 8;
    if(c >= '0' && c <= '9')
        result = 9;
    if(c == '.')
        result = 10;
    if(c == ' ')
        result = 11;
    return result;
}

/******************************************************************
 Función para determinar si un estado es final o no
 ***********************************************************/
bool isFinalState(int state)
{
    if(state == 0 || state == 14)
        return false;
    return true;
}

/******************************************************************
 Función para obtener el nombre del estado a partir de la posición en la tabla de transición
 ***********************************************************/
std::string getNameFromState(int state)
{
    if(state == 0)
        return "Inicio";
    if(state == 1)
        return "Variable";
    if(state == 2)
        return "Entero";
    if(state == 3)
        return "Real";
    if(state == 4)
        return "Comentario";
    if(state == 5)
        return "Paréntesis abre";
    if(state == 6)
        return "Paréntesis cierra";
    if(state == 7)
        return "Suma";
    if(state == 8)
        return "Resta";
    if(state == 9)
        return "Multiplicación";
    if(state == 10)
        return "División";
    if(state == 11)
        return "Potencia";
    if(state == 12)
        return "Asignación";
    if(state == 13)
        return "Error";
    if(state == 14)
        return "Punto Flotante";
    return "";
}

/******************************************************************
 Función para abrir un archivo .txt
 ***********************************************************/
void openFile(std::ifstream & _file, std::string route)
{
    try
    {
        if (std::ifstream(route))
        {
            std::cout << "File found... opening " << std::endl;
            _file.open(route);
            if(_file.fail())
            {
                std::cout << "Error reading file... aborting" << std::endl;
                throw 2;
            }
        }
        else
        {
            std::cout << "File not found... Aborting" << std::endl;
            throw 1;
        }
    }
    catch (int e)
    {
        std::cerr << "Error reading data... Exception " << e << " caught" << std::endl;
    }
}

/******************************************************************
 Función principal que evalúa un string e introduce los caracteres en el 
 autómata para determinar cuál es el token y de qué tipo es
 ***********************************************************/
void evaluateString(std::string entrada, int automata [15][12])
{
    int currentState = 0;
    int lastState = 0;
    std::string result = "";
    for(int i = 0; i < entrada.size(); i++)
    {
        lastState = currentState;
        currentState = automata[currentState][getInputFromChar(entrada[i])];//realizar transición
        if(currentState == 0 && i != 0)
        {
            if(entrada[i-1] != ' ')//Ignorar espacios
            {
                if(isFinalState(lastState))
                    std::cout << result << " --> " << getNameFromState(lastState) << std::endl;
                else
                    std::cout << result << " --> " << "Error" << std::endl;
            }
            
            result = "";
            if(entrada[i] != ' ')//Ignorar espacios
                i--;//Simular transición nula
        }
        else
        {
                result += entrada[i];
        }
    }
    if(!result.empty())//Último token leído
    {
        if(isFinalState(currentState))
            std::cout << result << " --> " << getNameFromState(currentState) << std::endl;
        else
            std::cout << result << " --> " << "Error" << std::endl;
    }
}

int main(int argc, const char * argv[]) {
    //Tabla de transición del autómata
    int automata[15][12] = {{7,	8,	9,	10,	11,	12,	5,	6,	1,	2,	13,	0},
                            {0,	0,	0,	0,	0,	0,	0,	0,	1,	1,	13,	0},
                            {0,	0,	0,	0,	0,	0,	0,	0,	0,	2,	14,	0},
                            {0,	0,	0,	0,	0,	0,	0,	0,	0,	3,	13,	0},
                            {4,	4,	4,	4,	4,	4,	4,	4,	4,	4,	4,	4},
                            {0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	13,	0},
                            {0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	13,	0},
                            {0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	13,	0},
                            {0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	13,	0},
                            {0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	13,	0},
                            {0,	0,	0,	4,	0,	0,	0,	0,	0,	0,	13,	0},
                            {0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	13,	0},
                            {0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	13,	0},
                            {0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	13,	0},
                            {13,	13,	13,	13,	13,	13,	13,	13,	13,	3,	13,	13},
                            };
    
    
    /******************************************************************
     Lectura del archivo y evaluación línea por línea en el autómata
       ***********************************************************/
    std::ifstream file;
    std::string route;
    std::cout << "Ruta del archivo a analizar: ";
    std::getline(std::cin, route);
    openFile(file, route);
    if(file.is_open())
    {
        std::string line;
        while(!file.eof())
        {
            std::string line("");
            if(getline(file,line))
            {
                evaluateString(line, automata);
            }
        }
    }
    else
    {
        std::cout << "File is not open" << std::endl;
    }
    file.close();
}
