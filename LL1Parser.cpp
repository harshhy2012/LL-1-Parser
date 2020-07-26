#include <bits/stdc++.h>
#define __USE_MINGW_ANSI_STDIO 0

using namespace std;

template<typename charT, typename traits = std::char_traits<charT> >
class center_helper {
    std::basic_string<charT, traits> str_;
public:
    center_helper(std::basic_string<charT, traits> str) : str_(str) {}
    template<typename a, typename b>
    friend std::basic_ostream<a, b>& operator<<(std::basic_ostream<a, b>& s, const center_helper<a, b>& c);
};

template<typename charT, typename traits = std::char_traits<charT> >
center_helper<charT, traits> centered(std::basic_string<charT, traits> str) {
    return center_helper<charT, traits>(str);
}

// redeclare for std::string directly so we can support anything that implicitly converts to std::string
center_helper<std::string::value_type, std::string::traits_type> centered(const std::string& str) {
    return center_helper<std::string::value_type, std::string::traits_type>(str);
}

template<typename charT, typename traits>
std::basic_ostream<charT, traits>& operator<<(std::basic_ostream<charT, traits>& s, const center_helper<charT, traits>& c) {
    std::streamsize w = s.width();
    if (w > c.str_.length()) {
        std::streamsize left = (w + c.str_.length()) / 2;
        s.width(left);
        s << c.str_;
        s.width(w - left);
        s << "";
    } else {
        s << c.str_;
    }
    return s;
}

map<char,vector<string> > production;
set<char> terminal;
set<char> epsilon;
map<char,set<char> > Fir;
map<char,set<char> > Follow;
string M[10][10];

//FINDS FIRST() FOR THE GIVEN GRAMMAR
set<char> find_first(char ch){
  Fir[ch];
  int count=0;
  for(string i:production[ch]) {
    // Checking if first of a production is terminal/non-terminal
    // and adding to First if it is terminal
    if(terminal.find(i[0])!=terminal.end()) {
      Fir[ch].insert(i[0]);
    }
    // If first is non terminal, calculate first using recursion.
    else{
      Fir[ch] = (find_first(i[0]));
    }
  }
  return Fir[ch];
}

//FUNCTION TO FIND FOLLOW() FOR THE GIVEN GRAMMAR
set<char> find_follow(char ch){
  if(ch=='E')
    Follow[ch].insert('$');
  vector<char> a,b,c,d;
  for(auto i: production) {
    for(auto j: i.second) {
      if(j=="#")
        continue;
      for(int k=0; k<j.size(); k++) {
        // Case1: A-> xBy and y is a terminal then follow(B) = y
        if((j[k]==ch) && (terminal.find(j[k+1])!=terminal.end()) && (k!=j.size()-1)) {
          a.push_back(j[k+1]);
        }
        // case2: A-> xBy and y is a non-terminal then follow(B) = first(y)
        else if((j[k]==ch) && (terminal.find(j[k+1])==terminal.end()) && (k!=j.size()-1)) {
          b.push_back(j[k+1]);
        }
        // case3: A-> xBy and y is a non terminal and has epsilon value, then follow(B) = follow(A)
        if((j[k]==ch) && (epsilon.find(j[k+1])!=epsilon.end()) && (k!=j.size()-1)) {
          c.push_back(i.first);
        }
        // case4: A-> xB, then follow(B) = follow(A)
        else if((j[k]==ch) && (k==j.size()-1)) {
          d.push_back(i.first);
        }
      }
    }
  }
  if(!a.empty()) {
    for(int i=0; i<a.size(); i++) {
      if(a[i]!='#')
        Follow[ch].insert(a[i]);
    }
  }
  if(!b.empty()) {
    for(int i=0; i<b.size(); i++) {
      for(auto j:Fir[b[i]]) {
        Follow[ch].insert(j);
      }
    }
  }
  if(!c.empty()) {
    for(int i=0; i<c.size(); i++) {
      if(c[i]!=ch) {
        for(auto j:find_follow(c[i])) {
          Follow[ch].insert(j);
        }
      }
    }
  }
  if(!d.empty()) {
    for(int i=0; i<d.size(); i++) {
      if(d[i]!=ch) {
        for(auto j:find_follow(d[i])) {
          Follow[ch].insert(j);
        }
      }
    }
  }
  //remove epsilon from Follow Set
  for(auto &&i: Follow) {
    i.second.erase('#');
  }

  return Follow[ch];
}

