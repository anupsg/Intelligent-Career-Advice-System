#include <iostream>
#include <string>
#include <fstream>
#include <stack>
#include <array>

using namespace std;
// Data structures used in the project
struct conclusion_list{
  int rule_number;
  string conclusion;
  string conclusion_txt;
};

struct variable_list{
  string key;
  string value;
  string status;
};

struct clause_variable_list{
  int rule_number;
  string variable_list_key;
};

struct conclusion_stack_list{
  int rule_number;
  int clause_number;
};

struct area_list{
  int rule_number;
  string area;
};

// conclusion stack
stack<conclusion_stack_list> s;

//global variables
int v_index;
int init_clause_no = 0;
int current_rule_number = 0;


int get_rule_number_index(string conclusion, int current_rule_number, conclusion_list c_list[], int c_list_size){
  for (int i = 0; i < c_list_size; i++) {
    /* code */
    if (!c_list[i].conclusion.compare(conclusion) || c_list[i].rule_number == current_rule_number) {
      return i;
    }
  }
  return -1;
}

//function returns the next rule number from conclusion list based on current rule number
int get_next_rule_number(string conclusion, int current_rule_number, conclusion_list c_list[], int c_list_size){
  for (int i = 0; i < c_list_size; i++) {
    /* code */
    if (!c_list[i].conclusion.compare(conclusion) && c_list[i].rule_number > current_rule_number) {
      return c_list[i].rule_number;
    }
  }
  return -1;
}

//returns the conclusion_text based on rule number passed
string get_conclusion_txt(int current_rule_number, conclusion_list c_list[], int c_list_size){
  for (int i = 0; i < c_list_size; i++) {

    if (c_list[i].rule_number != current_rule_number) {
      return c_list[i].conclusion_txt;
    }
  }
  return "INVALID PROFESSION";
}

//pushes into the stack
void push_into_conclusion_stack(conclusion_stack_list cs){
  s.push(cs);
}

//pops from the stack
void pop_into_conclusion_stack(){
  s.pop();
}
// checks whether the variable Instantiated
bool is_variable_initialized(variable_list v_list[], string v_key, int v_list_size){
  for (int i = 0; i < v_list_size; i++) {
    if (!v_list[i].value.compare(v_key) && !v_list[i].status.compare("I")){
      v_index = i;
      return true;
    }
  }
  return false;
}
// functions to print different data structures
void print_conclusion_list(conclusion_list c[]){
  cout << "*** CONCLUSION LIST ***" << endl;
  for (int i = 0; i < 10; i++) {
    cout << "CONCLUSION "<< c[i].rule_number << " " << c[i].conclusion << endl;
    if (i == 9) {
      cout << "HIT RETURN TO CONTINUE?" << endl;
      if (cin.get() == '\n') continue;
    }
  }
}

void print_variable_list(variable_list v[]){
  cout << "*** VARIABLE LIST ***" << endl;
  for (int i = 0; i < 26; i++) {
    cout << "VARIABLE "<< v[i].value << ": " << v[i].key << endl;
    if (i == 25) {
      cout << "HIT RETURN TO CONTINUE?" << endl;
      if (cin.get() == '\n') continue;
    }
  }
}

// functions to Initialize data from file for each data structures
void instantiate_conclusion_list(conclusion_list c_list[], int c_list_size){

  ifstream fin;
  fin.open("conclusion_list.txt");

  for (int i = 0; i < c_list_size; i++) {
    fin >> c_list[i].rule_number >> c_list[i].conclusion >> c_list[i].conclusion_txt;
  }

  print_conclusion_list(c_list);
}

void instantiate_clause_variable_list(clause_variable_list cv_list[], int cv_list_size, int max_clause_number){

  ifstream fin;
  fin.open("clause_variable_list.txt");

  int v_count = 1, c_count = 1;

  cout << "*** CLAUSE VARIABLE LIST ***" << endl;
  cout << "CLAUSE " << c_count << ":" << endl;

  for (int i = 0; i < cv_list_size; i++) {
    fin >> cv_list[i].variable_list_key;
    cout << "VARIABLE "<< v_count << ": " << cv_list[i].variable_list_key << endl;
    v_count ++;

    if ((i+1) % max_clause_number == 0) {
      v_count = 1;
      c_count ++;
      if (c_count < max_clause_number + 1) cout << "CLAUSE " << c_count << ":" << endl;
    }
  }
}

void instantiate_variable_list(variable_list v_list[], int v_list_size){

  ifstream fin;
  fin.open("variable_list.txt");

    for (int i = 0; i < v_list_size; i++) {
      fin >> v_list[i].value >> v_list[i].key >> v_list[i].status;
    }

  print_variable_list(v_list);
}

void instantiate_area_list(area_list a_list[], int a_list_size){
  ifstream fin;
  fin.open("area_list.txt");

    for (int i = 0; i < a_list_size; i++) {
      fin >> a_list[i].rule_number >>  a_list[i].area;
    }
}

// Function Instantiates the variable
void change_variable_status(variable_list v_list[]){
  v_list[v_index].status = "I";
}

