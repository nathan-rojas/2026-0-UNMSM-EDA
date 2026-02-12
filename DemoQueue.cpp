#include <iostream>
#include "containers/queue.h"

using namespace std;

void DemoQueue() {
    cout << "========================================" << endl;
    cout << "      DEMO: QUEUE (COLA FIFO)" << endl;
    cout << "========================================" << endl << endl;
    
    // 1. Crear queue y hacer Push (encolar)
    cout << "1. Crear queue y hacer Push (FIFO):" << endl;
    CQueue<int> q1;
    q1.Push(10, 1);
    q1.Push(20, 2);
    q1.Push(30, 3);
    q1.Push(40, 4);
    q1.Push(50, 5);
    cout << q1 << endl;
    
    // 2. Front y Rear - Ver frente y final
    cout << "2. Ver frente y final:" << endl;
    cout << "Front (frente): " << q1.Front() << endl;
    cout << "Rear (final): " << q1.Rear() << endl << endl;
    
    // 3. Pop - Sacar elementos (desde el frente)
    cout << "3. Sacar elementos con Pop() (desde el frente):" << endl;
    cout << "Elemento sacado: " << q1.Pop() << endl;
    cout << "Elemento sacado: " << q1.Pop() << endl;
    cout << "Queue despues de 2 Pops: " << q1 << endl;
    
    // 4. isEmpty
    cout << "4. Verificar si esta vacia:" << endl;
    cout << "¿Esta vacia? " << (q1.isEmpty() ? "SI" : "NO") << endl << endl;
    
    // 5. getSize
    cout << "5. Tamanio de la queue:" << endl;
    cout << "q1.getSize() = " << q1.getSize() << endl << endl;
    
    // 6. Constructor Copia
    cout << "6. Constructor Copia:" << endl;
    CQueue<int> q2(q1);
    cout << "Queue original: " << q1;
    cout << "Queue copiada:  " << q2 << endl;
    
    // 7. Move Constructor
    cout << "7. Move Constructor:" << endl;
    CQueue<int> q3(std::move(q2));
    cout << "Queue movida: " << q3;
    cout << "Queue original (vacia): " << q2 << endl;
    
    // 8. Operator < y >
    cout << "8. Comparacion de queues (operator < y >):" << endl;
    CQueue<int> q4;
    q4.Push(1);
    q4.Push(2);
    
    CQueue<int> q5;
    q5.Push(1);
    q5.Push(2);
    q5.Push(3);
    q5.Push(4);
    
    cout << "q4 size: " << q4.getSize() << endl;
    cout << "q5 size: " << q5.getSize() << endl;
    cout << "q4 < q5: " << (q4 < q5 ? "true" : "false") << endl;
    cout << "q4 > q5: " << (q4 > q5 ? "true" : "false") << endl << endl;
    
    // 9. Demostrar FIFO (First In First Out)
    cout << "9. Demostracion FIFO (First In First Out):" << endl;
    CQueue<int> q6;
    cout << "Encolando en orden: 1, 2, 3, 4, 5" << endl;
    q6.Push(1);
    q6.Push(2);
    q6.Push(3);
    q6.Push(4);
    q6.Push(5);
    
    cout << "Desencolando elementos (FIFO): ";
    while (!q6.isEmpty()) {
        cout << q6.Pop() << " ";
    }
    cout << endl;
    cout << "Orden: 1, 2, 3, 4, 5 (mismo orden de ingreso)" << endl << endl;
    
    // 10. Comparacion STACK vs QUEUE
    cout << "10. Comparacion STACK (LIFO) vs QUEUE (FIFO):" << endl;
    cout << "Datos insertados: 1, 2, 3, 4, 5" << endl << endl;
    
    cout << "STACK (LIFO):" << endl;
    cout << "  Push: 1, 2, 3, 4, 5" << endl;
    cout << "  Pop:  5, 4, 3, 2, 1  <- Orden INVERSO" << endl << endl;
    
    cout << "QUEUE (FIFO):" << endl;
    cout << "  Push: 1, 2, 3, 4, 5" << endl;
    cout << "  Pop:  1, 2, 3, 4, 5  <- MISMO orden" << endl << endl;
    
    // 11. Uso practico: Sistema de turnos
    cout << "11. Uso practico: Sistema de turnos (Banco):" << endl;
    CQueue<string> colaBanco;
    
    cout << "Clientes llegando:" << endl;
    colaBanco.Push("Juan");
    cout << "  - Juan llega" << endl;
    colaBanco.Push("Maria");
    cout << "  - Maria llega" << endl;
    colaBanco.Push("Pedro");
    cout << "  - Pedro llega" << endl;
    colaBanco.Push("Ana");
    cout << "  - Ana llega" << endl << endl;
    
    cout << "Atendiendo clientes (FIFO):" << endl;
    while (!colaBanco.isEmpty()) {
        cout << "  Atendiendo a: " << colaBanco.Pop() << endl;
    }
    cout << endl;
    
    // 12. Uso practico: Buffer de impresion
    cout << "12. Uso practico: Cola de impresion:" << endl;
    CQueue<string> colaImpresion;
    
    cout << "Documentos enviados a imprimir:" << endl;
    colaImpresion.Push("Documento1.pdf");
    colaImpresion.Push("Documento2.docx");
    colaImpresion.Push("Documento3.txt");
    
    cout << "Imprimiendo en orden (FIFO):" << endl;
    int numero = 1;
    while (!colaImpresion.isEmpty()) {
        cout << "  " << numero++ << ". Imprimiendo: " << colaImpresion.Pop() << endl;
    }
    cout << endl;
    
    // 13. Clear
    cout << "13. Limpiar queue con Clear():" << endl;
    q1.Clear();
    cout << "Despues de Clear(): " << q1;
    cout << "Esta vacia? " << (q1.isEmpty() ? "SI" : "NO") << endl << endl;
    
    cout << "========================================" << endl;
    cout << "         FIN DEMO QUEUE" << endl;
    cout << "========================================" << endl << endl;
}
