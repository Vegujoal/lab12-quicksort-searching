/*
 * Nombre: [Jose Vera]
 * Fecha: 11/4/2026
 * Práctica de vistas filtradas y ordenadas utilizando un arreglo de punteros LAB12
 * para gestionar órdenes de prueba (OrderPreview) sin alterar la colección original.
 */

#include <iostream>
#include <string>

using namespace std;

// Struct Definition for OrderPreview
struct OrderPreview
{
    int orderId;
    string orderType;
    string status;
    string building;
    int priority;
};

// Prototypes for functions
void displayOrders(const OrderPreview orders[], int count, const string& title);
void displayView(OrderPreview *view[], int count, const string& title);
bool compareOrders(const OrderPreview* a, const OrderPreview* b);
void quickSort(OrderPreview *view[], int first, int last);
int findOrderInView(OrderPreview *view[], int count, int targetId);

int main()
{
    const int CAPACITY = 6;

    // Colección original de registros dummy
    OrderPreview orders[CAPACITY] = {
        {420, "DELIVERY", "PENDING", "Biblioteca", 1},    
        {180, "DELIVERY", "IN_PROGRESS", "Ingenieria", 3}, 
        {650, "DINE_IN", "PENDING", "Cafeteria-Lunch", 1}, 
        {310, "DELIVERY", "PENDING", "Biblioteca", 3},
        {205, "DELIVERY", "PENDING", "Biblioteca", 3},
        {900, "DELIVERY", "COMPLETED", "Ingenieria", 2},    // Completada 
    };

    // Arreglo de punteros para la vista y su cantidad lógica
    OrderPreview *view[CAPACITY];
    int viewCount = 0;

    
    // E4: Muestra los estados de 'orders' ANTES de la vista y reporte
    cout << "=================================================" << endl;
    cout << " E4: Estado de 'orders' ANTES de la vista y reporte" << endl;
    cout << "=================================================" << endl;
    displayOrders(orders, CAPACITY, "Original collection:");

    // E1: Construye la vista filtrada (DELIVERY activas: PENDING o IN_PROGRESS)
    for (int i = 0; i < CAPACITY; i++)
    {
        if (orders[i].orderType == "DELIVERY" && 
            (orders[i].status == "PENDING" || orders[i].status == "IN_PROGRESS"))
        {
            view[viewCount] = &orders[i]; // Tomamos prestada la dirección
            viewCount++;
        }
    }

    // E2: Ordena la vista usando Quicksort con el comparador personalizado
    if (viewCount > 1)
    {
        quickSort(view, 0, viewCount - 1);
    }

    cout << "\n=================================================" << endl;
    cout << " E2: Vista filtrada y ordenada (Reporte)" << endl;
    cout << "=================================================" << endl;
    displayView(view, viewCount, "Vista ordenada:");

    // =========================================================
    // E3: Solicita por consola un ID y busca linealmente en la vista
    // =========================================================
    cout << "\n=================================================" << endl;
    cout << " E3: Busqueda de orden en la vista" << endl;
    cout << "=================================================" << endl;
    
    int searchId;
    cout << "Ingrese el ID de la orden a buscar: ";
    cin >> searchId;

    int foundIndex = findOrderInView(view, viewCount, searchId);

    if (foundIndex != -1)
    {
        cout << "Orden encontrada en la posicion " << foundIndex << " de la vista" << endl;
        cout << "  - Edificio: " << view[foundIndex]->building << endl;
        cout << "  - Estado: " << view[foundIndex]->status << endl;
        cout << "  - Prioridad: " << view[foundIndex]->priority << endl;
    }
    else
    {
        cout << "La orden con ID " << searchId << " no se encuentra en la vista activa de DELIVERY." << endl;
    }

    // =========================================================
    // E4: Muestra los estados de 'orders' DESPUÉS del reporte y la consulta
    // Comprobando que ningún campo ni posición original cambió.
    // =========================================================
    cout << "\n=================================================" << endl;
    cout << " E4: Estado de 'orders' DESPUES de la ejecucion" << endl;
    cout << "=================================================" << endl;
    displayOrders(orders, CAPACITY, "Original collection (intacta y sin alteraciones):");

    // Nota: No se ejecuta delete sobre los punteros de 'view' porque apuntan 
    // a elementos en la pila (stack) pertenecientes al arreglo estático 'orders'.

    return 0;
}

// Función para mostrar las órdenes originales
void displayOrders(const OrderPreview orders[], int count, const string& title)
{
    cout << title << endl;
    for (int index = 0; index < count; index++)
    {
        cout << "  - ID: " << orders[index].orderId
             << " | Tipo: " << orders[index].orderType
             << " | Estado: " << orders[index].status
             << " | Edificio: " << orders[index].building
             << " | Prioridad: " << orders[index].priority << endl;
    }
}

// Función para mostrar la vista basada en punteros
void displayView(OrderPreview *view[], int count, const string& title)
{
    cout << title << endl;
    for (int index = 0; index < count; index++)
    {
        cout << "  - ID en Vista: " << view[index]->orderId
             << " | Tipo: " << view[index]->orderType
             << " | Estado: " << view[index]->status 
             << " | Edificio: " << view[index]->building
             << " | Prioridad: " << view[index]->priority << endl;
    }
}

// Comparador personalizado según el criterio E2:
// 1. Edificio ascendente
// 2. Prioridad descendente (mayor valor primero)
// 3. ID ascendente
bool compareOrders(const OrderPreview* a, const OrderPreview* b)
{
    if (a->building != b->building)
    {
        return a->building < b->building; // Ascendente por edificio
    }
    if (a->priority != b->priority)
    {
        return a->priority > b->priority; // Descendente por prioridad
    }
    return a->orderId < b->orderId;       // Ascendente por ID si todo lo demás coincide
}

// Implementación de Quicksort adaptada para un arreglo de punteros (OrderPreview*)
void quickSort(OrderPreview *view[], int first, int last)
{
    if (first < last)
    {
        OrderPreview* pivot = view[last];
        int i = (first - 1);

        for (int j = first; j <= last - 1; j++)
        {
            if (compareOrders(view[j], pivot))
            {
                i++;
                swap(view[i], view[j]);
            }
        }
        swap(view[i + 1], view[last]);
        int pi = i + 1;

        quickSort(view, first, pi - 1);
        quickSort(view, pi + 1, last);
    }
}

// Función E3: Búsqueda lineal en la vista ordenada. Retorna la posición o -1.
int findOrderInView(OrderPreview *view[], int count, int targetId)
{
    for (int i = 0; i < count; i++)
    {
        if (view[i]->orderId == targetId)
        {
            return i; // Posición encontrada en la vista
        }
    }
    return -1; // No encontrado
}