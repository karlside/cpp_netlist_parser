# Class Structure


```text
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
