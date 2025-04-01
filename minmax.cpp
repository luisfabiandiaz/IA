#include <iostream>
#include <vector>
#include <utility>
using namespace std;
struct Node {
    vector<vector<char>> tablero;
    int profundidad;
    vector<Node*> hijos;
    int numhijos;
    int tamaño;
    char XoO;
    pair<int, int> coordenadas;
    Node(int n, int numh, int prof, vector<vector<char>> tab, char xo, int x, int y)
        : tamaño(n), numhijos(numh), profundidad(prof), tablero(tab), XoO(xo), coordenadas(x,y) // Hace una copia del tablero
    {
        hijos.resize(numhijos, nullptr);
    }

    ~Node() {
        for (Node* hijo : hijos) {
            delete hijo;
        }
    }
    int calcpuntaje(char XoO) {
        int res = 0;
        int i = 0;
        bool tmp = 1;
        for (; i < tamaño; i++) {
            for (int j = 0; j < tamaño; j++) {
                if (tablero[i][j] == (XoO == 'X' ? 'O' : 'X')) {
                    tmp = 0;
                }
            }
            if (tmp) {
                res++;
            }
            tmp = 1;
        }
        i = 0;
        tmp = 1;
        for (; i < tamaño; i++) {
            for (int j = 0; j < tamaño; j++) {
                if (tablero[j][i] == (XoO == 'X' ? 'O' : 'X')) {
                    tmp = 0;
                }
            }
            if (tmp) {
                res++;
            }
            tmp = 1;
        }
        i = 0;
        tmp = 1;
        for (; i < tamaño; i++) {
            if (tablero[i][i] == (XoO == 'X' ? 'O' : 'X')) {
                tmp = 0;
            }
        }
        if (tmp) {
            res++;
        }
        i = 0;
        int j = tamaño-1;
        tmp = 1;
        for (; i < tamaño; i++, j--) {
            if (tablero[i][j] == (XoO == 'X' ? 'O' : 'X')) {
                tmp = 0;
            }
        }
        if (tmp) {
            res++;
        }
        return res;
    }
    int MinMax() {
        int hijoganador;
        int mejorpuntaje;
        bool encontrado = false;
        if (profundidad != 0) {
            int i=0;
            for (int j = 0; j < tamaño; j++) {
                for (int k = 0; k < tamaño; k++) {
                    if (tablero[j][k] == ' ') {
                        tablero[j][k] = XoO;
                        encontrado = true; 
                        hijos[i] = new Node(tamaño, numhijos - 1, profundidad - 1, tablero, XoO == 'X' ? 'O' : 'X', j, k);
                        i++;
                        tablero[j][k] = ' ';
                        }
                    }
                } 
            if (encontrado) {
                hijoganador = 0;
                mejorpuntaje = hijos[0]->MinMax();
            }
            else {
                return (calcpuntaje('X') - calcpuntaje('O'));
            }
            for (int i = 1;i<numhijos;i++) {
                if (XoO == 'X') {
                    int tmp = hijos[i]->MinMax();
                    if (tmp > mejorpuntaje) {
                        hijoganador = i;
                        mejorpuntaje = tmp;
                    }
                }
                else {
                    int tmp = hijos[i]->MinMax();
                    if (tmp < mejorpuntaje) {
                        hijoganador = i;
                        mejorpuntaje = tmp;
                    }
                }
            }
            coordenadas.first = hijos[hijoganador]->coordenadas.first;
            coordenadas.second = hijos[hijoganador]->coordenadas.second;
        }
        else {
            int res = calcpuntaje('X') - calcpuntaje('O');
            return (res);
        }

    }
};
struct Arbol {
    int tamaño;
    int profundidad;
    int nhijos;
    vector< vector<char>> tablero;
    Node* root;
    Arbol(int tam, int p, int hijos,const vector<vector<char>> tab)
        : tamaño(tam), profundidad(p), nhijos(hijos), tablero(tab) // Hace una copia del tablero
    {
        root = new Node(tamaño, nhijos, p, tablero, 'X', 0, 0);
    }
    ~Arbol() { delete root; }

    pair<int, int> Respuesta() {
        root->MinMax();
        return root->coordenadas;
    }

};
bool nenraya(vector<vector<char>> tablero, int n, char XoO) {
    // Recorre filas
    for (int i = 0; i < n; i++) {
        bool fila = true, columna = true;
        for (int j = 0; j < n; j++) {
            if (tablero[i][j] != XoO) fila = false;  // Fila no completa
            if (tablero[j][i] != XoO) columna = false;  // Columna no completa
        }
        if (fila || columna) return true;
    }

    // Recorre diagonales
    bool diag1 = true, diag2 = true;
    for (int i = 0; i < n; i++) {
        if (tablero[i][i] != XoO) diag1 = false;  // Diagonal principal
        if (tablero[i][n - i - 1] != XoO) diag2 = false;  // Diagonal secundaria
    }

    return diag1 || diag2;
}
void Print(vector<vector<char>> tablero, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            char tmp = tablero[i][j];
            cout << tmp << " ";
        }
        cout << endl;
    }
}
int main() {
    int n, p;
    char turno;

    cout << "=== N-en-Raya ===" << endl;
    cout << "Ingrese el tamaño del tablero (n): ";
    cin >> n;
    vector<vector<char>> tablero(n, vector<char>(n, ' '));

    cout << "Ingrese la profundidad de busqueda del AI: ";
    cin >> p;

    cout << "¿Quien empieza? (M = Maquina, H = Humano): ";
    cin >> turno;

    int espacios_disponibles = n * n;

    while (true) {
        Print(tablero, n);

        if (turno == 'M') {
            cout << "\nTurno de la maquina (X):\n";
            Arbol arbol(n, p, espacios_disponibles, tablero);
            pair<int, int> res = arbol.Respuesta();
            tablero[res.first][res.second] = 'X';
            turno = 'H';
        }
        else {
            int x, y;
            cout << "\nTu turno (O). Ingresa fila y columna: ";
            cin >> x >> y;

            if (x < 0 || x >= n || y < 0 || y >= n || tablero[x][y] != ' ') {
                cout << "Movimiento invalido. Intentalo de nuevo.\n";
                continue;
            }

            tablero[x][y] = 'O';
            turno = 'M';
        }

        espacios_disponibles--;

        if (nenraya(tablero, n, 'X')) {
            Print(tablero, n);
            cout << "\nLa maquina gana. ¡Sigue practicando!\n";
            break;
        }
        else if (nenraya(tablero, n, 'O')) {
            Print(tablero, n);
            cout << "\n¡Ganaste! Felicidades.\n";
            break;
        }
        else if (espacios_disponibles == 0) {
            Print(tablero, n);
            cout << "\nEmpate. ¡Buen juego!\n";
            break;
        }
    }

    return 0;
}