// Prints the output
void print_output(string profession, string area){
  cout<<"************************************************************"<<endl;
  cout<<"*                  ADVISE EXPERT SYSTEM                    *"<<endl;
  cout<<"*                                                          *"<<endl;
  cout<<"*                                                          *"<<endl;
  cout<<"*                                                          *"<<endl;
  cout<<"       Profession: "<< profession <<endl;
  cout<<"       Area: "<< area <<endl;
  cout<<"*                                                          *"<<endl;
  cout<<"*                                                          *"<<endl;
  cout<<"*                                                          *"<<endl;
  cout<<"*                                                          *"<<endl;
  cout<<"************************************************************"<<endl;
}

// returns the rule number based on the input
int profession_bw(conclusion_list c_list[], variable_list v_list[], clause_variable_list cv_list[], int c_list_size, int v_list_size, int cv_list_size, int max_clause_number){
  //initialize response string
  string response, current_conclusion;


  cout << "ENTER A CONCLUSION: ";
  cin >> response;
  current_conclusion = response;

  while (current_rule_number <= (c_list_size * 10)) {
    //get the index of the conclusion in order to retrieve rule number
    int rule_index = get_rule_number_index(response, current_rule_number, c_list, c_list_size);

    //initialize conclusion stack
    conclusion_stack_list cs;

    //prepare conclusion stack input
    current_rule_number = c_list[rule_index].rule_number;
    cs.rule_number = c_list[rule_index].rule_number;

    //use hash formula to calculate CLAUSE_NUMBER
    cs.clause_number = max_clause_number * ( c_list[rule_index].rule_number / 10 - 1) + 1;

    //save initial CLAUSE_NUMBER to help iterate the loop
    init_clause_no = cs.clause_number;

    //push into conclution stack
    push_into_conclusion_stack(cs);

    //check if conclusion exists
    if (rule_index != -1) {

      //loop through each variable in the clause variable list
      for(int i = 0; i < max_clause_number; i++){

        //retrieve each variable
        string v_key = cv_list[cs.clause_number - 1].variable_list_key;

        bool is_vlist_initialized = is_variable_initialized(v_list, v_key, v_list_size);

        if (!is_vlist_initialized && cs.clause_number < (init_clause_no + max_clause_number) && v_key.compare("\"\"")) {
          /* code */
          cout << "Are you inerested in "<< v_key << "? : (Y/N): ";
          cin >> response;

          if (!response.compare("Y")) {
            change_variable_status(v_list);
            cs.clause_number ++;
          }
          else if(!response.compare("N")){
            //remove rule from stack
            //increase rule number by 10
            //push new rule number to stack

            cout << "FINDING THE NEXT RULE..." <<endl;

            //remove rule from stack
            pop_into_conclusion_stack();

            int next_rule = get_next_rule_number(current_conclusion, current_rule_number, c_list, c_list_size);

            if (next_rule == -1) {
              return -1;
            }else{
              // we set the new rule, after it breaks, the program continues with the next rule
              current_rule_number = next_rule;
              break;
            }
          }
          else{
            /** user entered invalid answer **/
            cout << "INVALID ANSWER!" << endl;
            break;
          }
        }
      }//clause variable loop ends

    }else{
      /** user entered invalid conclusion code **/
      cout << "INVALID CONCLUSION!" << endl;
      break;
    }

    //if clause variable loop ends with a Y, we advise profession
    if(!response.compare("Y")){
      return rule_index;
    }
  }

  return -1;
}
// forward chaining returns the area of specialization
string area_fw(int rule_number, string conclusion_txt, area_list a_list[], int a_list_size){

  string response;

  for (int i = 0; i < a_list_size; i++) {
    if (rule_number == a_list[i].rule_number) {
      cout << "Are you interested in " << a_list[i].area << " ?(Y/N): ";
      cin >> response;

      if (!response.compare("Y")) {
        /* User found an area */
        return a_list[i].area;
      }
    }
  }

  return "UNDECIDED";
}

// calls other functions
int main(){
  int c_list_size = -1, v_list_size = -1,
  cv_list_size = -1, a_list_size = -1,
  max_clause_number = -1, profession_index = -1;

  // reading the data size from the data_size file
  string s;
  ifstream fin;
  fin.open("data_size.txt");
  fin>>s>>c_list_size;
  fin>>s>>v_list_size;
  fin>>s>>cv_list_size;
  fin>>s>>a_list_size;
  fin>>s>>max_clause_number;

  //dynamic memory allocation using pointers
  conclusion_list *c_list = new conclusion_list[c_list_size];
  variable_list *v_list = new variable_list[v_list_size];
  clause_variable_list *cv_list = new clause_variable_list[cv_list_size];
  area_list *a_list = new area_list[a_list_size];

  instantiate_conclusion_list(c_list, c_list_size);
  instantiate_variable_list(v_list, v_list_size);
  instantiate_clause_variable_list(cv_list, cv_list_size, max_clause_number);
  profession_index  = profession_bw(c_list, v_list, cv_list, c_list_size, v_list_size, cv_list_size, max_clause_number);

  //program only process area if profession was exists
  if (profession_index != -1) {
    //load areas from file
    instantiate_area_list(a_list, a_list_size);
    //determine user's are using forward chaining
    string area = area_fw(c_list[profession_index].rule_number, c_list[profession_index].conclusion_txt, a_list, a_list_size);
    //print results
    print_output(c_list[profession_index].conclusion_txt, area);

    //free memory space after program completion
    delete []c_list;
    delete []v_list;
    delete []cv_list;
    delete []a_list;
    return 0;
  }

  cout << "EXPERT SYSTEM UNABLE TO ADVISE. PLEASE TRY AGAIN." << endl;

  return 0;
}
