#include <iostream>
#include <string>
#include <fstream>
#include <vector>
void translate(std::vector<std::string>&);
void tokenazing(std::ifstream& fin);
void do_decleration(std::vector <std::string>& vec);
void do_operation(std::vector <std::string>& vec);
bool do_condition(std::vector <std::string>& vec);
void do_loop(std::vector<std::string>& vector);
void do_printing(std::vector<std::string>& vector);
int ioperators(std::string o, std::string a, std::string b);
double doperators(std::string o, std::string a, std::string b);
std::string soperator(std::string a, std::string b);
bool boperators(std::string o, std::string a, std::string b);
void do_body_cond(std::vector <std::string>& tokens);
bool boper(std::string o, bool a, bool b);
bool bop(std::string, std::string a);
std::vector<std::string> cond;
void det_cond(std::vector<std::string>& tokens);
std::vector <std::vector<std::string>> body;

struct variable {
    std::string name;
    std::string type;
    void* value;
};
std::vector<variable> vars;

int main() {
    std::string path;
    std::ifstream fin;
    fin.open("Text.txt");
    if (fin.is_open()) {
        tokenazing(fin);
    }
    else {
        std::cout << "Failed to open the file" << std::endl;
    }
}
std::string w;
bool mutq = false;
bool condres = false;
void tokenazing(std::ifstream& fin) {
    while (!fin.eof()) {
        std::string line;
        getline(fin, line);
        std::vector <std::string> tokens;
        std::string tmp;
        for (int i = 0; i < line.length(); ++i) {
            if (line[i] != ' ' && line[i] != '\0') {
                tmp += line[i];
            }
            else {
                tokens.push_back(tmp);
                tmp = "";
            }
        }
        if (tokens.size() == 0) {
            continue;
        }
        if (mutq != true ) translate(tokens);
        if (w == "ete" ) {
            if (mutq != true) {
                det_cond(tokens);
                tokens.push_back("{");
                for (int i = 0; i < tokens.size(); ++i) {
                    if (tokens[i] == "{") {
                        mutq = true;
                        break;
                    }
                    else continue;
                }
                continue;
            }
        }
        if (mutq == true && w == "ete") {
            do_body_cond(tokens);
        }
       if (w == "qani") {
            if (mutq != true) {
                det_cond(tokens);
                tokens.push_back("{");
                for (int i = 0; i < tokens.size(); ++i) {
                    if (tokens[i] == "{") {
                        mutq = true;
                        break;
                    }
                }
                continue;
            }
        }
        if (mutq == true && w == "qani") {
                int i = 0;
                if (tokens[i] == "}") mutq = false;
                if (mutq == true) {
                    if (tokens[0] != "}") {
                        body.push_back(tokens);
                    }
                    continue;
                }
              bool  u = do_condition(cond);
             while (u == true) {
                 for (int i = 0; i < body.size(); ++i) {
                     translate(body[i]);
                 }
                 u = do_condition(cond);
             }
        }
    }
}

void translate(std::vector<std::string>& tokens) {
    
    if (tokens[0] == "amb" || tokens[0] == "rac" || tokens[0] == "tox" || tokens[0] == "tram") {
        do_decleration(tokens);
    }
    else if (tokens[0] == "ete") {
        cond.clear();
        w = "ete";
        return;
    }
    else if (tokens[0] == "qani") {
        cond.clear();
        w = "qani"; 
        return;
    }
    else if (tokens[0] == "tpel") {
        do_printing(tokens);
    }
    else {
        bool a = false;
        for (int j = 0; j < vars.size(); ++j) {
            if (tokens[0] == vars[j].name) {
                do_operation(tokens);
                return;
            }
        }
        std::cout << "ERROR:: undefined variable  " << tokens[0] << std::endl;
    }
}

