#include<bits/stdc++.h>
#include "avl.cpp"
using namespace std;

//Change the color of the output text and make it bold - https://stackoverflow.com/questions/4053837/colorizing-text-in-the-console-with-c
std::ostream& bold_on(std::ostream& os)
{   
    return os << "\e[1m \x1B[31m";
}

std::ostream& bold_off(std::ostream& os)
{
    return os << "\e[0m \033[0m\t\t";
}

vector<pair<string, vector<pair<string, pair<Node*, Node*>>> >> categoriesAlongWithSubcategoriesAndWorkers;


//function to insert a worker into Available Tree
void insertIntoAvailable(int c1, int c2){
    categoriesAlongWithSubcategoriesAndWorkers[c1].second[c2].second.first = insert(categoriesAlongWithSubcategoriesAndWorkers[c1].second[c2].second.first, categoriesAlongWithSubcategoriesAndWorkers[c1].second[c2].second.second->key, categoriesAlongWithSubcategoriesAndWorkers[c1].second[c2].second.second->name, categoriesAlongWithSubcategoriesAndWorkers[c1].second[c2].second.second->phone_number);
}


//function to remove a worker from Unavailable Tree
void removeFromUnavailable(int c1, int c2){
    categoriesAlongWithSubcategoriesAndWorkers[c1].second[c2].second.second = NULL;
}


