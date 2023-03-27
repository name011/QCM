#include <iostream>
#include "pugixml.hpp"
#include <string>
#include <cstring>
#include <algorithm>
#include <ctime>
#include <fstream>
using namespace std;
using std::cerr; //ne pas oublier le std:: devant la fonction
//pour ajouter correctement la bibliothèque pugixml il faut modifier le task.json en rajoutant : 
/*
                "-L",
                "/home/soko/Documents/pugixml-1.13",
                "-lpugixml"  
Dans le args[...]
*/
string tab2[3], reponseR = "";
string const Fichierlog ="/home/soko/Documents/GitHub/tp4-name011-1/log.txt", fichier = "/home/soko/Documents/GitHub/tp4-name011-1/qcm.xml";
int main(){
do {
    ofstream fiichierlog(Fichierlog.c_str(), ios::out | ios::trunc);// ne peut pas avoir le même nom faire attention
    int points = 0;
    pugi::xml_document doc;
    if (!doc.load_file(fichier.c_str())){ /* /!\ ne pas oublier le .c_str */
        cerr << "Error: " << doc.load_file(fichier.c_str()).description() << endl;
        return 1;
    }
    else{
        cout << "XML file loaded successfully" << endl;
    }
    // ouvre le fichier log
    ofstream fichierlog(Fichierlog.c_str(), ios::out | ios::app);
    if(fichierlog)
    {
        cout << "log file loaded successfully" << endl;
    }
    else
    {
        cerr << "Impossible d'ouvrir le fichier !" << endl;
    }
    // prendre ce qui y a dans le fichier xml
    pugi::xml_node qcm = doc.child("qcm");
    pugi::xml_node question = qcm.child("question");
    pugi::xml_node choix = question.child("choix");
    pugi::xml_node reponse = question.child("reponse"); 

    int qcm_size = distance(qcm.begin(), qcm.end());//calcul le nombre de question(noeud)

    //création d'un tableau d'indices totalement modulable (on peut add autant de questions qu'on veut)
    int *question_indices = new int[qcm_size];
    for (int i = 0; i < qcm_size; i++) {
        question_indices[i] = i;
    }

    //mélange le tableau d'indices de question
    srand(time(NULL));
    random_shuffle(question_indices, question_indices + qcm_size); // mélange le tabelau

    // boucle sur les questions
    for (int q_index = 0; q_index < qcm_size; q_index++) {
        pugi::xml_node question = qcm.child("question");
        for (int i = 0; i < question_indices[q_index]; i++) {
            question = question.next_sibling("question");
        }

        string tab1[4]; // tableau pour stocker les réponses pour chaque question 
        int i = 0, j=0;
        tab2[j] = question.child_value("enonce");
        j++;

        // parcourir tous les choix pour cette question et stocker les réponses possibles dans le tableau
        for (pugi::xml_node option = question.child("choix").child("option"); option; option = option.next_sibling("option")) {
            tab1[i] = option.child_value();
            i++;
        }

        //création d'un tableau d'indices pour les réponses
        int answer_indices[4] = {0, 1, 2, 3};

        //mélanger les réponses possibles dans le tableau
        srand(time(NULL));
        random_shuffle(answer_indices, answer_indices + 4); // mélange le tabelau

        //afficher la question
        // demander à l'utilisateur de choisir une réponse
        for (int i = 0; i < 1; i++){
        cout <<"\033[34m"<< "\n"<<"Question " << q_index + 1 << " : " << tab2[0] << "\033[0m"<<endl;
        //afficher les réponses possibles pour cette question
        for (int i = 0; i < 4; i++){
            cout << "Option " << i+1 << " : " << tab1[answer_indices[i]] << endl;
            
        }
            string reponse;
            cout <<"\033[35m"<< "\n"<<"Votre réponse : "<<"\033[0m";
            cin >> reponse;
            transform (reponse.begin(), reponse.end(), reponse.begin(), ::tolower); // convertir la réponse en minuscule
            //verifie si la reponse saisie est correcte avec la réponse attendu dans le fichier xml
            // ou si la reponse saisie est le chiffre correspondant à la réponse attendu dans le fichier xml
            if (reponse == question.child("reponse").child_value() || reponse == to_string(answer_indices[0] + 1)){
                cout << "\033[32m" << "Bonne réponse" << "\033[0m" << endl;
                points++;
            }
            else{
                cout << "\033[31m"<< "Mauvaise réponse" <<"\033[0m"<< endl;
            }

        // ouvre le fichier log.txt et écrit la question et la réponse saisie par l'utilisateur
        fichierlog << "Question " << q_index + 1 << " : " << tab2[0] << endl;
        fichierlog << "Votre reponse : " << reponse << endl;
        fichierlog << "Bonne reponse : " << question.child("reponse").child_value() << endl;
        fichierlog << "----------------------------------------" << endl;
        }
    }
    cout << "voulez-vous consulter le fichier les réponses aux questions ? "<<"\033[0m" << endl;
    string reponse2;
    cin >> reponse2;
    transform (reponse2.begin(), reponse2.end(), reponse2.begin(), ::tolower); // convertir la réponse en minuscule
    if (reponse2 == "o"){
        ifstream fichierlog(Fichierlog);
        if (fichierlog){
            string ligne;
            while (getline(fichierlog, ligne)){
                cout << ligne << endl;
            }
        }
        else{
            cout << "Impossible d'ouvrir le fichier log" << endl;
        }
        fichierlog.close();
    }
    else{
        cout << "ok" << endl;
    }
    cout << "\033[33m"<<"==================== fin du qcm ===================="<<"\033[0m" << endl;// c'est BEAUW
    delete[] question_indices; //important ne pas oublier
    cout << "Voulez-vous rejouer ? (o/n) : ";
    cin >> reponseR;
    transform (reponseR.begin(), reponseR.end(), reponseR.begin(), ::tolower); // convertir la réponse en minuscule
} while (reponseR == "o");
    return 0;
}