//PRINTS FOLLOW
void print_follow(){
  for(auto i:Follow) {
    if(i.first!='#') {
      cout<<"FOLLOW("<<i.first<<") -> ";
      for(auto j: i.second) {
        cout<<j<<", ";
      }
      cout<<endl;
    }
  }
}

//PRINTS FIRST()
void print_first(){

  for(auto i:Fir) {
    if(i.first!='#') {
      cout<<"FIRST("<<i.first<<") -> ";
      for(auto j: i.second) {
        cout<<j<<", ";
      }
      cout<<endl;
    }
  }
}

//MAKES PARSING TABLE FROM FIRST AND FOLLOW
void parse_table(){

//intitializing parsing table 0th row and coloumns to non-terminals and terminals repsectively
  M[0][0] = " ";
  M[0][1] = "+";
  M[0][2] = "*";
  M[0][3] = "(";
  M[0][4] = ")";
  M[0][5] = "i";
  M[0][6] = "$";
  M[1][0] = "E";
  M[2][0] = "R";
  M[3][0] = "T";
  M[4][0] = "S";
  M[5][0] = "F";

  for (int i = 1; i <= 5; i++) {
    // Character for which we are filling for
    char cx = M[i][0][0];
    vector<string> pro = production[cx];
    //s1 contains normals productions, s2 contains production with terminal 'i', s3 contains production with '#'
    //insertin1 contains all the coloums in which s1 is to be inserted
    //insertin2 contains all the coloumns in which s2 is to be inserted
    //insertin3 contains all the coulouns in which s3 is to be insetred
    string s1,s2,s3;
    string insertin1,insertin2,insertin3;
    for(int i=0; i<pro.size(); i++) {
      if(i==0)
        s1+=pro[i];
      if(pro[i]=="i")
        s2+=pro[i];
      if(pro[i]=="#")
        s3+="#";
    }
    if(terminal.find(s1[0])!=terminal.end()) {
      insertin1+=s1[0];
    }
    if(isupper(s1[0])) {
      for(auto i:Fir[s1[0]]) {
        insertin1+=i;
      }
    }
    if(!s2.empty()) {
      insertin2+=s2;
    }
    if(!s3.empty()) {
      for(auto i: Follow[cx]) {
        if(i=='#') {
          insertin3+='$';
        }
        else{
          insertin3+=i;
        }
        cout<<"FOLLOW : "<<i<<" ";
      }
    }
    //Place all the Production in their respective cells
    for (int j = 1; j <= 7; j++) {
      for(int k=0; k<insertin1.size(); k++) {
        if(M[0][j][0]==insertin1[k]) {
          string temp;
          temp+=cx;
          temp+=" -> ";
          temp+=s1;
          M[i][j]+=temp;
        }
      }
      if(!insertin2.empty()) {
        for(int k=0; k<insertin2.size(); k++) {
          if(M[0][j][0]==insertin2[k]) {
            string temp;
            temp+=cx;
            temp+=" -> i";
            M[i][j]+=temp;
          }
        }
      }
      if(!insertin3.empty()) {
        for(int k=0; k<insertin3.size(); k++) {
          if(M[0][j][0]==insertin3[k]) {
            string temp;
            temp+=cx;
            temp+=" -> #";
            M[i][j]+=temp;
          }
        }
      }
    }
  }
}
//PRINTS PARSING TABLE
void print_parse_table(){
  for (int i = 0; i < 6; i++)
  {
    cout << setw(10);
    for (int j = 0; j < 7; j++)
      cout  << setw(10) << centered(M[i][j]) <<  "|";
    cout << endl << "------------------------------------------------------------------------------" << endl;
  }
}
//HELPS IN EXTRACTING RHS OF A PRODUCTION
bool validProduction(char c)
{
  if(c == ' ' || c == '-' || c == '>')
    return false;
  return true;
}
//PROVIDES THE RHS OF PRODUCTION IN REVERSE ORDER FOR PUSHING INTO PARSING STACK 
string getproduction(char upper, char lower){
  string search;
  //Finds production at M[E,v] in the parse table
  for(int i=1; i<=5; i++) {
    if(M[i][0][0]==upper) {
      for(int j=1; j<=6; j++) {
        if(M[0][j][0]==lower)
          search = M[i][j];
      }
    }
  }
  int sz = search.size();
  string rhs;
  for (int i = sz-1; i >=0 && validProduction(search[i]); i--)
    rhs += search[i];
  //return reverse order RHS to parser()
  return rhs;
}
//PRINTS STEPS OF STRING PARSING
void printparser(stack<char> pstack,string pstring,string oprtn){
  static int i=0;
  stack<char> print;
  string stk;
  if(i==0){
    cout<<setw(25)<<centered("STACK")<<"|"<<setw(25)<<centered("STRING")<<"|"<<setw(25)<<centered("OPERATION")<<endl;
     cout << "------------------------------------------------------------------------------" << endl;
  }
  else{
    while(!pstack.empty()){
      print.push(pstack.top());
      // cout<<pstack.top()<<" -|-"<<endl;
      pstack.pop();
    }
    while(!print.empty()){
      stk.push_back(print.top());
      print.pop();
    }
    string str;
    str.push_back(char(i));
    cout<<centered(str)<<setw(25)<<centered(stk)<<"|"<<setw(25)<<centered(pstring)<<"|"<<setw(25)<<centered(oprtn)<<endl;
    }
    i++;
}
//STRING PARSER
bool parser(string s){
  bool ans = true;
  //Stack is pushed with '$' and start symbol and string pointer is initialized to 0
  stack<char> STACK;
  STACK.push('$');
  STACK.push('E');
  int i=0;
  printparser(STACK,"","");
  //loop for starting parsing
  while(!STACK.empty()) {
    //if strig is over and stack is not empty parsing fails
    if(i==(s.size()-1)&&STACK.top()!='$') {
      return false;
      break;
    }
    //poping maching elements of stack and string and moving the pointer to next position
    if(STACK.top()==s[i]) {
      STACK.pop();
      if (STACK.empty()) break;
      i++;
    }
    //start checking productions at M[E,v](Where parsing actually begins)
    while(STACK.top()!=s[i]) {
      //temp gets RHS of Production at M[E,v] in reverse order
      string temp = getproduction(STACK.top(),s[i]);
      string print1;
      string print2;
      for(int x =i ;x<s.size();x++){
        print1+=s[x];
      }
      //if there is no entry for M[E,v] in parsing table, parsing failed.
      if(temp.empty()) {
        print2="BAD INPUT";
        printparser(STACK,print1,print2);
        return false;
      }
      //if RHS is not epsilon(#), then replace top element with reverse RHS production
      //at M[E,v], i.e. pop top and push string temp in stack
      if(temp!="#") {
        STACK.pop();
        if (STACK.empty()) break;
        for(auto pr:temp) {
          STACK.push(pr);
        }
        print2 = "PUSH: ";
        print2+=temp;
        printparser(STACK,print1,print2);
      }
      //if RHS of M[E,v] is epsilon, pop current top()
      if(temp=="#") {
        print2 = "POP, PRODUCTION=#";
        printparser(STACK,print1,print2);
        STACK.pop();
        
      if (STACK.empty()) break;
      }
      if(STACK.top()==s[i]) {
        break;
      }
      if (STACK.empty())
        break;
    }
  }
  return ans;
}

int main(){
  // inputting grammar
  production['E'].push_back("TR");
  production['R'].push_back("+TR");
  production['R'].push_back("#");
  production['T'].push_back("FS");
  production['S'].push_back("*FS");
  production['S'].push_back("#");
  production['F'].push_back("(E)");
  production['F'].push_back("i");
  // establishing terminals
  terminal.insert('+');
  terminal.insert('*');
  terminal.insert('(');
  terminal.insert(')');
  terminal.insert('i');
  terminal.insert('#');
  // keeping record of productions with #(Epsilon)
  epsilon.insert('R');
  epsilon.insert('S');

  find_first('E');
  find_first('T');
  find_first('F');
  find_first('R');
  find_first('S');

  find_follow('E');
  find_follow('T');
  find_follow('F');
  find_follow('R');
  find_follow('S');

  print_first();
  cout<<endl;
  print_follow();
  cout<<endl << endl;
  parse_table();
  cout << endl;
  print_parse_table();
  cout<<endl;
  string s;
  cin>>s;
  s.push_back('$');
  if(parser(s)) {
    cout<<endl<<endl<<"SUCCESSFULL"<<endl;
  }
  else{
    cout<<endl<<endl<<"PARSING FAILED"<<endl;
  }
  return 0;
}