#include<fstream>
#include<iostream>
#include<vector>
using namespace std;

void textfileprocess(){
vector<string> classes;
vector<string> contents;
string textfile ="SMSnew",lines;
ifstream infile(textfile);
ofstream outfile1("classes.csv");
ofstream outfile2("testing.csv");
while(getline(infile,lines)){
  //int position = lines.find(' ');
  string str1 = lines.substr(0,4);

  string str2 = lines.substr(4);
  cout<<str2<<endl;
  classes.push_back(str1);
  contents.push_back(str2);
}


for(int i =0; i<classes.size();i++){
   outfile1<<classes[i]<<"\n";
   outfile2<<contents[i]<<"\n";
}
infile.close();
outfile1.close();
outfile2.close();
}












int main(){
 textfileprocess();
return 0;
}
