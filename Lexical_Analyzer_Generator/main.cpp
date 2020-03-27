#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
#include <string>
#include <sstream>
#include<map>

using namespace std;

struct transition
{
    int vertex_from;
    int vertex_to;
    char symbol;
};
struct DFAelement
{
    int index;
    vector<int> eq;
    bool mark;
};
int dummyState;
class FA
{
public:
    int no_vertices;
    int final_state;
    int start_state;
    vector<int> final_To_DFA;
    vector<transition> transitions;
    vector<int> vertices;

    FA ()
    {
        no_vertices = 2;
        final_state = 1;
        start_state = 0;
    }
    void set_vertices(int number)
    {
        no_vertices = number;
        for(int i=0; i<no_vertices; i++)
        {
            vertices.push_back(i);
        }
    }

    vector<int> get_vertices (){
      return vertices;
    }
    void set_final_to_DFA(int state)
    {
        final_To_DFA.push_back(state);
    }
    vector<int> get_final_to_DFA()
    {
        return final_To_DFA;
    }
    void set_transtions(int from, int to, char tran_symbol)
    {
        struct transition tran;
        tran.vertex_from = from;
        tran.vertex_to = to;
        tran.symbol = tran_symbol;
        transitions.push_back(tran);
    }

    vector<transition> get_tran()
    {
        return transitions;
    }

    int get_no_vertices()
    {
        return no_vertices;
    }

    void set_startState(int state)
    {
        start_state = state;
    }

    int get_startState()
    {
        return start_state;
    }

    void set_finalState(int state)
    {
        final_state = state;
    }

    int get_finalState()
    {
        return final_state;
    }
    void display()
    {
        struct transition new_trans;
        for(int i = 0; i < transitions.size(); i++)
        {
            new_trans = transitions.at(i);
            cout<<"q"<<new_trans.vertex_from<<" --> q"<<new_trans.vertex_to<<" : Symbol - "<<new_trans.symbol<<endl;
        }
        cout<<"\nThe final state is q"<<get_finalState()<<endl;
    }
};

map<string, FA> regular_Definitions;
map<string, FA> regular_Expression;
map<string, FA> key_words;
map<char, FA> punctuations;

FA concatenation (FA a, FA b)
{
    FA result;
    result.set_vertices((a.get_no_vertices() + b.get_no_vertices()));
    result.set_startState(a.get_startState());
    result.set_finalState(a.get_no_vertices() + b.get_no_vertices()-1);
    for (int i = 0 ; i < a.get_tran().size(); i++)
    {
        struct transition tran;
        tran = a.get_tran().at(i);
        result.set_transtions(tran.vertex_from,tran.vertex_to,tran.symbol);
    }
    result.set_transtions(a.get_finalState(),b.get_startState()+a.get_no_vertices(),'L');
    for (int i = 0 ; i < b.get_tran().size(); i++)
    {
        struct transition tran;
        tran = b.get_tran().at(i);
        result.set_transtions(tran.vertex_from+a.get_no_vertices(),tran.vertex_to+a.get_no_vertices(),tran.symbol);
    }
    return result;

}

FA kleene(FA a)
{
    FA result;
    result.set_vertices(a.get_no_vertices() + 2);
    result.set_startState(a.get_startState());
    result.set_finalState(a.get_finalState() + 2);
    result.set_transtions(result.get_startState(), a.get_startState() + 1, 'L');
    result.set_transtions(result.get_startState(), result.get_finalState(), 'L');
    for (int i = 0 ; i < a.get_tran().size(); i++)
    {
        struct transition tran;
        tran = a.get_tran().at(i);
        result.set_transtions(tran.vertex_from+1,tran.vertex_to+1,tran.symbol);
    }
    result.set_transtions(a.get_finalState()+1,result.get_finalState(),'L');
    result.set_transtions(result.get_finalState()- 1,result.get_startState() + 1,'L');
    return result;
}
FA positive_closure(FA a)
{
    FA result;
    result.set_vertices(a.get_no_vertices()+2);
    result.set_startState(a.get_startState());
    result.set_finalState(a.get_finalState()+2);
    result.set_transtions(result.get_startState(),a.get_startState()+1,'L');
    for(int i=0; i<a.get_tran().size(); i++)
    {
        struct transition tran;
        tran = a.get_tran().at(i);
        result.set_transtions(tran.vertex_from+1,tran.vertex_to+1,tran.symbol);
    }
    result.set_transtions(a.get_finalState()+1,result.get_finalState(),'L');
    result.set_transtions(result.get_finalState()-1,result.get_startState()+1,'L');
    return result;
}

