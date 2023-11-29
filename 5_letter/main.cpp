#include <iostream>
#include <string>
#include <fstream>
#include "termcolor.h"
#include <cstdlib>


bool checkAccount(std::string Login, std::string Password){
    std::ifstream file;
    file.open("database.txt", std::ios::in);
    std::string line;
    while (getline(file, line))
    {
        if (line==Login+" "+Password){
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

bool checkNickname(std::string Nickname){
    std::ifstream file;
    file.open("database.txt", std::ios::in);
    std::string line;
    while (getline(file, line))
    {
        std::string usedLogin = line.substr(0, line.find(' '));
        if (usedLogin==Nickname){
            file.close();
            return false;
        }
    }
    file.close();
    return true;
}

void createAccount(const std::string& Login, const std::string& Password) {
    std::ofstream file;
    file.open("database.txt", std::ios::app);  
    file << Login << " " << Password << "\n";
    file.close();
    std::ofstream file2;
    file2.open("RecordsTable.txt", std::ios::app);
    file2 << Login << " " << 0 << "\n";
}

int getGamerPoints(std::string Nickname){
    std::ifstream file;
    file.open("RecordsTable.txt", std::ios::in);
    std::string line;
    while (getline(file, line))
    {
        std::string usedLogin = line.substr(0, line.find(' '));
        if (usedLogin==Nickname){
            file.close();
            return std::stoi(line.substr(line.find(' ')+1, line.find(' ', line.find(' ')+1)));
        }
    }
}


void setGamerPoints(std::string Nickname){
    std::ifstream file;
    file.open("RecordsTable.txt", std::ios::in);
    std::string line;
    std::string newFile;
    int points = getGamerPoints(Nickname);
    while (getline(file, line))
    {
        std::string usedLogin = line.substr(0, line.find(' '));
        if (usedLogin==Nickname){
            newFile+=usedLogin+" "+std::to_string(points+1)+"\n";
        }
        else
            newFile+=line+"\n";
    }
    file.close();
    std::ofstream file2;
    file2.open("RecordsTable.txt", std::ios::out);
    file2<<newFile;
    file2.close();
}

std::string createWord(){
    std::srand(time(NULL));
    std::ifstream file("WORDS.txt", std::ios::in);
    std::string line;
    int random = std::rand()%3319;
    for (int i=0; i<random; i++)
        getline(file, line);
    file.close();
    return line;

}

bool checkWord(std::string Word){
    std::ifstream file("WORDS.txt", std::ios::in);
    std::string line;
    while (getline(file, line))
    {
        if (line==Word){
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}
void colorizeLetters(std::string answer, std::string word){
    for (int i = 0; i<5;i++){
        if (answer[i]==word[i])
            std::cout<<termcolor::on_green<<answer[i]<<termcolor::reset;
        else if (word.find(answer[i])!=std::string::npos)
            std::cout<<termcolor::on_yellow<<answer[i]<<termcolor::reset;
        else
            std::cout<<termcolor::on_red<<answer[i]<<termcolor::reset;
    }
}
void TopFiveBestGamers(){
    std::ifstream file;
    file.open("RecordsTable.txt", std::ios::in);
    std::string line;
    std::string topFive[5];
    int topFivePoints[5];
    for (int i=0; i<5; i++)
        topFivePoints[i]=0;
    while (getline(file, line))
    {
        std::string usedLogin = line.substr(0, line.find(' '));
        int points = std::stoi(line.substr(line.find(' ')+1, line.find(' ', line.find(' ')+1)));
        for (int i=0; i<5; i++)
        {
            if (points>topFivePoints[i])
            {
                for (int j=4; j>i; j--)
                {
                    topFive[j]=topFive[j-1];
                    topFivePoints[j]=topFivePoints[j-1];
                }
                topFive[i]=usedLogin;
                topFivePoints[i]=points;
                break;
            }
        }
    }
    file.close();
    for (int i=0; i<5; i++)
        std::cout<<i+1<<") "<<topFive[i]<<" - "<<topFivePoints[i]<<" очков\n";
}

void soloGame(){
    std::cout<<"У вас уже есть аккаунт?\n1) Да;\n2) Нет;\n";
    int haveAnAccount;
    std::cin>>haveAnAccount;
    while (haveAnAccount!=1 && haveAnAccount!=2)
    {
        std::cout<<"Неверный ответ! Попробуйте еще раз:\n";
        std::cin>>haveAnAccount;
    }
    std::string Login, Password;
    if (haveAnAccount==1)
    {
        std::cout<<"Введите логин и пароль:\n";
        std::cin>>Login>>Password;
        while (!checkAccount(Login, Password))
        {
            std::cout<<"Неверный логин или пароль! Попробуйте еще раз:\n";
            std::cin>>Login>>Password;
        }
    }
    else
    {
        std::cout<<"Введите логин и пароль:\n";
        std::cin>>Login>>Password;
        while (!checkNickname(Login))
        {
            std::cout<<"Такой логин уже существует! Попробуйте еще раз:\n";
            std::cin>>Login>>Password;
        }
        createAccount(Login, Password);
    }
    std::cout<<"Ваше количество очков: "<<getGamerPoints(Login)<<"\n";
    std::cout<<"Игра началась!\n";
    std::string word = createWord();
    std::string answer="";
    int count = 0;
    while (answer!=word && count <5)
    {   
        count++;
        std::cout<<"Введите слово из 5 букв:\n";
        std::cin>>answer;
        while (answer.length()!=5 || !checkWord(answer))
        {
            std::cout<<"Некорректное слово! Попробуйте еще раз:\n";
            std::cin>>answer;
        }
        colorizeLetters(answer, word);
        std::cout<<"\n";
        
    }
    if (answer!=word)
        std::cout<<"\nВы проиграли! Правильный ответ: "<<word<<"\n";
    else{
        std::cout<<"\nПоздравляем! Вы угадали слово!\n";
        setGamerPoints(Login);
        std::cout<<"Ваше количество очков: "<<getGamerPoints(Login)<<"\n";}
    std::cout<<"Текущий топ игроков:\n";
    TopFiveBestGamers();




}

void coopGame(){
    std::cout<<"Введите имя игрока 1:\n";
    std::string Login1, Login2;
    std::cin>>Login1;
    std::cout<<"Введите имя игрока 2:\n";
    std::cin>>Login2;
    std::cout<<"Игра началась!\n";
    std::string word = createWord();
    std::string answer1="", answer2="";
    int count = 0;
    while ((answer1!=word || answer2!=word))
    {   
        count++;
        std::cout<<"Введите слово из 5 букв, "<<Login1<<":\n";
        std::cin>>answer1;
        while (answer1.length()!=5 || !checkWord(answer1))
        {
            std::cout<<"Некорректное слово! Попробуйте еще раз:\n";
            std::cin>>answer1;
        }
        colorizeLetters(answer1, word);
        std::cout<<"\n";
        if (answer1==word)
            break;
        std::cout<<"Введите слово из 5 букв, "<<Login2<<":\n";
        std::cin>>answer2;
        while (answer2.length()!=5 || !checkWord(answer2))
        {
            std::cout<<"Некорректное слово! Попробуйте еще раз:\n";
            std::cin>>answer2;
        }
        colorizeLetters(answer2, word);
        std::cout<<"\n";
        if (answer2==word)
            break;
        
    }
    if (answer1==word)
        std::cout<<"\nПоздравляем, "<<Login1<<"! Вы угадали слово!\n";
    else{
        std::cout<<"\nПоздравляем, "<<Login2<<"! Вы угадали слово!\n";
    }
    

}



int main()
{
    std::cout<<"Добро пожаловать в игру 5 букв! Выберите режим игры:\n1) Одиночная игра;\n2) Кооперативная игра;\n";
    int mode;
    std::cin>>mode;
    while (mode!=1 && mode!=2)
    {
        std::cout<<"Неверный режим игры! Попробуйте еще раз:\n";
        std::cin>>mode;
    }
    if (mode==1)
        soloGame();
    else
        coopGame();
    main();
    

    
    

}
