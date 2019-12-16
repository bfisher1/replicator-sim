#include "resourceGraph.hpp"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdlib.h>
#include <boost/algorithm/string.hpp>

Action::Action(string actionName) {
  name = actionName;
}

ResourceProduct::ResourceProduct(ResourceOutput *out, Action *act, vector<ResourceInput *> *in) {
  output = out;
  action = act;
  inputs = in;
}


/*
 Splits a list of ', ' separated values inside of brackets.
*/
void addResource(string line, vector<string> *resource) {
  int startBracket = line.find_first_of("{");
  int endBracket = line.find_first_of("}");
  if( startBracket >= 0 && endBracket >= 0) {
    string raw = line.substr(startBracket + 1, endBracket - startBracket - 1).data();
    vector<string> rawResources;
    boost::split(rawResources, raw, boost::is_any_of(", "), boost::token_compress_on);
    resource->insert(resource->end(), rawResources.begin(), rawResources.end());
  }
  else {
    printf("Error reading raw materials\n");
  }
}

void addActions(ResourceGraph *graph, ifstream *inFile) {
  static string line;
  while(getline(*inFile, line) && line.rfind("}", 0) != 0) {
    printf("%s -- \n", line.data());
  }
}

/*
 Input string is either `name` or `name(quantity)`
*/
ResourceInput::ResourceInput(string inputString) {
  int parenStartIdx = inputString.find("(");
  if(parenStartIdx == string::npos) {
    name = inputString;
    amountRequired = 1;
  }
  else {
    int parenEndIdx = inputString.find(")");
    name = inputString.substr(0, parenStartIdx);
    amountRequired = atof(inputString.substr(parenStartIdx + 1, parenEndIdx - parenStartIdx).data());
  }
}

/*
 Output string is either `name` or `name(quantity)`
*/
ResourceOutput::ResourceOutput(string outputString) {
  int parenStartIdx = outputString.find("(");
  if(parenStartIdx == string::npos) {
    name = outputString;
    amountCreated = 1;
  }
  else {
    int parenEndIdx = outputString.find(")");
    name = outputString.substr(0, parenStartIdx);
    amountCreated = atof(outputString.substr(parenStartIdx + 1, parenEndIdx - parenStartIdx).data());
  }
}

/*
 For processing input functions in the form func(input1,input2(quantity),...)
*/
string processFuncName(string func) {
  return func.substr(0, func.find_first_of("("));
}

vector<ResourceInput*> *processInputResources(string func) {
  int parenOpenIdx = func.find_first_of("(");
  int parenCloseIdx = func.find_last_of(")");
  
  // just the input params without parens
  string params = func.substr(parenOpenIdx + 1, parenCloseIdx - parenOpenIdx - 1);
  vector<string> inputStrings;
  boost::split(inputStrings, params, boost::is_any_of(","), boost::token_compress_on);
  vector<ResourceInput*> *resourceInputs = new vector<ResourceInput *>();
  for(int i = 0; i < inputStrings.size(); i++) {
    resourceInputs->push_back(new ResourceInput(inputStrings[i]));
  }
  return resourceInputs;
}


ResourceGraph::ResourceGraph(string fileName) {
  ifstream inFile;
  inFile.open(fileName.data());
    if (!inFile) {
        cout << "Unable to open file";
        exit(1); // terminate with error
    }
    string line;
    
    while (getline(inFile, line)) {
      if(line.rfind("Raw =", 0) == 0) {
        addResource(line, &raw);
      }
      else if(line.rfind("Molds =", 0) == 0) {
        addResource(line, &molds);
      }
      else if(line.rfind("Producers =", 0) == 0) {
        addResource(line, &producers);
      }
      else if(line.rfind("Actions {", 0) == 0) {
        Action *action;
        while (getline(inFile, line) && !(line.rfind("}", 0) == 0)) {
          // if the line ends with a ':'
          if(line.size() - 1 == line.rfind(":", line.size() - 1)) {
            boost::erase_all(line, ":");
            boost::erase_all(line, "\t");
            boost::erase_all(line, " ");
            action = new Action(line);
            actions[action->name] = action;
          }
          else if(line.rfind("Precond:", string::npos) != string::npos) {
            int start = line.find_first_of(":") + 1;
            addResource(line.substr(start, line.size()), &action->preconds);
          }
          else if(line.rfind("Effect:", string::npos) != string::npos) {
            int start = line.find_first_of(":") + 1;
            addResource(line.substr(start, line.size()), &action->effects);
          }
          else if(line.rfind("Duration =", string::npos) != string::npos) {
            int start = line.rfind("Duration =", string::npos) + strlen("Duration =");
            string durString = line.substr(start + 1, line.size());
            action->duration = atof(durString.data());
          }
          else {
            cout << "Unable to parse actions for line:" << endl;
            cout << line << endl;
            exit(0);
          }
        }
      }
      else if(line.rfind("Resource Transformations:", 0) == 0) {
        while (getline(inFile, line)) {
          if(line.size() == 0) {
            continue;
          }
          int start = line.find_first_of("=");
          string product = line.substr(0, start);
          boost::erase_all(product, " ");

          ResourceOutput *output = new ResourceOutput(product);

          string func = line.substr(start + 1, line.size());
          boost::erase_all(func, " ");

          string funcName = processFuncName(func);
          vector<ResourceInput*> *inputs = processInputResources(func);
          resources[output->name] = new ResourceProduct(output, actions[funcName], inputs);
        }
      }
      
    }
    
    inFile.close();
}

void printVector(vector<string> *vec) {
  vector<string> vec2 = *vec;
  cout << " ";
  for(int i = 0; i < vec2.size(); i++) {
    cout << vec2[i] << " ";
  }
  cout << endl;
}

void ResourceGraph::print() {
  cout << "Raw Materials:" << endl;
  printVector(&raw);
  cout << "Molds:" << endl;
  printVector(&molds);
  cout << "Producers:" << endl;
  printVector(&producers);
  cout << endl << "Actions:" << actions.size() << endl;
  auto it = actions.begin();
  while(it != actions.end()) {
    Action *action = it->second;
    cout << " " << action->name << " (" << action->duration << ")" << endl;
    cout << "  Preconds:" << endl;
    for(int j = 0; j < action->preconds.size(); j++) {
      cout << "    " << action->preconds[j] << " " << endl;
    }
    cout << "  Effects:" << endl;
    for(int j = 0; j < action->effects.size(); j++) {
      cout << "    " << action->effects[j] << " " << endl;
    }
    it++;
  }
  cout << endl << "Resources:" << endl;
  auto rsrcIt = resources.begin();
  while(rsrcIt != resources.end()) {
    ResourceProduct *resource = rsrcIt->second;
    cout << resource->output->name << " = " << resource->action->name << endl;
    vector<ResourceInput*> in = *resource->inputs;
    for(int i = 0; i < resource->inputs->size(); i++) {
      cout << "  " << in[i]->name << endl;
    }
    rsrcIt++;
  }
  // auto it = 
  // for()
  // cout << endl;
}