FA Union (FA a, FA b)
{
    FA result;
    result.set_vertices(a.get_no_vertices()+b.get_no_vertices()+2);
    result.set_startState(a.get_startState());
    result.set_finalState(result.get_no_vertices()-1);
    result.set_transtions(result.get_startState(),a.get_startState()+1,'L');
    for(int i=0; i<a.get_tran().size(); i++)
    {
        struct transition tran;
        tran = a.get_tran().at(i);
        result.set_transtions(tran.vertex_from+1,tran.vertex_to+1,tran.symbol);
    }
    result.set_transtions(a.get_finalState()+1,result.get_finalState(),'L');
    result.set_transtions(result.get_startState(),b.get_startState()+a.get_no_vertices()+1,'L');
    for(int i=0; i<b.get_tran().size(); i++)
    {
        struct transition tran;
        tran = b.get_tran().at(i);
        result.set_transtions(tran.vertex_from+a.get_no_vertices()+1,tran.vertex_to+a.get_no_vertices()+1,tran.symbol);
    }
    result.set_transtions(b.get_finalState()+a.get_no_vertices()+1,result.get_finalState(),'L');
    return result;
}

FA range (FA a, FA b){
    char first = a.transitions[0].symbol;
    char last = b.transitions[0].symbol;
    vector<char> v_char;
    vector<FA> v_fa;
    for (char i = first ; i <= last ; i++){
        v_char.push_back(i);
    }

    for(int i=0; i< v_char.size(); i++)
    {
            FA sym;
            sym.set_vertices(2);
            sym.set_startState(0);
            sym.set_finalState(1);
            sym.set_transtions(sym.get_startState(),sym.get_finalState(),v_char[i]);
            v_fa.push_back(sym);
    }
    FA result;
    result = v_fa[0];
    for (int i = 1 ; i < v_fa.size() ; i++){
        result = Union (result, v_fa[i]);
    }
    return result;
}

void keyWords(string input)
{
    istringstream ss(input);
    do
    {
        string word;
        ss >> word;
        if(word == "")
        {
            continue;
        }
        FA key;
        key.set_vertices(word.length()+1);
        key.set_startState(0);
        key.set_finalState(word.length());
        for(int i=0; i<word.length(); i++)
        {
            key.set_transtions(i,i+1,word[i]);
        }
        key_words.insert(pair<string, FA>(word, key));
    }
    while (ss);
}