void do_decleration(std::vector <std::string>& vec) {
    for (int i = 0; i < vec.size(); ++i) {
        if (vec[i] == ";") {
            break;
        }
        variable var;
        var.type = vec[i];
        ++i;
        var.name = vec[i];
        ++i;
        if (vec[i] == ",") {
            continue;
        }
        else if (vec[i] == "=") {
            ++i;
        }
        if (var.type == "amb") {
            var.value = static_cast<void*>(new int);
            *(static_cast<int*>(var.value)) = stoi(vec[i]);
        }
        else if (var.type == "rac") {
            var.value = static_cast<void*>(new double);
            *(static_cast<double*>(var.value)) = stod(vec[i]);
        }
        else if (var.type == "tox") {
            var.value = static_cast<void*>(new std::string);
            *(static_cast<std::string*>(var.value)) = vec[i].substr(1, (vec[i].length() - 2));
        }
        else if (var.type == "tram") {
            var.value = static_cast<bool*>(new bool);
            if (vec[i] == "false" || vec[i] == "0") {
                *(static_cast<bool*>(var.value)) = false;
            }
            else {
                *(static_cast<bool*>(var.value)) = true;
            }
        }
        else std::cout << "ERROR:: unknown type";
        vars.push_back(var);
    }
}

void do_operation(std::vector <std::string>& vec) {

    if (vec[1] == "=") {
        for (int i = 0; i < vars.size(); ++i) {
            if (vars[i].name == vec[0]) {
                if (vars[i].type == "amb") {
                    if (vec.size() > 3 && (vec[3] == "+" || vec[3] == "-" || vec[3] == "*" || vec[3] == "/")) {
                        *(static_cast<int*> (vars[i].value)) = ioperators(vec[3], vec[2], vec[4]);
                    }
                    else {
                        bool a = false;
                        for (int j = 0; j < vars.size(); ++j) {
                            if (vars[j].name == vec[2] && vec.size() == 3) {
                                *(static_cast<int*> (vars[i].value)) = *(static_cast<int*> (vars[j].value));
                                a = true;
                            }
                        }
                        if (a == false) {
                            *(static_cast<int*> (vars[i].value)) = stoi(vec[2]);
                        }

                    }
                }
                else if (vars[i].type == "rac") {
                    if (vec.size() > 3 && (vec[3] == "+" || vec[3] == "-" || vec[3] == "*" || vec[3] == "/")) {
                        *(static_cast<double*> (vars[i].value)) = doperators(vec[3], vec[2], vec[4]);
                    }
                    else {
                        bool a = false;
                        for (int j = 0; j < vars.size(); ++j) {
                            if (vars[j].name == vec[2] && vec.size() == 3) {
                                *(static_cast<double*> (vars[i].value)) = *(static_cast<double*> (vars[j].value));
                                a = true;
                            }
                        }
                        if (a == false) {
                            *(static_cast<double*> (vars[i].value)) = stod(vec[2]);
                        }

                    }
                }
                else if (vars[i].type == "tox") {
                    if (vec.size() > 3 && vec[3] == "+") {
                        *(static_cast<std::string*> (vars[i].value)) = soperator(vec[2], vec[4]);
                    }
                    else {
                        bool a = false;
                        for (int j = 0; j < vars.size(); ++j) {
                            if (vars[j].name == vec[2] && vec.size() == 3) {
                                *(static_cast<std::string*> (vars[i].value)) = *(static_cast<std::string*> (vars[j].value));
                                a = true;
                            }
                        }
                        if (a == false) {
                            *(static_cast<std::string*> (vars[i].value)) = vec[2].substr(1, (vec[2].length() - 2));
                        }

                    }
                }
                else if (vars[i].type == "tram") {
                    if (vec.size() > 3 && vec[2] == "!") {
                        for (int j = 0; j < vars.size(); ++j) {
                            if (vars[j].name == vec[3]) {
                                *(static_cast<bool*> (vars[i].value)) = !(*(static_cast<bool*> (vars[j].value)));
                                std::cout << *(static_cast<bool*> (vars[i].value));
                            }
                        }

                    }
                    else if (vec.size() > 3 && (vec[3] == "&&" || vec[3] == "||" ||vec[3]==">" || vec[3]=="==" || vec[3]=="<")) {
                        *(static_cast<bool*> (vars[i].value)) = boperators(vec[3], vec[2], vec[4]);
                    }
                    else {
                        bool a = false;
                        for (int j = 0; j < vars.size(); ++j) {
                            if (vars[j].name == vec[2] && vec.size() == 3) {
                                *(static_cast<bool*> (vars[i].value)) = *(static_cast<bool*> (vars[j].value));
                                a = true;
                            }
                        }
                        if (a == false) {
                            if (vec[2] == "false" || vec[2] == "0") {
                                *(static_cast<bool*>(vars[i].value)) = false;
                            }
                            else {
                                *(static_cast<bool*>(vars[i].value)) = true;
                            }
                        }
                    }
                }
            }
        }
    }
}

