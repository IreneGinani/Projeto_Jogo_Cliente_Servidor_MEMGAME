
/*!
 *  @file classes.h
 *  @brief Corpo da Classes do servidor
 *
 *  Arquivo com a classe do servidor
 */
 
#ifndef _CLASSES_H_
#define _CLASSES_H_
 
using namespace std;

class Carta{

public:

    Carta(int valor, bool virada){
        this->valor = valor;
        this->virada = virada;
    }
    Carta(){}

    void setValor(int valor){
        this->valor = valor;
    }

    int getValor(){
        return valor;
    }
    void setVirada(bool virada){
        this->virada = virada;
    }
    bool getVirada(){
        return virada;
    }
    void setMatch(bool match){
        this->match = match;
    }
    bool getMatch(){
        return match;
    }


private:

    int valor;
    bool virada;
    bool match = false;

};

class Tabuleiro{

public:

    Tabuleiro(vector<vector<Carta>> &cartas){
        this->cartas = cartas;
    }


    void ImprimirTab(){



        cout << "----------------"<<endl;
        for (int i = 0; i < cartas.size(); i++){
            for (int j = 0; j < cartas[0].size(); j++)
            {
                if ((posicaoi == i) && (posicaoj == j)){
                    if(cartas[i][j].getVirada()){
                        cout << "["<< cartas[i][j].getValor() << "]";
                    }else{
                        cout << "[**]";
                    }
                }else{

                    if(cartas[i][j].getVirada()){
                        cout << " "<< cartas[i][j].getValor() << " ";
                    }else{
                        cout << " ** ";
                    }
                }

            }

            cout << endl;
                
        }

        cout << "----------------"<<endl;
    }

    void movimenta(int mexer){
        if(mexer == 1){
            if(posicaoi > 0){
                posicaoi = posicaoi - 1;
                posicaoj = posicaoj;
                ImprimirTab();
            }
        }else if (mexer == 2){
            if (posicaoi < cartas.size()-1){
                posicaoi = posicaoi + 1;
                posicaoj = posicaoj;
                ImprimirTab();
            }
        }else if(mexer == 3){
            if (posicaoj < cartas[0].size()-1){
                posicaoj = posicaoj + 1;
                posicaoi = posicaoi;
                ImprimirTab();
            }
        }else if (mexer == 4){
            if (posicaoj > 0){
                posicaoj = posicaoj - 1;
                posicaoi = posicaoi;
                ImprimirTab();
            }
        }else if (mexer == 5){

                if(viradas.empty()){
                    cartas[posicaoi][posicaoj].setVirada(true);
                    viradas.push_back(cartas[posicaoi][posicaoj]);
                    ImprimirTab();
                }else if (viradas.size() == 1){
                    cartas[posicaoi][posicaoj].setVirada(true);
                    viradas.push_back(cartas[posicaoi][posicaoj]);
                    ImprimirTab();
                    DeuMatch(viradas[0], viradas[1]);
                    for (int i = 0; i < cartas.size(); ++i)
                    {
                        for (int j = 0; j < cartas[0].size(); ++j)
                        {
                            if((cartas[i][j].getValor() == viradas[0].getValor()) && (!cartas[i][j].getMatch())){
                                cartas[i][j].setVirada(viradas[0].getVirada());
                            }
                            if((cartas[i][j].getValor() == viradas[1].getValor()) && (!cartas[i][j].getMatch())){
                                cartas[i][j].setVirada(viradas[1].getVirada());
                            }
                        }
                    }
                }else 
                    cout<< "Não é possível virar mais cartas!";

            
        }

    }

    void DeuMatch(Carta &carta, Carta &carta1){

        if (carta.getValor() == carta1.getValor()){
            carta.setMatch(true);
            carta1.setMatch(true);
            pares++;
            if(pares == (cartas[0].size()*cartas.size())/2){
                cout<< "PARABÉNS VC VENCEU";
                exit(0);
            }
        }else{
            carta.setVirada(false);
            carta1.setVirada(false);
        }


        viradas.clear();
    }

    int getI(){
        return posicaoi;
    }
    int getJ(){
        return posicaoj;
    }     

private:

    vector<vector<Carta>> cartas;
    int posicaoi = 0;
    int posicaoj = 0;
    Carta carta1;
    Carta carta2;
    vector<Carta> viradas;
    int pares = 0;
};


#endif