void punc(string input)
{
    for(int i=0; i<input.length(); i++)
    {
        FA key;
        if(input[i]==' ' || input[i]=='\\')
        {
            continue;
        }
        else
        {
            key.set_vertices(2);
            key.set_startState(0);
            key.set_finalState(1);
            key.set_transtions(key.get_startState(),key.get_finalState(),input[i]);
        }
        punctuations.insert(pair<char, FA>(input[i], key));
    }
}
FA NFAtoDFA (FA a)
{
    FA DFA;

    vector<transition> transitions = a.get_tran();
    vector<int> finalStates = a.get_final_to_DFA();
    vector<char> symbols;
    DFA.set_startState(0);
    for(int i=0; i<transitions.size(); i++) //GET ALL SYMBOLS IN NFA
    {
        char s = transitions.at(i).symbol;
        bool contain= false;
        for(int j=0; j<symbols.size(); j++)
        {
            if(symbols.at(j) == s)
            {
                contain=true;
                break;
            }
        }
        if(!contain)
        {
            symbols.push_back(s);
        }
    }
    vector<DFAelement> elements;
    struct DFAelement elem;
    elem.index =0;
    elem.mark= false;
    elem.eq.push_back(a.get_startState());
    int size_elem =0;
    while(size_elem < elem.eq.size())  //get S0 closure
    {
        int j = elem.eq.at(size_elem);
        for(int i =0; i< transitions.size(); i++)
            {
            struct transition t = transitions.at(i);
            if(t.vertex_from == j && t.symbol == 'L')
            {
                elem.eq.push_back(t.vertex_to);
            }
        }
        size_elem ++;
    }
    elements.push_back(elem);

    int in=0;
    int increase=0;
    while(in<elements.size()) //CALCULATE THE NEW STATES
    {
        struct DFAelement e = elements.at(in);

        if(!e.mark) //CHECK IF WE TAKE IT BEFORE OR NOT
        {
            e.mark= true;
            e.index=in;

            for(int sym=0; sym<symbols.size(); sym++) //GET THE NEXT STATE FOR EVERY SYMBOL
            {
                char currentSymbol = symbols.at(sym);
                if(currentSymbol != 'L'){
                struct DFAelement eNew;
                eNew.mark=false;
                for(int j=0; j<e.eq.size(); j++)
                {
                    int from = e.eq.at(j);
                    for(int k =0; k< transitions.size(); k++)
                    {
                        struct transition t = transitions.at(k);
                        if(t.vertex_from == from && t.symbol == currentSymbol){
                            eNew.eq.push_back(t.vertex_to);
                        }
                    }
                }
                for(int j=0; j<eNew.eq.size(); j++)
                {
                    int from = eNew.eq.at(j);
                    bool inE = false;
                    for(int k =0; k< transitions.size(); k++)
                    {
                        struct transition t = transitions.at(k);
                        if(t.vertex_from == from && t.symbol == 'L'){
                                for(int s=0;s<eNew.eq.size();s++){
                                    if(eNew.eq.at(s) == t.vertex_to){
                                        inE=true;
                                        break;
                                    }
                                }
                            if(!inE){
                            eNew.eq.push_back(t.vertex_to);
                            }
                        }
                    }
                }
                bool inElements = false;
                int indexNew=0;
                for(int elem=0;elem<elements.size();elem++){ //CHECK IF THIS STATE ADDED BEFORE TO ELEMENTS OR NOT
                    struct DFAelement d = elements.at(elem);
                    if(d.eq.size() == eNew.eq.size()){
                        int tempSize=0;
                        for(int temp1=0;temp1<d.eq.size();temp1++){
                            for(int temp2=0;temp2<eNew.eq.size();temp2++){
                                if(d.eq.at(temp1) == eNew.eq.at(temp2)){
                                    tempSize++;
                                }
                            }
                        }
                        if(tempSize == d.eq.size()){
                            inElements = true;
                            indexNew= d.index;
                            break;
                        }
                    }
                }
                if(!inElements){ //if it is a new state then push it to elements
                    increase++;
                    eNew.index=increase;
                    elements.push_back(eNew);
                    if(eNew.eq.size()==0){
                        dummyState = increase;
                    }
                    bool fin = false; //check if this state is a final state or not
                    for(int finalS=0;finalS<eNew.eq.size();finalS++){
                        for(int final2=0;final2<finalStates.size();final2++){
                            if(eNew.eq.at(finalS) == finalStates.at(final2)){
                                fin = true;
                                break;
                            }
                        }
                    }
                    if(fin){
                       DFA.set_final_to_DFA(eNew.index);
                    }
                }
                else{
                    eNew.index=indexNew;
                }
                DFA.set_transtions(in,eNew.index,currentSymbol);
            }
        }
        }
        in++;
    }
    DFA.set_vertices(elements.size());
    return DFA;
}