void do_body_cond(std::vector <std::string>&  tokens) {  
    if(condres==true) {
        int i = 0;
        if (tokens[i] == "}") mutq = false; 
        if (mutq == true) {
            translate(tokens);
            return;
        }
    }
    else {
        int i = 0;
        if (tokens[i] == "}") mutq = false; 
        return; }
}

void det_cond(std::vector<std::string>& tokens) {
    int t = 1;

    while (tokens[t] != ")") {
        if (tokens[t] == "(") {
            ++t;
        }
        cond.push_back(tokens[t]);
        ++t;
    }
    do_condition(cond);
}

bool do_condition(std::vector<std::string>& cond) {
    bool o=false;
    if (cond.size() == 1) {
        for (int i = 0; i < vars.size(); ++i) {
            if (vars[i].name == cond[0] && vars[i].type == "tram") {
                if (*(static_cast<bool*>(vars[i].value)) == true) {
                    condres = true;
                    return true;
                }
                else {
                    condres = false;
                    return false;
                }
            }
        }
    }
    else if (cond.size() == 2) {
        o = bop(cond[0], cond[1]);
        if (o == true) {
            condres = true;
            return true;
        }
        else {
            condres = false;
            return false;
        }
    }
    else if (cond.size() == 3) {
        o = boperators(cond[1], cond[0], cond[2]);
        if (o == true) {
            condres = true;
            return true;
        }
        else {
            condres = false;
            return false;
        }
    }
    else if (cond.size() == 7) {
        bool a1 = boperators(cond[1], cond[0], cond[2]);
        bool a2 = boperators(cond[5], cond[4], cond[6]);
        o = boper(cond[3], a1, a2);
        if (o == true) {
            condres = true;
            return true;
        }
        else {
            condres = false;
            return false;
        }
    }
}


void do_printing(std::vector<std::string>& vec) {
    for (int i = 0; i < vec.size(); ++i) {
        if (vec[i] == ";") {
            break;
        }
        if (i == 0) ++i;
        if (vec[i] == "->") {
            ++i;
        }
        for (int j = 0; j < vars.size(); ++j) {
            if (vec[i] == vars[j].name) {
                if (vars[j].type == "amb") {
                    std::cout << *(static_cast<int*> (vars[j].value)) << " ";
                }
                else if (vars[j].type == "rac") {
                    std::cout << *(static_cast<double*> (vars[j].value)) << " ";
                }
                else if (vars[j].type == "tox") {
                    std::cout << *(static_cast<std::string*> (vars[j].value)) << " ";
                }
                else if (vars[j].type == "tram") {
                    std::cout << *(static_cast<bool*> (vars[j].value)) << " ";
                }
                break;
            }
            else if (vec[i] == "(") {
                ++i;
                while (vec[i] != ")") {
                    if (vec[i] == "et") {
                        std::cout << std::endl;
                        ++i;
                    }
                    else {
                        std::cout << vec[i] << " ";
                        ++i;
                    }
                }
                continue;
            }
            
        }
    }
}

