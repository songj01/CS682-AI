

//#include<spam_filter.h>
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<sstream>
#include<cctype>
#include<regex>
#include<iterator>
#include<map>
#include<set>
#include<cmath>
#include<numeric>

using std::string;

 std::map<string,int> wordlist_map_init;
 double prob_spam = 0.133077;
 double prob_ham =0.866923;

void read_map_file(string &filename, std::vector<double> &arr);
void eachline_text_preprocessing(string &original_lines,std::vector<string> &original_words);
void convert_vector_to_set(std::vector<string> &vec, std::set<string> &setc);


void classify(string testfile, string spam_probability, string ham_probability, string classification){
  string test_lines;
  std::set<string> test_line_set;
  std::vector<string> test_line_words;
  std::map<string,int> test_map;
  std::vector<double> spam_prob_vec;
  std::vector<double> ham_prob_vec;
  std::vector<int> test2_vec;
  std::vector<string> test_sentencs;

//  std::ofstream out_final_file("classification.csv");
std::ofstream out_final_file(classification);
  //string filename_ham = "ham_probability.csv";
    string filename_ham =  ham_probability;
  //string filename_spam = "spam_probability.csv";
    string filename_spam = spam_probability;
  read_map_file(filename_spam, spam_prob_vec);
  wordlist_map_init.clear();
  read_map_file(filename_ham, ham_prob_vec);
  //string testfilename = "testing.csv";
  string testfilename = testfile;
  std::ifstream in_test_file(testfilename);
//  if(!in_test_file)
  //   std::cout<<"please put the testing.csv file into the folder "<<"\n";
  while(getline(in_test_file,test_lines))
      test_sentencs.push_back(test_lines);
  for(int i = 0; i< test_sentencs.size();i++){
    test_map = wordlist_map_init;
    test_line_words.clear();
    eachline_text_preprocessing(test_sentencs[i], test_line_words);
    test_line_set.clear();
    convert_vector_to_set(test_line_words,test_line_set);

    for(std::set<string> ::iterator it = test_line_set.begin(); it!= test_line_set.end();it++){
      std::map<string, int> ::iterator iter = test_map.find(*it);
      if(iter != test_map.end())
        {iter->second ++;}
      }
    test2_vec.clear();
      for(std::map<string,int> ::iterator iter1 = test_map.begin();iter1 !=test_map.end();iter1++){
          int temp = iter1->second;
          test2_vec.push_back(temp);
        }

  double init = 0;
  double prob_words_given_spam = std::inner_product(spam_prob_vec.begin() ,spam_prob_vec.end(),test2_vec.begin(),init);
  double prob_words_given_ham = std::inner_product(ham_prob_vec.begin() ,ham_prob_vec.end(),test2_vec.begin(),init);
  double prob_spam_given_words = prob_words_given_spam + log10(prob_spam);
  double prob_ham_given_words = prob_words_given_ham + log10(prob_ham);

  if(prob_spam_given_words > prob_ham_given_words)
  out_final_file<<"spam"<<"\n";
  else
  out_final_file<<"ham"<<"\n";

  }

  out_final_file.close();
}




void read_map_file(string &filename, std::vector<double> &arr){
  string str1,str2,lines;
  std::vector<int> arr1;
  int  i =0;
  std::ifstream in_prob_spam_file(filename);
  while(getline(in_prob_spam_file, lines)){
    std::stringstream temp_line(lines);
    getline(temp_line,str1,',');
    if(str1 != "spam_total_words"|str1 != "ham_total_words")
        wordlist_map_init.insert(std::make_pair(str1,0));
    getline(temp_line,str2,'\n');
    int dou = std::stoi(str2);
     arr1.push_back(dou);
  }
  for(int i = 1; i<arr1.size();i++){
    double dou2 = log10(arr1[i]/double(arr1[0]));
    arr.push_back(dou2);
  }

  in_prob_spam_file.close();
}

void eachline_text_preprocessing(string &original_lines,std::vector<string> &original_words){
  string word;
  //original_lines = std::regex_replace(original_lines,std::regex(",(?=[\\d,]*\\.\\d{2}\\b)"), "ff");
  original_lines = std::regex_replace(original_lines,std::regex("\\b[\\w\\-.]+?@\\w+?\\.\\w{2,4}\\b"),"emailaddr");
  original_lines = std::regex_replace(original_lines,std::regex("(http[s]?\\S+)|(\\w+\\.[A-Za-z]{2,4}\\S*)"),"httpaddr");
  original_lines = std::regex_replace(original_lines,std::regex("£|\\$"), "moneysymb");
  original_lines = std::regex_replace(original_lines,std::regex("\\b(\\+\\d{1,2}\\s)?\\d?[\\-(.]?\\d{3}\\)?[\\s.-]?\\d{3}[\\s.-]?\\d{4}\\b"), "phonenumbr");
  original_lines = std::regex_replace(original_lines,std::regex("\\d+(\\.\\d+)?"), "numbr");

  for(int i =0; i<original_lines.size();i++){
   if(ispunct(original_lines[i]))
      original_lines.replace(i,1,1, ' ');
   else if(isupper(original_lines[i]))
       original_lines[i] = tolower(original_lines[i]);
  }

  std::stringstream stream_lines(original_lines);
  while(getline(stream_lines, word,' ')){
    if(word.size() > 2)
      original_words.push_back(word);
  }

}

void convert_vector_to_set(std::vector<string> &vec, std::set<string> &setc){
  for(std::vector<string>::iterator iter = vec.begin();iter != vec.end(); ++iter){
    setc.insert(*iter);
  }
}




int main(int argc, char *argv[]){
  classify(argv[2],argv[4],argv[6],argv[8]);
}
