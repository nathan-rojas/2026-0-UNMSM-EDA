#include <iostream>
#include "containers/stack.h"

using namespace std;

void DemoStack() {
    cout << endl;
    cout << "      DEMO: STACK (PILA LIFO)" << endl;
    cout << endl << endl;
    
    // 1. Crear stack y hacer Push

    cout << "1. Crear stack y hacer Push :" << endl;
    CStack<int> s1;
    s1.Push(10, 1);
    s1.Push(20, 2);
    s1.Push(30, 3);
    s1.Push(40, 4);
    s1.Push(50, 5);
    cout << s1 << endl;
    
    // 2. Top - Ver el tope
    cout << "2. Ver el tope con Top():" << endl;
    cout << "Tope actual: " << s1.Top() << endl << endl;
    
    // 3. Pop - Sacar elementos
    cout << "3. Sacar elementos con Pop():" << endl;
    cout << "Elemento sacado: " << s1.Pop() << endl;
    cout << "Elemento sacado: " << s1.Pop() << endl;
    cout << "Stack despues de 2 Pops: " << s1 << endl;
    
    // 4. isEmpty
    cout << "4. Verificar si esta vacia:" << endl;
    cout << "Esta vacia? " << (s1.isEmpty() ? "SI" : "NO") << endl << endl;
    
    // 5. getSize
    cout << "5. Tamanio del stack:" << endl;
    cout << "s1.getSize() = " << s1.getSize() << endl << endl;
    
    // 6. Constructor Copia
    cout << "6. Constructor Copia:" << endl;
    CStack<int> s2(s1);
    cout << "Stack original: " << s1;
    cout << "Stack copiado:  " << s2 << endl;
    
    // 7. Move Constructor
    cout << "7. Move Constructor:" << endl;
    CStack<int> s3(std::move(s2));
    cout << "Stack movido: " << s3;
    cout << "Stack original (vacio): " << s2 << endl;
    
    // 8. Operator < y >
    cout << "8. Comparacion de stacks (operator < y >):" << endl;
    CStack<int> s4;
    s4.Push(1);
    s4.Push(2);
    
    CStack<int> s5;
    s5.Push(1);
    s5.Push(2);
    s5.Push(3);
    s5.Push(4);
    
    cout << "s4 size: " << s4.getSize() << endl;
    cout << "s5 size: " << s5.getSize() << endl;
    cout << "s4 < s5: " << (s4 < s5 ? "true" : "false") << endl;
    cout << "s4 > s5: " << (s4 > s5 ? "true" : "false") << endl << endl;
    
    // 9. Demostrar LIFO (Last In First Out)
    cout << "9. Demostracion LIFO :" << endl;
    CStack<int> s6;
    cout << "Insertando en orden: 1, 2, 3, 4, 5" << endl;
    s6.Push(1);
    s6.Push(2);
    s6.Push(3);
    s6.Push(4);
    s6.Push(5);
    
    cout << "Sacando elementos (LIFO): ";
    while (!s6.isEmpty()) {
        cout << s6.Pop() << " ";
    }
    cout << endl;
    cout << "Orden: 5, 4, 3, 2, 1 (inverso al ingreso)" << endl << endl;
    
    // 10. Uso practico: Verificar parentesis balanceados
    cout << "10. Uso practico: Verificar parentesis balanceados:" << endl;
    CStack<char> stackParentesis;
    string expresion = "((2+3)*(4-1))";
    cout << "Expresion: " << expresion << endl;
    
    bool balanceado = true;
    for (char c : expresion) {
        if (c == '(') {
            stackParentesis.Push(c);
        } else if (c == ')') {
            if (stackParentesis.isEmpty()) {
                balanceado = false;
                break;
            }
            stackParentesis.Pop();
        }
    }
    
    if (!stackParentesis.isEmpty()) balanceado = false;
    
    cout << "Parentesis balanceados? " << (balanceado ? "SI" : "NO") << endl << endl;
    
    // 11. Uso practico: Invertir cadena
    cout << "11. Uso practico: Invertir una cadena:" << endl;
    CStack<char> stackCadena;
    string palabra = "UNMSM";
    cout << "Palabra original: " << palabra << endl;
    
    // Insertar cada letra
    for (char c : palabra) {
        stackCadena.Push(c);
    }
    
    // Sacar en orden inverso (LIFO)
    cout << "Palabra invertida: ";
    while (!stackCadena.isEmpty()) {
        cout << stackCadena.Pop();
    }
    cout << endl << endl;
    
    // 12. Clear
    cout << "12. Limpiar stack con Clear():" << endl;
    s1.Clear();
    cout << "Despues de Clear(): " << s1;
    cout << "Esta vacia? " << (s1.isEmpty() ? "SI" : "NO") << endl << endl;
 
    // 13. Prueba con otro tipo de dato (Template <string>)
    cout << "13. Prueba de Template con Strings:" << endl;
    CStack<string> sString;
    
    cout << "Pushing: 'Hola', 'Mundo', 'EDA', '2026'" << endl;
    sString.Push("Hola", 1);
    sString.Push("Mundo", 2);
    sString.Push("EDA", 3);
    sString.Push("2026", 4);

    cout << "Stack de Strings: " << endl;
    cout << sString; 

    cout << "Haciendo Pop: " << sString.Pop() << endl;
    cout << "Top actual: "   << sString.Top() << endl << endl;
   
    cout << "========================================" << endl;
    cout << "         FIN DEMO STACK" << endl;
    cout <<endl << endl;
}