int ioperators(std::string o, std::string a, std::string b) {
    int size = vars.size();
    if (o == "+") {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                if ((vars[i].name == a && vars[j].name == b)) {
                    int x = *(static_cast<int*> (vars[i].value));
                    int y = *(static_cast<int*> (vars[j].value));
                    return x + y;
                }
            }
            if (vars[i].name == a) {
                int x = *(static_cast<int*> (vars[i].value));
                int y = stoi(b);
                return x + y;
            }
        }
    }
    if (o == "-") {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                if ((vars[i].name == a && vars[j].name == b)) {
                    int x = *(static_cast<int*> (vars[i].value));
                    int y = *(static_cast<int*> (vars[j].value));
                    return x - y;
                }
                else if (vars[j].name == b) {
                    int x = *(static_cast<int*> (vars[j].value));
                    int y = std::stoi(a);
                    return x - y;
                }
            }
            if (vars[i].name == a) {
                int x = *(static_cast<int*> (vars[i].value));
                int y = stoi(b);
                return x - y;
            }
        }
    }
    if (o == "*") {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                if ((vars[i].name == a && vars[j].name == b)) {
                    int x = *(static_cast<int*> (vars[i].value));
                    int y = *(static_cast<int*> (vars[j].value));
                    return x * y;
                }
                else if (vars[j].name == b) {
                    int x = *(static_cast<int*> (vars[j].value));
                    int y = std::stoi(a);
                    return x * y;
                }
            }
            if (vars[i].name == a) {
                int x = *(static_cast<int*> (vars[i].value));
                int y = stoi(b);
                return x * y;
            }
        }
    }
    if (o == "/") {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                if ((vars[i].name == a && vars[j].name == b)) {
                    int x = *(static_cast<int*> (vars[i].value));
                    int y = *(static_cast<int*> (vars[j].value));
                    return x / y;
                }
                else if (vars[j].name == b) {
                    int x = *(static_cast<int*> (vars[j].value));
                    int y = std::stoi(a);
                    return x / y;
                }
            }
            if (vars[i].name == a) {
                int x = *(static_cast<int*> (vars[i].value));
                int y = stoi(b);
                return x / y;
            }
        }
    }
}

double doperators(std::string o, std::string a, std::string b) {
    int size = vars.size();
    if (o == "+") {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                if ((vars[i].name == a && vars[j].name == b)) {
                    double x = *(static_cast<double*> (vars[i].value));
                    double y = *(static_cast<double*> (vars[j].value));
                    return x + y;
                }
                else if (vars[j].name == b) {
                    double x = *(static_cast<double*> (vars[j].value));
                    double y = std::stod(a);
                    return x + y;
                }
            }
            if (vars[i].name == a) {
                double x = *(static_cast<double*> (vars[i].value));
                double y = stod(b);
                return x + y;
            }
        }
    }
    if (o == "-") {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                if ((vars[i].name == a && vars[j].name == b)) {
                    double x = *(static_cast<double*> (vars[i].value));
                    double y = *(static_cast<double*> (vars[j].value));
                    return x - y;
                }
                else if (vars[j].name == b) {
                    double x = *(static_cast<double*> (vars[j].value));
                    double y = std::stod(a);
                    return x - y;
                }
            }
            if (vars[i].name == a) {
                double x = *(static_cast<double*> (vars[i].value));
                double y = stod(b);
                return x - y;
            }
        }
    }
    if (o == "*") {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                if ((vars[i].name == a && vars[j].name == b)) {
                    double x = *(static_cast<double*> (vars[i].value));
                    double y = *(static_cast<double*> (vars[j].value));
                    return x * y;
                }
                else if (vars[j].name == b) {
                    double x = *(static_cast<double*> (vars[j].value));
                    double y = std::stod(a);
                    return x * y;
                }
            }
            if (vars[i].name == a) {
                double x = *(static_cast<double*> (vars[i].value));
                double y = stod(b);
                return x * y;
            }
        }
    }
    if (o == "/") {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                if ((vars[i].name == a && vars[j].name == b)) {
                    double x = *(static_cast<double*> (vars[i].value));
                    double y = *(static_cast<double*> (vars[j].value));
                    return x / y;
                }
                else if (vars[j].name == b) {
                    double x = *(static_cast<double*> (vars[j].value));
                    double y = std::stod(a);
                    return x / y;
                }
            }
            if (vars[i].name == a) {
                double x = *(static_cast<double*> (vars[i].value));
                double y = stod(b);
                return x / y;
            }
        }
    }
}