void expression_to_NFA(string name, string expression , string type)
{
    stack<FA> finite_automata;// stores the NFA for each expression that is read, at the end it will hold the NFA for Re EX el kber :)
    stack<char> operators; // to store operators that must wait until read the next word or expression like union and concatenation
    string symbols=""; // stores in it the word like letter or digit to check if it is in RE DE map
    bool flag = false; // used to check if we get symbol (
    for (int i=0; i<expression.length(); i++)
    {
        if(expression[i]=='\\') // for Any reserved symbol like \+ as we will take + as input symbol not as positive closure
        {
            symbols+= expression[i+1];
            i++;
            continue;
        }
        if((expression[i]==' '&&expression[i-1]!='*' && expression[i-1]!='+') || expression[i+1]=='*' || expression[i+1]=='+')
        { // if we have digit then space or digit+ or digit* so we need to make NFA for digit but if we have digit+ then space so we have already calculate NFA
            if(expression[i]==' ' && expression[i+1]!='|' && expression[i+1]!='-')
            {
                operators.push('#'); // for concatenation symbol
            }
            if((expression[i+1]=='*' || expression[i]=='+')&&expression[i]!=')')
            {
                symbols+= expression[i+1]; // if we have (letter|digit)* so we don't need to add ) to string symbol that hold word digit
            }
            map<string,FA>::iterator it = regular_Definitions.find(symbols); // check if symbol exists in Re DE map
            if(it != regular_Definitions.end())
            {
                finite_automata.push(it->second);
            }
            else // if not exist so we need to calculate NFA for it
            {
                FA key;
                key.set_vertices(symbols.length()+1);
                key.set_startState(0);
                key.set_finalState(symbols.length());
                for(int i=0; i<symbols.length(); i++)
                {
                    key.set_transtions(i,i+1,symbols[i]);
                }
                finite_automata.push(key);
            }
            symbols = "";
            continue;
        }
        else if(expression[i]!='|' && expression[i]!='+'&& expression[i]!='*'&& expression[i]!='('&& expression[i]!=')'&& expression[i]!='-')
        { // here we hold a character so we add it to symbols
            symbols+= expression[i];
            if(i==expression.length()-1){ // if we are in the end of the expression so we need to check NFA for the string that we hold
                FA key;
                key.set_vertices(symbols.length()+1);
                key.set_startState(0);
                key.set_finalState(symbols.length());
                for(int i=0; i<symbols.length(); i++)
                {
                    key.set_transtions(i,i+1,symbols[i]);
                }
                finite_automata.push(key);
            }
        }
        else // here we have a special symbol of operators
        {
            if(expression[i]=='*')
            { // kleene closure
                FA re = finite_automata.top();
                finite_automata.pop();
                re = kleene(re);
                finite_automata.push(re);
            }
            else if(expression[i]=='+')
            { // positive closure
                FA re = finite_automata.top();
                finite_automata.pop();
                re = positive_closure(re);
                finite_automata.push(re);
            }
            else if(expression[i]=='|')
            { // union so we need to wait until we get the next word or expression so we add it to stack
                operators.push(expression[i]);
            }
            else if(expression[i]=='-')
            { // range **** new **** so we need to wait until we get the next word or expression so we add it to stack
                operators.push(expression[i]);
            }
            else if(expression[i]=='(')
            { // same for union we must wait
                flag = true;
                operators.push(expression[i]);
            }
            else if(expression[i]==')')
            { // here we need to take action about what operator was in stack union or concatenation
                if(operators.top()!='(')
                {
                    if(operators.top()=='|')
                    {
                        FA one = finite_automata.top();
                        finite_automata.pop();
                        FA two = finite_automata.top();
                        finite_automata.pop();
                        finite_automata.push(Union(one,two));
                        operators.pop();//to pop |
                        operators.pop(); //to pop (
                    }
                    else if(operators.top()=='#')
                    {
                        FA one = finite_automata.top();
                        finite_automata.pop();
                        FA two = finite_automata.top();
                        finite_automata.pop();
                        finite_automata.push(concatenation(one,two));
                        operators.pop(); // to pop #
                        operators.pop(); // to pop (
                    }
                    else if(operators.top()=='-')
                    {
                        FA one = finite_automata.top();
                        finite_automata.pop();
                        FA two = finite_automata.top();
                        finite_automata.pop();
                        finite_automata.push(range(one, two));
                        operators.pop(); // to pop -
                        operators.pop(); // to pop (
                    }

                }
                else {
                    operators.pop(); // pop (
                }
                flag = false; // as the ( is removed from stack
            }
        }
        if((finite_automata.size()-operators.size()==1) && !flag){ // lw 3nde fe el stack finite (NFA1,NFA2) wa el operators (|) wa mknsh feh ( kda el mfrod a3ml union l NFA1 wa NFA2 y3ne lma al2e stack finite > stack operators b 1
            if(operators.top()=='|')
                    {
                        FA one = finite_automata.top();
                        finite_automata.pop();
                        FA two = finite_automata.top();
                        finite_automata.pop();
                        finite_automata.push(Union(one,two));
                        operators.pop();
                        operators.pop();
                    }
                    else if(operators.top()=='#')
                    {
                        FA one = finite_automata.top();
                        finite_automata.pop();
                        FA two = finite_automata.top();
                        finite_automata.pop();
                        finite_automata.push(concatenation(one,two));
                        operators.pop();
                        operators.pop();
                    }
        }
        else if((finite_automata.size()==operators.size()) && flag){ // nfs el klam fe ely fo2 bs lma ykon 3nde ( kda el stack finite = stack operators
             if(operators.top()=='|')
                    {
                        FA one = finite_automata.top();
                        finite_automata.pop();
                        FA two = finite_automata.top();
                        finite_automata.pop();
                        finite_automata.push(Union(one,two));
                        operators.pop();
                        operators.pop();
                    }
                    else if(operators.top()=='#')
                    {
                        FA one = finite_automata.top();
                        finite_automata.pop();
                        FA two = finite_automata.top();
                        finite_automata.pop();
                        finite_automata.push(concatenation(one,two));
                        operators.pop();
                        operators.pop();
                    }
        }
        if(i==expression.length()-1) // here we are finish so we add the NFA in stack in Re EX map
        {
            if (type == "re")
            {
                regular_Expression.insert(pair<string, FA>(name,finite_automata.top()));
            }else if  (type == "def")
            {
                regular_Definitions.insert(pair<string, FA>(name,finite_automata.top()));
            }
        }
    }
}

