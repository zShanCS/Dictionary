#include<iostream>
using namespace std;
class Trie
{
   //making these private so they are not usable outside this class
   //since changing these without methods could
   //render the trie useless
private:
   //isword stores true iff the current sequence makes a word its false otherwise
   bool isWord;
   //save word itself
   string word;
   //this stores meaning for words
   string meaning;
   //to store synonyms I have used list object
   //because we dont know beforehand the number of synonyms
   list<string> synonyms;
   //since we are implementing in english
   //our trie has an array of children with 26 charcter spce
   Trie *children[26];
   //this is internal method to return current word's node
   //so we can get its synonyms and meanings without having to loop through everything again
   Trie *getWord(string key);
   //this is the internal element to insert a word.
   //a similar method is available outside with name addNewElement
   void insert(string key, string meaning, list<string> synonyms);
   //display
   //this is internal method to get index that equates to the chracters value
   //for example 0 for a and 2 for c
   int index(char c)
   {
      //since a is 97 thus a-97 is zero
      //for z it will become 25
      //thus it will serve as index for our array of children
      return c - 97;
   }
   //this is used to remove the - and -- and other special characters in front of words
   //and return index of the start of first character in alphabet
   //WARNING----- this will kill capital letters
   //this rule is only for dictionary creation from file
   //since dictionary.txt file follows certain rules
   //WARNING----- cpp allows you to access array out of bounds
   //so i am  ignoring capital chars
   //but later if user inserts custom words in different languages they will work too
   int getFirstChar(string s)
   {
      int i;
      //till end is encountered loop continues
      //this assumes the string has atleast one character
      //otherwise this might give exception
      //altho in ideal case...
      //all lines in our dictionary will have a character a to z
      //at pos 3 or less
      for (i = 0; i < s.length(); i++)
      {
         //s.at(i) is a c++ method that returns a charcters at some posutuons in a string
         char c = s.at(i);
         int num = c;
         //all printable characters except '-'
         if (num > 31 && num < 127 && num != 45)
         {
            //if a char from 'a' to 'z' is found break loop
            //and return it
            break;
         }
      }
      return i;
   }

public:
   int countWords = 0;
   //this displays all words starting from some root
   void display(Trie *t)
   {
      if (t == nullptr)
      {
         return;
      }
      if (t->isWord)
      {
         countWords++;
         cout << t->word << "\n";
      }
      for (int i = 0; i < 26; i++)
      {
         if (t->children[i] == nullptr)
         {
            continue;
         }
         else
         {
            display(t->children[i]);
         }
      }
      return;
   }

   //prints all words starting from some letter(s)
   void startFrom(string key)
   {
      Trie *t = this;
      t = t->getWord(key);
      if (t != nullptr)
         display(t);
      else
         cout << "\n-----------nothing found\n";
   }

   //these methods will be visible outside our class
   //constructor
   Trie();

   //to create dictionary it takes name or link of file
   //and inserts the words into the trie object
   void createDictionary(string fileName);
   //return true only iff word is found in trie
   bool findWord(string key);
   //returns a vector of synonyms iff word is found
   vector<string> findSynonyms(string key);
   //return meaning as a string iff word exists
   string findMeaning(string key);
   //debug methods
   void addNewWord(string key, string meaning, list<string> synonyms);
   //to run as test case
   void test();
};
int main(){
return 0;
}
