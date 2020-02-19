
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
double prob_ham;
double prob_spam;

void get_training_data(string &filename,std::vector<string> &sentences,std::vector<string> &ham_sentences,std::vector<string> &spam_sentences);
void eachline_text_preprocessing(string &original_lines,std::vector<string> &original_words);
void alllines_text_preprocessing(std::vector<string> &all_lines,std::vector<string> &original_words);
void get_word_set(std::map<string, int> &word_occurent, std::vector<string> & single_word);
void convert_vector_to_set(std::vector<string> &vec, std::set<string> &setc);

void add_training_data(string &v1, string &v2){
  std::ofstream outfile("spam2.csv",std::ios_base::app);
	outfile<<v1<<","<<v2<<"\n";
	outfile.close();
}

void training(string spamdataset, string spam_probability, string ham_probability){

  //std::ofstream sfile("spam_probability.csv");
//  std::ofstream hfile("ham_probability.csv");
  //string filename= "spam.csv";

  std::ofstream sfile(spam_probability);
 std::ofstream hfile(ham_probability);
  string filename= spamdataset;

  std::vector<string> all_sentences;
  std::vector<string> ham_sentences;
  std::vector<string> eachline_ham_words;
  std::vector<string> eachline_spam_words;
  std::vector<string> spam_sentences;
  std::vector<string> eachlines_words;
  std::vector<string> alllines_words;
  std::map<string,int> wordlist_ham_map;
  std::map<string,int> wordlist_spam_map;
  std::map<string,int> wordlist_map;
  std::set<string> eachline_spam_set;
  std::set<string> eachline_ham_set;

  get_training_data(filename,all_sentences,ham_sentences,spam_sentences);
  alllines_text_preprocessing(all_sentences,alllines_words);
  get_word_set(wordlist_map_init,alllines_words);
  wordlist_map = wordlist_map_init;
  for(std::map<string, int> ::iterator iter = wordlist_map.begin();iter != wordlist_map.end();iter++)
  iter->second ++;

  wordlist_spam_map = wordlist_map;
  wordlist_ham_map = wordlist_map;

  double spam_total_words = 0;
  for(int i = 0; i< spam_sentences.size();i++){
    eachline_spam_words.clear();
    eachline_text_preprocessing(spam_sentences[i],eachline_spam_words);
    eachline_spam_set.clear();
    convert_vector_to_set(eachline_spam_words,eachline_spam_set);
    for(std::set<string> ::iterator it = eachline_spam_set.begin(); it!= eachline_spam_set.end();it++){
      std::map<string, int> ::iterator iter = wordlist_spam_map.find(*it);
      if(iter != wordlist_spam_map.end())
        iter->second ++;
      }
    }
  for(std::map<string, int> ::iterator iter = wordlist_spam_map.begin();iter != wordlist_spam_map.end();iter++)
  spam_total_words += iter->second;
  sfile<<"spam_total_words"<<","<<spam_total_words<<"\n";
  for(std::map<string, int> ::iterator iter = wordlist_spam_map.begin();iter != wordlist_spam_map.end();iter++){
//  double temp = log10(iter->second/spam_total_words);
  sfile<<iter->first<<","<<iter->second<<std::endl;
  }
sfile.close();
//************************************************************************************************************//
  double ham_total_words = 0;
  for(int i = 0; i< ham_sentences.size();i++){
    eachline_ham_words.clear();
    eachline_text_preprocessing(ham_sentences[i],eachline_ham_words);
    eachline_ham_set.clear();
    convert_vector_to_set(eachline_ham_words,eachline_ham_set);
    for(std::set<string> ::iterator it = eachline_ham_set.begin(); it!= eachline_ham_set.end();it++){
      std::map<string, int> ::iterator iter = wordlist_ham_map.find(*it);
      if(iter != wordlist_ham_map.end())
        iter->second ++;
      }
    }

  for(std::map<string, int> ::iterator iter = wordlist_ham_map.begin();iter != wordlist_ham_map.end();iter++)
    ham_total_words += iter->second;
    hfile<<"ham_total_words"<<","<<ham_total_words<<"\n";
  for(std::map<string, int> ::iterator iter = wordlist_ham_map.begin();iter != wordlist_ham_map.end();iter++){
//    double temp= log10(iter->second/ham_total_words);
    hfile<<iter->first<<","<<iter->second<<std::endl;
  }
  hfile.close();
}




void get_training_data(string &filename,std::vector<string> &sentences,std::vector<string> &ham_sentences,std::vector<string> &spam_sentences){
	int ham_number = 0, spam_number = 0;
  string original_lines,lines;
  std::ifstream infile(filename);
//  if(!infile)
  //   std::cout<<"please put the spam.csv file into the folder "<<"\n";
	while(getline(infile, lines)){
    int comma_position = lines.find(',');
    string substr1 = lines.substr(0, comma_position);
    string substr2 = lines.substr(comma_position + 1);

    if(substr1 =="spam"|substr1 =="ham"){
      sentences.push_back(substr2);
		 if(substr1=="spam"){
	     spam_sentences.push_back(substr2);
       spam_number++;
       }
		else if(substr1 =="ham"){
      ham_sentences.push_back(substr2);
      ham_number++;
    }
  }
	}
 prob_ham = ham_number/double(ham_number+spam_number);

 prob_spam = 1 - prob_ham;
//  std::cout<<"prob_ham: "<<prob_ham<<","<<"prob_spam" <<prob_spam;

}


void eachline_text_preprocessing(string &original_lines,std::vector<string> &original_words){
  string word;
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
    if(word.size() > 3)
      original_words.push_back(word);
  }

}



void alllines_text_preprocessing(std::vector<string> &all_lines,std::vector<string> &original_words){
  for(int i = 0; i< all_lines.size(); i++)
    eachline_text_preprocessing(all_lines[i],original_words);
}



void get_word_set(std::map<string, int> &word_occurent, std::vector<string> & single_word){
 std::pair<string, int> word_initcount;
    for(int j =0 ; j< single_word.size(); j++){
      word_initcount = std::make_pair(single_word[j], 0);
      word_occurent.insert(word_initcount);
  }
}


void convert_vector_to_set(std::vector<string> &vec, std::set<string> &setc){
  for(std::vector<string>::iterator iter = vec.begin();iter != vec.end(); ++iter){
    setc.insert(*iter);
  }
}


int main(int argc, char *argv[]){
   training(argv[2], argv[4], argv[6]);
}