void read_file (const char* input_file){
     fstream newfile;
     newfile.open(input_file,ios::in); //open a file to perform read operation using file object
   if (newfile.is_open()){   //checking whether the file is open
      string line;
      while(getline(newfile, line)){ //read data from file object and put it into string.
         string word = "";
         for (std::string::size_type i = 0; i < line.size(); i++)
        {
            if (line[i] == '{'){ // keywords
                line.erase(line.begin() + 0);
                line.erase(line.end() - 1);
                keyWords(line);
                break;
            }else if (line[i]  == '['){ // punctuation
                line.erase(line.begin() + 0);
                line.erase(line.end() - 1);
                punc(line);
                break;
            }else if (line[i] == ':'){ // regular expression
                line.erase(line.begin() + 0 , line.begin() + i + 1);
                expression_to_NFA(word, line, "re");
                break;
            }else if (line[i] == '='){ // regular definition
                line.erase(line.begin() + 0 , line.begin() + i + 1);
                expression_to_NFA(word, line, "def");
                break;
            }else{ //read the label of a regular expression or a regular definition
                    word = word + line[i];
            }
        }
      }
      newfile.close(); //close the file object.
   }
 }
vector<int> getNonFinalStates(vector<int> first, vector<int> second){
  std::vector<int> v(first.size()+second.size());
  std::vector<int>::iterator it;

  std::sort (first.begin(),first.end());
  std::sort (second.begin(),second.end());

  it=std::set_difference (first.begin(), first.end(), second.begin(), second.end(), v.begin());
  v.resize(it-v.begin());
  return v;

}
vector<char> getInputs(vector<transition> transitions){
    vector<char> symbols;
    for(int i=0; i<transitions.size(); i++)
    {
        char s = transitions.at(i).symbol;
        bool contain= false;
        for(int j=0; j<symbols.size(); j++)
        {
            if(symbols.at(j) == s)
            {
                contain=true;
                break;
            }
        }
        if(!contain)
        {
            symbols.push_back(s);
        }
    }
    return symbols;
}
string findNextStats (int state, vector<transition> transitions, vector<char> inputs ,map<int, int> keyOfStates){
    std::stringstream mySS;
    for(int j=0; j<inputs.size(); j++){
      for(int i=0; i<transitions.size(); i++){
        if(transitions[i].vertex_from == state && transitions[i].symbol == inputs.at(j)){
              mySS << keyOfStates[transitions[i].vertex_to];
        }
        }
    }
    return mySS.str();
}

