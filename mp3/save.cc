#include<map>
#include<iostream>
#include<fstream>
#include<queue>
#include<sstream>
#include<unordered_map>
#include<bitset>

using namespace std;

struct Node {
    char ch;
    int freq;
    Node *left, *right;
};

Node* newNode(char ch, int freq, Node* left, Node* right) { //node 생성
    Node* node = new Node();

    node->ch = ch;
    node->freq = freq;
    node->left = left;
    node->right = right;

    return node;
}

struct comp {
    bool operator()(Node* l, Node* r) {
        return l->freq > r->freq;   //오름차순
    }
};



void TreetoHuffTable(Node* root, string str, unordered_map<char, string> &huffmanCode) { //huffmanCode map에 문자와 코드 저장
    if (root == nullptr) return;

    if (!root->left && !root->right) {  
        huffmanCode[root->ch] = str;    
    }

    TreetoHuffTable(root->left, str + "0", huffmanCode);     //왼쪽 자식 노드면 0 추가
    TreetoHuffTable(root->right, str + "1", huffmanCode);    //오른쪽 자식 노드면 1추가
}


void buildHuffmanTree(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error: File not found." << endl;
        return;
    }
    
    string line;
    unordered_map<char, int> freq;  // 빈도 수 저장 Map
    string fullText; // 모든 텍스트를 저장할 변수 추가
    string str = "";

    while (getline(file, line)) {
        fullText += line + "\n";  // 줄바꿈 문자 추가
    }
    
    for (char ch: fullText) {  // 전체 텍스트에 대해 빈도 계산
        freq[ch]++;
    }
    priority_queue<Node*, vector<Node*>, comp> pq;  //Node 오름차순 우선순위 큐
    
    for (auto pair: freq) { //priority queue에 모두 집어넣기
        pq.push(newNode(pair.first, pair.second, nullptr, nullptr));    //pair.first = ch, pair.second = freq
    }

    while (pq.size() != 1) {    //1개 남을 때까지 새로운 트리 만들기
        Node *left = pq.top(); pq.pop();    //new node의 왼쪽 자식
        Node *right = pq.top(); pq.pop();   //new node의 오른쪽 자식

        int sum = left->freq + right->freq;
        pq.push(newNode('\0', sum, left, right));   //아무것도 없는거 표시
    }
    
    Node* root = pq.top();

    unordered_map<char, string> hufftable;    //Huffman Code 설계도? 저장하는 Map
    TreetoHuffTable(root, "", hufftable);  //huf
    
    for (char ch: fullText) {  // text 대신 fullText 사용
        str += hufftable[ch];
    }

    ofstream ofs(filename + ".yy", ios::binary);
    for (const auto& pair : hufftable) {
        ofs << pair.first << '\x1e' << pair.second << '\x1f';
    }
    ofs << '\x1c';

    // 원래 허프만 코드의 길이를 파일에 쓰기
    int length = str.size();
    ofs.write(reinterpret_cast<char*>(&length), sizeof(length));
    int padding = 8 - (str.size() % 8);
    str += string(padding, '0');
    for (int i = 0; i < str.size(); i += 8) {
        bitset<8> bits(str.substr(i, 8));
        char c = bits.to_ulong();
        ofs.write(&c, sizeof(c));
    }
    ofs.close();
}



string HuffToText(unordered_map<string, char>& huffmanCode, string str) {
    string result = "";
    string currentCode = "";

    for (char bit : str) {
        currentCode += bit;
        if (huffmanCode.count(currentCode)) {
            result += huffmanCode[currentCode];
            currentCode = "";
        }
    }
    if (!currentCode.empty()) {
        cout << "hufftotext func problem : " << endl;
        throw std::runtime_error("Invalid Huffman code.");
    }
    return result;
}


void loadAndDecode(string& filename) {
    string line;
    string HuffTableStr;
    string textStr;

    ifstream ifs(filename, ios::binary);
    unordered_map<string, char> re_ordered_hufftable;    
    unordered_map<char, string> hufftable;


    while (std::getline(ifs, line, '\x1f')) {
        istringstream iss(line);
        char key = iss.get();
        iss.ignore();
        string value;
        getline(iss, value, '\x1f');
        hufftable[key] = value;
        if (!ifs.eof() && ifs.peek() == '\x1c') {
            HuffTableStr = line;
            break;
        }   
    }
    ifs.ignore();  // '\x1c' 문자 무시

    // 원래 허프만 코드의 길이를 읽기
    int length;
    ifs.read(reinterpret_cast<char*>(&length), sizeof(length));

    char c;
    while (ifs.read(&c, sizeof(c))) {
        bitset<8> bits(c);
        textStr += bits.to_string();
    }


    // 필요한 비트만 남기고 나머지 비트 제거
    textStr = textStr.substr(0, length);


    ifs.close();   
    for (auto& pair : hufftable) {
        re_ordered_hufftable[pair.second] = pair.first;
    }

    string decodedStr = HuffToText(re_ordered_hufftable, textStr);

    ofstream outputFile(filename + ".zz");
    outputFile << decodedStr;
    outputFile.close();
}

int main(int argc, char* argv[]){
    string option = argv[1];
    string ifile = argv[2];

    if(option.compare("-c") == 0){
        buildHuffmanTree(ifile);  // 텍스트 대신 파일 이름을 인자로 전달
    }
    else if(option.compare("-d") == 0){
        loadAndDecode(ifile);
    }
    else{
        cerr << "Error : use option only '-c' or '-d'" << endl;
        return 1;
    }
    return 0;
}