std::string soperator(std::string a, std::string b) {
    for (int i = 0; i < vars.size(); ++i) {
        for (int j = 0; j < vars.size(); ++j) {
            if ((vars[i].name == a && vars[j].name == b)) {
                std::string x = *(static_cast<std::string*> (vars[i].value));
                std::string y = *(static_cast<std::string*> (vars[j].value));
                return x + y;
            }
        }
    }
}

bool boper(std::string o, bool a, bool b) {
    if (o == "&&") {
        if (a == true && b == true) {
            return true;
        }
        else return false;
    }
    else if (o == "||") {
        if (a == true || b == true) {
            return true;
        }
        else return false;
    }
}

bool  bop(std::string o, std::string a) {
    if (o == "!") {
        for (int j = 0; j < vars.size(); ++j) {
            if ((vars[j].name == a) && vars[j].type=="tram") {
                bool i = !*(static_cast<bool*> (vars[j].value));
                return i;
            }
        }
    }
}

bool boperators(std::string o, std::string a, std::string b) {
    int size = vars.size();
    if (o == "&&") {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                if ((vars[i].name == a && vars[j].name == b)) {
                    bool x = *(static_cast<bool*> (vars[i].value));
                    bool y = *(static_cast<bool*> (vars[j].value));
                    return x && y;
                }
            }
        }
    }
    else if (o == "||") {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                if ((vars[i].name == a && vars[j].name == b)) {
                    bool x = *(static_cast<bool*> (vars[i].value));
                    bool y = *(static_cast<bool*> (vars[j].value));
                    return x || y;
                }
            }
        }
    }
    else if (o == "<") {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                if ((vars[i].name == a && vars[j].name == b) && (vars[i].type=="amb")) {
                    int x = *(static_cast<int*> (vars[i].value));
                    int y = *(static_cast<int*> (vars[j].value));
                    return x < y;
                }else if ((vars[i].name == a && vars[j].name == b) && (vars[i].type == "rac")) {
                    double x = *(static_cast<double*> (vars[i].value));
                    double y = *(static_cast<double*> (vars[j].value));
                    return x < y;
                }
            }
        }
    }
    else if (o == "==") {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                if ((vars[i].name == a && vars[j].name == b) && (vars[i].type == "amb")) {
                    int x = *(static_cast<int*> (vars[i].value));
                    int y = *(static_cast<int*> (vars[j].value));
                    return x == y;
                }
                else if ((vars[i].name == a && vars[j].name == b) && (vars[i].type == "rac")) {
                    double x = *(static_cast<double*> (vars[i].value));
                    double y = *(static_cast<double*> (vars[j].value));
                    return x == y;
                }
                else if ((vars[i].name == a && vars[j].name == b) && (vars[i].type == "tox")) {
                    std::string x = *(static_cast<std::string*> (vars[i].value));
                    std::string y = *(static_cast<std::string*> (vars[j].value));
                    return x == y;
                }
                else if ((vars[i].name == a && vars[j].name == b) && (vars[i].type == "tram")) {
                    bool x = *(static_cast<bool*> (vars[i].value));
                    bool y = *(static_cast<bool*> (vars[j].value));
                    return x == y;
                }
            }
        }
    }
    
    else if (o == ">") {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                if ((vars[i].name == a && vars[j].name == b) && (vars[i].type == "amb")) {
                    int x = *(static_cast<int*> (vars[i].value));
                    int y = *(static_cast<int*> (vars[j].value));
                    return x > y;
                }
                else if ((vars[i].name == a && vars[j].name == b) && (vars[i].type == "rac")) {
                    double x = *(static_cast<double*> (vars[i].value));
                    double y = *(static_cast<double*> (vars[j].value));
                    return x > y;
                }
            }
        }
    }
}
