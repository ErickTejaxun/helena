int main()
{    
    int[] nuevo = new int[10];
    nuevo[1] = 100;
    nuevo[1] = nuevo[1]+nuevo[1];
    print(nuevo[1]);
    nuevo[1] = nuevo[1]-nuevo[1];
    string separador = "----------------";
    print(separador);
    print(nuevo[1]);
    nuevo[1] = 100;
    nuevo[1] = nuevo[1]*nuevo[1];
    print(separador);
    print(nuevo[1]);
    nuevo[1] = nuevo[1]/nuevo[1];
    print(separador);
    print(nuevo[1]);
    return 0;
}