void updateMapValues(int value,vector<int> v, std::map<int, int> &keyOfStates){
     for(int i=0; i<v.size(); i++){
        keyOfStates[v.at(i)] = value;
     }
}
 map<int, vector<int> > minimizaion (FA DFA){
    int numberOfSets = 2;
    int counter = 0;
    int whichPartition = 1;

    map<string,vector<int> > pre_result;
    std::map<int, int> keyOfStates;
    map<int,vector<int> > partitions;
    vector<transition> transitions = DFA.get_tran();
    vector<int> finalStates = DFA.get_final_to_DFA();
    vector<int> vertices = DFA.get_vertices();
    vector<int> NonFinalStates = getNonFinalStates(vertices,finalStates);
    vector<char> inputs = getInputs(transitions);
    vector<int> currntVector = NonFinalStates;
    vector<int> mySS;

    updateMapValues(1,NonFinalStates,keyOfStates);
    updateMapValues(2,finalStates,keyOfStates);
    partitions.insert(pair<int, vector<int> >(1,NonFinalStates));
    partitions.insert(pair<int, vector<int> >(2,finalStates));
    mySS.push_back(1);
    mySS.push_back(2);

    while(currntVector.size() > counter){
      string nextStates = findNextStats (currntVector[counter], transitions, inputs , keyOfStates);
      pre_result[nextStates].push_back(currntVector.at(counter));
      counter++;
      if(counter >= currntVector.size()){
        if (pre_result.size() > 1){
          for (std::map<string,vector<int> >::iterator it = pre_result.begin(); it!=pre_result.end(); ++it){
              numberOfSets++;
              partitions.insert(pair<int,vector<int> >(numberOfSets, it->second));
          }
           partitions[0].push_back(whichPartition);
        }
        mySS.erase(mySS.begin());
        if(!mySS.empty()){
          whichPartition= mySS.at(0);
          currntVector = partitions[whichPartition];
        } else if(!partitions[0].empty()){
            for(int i = 0; i< partitions[0].size();i++)
                partitions.erase(partitions[0].at(i));

            partitions.erase(0);
            for (std::map<int,vector<int> >::iterator it = partitions.begin(); it!=partitions.end(); ++it){
                    updateMapValues(it->first,it->second,keyOfStates);
                    mySS.push_back(it->first);
            }
                 whichPartition = partitions.begin()->first;
                 currntVector = partitions[whichPartition];
        }else {
            partitions.erase(0);
            break;
        }
        pre_result.clear();
        counter = 0;
    }
    }
    return partitions;
 }
