# Class Structure


# Object Hierarchy
NetlistStatement
├── InstanceStatement
│   └── ...
├── AnalysisStatement
│   └── ...
└── MultilineStatement 
    ├── ... 
    └── ...

# Compile
'g++ -std=c++17 -I dependencies/ main.cpp dependencies/*.cpp'

# Words - Object Structure and Inheritance
Word is an abstract class. Specificly it has the analyse() method return a pointer to itself (this).

Below Word is the first inlevel of inheritance, as BaseWord class that reads in charactes. When this calls analysis it try to return another DerivedWordClass. These derived word classes inherit the analyse() method that points a it self.

The benefi of this is that I can analyse words as they are read, and save time when I iterate over the vector a second time. 

## entires vector
The entires vector holds pointes to Word object. These as all derived classes, and will use dynamic dispact when called. When running analyse_entires() on the Line object, the method loops through the entires vector, replacing the pointers with pointesr to new objects. If the object already is analyes, it will return a pointer to itself. 

## Continues Word Analysis
While the FSM is running, adding up characters to make Words, I can add some logic that tries to identify what type the word is, and turn it into the correct object immediatly. 

This can be done at a later stage, and don't need to happen to implement the above idea. 

## All of the above is wrong
I think I would rather have word just be single class. Then have Line have the logic described above.

Words have the following members.

string text;
string key;
string value;
bool has_value;
void parse();
bool has_been_parsed;
void add_char(char input);
string get_text()

When characters are added using add_char, they are appended to text. When calling parse() the text is parsed into key and value. 

# Input Formatting
The goal is to format everything as it is read in, then move the text around using pointers. This ensures that the text is only 'read' once. 

## Copy on write apporeache
For derivetives of StatementWord, where different entries are assigned to different variables text are only moved to the different variables once it becomes necessary. 
