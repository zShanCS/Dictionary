//This is for educational purposes
#include <iostream>
#include <list>
#include <fstream>
#include <iterator>
#include <vector>
using namespace std;
//making class Trie with all the required functionality for project
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
      //gets the word
      t = t->getWord(key);
      if (t != nullptr)
      {
         cout << "\nThe words starting from " << key << " are:\n";
         display(t);
      }

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
   //official method for insertions of new words
   void addNewWord(string key, string meaning, list<string> synonyms);
   //to run as test case
   void test();
};
//constructor
Trie::Trie()
{
   //since there are no words here
   isWord = false;
   //empty string set as meaning
   meaning = "";
   //setting all children to null pointer so we know no child node exists
   for (int i = 0; i < 26; i++)
   {
      children[i] = nullptr;
   }
}
//creating dictionary from a file
//this requires using special rules
//words starts with 0 meaning with -- and synonyms have no charcter before them
//for example
//-word
//--meaning
//synonyms
//repeat
//.............
//it takes file path as input and inserts all elements in trie
void Trie::createDictionary(string fileLink)
{
   //making varables to hold each word and relevant data to hold
   string thisWord = "";
   string thisMeaning = "";
   string thisSynonym = "";
   //synonyms go in a list as explained before
   list<string> synonyms;
   //take each next line from file
   string myNextLine;
   //input file stream
   ifstream myfile;
   //opening the file
   myfile.open(fileLink);

   //we loop through the file until the end of file is reached
   if (!myfile.is_open())
   {
      cout << "\nERROR WHILE TRYING TO READ FROM FILE " << fileLink << " please check if file exists and try again\n";
      return;
   }
   while (!myfile.eof())
   {
      //getline automatically gets the next line from file
      //we are saving each new line in mynextline
      getline(myfile, myNextLine);
      //getfirstchar return where the first non '-' printable char appears
      //if 1 then word
      int firstChar = getFirstChar(myNextLine);
      //4 is for special case of start of file
      //if dictioanry text file follows all rules it should be 1 only
      if (firstChar == 1 || firstChar == 4)
      {
         //i.e we have word already saved
         if (!(thisWord == ""))
         {
            //then we insert it in to the trie
            insert(thisWord, thisMeaning, synonyms);
            //now we clear all elements from synonym list so we have an empty list for next word
            synonyms.clear();
         }
         //we use substr to get the word strating from where first char appears
         //this will delete all - -- etc
         thisWord = myNextLine.substr(firstChar);
         //continue because we have gotten what we needed from this iteration no need to try other checks
         continue;
      }
      //if string is meaning then 2
      //it shouldnt be six but isnce the given dictionary file has some error
      //i decided to use it as specila case
      if (firstChar == 2 || firstChar == 6)
      {
         //get meaning
         thisMeaning = myNextLine.substr(firstChar);
         //continue since we got  a meaning
         continue;
      }
      //0 for synonyms as defined in dictionary
      if (firstChar == 0)
      {
         //get synonym
         thisSynonym = myNextLine.substr(firstChar);
         //put synonym in our list
         synonyms.insert(synonyms.end(), thisSynonym);
         //no needd for continue since our last line of loop
      }
   }
   //this iwll insert the last word too since loop wont go up...
   // because our end of file has reached
   insert(thisWord, thisMeaning, synonyms);
   //close th efile and deallocate memory
   myfile.close();
}
//insert new word's official method
void Trie::addNewWord(string key, string meaning, list<string> synonyms)
{
   //it juts uses internal insert method
   //that is used by create dictionary method
   insert(key, meaning, synonyms);
}
//inserting a word and its meaning etc
void Trie::insert(string key, string meaning, list<string> synonyms)
{
   //insert on the node that called insert obviosuly
   Trie *thisNode = this;
   //loop till end of key is recahed
   for (int i = 0; i < key.length(); i++)
   {
      //get index its - for a and 25 for z and so on
      int c = index(key[i]);
      //if at c nothing exists
      if (thisNode->children[c] == nullptr)
      {
         //if it was null already now its nnot showing that some word exists after this
         thisNode->children[c] = new Trie();
      }
      //now we set thisnode as the node afterof char c
      //so we can enter next element's here
      thisNode = thisNode->children[c];
   }
   //when we have reached the last node of our word
   //save the actual word too
   thisNode->word = key;
   //we set the is word to true
   thisNode->isWord = true;
   //we set the meaning
   thisNode->meaning = meaning;
   //we set synonyms
   thisNode->synonyms = synonyms;
}
//check if a word exists
//amd return a poiter to its location
//this saves us from having to run the loop twice
Trie *Trie::getWord(string key)
{
   Trie *thisTrie = this;
   //if root is null it means nothing was added
   if (thisTrie == nullptr)
      //so return null because we need to end our method right here
      return nullptr;
   //if root isnt null we need to scan the whole trie for the key
   //and return it when found
   for (int i = 0; i < key.length(); i++)
   {
      //get the next node for each letter
      thisTrie = thisTrie->children[index(key[i])];
      //if we hit a nullptr it means the next letter of our key didnt exists
      //so we return nullptr as indication that word doest exist
      if (thisTrie == nullptr)
         return nullptr;
   }
   //but if nullptr was not encountered we have found our word or part of word and we return the node for that
   //so further methods can use it to find synonyms or meanings or check for existence etc
   return thisTrie;
}
//takes a string word and return true iff word exists in the tree
bool Trie::findWord(string key)
{
   //gets the node for that word
   Trie *foo = this->getWord(key);
   //if foo is null word didnt exist
   if (foo == nullptr)
      //so return fakse
      return false;
   //else we return isWord attribute of node which will be true if word exists
   return foo->isWord;
}
//takes words as input and return meaning as string
string Trie::findMeaning(string key)
{
   Trie *thisTrie = this;
   //gets node for word
   thisTrie = thisTrie->getWord(key);
   //if null or is not a word
   //we have not found the word and thus cant retun meaning
   if (thisTrie == nullptr || !thisTrie->isWord)
   {
      //in that case we print word not found
      cout << "---------------------the word \"" << key << "\" not found";
   }
   else
   {
      //else we return meaning
      return thisTrie->meaning;
   }
   //this is a place holder so that we dont encounter warnings from our IDE
   return "";
}
//takes string key and returns a vector containing all the synonyms of the word
vector<string> Trie::findSynonyms(string key)
{

   Trie *thisTrie = this;
   //gets the word node
   thisTrie = thisTrie->getWord(key);
   //if node is null or word doesnt exist
   if (thisTrie == nullptr || !thisTrie->isWord)
   {
      //we say word not found
      cout << "---------------------the word \"" << key << "\" not found";
   }
   else
   {
      //else we store all the synonyms stored in trie node for that word in a vector that we return
      int size = thisTrie->synonyms.size();
      vector<string> syn(size);
      int k = 0;
      //using enhanced for loop to quickly store all synonyms (from list) in a vector
      for (string s : thisTrie->synonyms)
      {
         syn[k++] = s;
      }
      //now we return the complete vector
      return syn;
   }
   //otherwise we return an empty vector to avoid warnings
   vector<string> synn(0);
   return synn;
}
//checks if word is allowed i.e contains lower chars only
bool allowed(string s)
{
   bool flag = true;
   char c;
   //chwck for length of word
   for (int i = 0; i < s.length(); i++)
   {
      //save each char in c
      c = s.at(i);
      //check if c is low a or greater than z
      if (c < 'a' || c > 'z')
      {
         //the we have a problem
         flag = false;
      }
   }
   //return flag which will be true iff all charcters are lower case english
   return flag;
}
//this is an internal method to get word, its meaning and synonyms from user and insert them in trie
void enterWord(Trie *t)
{
   //make storage variables
   string w, mean, s;
   list<string> synm;
   //take inputs
   cout << "\n enter word... ";
   cin >> w;
   if (allowed(w))
   {
      cout << "\n enter meaning... ";
      cin.ignore();
      getline(cin, mean);
      cout << "\n enter synonym (-1) to stop... ";
      cin >> s;
      //as long as user doesnt want to stop we take synonyms
      while (s != "-1")
      {
         //and insert synonyms into the list
         synm.insert(synm.end(), s);
         cout << "\n enter synonym (-1) to stop... ";
         cin >> s;
      }
      //all storage variables are put into the tree
      t->addNewWord(w, mean, synm);
   }
   else
   {
      cout << "\n word must have english lower caps characters only.\n";
   }
}
//this is a test method
//so we can use it in our main class for testing purposes
void Trie::test()
{

   Trie *myTrie = this;
   //create a dictionary from given file as required by instructor
   myTrie->createDictionary("dictionary.txt");
   //take choice fro user
   int choice = -1;
   //using do while loop so this runs at least once
   do
   {
      cout << "\n---------------------------------MENU--------------------------------\n";
      //give user the commands to select menu
      cout
          << " 1. insert new,\n 2. fine meaning,\n 3. find synonyms,\n 4. does word exists,\n 5. Words starting from,\n 6. DIsplay all Words\n";
      //take choice from user
      cout << "\n enter choice (use -1 to exit) . . . ";
      cin >> choice;
      string s;
      //using switch case to decide which chunk to run upon which command
      switch (choice)
      {
         //insert new word
      case (1):
         //calls enterWord method as stated above
         cout << "\nonly words containing all small caps alphabets can be inserted\n";
         enterWord(myTrie);
         break;
         //find meaning of a word
      case 2:
         cout << "\nenter word... ";
         cin >> s;
         cout << "\nmeaning is:    " << myTrie->findMeaning(s);
         break;
         //find synonyms of a word
      case 3:
         cout << "\nenter word... ";
         cin >> s;
         cout << "\n synonyms are:   \n";
         //print all synonyms via enhanced for loop
         for (string p : myTrie->findSynonyms(s))
            cout << p << "\n";
         break;
         //check for existence of a word
      case 4:
         cout << "\nenter word... ";
         cin >> s;
         //iff word found print exists
         myTrie->findWord(s) ? cout << "\nexists\n" : cout << "\ndoesnt exist\n";
         break;
         //display all words starting from specific letter(s)
      case 5:
         cout << "\nenter starting letters...  ";
         cin >> s;
         myTrie->startFrom(s);
         break;
         //display ALL words stored in trie
      case 6:
         myTrie->display(myTrie);
         break;
      }
      //do this as long as choice is not -1
      //if choice is -1 then exit
   } while (choice != -1);
}

int main()
{
   Trie *t = new Trie();
   t->test();
   system("pause");
   return 0;
}