int main(){
    ifstream file_input;
    string line;
    file_input.open("input.txt");  //take input from input.txt file to create the dataset

    //variable declarations
    int numberOfCategories;
    string nameOfCategory;
    int numberOfSubcategories;
    string nameOfSubcategory;
    int numberOfWorkersInACategory;
    string nameOfWorker;
    string phoneNumber;
    Node* searchResult = NULL;
    int index = 0;

    int choice5;

    int z = 0;

    vector<pair<pair<int, int>, int>> arrWithSelectedWorker;
    
    
    //Creating an array of Node* and initializing it to NULL line->55-63
    vector<Node*> roots; 
    vector<Node*>::iterator it;
    Node * p = NULL;  
    for(int i = 0; i < 5000; i++){
        roots.push_back(new Node());
    }
    for(it = roots.begin(); it < roots.end(); it++){
        *it = NULL;
    }

    //Building the complete data structure by taking input from input.txt, line->66-102
    while (file_input) {
        file_input >> line;
        
        numberOfCategories = stoi(line);
        
        
        for(int i = 1; i <= numberOfCategories; i++){
            file_input >> nameOfCategory;
            file_input >> line;
            
            numberOfSubcategories = stoi(line);

            vector<pair<string, pair<Node*, Node*>>> temp_subcategoriesAndWorkers;

            for(int j = 0; j < numberOfSubcategories; j++){
                file_input >> nameOfSubcategory;
                file_input >> line;

                numberOfWorkersInACategory = stoi(line);
                vector<pair<Node*, Node*>> temp_workers;

                for(int k = 0; k < numberOfWorkersInACategory; k++){
                    file_input >> nameOfWorker;
                    file_input >> phoneNumber;
                    roots[index] = insert(roots[index], k, nameOfWorker, phoneNumber);

                }
                
                temp_subcategoriesAndWorkers.push_back(make_pair(nameOfSubcategory, make_pair(roots[index], roots[5000-index-1])));
                index++;
            }
            categoriesAlongWithSubcategoriesAndWorkers.push_back(make_pair(nameOfCategory,temp_subcategoriesAndWorkers ));
        }
        break;
    }
    
    file_input.close();

    //user interaction begins
    begin:                  //begin label to start the selection process again if required later

    int choice1;
    cout <<endl<<bold_on<< "Categories" <<bold_off<<endl;

    vector<pair<string, vector<pair<string, pair<Node*, Node*>>> >>::iterator it1; int x = 0;
    for(it1 = categoriesAlongWithSubcategoriesAndWorkers.begin(); it1 < categoriesAlongWithSubcategoriesAndWorkers.end(); it1++){
        cout << x++ <<". " <<(*it1).first << endl;      //printing all category names
    }

    cout << "Enter your Choice: ";
    cin >> choice1;

    cout <<endl<<bold_on<< "Sub-categories" <<bold_off<<endl;
    vector<pair<string, pair<Node*, Node*>>>::iterator it2; x = 0; int choice2;
    for(it2 = categoriesAlongWithSubcategoriesAndWorkers[choice1].second.begin(); it2 < categoriesAlongWithSubcategoriesAndWorkers[choice1].second.end(); it2++){
        cout << x++ <<". " <<(*it2).first << endl;      //printing name of subcategories in the selected category
    }
    
    cout << "Enter your Choice: ";
    cin >> choice2;

    /*Checking if unavailable tree is empty or not - if it is not empty then the user is asked to first 
    deselect the selected worker then only he/she can select more workers in that subcategory*/
    if(categoriesAlongWithSubcategoriesAndWorkers[choice1].second[choice2].second.second!=NULL){
        cout <<bold_on<< "Worker already selected from the category, select another category or free the worker first!" <<bold_off<< endl <<endl;
        goto switchcase;
    }

    cout <<endl<<bold_on<< "Available Workers" <<bold_off<<endl;
    inOrder(categoriesAlongWithSubcategoriesAndWorkers[choice1].second[choice2].second.first);      //Printing the workers available from the Available Tree
    cout << endl;
    // cout << endl << bold_on << "Non-Available Workers" << bold_off << endl;
    // inOrder(categoriesAlongWithSubcategoriesAndWorkers[choice1].second[choice2].second.second);

    int choice3;
    cout << "Select a worker by typing their id: ";
    cin >> choice3;     //Selecting a worker by their id
    
    searchResult = search(categoriesAlongWithSubcategoriesAndWorkers[choice1].second[choice2].second.first, choice3);
    /*Searching the worker by their id, then doing 2 things - 
    1. inserting it into unavailable tree
    2. deleting it from available tree*/
    if(searchResult!=NULL){
        categoriesAlongWithSubcategoriesAndWorkers[choice1].second[choice2].second.second = insert(categoriesAlongWithSubcategoriesAndWorkers[choice1].second[choice2].second.second, searchResult->key, searchResult->name, searchResult->phone_number);
        categoriesAlongWithSubcategoriesAndWorkers[choice1].second[choice2].second.first = deleteNode(categoriesAlongWithSubcategoriesAndWorkers[choice1].second[choice2].second.first, choice3);
    }

    
    arrWithSelectedWorker.push_back(make_pair(make_pair(choice1, choice2), choice3));


    cout <<endl<<bold_on<< "Selected By You:" <<bold_off<<endl;
    inOrderForSelected(categoriesAlongWithSubcategoriesAndWorkers[choice1].second[choice2].second.second);      //Printing the inOrder traversal of unavailable tree
    cout << endl;

    cout << endl;

    switchcase:     //Switchcase label
    cout << "1. Free a worker" << endl;
    cout << "2. Show selected workers" << endl;
    cout << "3. Select another worker" <<endl;
    cout << "4. Exit" << endl;

    int choice4;
    cin >> choice4;
    cout << endl;

    switch(choice4){
        case 1:
            z = 0;
            //Printing all the workers selected by the user
            for(auto & element : arrWithSelectedWorker){
                cout <<z  << ". " << categoriesAlongWithSubcategoriesAndWorkers[element.first.first].second[element.first.second].second.second->name << endl;
                z++;
            }
            cout << "Enter the key to be deleted" << endl;
            cin >> choice5;
            cout << endl;

            //Inserting the worker back into the Available tree and removing it from the Unavailable tree
            insertIntoAvailable(arrWithSelectedWorker[choice5].first.first, arrWithSelectedWorker[choice5].first.second);
            removeFromUnavailable(arrWithSelectedWorker[choice5].first.first, arrWithSelectedWorker[choice5].first.second);

            arrWithSelectedWorker.erase(find(arrWithSelectedWorker.begin(),arrWithSelectedWorker.end(),arrWithSelectedWorker[choice5]));

            goto switchcase;
        case 2:
            z = 0;
            //Printing all the workers selected by the user
            for(auto & element : arrWithSelectedWorker){
                cout <<z  << ". " << categoriesAlongWithSubcategoriesAndWorkers[element.first.first].second[element.first.second].second.second->name << endl;
                z++;
            }
            cout << endl;
            goto switchcase;
        case 3:
            goto begin;
        case 4:
            break;
        default:
            cout << "Wrong choice\n";
            break;
    }

    return 0;
}