FA  minimizedTable (map<int, vector<int> > partitions,FA minimizedTable){
        FA result;
        std::map<int, int> keyOfStates;
        vector<transition> transitions = minimizedTable.get_tran();
        vector<int> finalStates= minimizedTable.get_final_to_DFA();
        for (std::map<int,vector<int> >::iterator it = partitions.begin(); it!=partitions.end(); ++it){
                    if(it->second.size() > 1){
                    vector<int>  v ;
                    v.insert(v.end(),it->second.begin()+1,it->second.end());
                    updateMapValues(it->second[0],v,keyOfStates);
                    }
        }
        for(int i=0; i<transitions.size(); i++){
            if(keyOfStates.count(transitions[i].vertex_from)){
                transitions.erase(transitions.begin()+ i);
                i--;
            }else if(keyOfStates.count(transitions[i].vertex_to)){
                transitions[i].vertex_to = keyOfStates[transitions[i].vertex_to];
            }
        }
        for(int i=0; i<finalStates.size(); i++){
         if(!keyOfStates.count(finalStates[i])){
             result.set_final_to_DFA(finalStates[i]);
         }
        }

        result.set_vertices(keyOfStates.size());
       // result.set_tran(transitions);
        return result;
}
int main()
{
    FA result;
    result.set_vertices(9);
    result.set_transtions(0, 1, '0');
    result.set_transtions(0, 8, '1');
    result.set_transtions(1, 2, '0');
    result.set_transtions(1, 3, '1');
    result.set_transtions(2, 4, '0');
    result.set_transtions(2, 2, '1');
    result.set_transtions(3, 5, '0');
    result.set_transtions(3, 2, '1');
    result.set_transtions(4, 8, '0');
    result.set_transtions(4, 6, '1');
    result.set_transtions(5, 7, '0');
    result.set_transtions(5, 3, '1');
    result.set_transtions(6, 5, '0');
    result.set_transtions(6, 2, '1');
    result.set_transtions(7, 4, '0');
    result.set_transtions(7, 3, '1');
    result.set_transtions(8, 8, '0');
    result.set_transtions(8, 8, '1');
    result.set_final_to_DFA(1);
    result.set_final_to_DFA(3);
    result.set_final_to_DFA(4);
    result.set_final_to_DFA(5);
    result.set_final_to_DFA(6);
    result.set_final_to_DFA(7);
    FA nfa;
    nfa.set_vertices(11);
    nfa.set_transtions(0, 1, 'L');
    nfa.set_transtions(0,7 , 'L');
    nfa.set_transtions(1,2 , 'L');
    nfa.set_transtions(1,4 , 'L');
    nfa.set_transtions(2,3 , 'a');
    nfa.set_transtions(3,6 , 'L');
    nfa.set_transtions(4,5, 'b');
    nfa.set_transtions(5,6 , 'L');
    nfa.set_transtions(6,7 , 'L');
    nfa.set_transtions(6,1 , 'L');
    nfa.set_transtions(7,8 , 'a');
    nfa.set_transtions(8,9 , 'b');
    nfa.set_transtions(9,10 , 'b');
    nfa.set_final_to_DFA(10);
    FA nfa2;
    nfa2.set_vertices(13);
    nfa2.set_transtions(0, 1, 'L');
    nfa2.set_transtions(0, 4, 'L');
    nfa2.set_transtions(0, 11, 'L');
    nfa2.set_transtions(1, 2, 'd');
    nfa2.set_transtions(2, 3, '0');

    FA r = NFAtoDFA(nfa);
    for(int i=0;i<r.get_tran().size();i++){
        cout << r.get_tran().at(i).vertex_from <<" "<< r.get_tran().at(i).vertex_to<<" "<< r.get_tran().at(i).symbol<<"\n";
    }
    for(int i=0;i<r.get_final_to_DFA().size();i++){
        cout<<r.get_final_to_DFA().at(i) << "\n";
    }
 cout << r.start_state;
//    map<int,vector<int> > results = minimizaion (result);
//    for (std::map<int,vector<int> >::iterator itr = results.begin(); itr!=results.end(); ++itr){
//            std::cout << itr->first <<" " ;
//              for (std::vector<int> ::iterator it = itr->second.begin(); it!=itr->second.end(); ++it){
//                  std::cout << *it ;
//              }
//                  std::cout << '\n' ;
//       }
    return 0;
}